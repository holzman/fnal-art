////////////////////////////////////////////////////////////////////////
// Class:       OpenMPSimpleTest
// Module Type: analyzer
// File:        OpenMPSimpleTest_module.cc
//
// Generated at Mon Dec 19 16:04:08 2011 by Christopher Green using artmod
// from art v1_00_06.
////////////////////////////////////////////////////////////////////////

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"

#include "boost/test/included/unit_test.hpp"

#include "omp.h"

namespace arttest {
  class OpenMPSimpleTest;
}

extern "C" {
  size_t openmpTestFunc(size_t numLoops, size_t mult) {
    size_t total = 0;
#pragma omp parallel for reduction(+:total)
    for (size_t i = 0; i<numLoops; ++i) {
      total += i * mult;
      std::cerr << omp_get_thread_num() << "\n";
    }
    return total;
  }
}

class arttest::OpenMPSimpleTest : public art::EDAnalyzer {
public:
  explicit OpenMPSimpleTest(fhicl::ParameterSet const & p);
  virtual ~OpenMPSimpleTest();

  virtual void analyze(art::Event const & e);

private:

};


arttest::OpenMPSimpleTest::OpenMPSimpleTest(fhicl::ParameterSet const &)
{
}

arttest::OpenMPSimpleTest::~OpenMPSimpleTest()
{
}

void arttest::OpenMPSimpleTest::analyze(art::Event const &)
{
  size_t total = openmpTestFunc(10, 20);
  BOOST_REQUIRE_EQUAL(total, 900ul);
}

DEFINE_ART_MODULE(arttest::OpenMPSimpleTest)
