////////////////////////////////////////////////////////////////////////
// Class:       DropTestAnalyzer
// Module Type: analyzer
// File:        DropTestAnalyzer_module.cc
//
// Generated at Mon Aug  1 13:28:48 2011 by Chris Green using artmod
// from art v0_07_12.
////////////////////////////////////////////////////////////////////////

#include "cetlib/quiet_unit_test.hpp"

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "canvas/Persistency/Common/Ptr.h"

#include <string>

namespace arttest {
  class DropTestAnalyzer;
}

class arttest::DropTestAnalyzer : public art::EDAnalyzer {
public:
  explicit DropTestAnalyzer(fhicl::ParameterSet const &p);

  void analyze(art::Event const &e) override;


private:
  std::string inputLabel_;
   bool keepString_;
   bool keepMapVector_;
};

arttest::DropTestAnalyzer::DropTestAnalyzer(fhicl::ParameterSet const &p)
  :
   art::EDAnalyzer(p),
   inputLabel_(p.get<std::string>("input_label")),
   keepString_(p.get<bool>("keepString", false)),
   keepMapVector_(p.get<bool>("keepMapVector", true))
{
}

void arttest::DropTestAnalyzer::analyze(art::Event const &e) {
   art::Handle<art::Ptr<std::string> > sh;
   BOOST_CHECK_EQUAL((e.getByLabel(inputLabel_, sh)), keepString_);
   BOOST_REQUIRE_EQUAL(sh.isValid(), keepString_);
   if (keepString_ && keepMapVector_) {
      BOOST_CHECK_EQUAL(**sh, "TWO");
   }

   using mv_t = cet::map_vector<std::string>;
   art::Handle<mv_t> mvth;
   BOOST_CHECK_EQUAL((e.getByLabel(inputLabel_, mvth)), keepMapVector_);
   BOOST_REQUIRE_EQUAL(mvth.isValid(), keepMapVector_);
   if (keepMapVector_) {
      mv_t const & mapvec = *mvth;
      BOOST_REQUIRE(mapvec[cet::map_vector_key(7)] == "FOUR");
      BOOST_REQUIRE(mapvec[cet::map_vector_key(5)] == "THREE");
      BOOST_REQUIRE(mapvec[cet::map_vector_key(3)] == "TWO");
      BOOST_REQUIRE(mapvec[cet::map_vector_key(0)] == "ONE");
   }
}

DEFINE_ART_MODULE(arttest::DropTestAnalyzer)
