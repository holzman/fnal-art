
// ------------------------------------------------------------
//
// ToyRawInput is a RawInputSource that pretends to reconstitute several
// products. It exercises the FileReaderSource template.
//
// ------------------------------------------------------------

#include "art/Framework/Core/Frameworkfwd.h"
#include "art/Framework/Core/InputSourceMacros.h"
#include "art/Framework/Core/ProductRegistryHelper.h"
#include "art/Framework/IO/Sources/FileReaderSource.h"
#include "art/Framework/IO/Sources/put_product_in_principal.h"
#include "fhiclcpp/ParameterSet.h"
#include "test/TestObjects/ToyProducts.h"

#include "boost/shared_ptr.hpp"

#include <cstdio>

using fhicl::ParameterSet;
using std::string;
using std::vector;
using boost::shared_ptr;
using namespace art;


// Out simulated input file format is:
// A parameter in a parameter set, which contains a vector of vector of int.
// Each inner vector is a triplet of run/subrun/event number.
//   -1 means no new item of that type
//

namespace arttest
{

  //
  // ToyFile is the sort of class that experimenters who make use of
  // FileReaderSource must write.
  //
  class ToyFile
  {
  public:
    ToyFile(fhicl::ParameterSet const& ps,
            art::ProductRegistryHelper& help,
            art::PrincipalMaker const& pm);

    void closeCurrentFile();

    bool moveToFile(std::string const& name);

    bool readNext(art::RunPrincipal* const& inR,
                  art::SubRunPrincipal* const& inSR,
                  art::RunPrincipal*& outR,
                  art::SubRunPrincipal*& outSR,
                  art::EventPrincipal*& outE);

    bool readFileBlock(art::FileBlock*& fb);

  private:
    typedef std::vector<std::vector<int> > vv_t;
    typedef vv_t::const_iterator iter;

    art::PrincipalMaker pm_;

    iter    current_;
    iter    end_;
    ParameterSet data_;
    vv_t fileData_;
    string currentFilename_;
  };

  ToyFile::ToyFile(fhicl::ParameterSet const& ps,
                   art::ProductRegistryHelper& helper,
                   art::PrincipalMaker const &pm) :
    pm_(pm),
    current_(),
    end_(),
    data_(ps),
    fileData_()
  {
    helper.reconstitutes<int, InEvent>("m1", "DAQ");
    helper.reconstitutes<double, InSubRun>("s1", "LUMCALC");
    helper.reconstitutes<double, InRun>("r1", "RUNCALC");
    helper.reconstitutes<bool, InEvent>("m2", "SILLY", "a");
    helper.reconstitutes<bool, InEvent>("m2", "SILLY", "b");
  }

  void ToyFile::closeCurrentFile()
  { 
    fileData_.clear();
    current_ = iter();
    end_ = iter();
  }

  bool ToyFile::moveToFile(std::string const& filename)
  {
    bool result = data_.get_if_present(filename, fileData_);
    currentFilename_ = filename;
    current_ = fileData_.begin();
    end_ = fileData_.end();
    return result;
  }

  bool ToyFile::readNext(art::RunPrincipal* const& inR,
                         art::SubRunPrincipal* const& inSR,
                         art::RunPrincipal*& outR,
                         art::SubRunPrincipal*& outSR,
                         art::EventPrincipal*& outE)
  {
    // Have we any more to read?
    if (current_ == end_) return false;

    if (data_.get<bool>("returnTrueWithoutNewFault", false)) return true;

    bool readSomething = false;

    if ((*current_)[0] != -1) 
      {
        Timestamp runstart; // current time?
        outR = pm_.makeRunPrincipal((*current_)[0],  // run number
                                    runstart);     // starting time
        put_product_in_principal(std::auto_ptr<double>(new double(76.5)),
                                 *outR,
                                 "r1",
                                 "RUNCALC");
         readSomething = true;
      }
    if ((*current_)[1] != -1) 
      {
        assert(outR || inR);
        SubRunID newSRID;
        if (data_.get<bool>("newRunSameSubRunFault", false) && inSR)
          {
            newSRID = SubRunID(inSR->id());
          }
        else
          {
            newSRID = SubRunID(outR?outR->run():inR->run(),
                               (*current_)[1]);
          }
        Timestamp runstart; // current time?
        outSR = pm_.makeSubRunPrincipal(newSRID.run(),
                                        newSRID.subRun(),
                                        runstart);     // starting time
        put_product_in_principal(std::auto_ptr<double>(new double(7.0)),
                                 *outSR,
                                 "s1",
                                 "LUMCALC");
        readSomething = true;
      }
    if ((*current_)[2] != -1) 
      {
        assert(outSR || inSR);
        Timestamp runstart; // current time?
        outE = pm_.makeEventPrincipal(outR?outR->run():inR->run(),
                                      outSR?outSR->subRun():inSR->subRun(),
                                      (*current_)[2],  // event number
                                      runstart);     // starting time
        put_product_in_principal(std::auto_ptr<int>(new int(26)),
                                 *outE,
                                 "m1",
                                 "DAQ");
        put_product_in_principal(std::auto_ptr<bool>(new bool(false)),
                                 *outE,
                                 "m2",
                                 "SILLY",
                                 "a");
        put_product_in_principal(std::auto_ptr<bool>(new bool(true)),
                                 *outE,
                                 "m2",
                                 "SILLY",
                                 "b");
         readSomething = true;
      }
    if (readSomething)
      {
        if (data_.get<bool>("returnFalseWithNewFault", false))
          return false;
      }
    else
       {
         throw Exception(errors::DataCorruption)
           << "Did not read expected info from 'file.'\n";
       }
    ++current_;
    return true;
  }

  bool ToyFile::readFileBlock(art::FileBlock*& fb) {
    fb = new art::FileBlock(FileFormatVersion(1, "ToyFile 2011a"),
                            currentFilename_);
    return true;
  }

  //
  // TowRawInput is an instantiation of the FileReaderSource template.
  //

  typedef art::FileReaderSource<ToyFile> ToyRawInput;
}

DEFINE_ART_INPUT_SOURCE(arttest::ToyRawInput);
