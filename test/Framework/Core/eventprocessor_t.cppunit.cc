/*----------------------------------------------------------------------

Test of the EventProcessor class.



----------------------------------------------------------------------*/
#include <exception>
#include <iostream>
#include <string>
#include <sstream>
#include "boost/regex.hpp"

//I need to open a 'back door' in order to test the functionality
#include "art/Framework/Services/Registry/ActivityRegistry.h"
#define private public
#include "art/Framework/Services/Registry/ServiceRegistry.h"
#undef private

#include "art/Framework/Core/EventProcessor.h"
#include "art/Utilities/Exception.h"
#include "art/Utilities/Presence.h"
#include "art/Framework/PluginManager/PresenceFactory.h"
#include "FWCore/Framework/test/stubs/TestBeginEndJobAnalyzer.h"

#include "art/Framework/PluginManager/ProblemTracker.h"

#include "art/ParameterSet/PythonProcessDesc.h"

#include "art/Persistency/Provenance/ModuleDescription.h"


#include "cppunit/extensions/HelperMacros.h"

class testeventprocessor: public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE(testeventprocessor);
  CPPUNIT_TEST(parseTest);
  CPPUNIT_TEST(prepostTest);
  CPPUNIT_TEST(beginEndTest);
  CPPUNIT_TEST(cleanupJobTest);
  CPPUNIT_TEST(activityRegistryTest);
  CPPUNIT_TEST(moduleFailureTest);
  CPPUNIT_TEST(endpathTest);
  CPPUNIT_TEST(asyncTest);
  CPPUNIT_TEST_SUITE_END();

 public:

  void setUp()
  {
    m_handler = std::auto_ptr<art::AssertHandler>(new art::AssertHandler());
    sleep_secs_ = 0;
  }

  void tearDown(){ m_handler.reset();}
  void parseTest();
  void prepostTest();
  void beginEndTest();
  void cleanupJobTest();
  void activityRegistryTest();
  void moduleFailureTest();
  void endpathTest();

  void asyncTest();
  bool asyncRunAsync(art::EventProcessor& ep);
  bool asyncRunTimeout(art::EventProcessor& ep);
  void driveAsyncTest( bool (testeventprocessor::*)(art::EventProcessor&),
		       const std::string& config_string);

 private:
  std::auto_ptr<art::AssertHandler> m_handler;
  void work()
  {
    std::string configuration(
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "    input = cms.untracked.int32(5))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDProducer('TestMod',\n"
      "    ivalue = cms.int32(10))\n"
      "process.m2 = cms.EDProducer('TestMod',\n"
      "    ivalue = cms.int32(-3))\n"
      "process.p1 = cms.Path(process.m1*process.m2)\n");
    art::EventProcessor proc(configuration, true);
    proc.beginJob();
    proc.run();
    proc.endJob();
  }

  int sleep_secs_;
};

///registration of the test so that the runner can find it
CPPUNIT_TEST_SUITE_REGISTRATION(testeventprocessor);

static std::string makeConfig(int event_count)
{
  static const std::string start =
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.MessageLogger = cms.Service('MessageLogger',\n"
      "    destinations = cms.untracked.vstring(\n"
      "        'cout',\n"
      "        'cerr'),\n"
      "    categories = cms.untracked.vstring(\n"
      "        'FwkJob',\n"
      "        'FwkReport'),\n"
      "    cout = cms.untracked.PSet(\n"
      "        threshold = cms.untracked.string('INFO'),\n"
      "        FwkReport = cms.untracked.PSet(\n"
      "            limit = cms.untracked.int32(0))),\n"
      "    cerr = cms.untracked.PSet(\n"
      "        threshold = cms.untracked.string('INFO'),\n"
      "        FwkReport = cms.untracked.PSet(\n"
      "            limit = cms.untracked.int32(0)))\n"
      ")\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "    input = cms.untracked.int32(";
  static const std::string finish =
      "))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDProducer('IntProducer',\n"
      "    ivalue = cms.int32(10))\n"
      "process.p1 = cms.Path(process.m1)\n";

  std::ostringstream ost;
  ost << start << event_count << finish;
  return ost.str();
}

void testeventprocessor::asyncTest()
{
  std::string test_config_2 = makeConfig(2);
  std::string test_config_80k = makeConfig(20000);

  // Load the message service plug-in
  boost::shared_ptr<art::Presence> theMessageServicePresence;
  try {
    theMessageServicePresence =
      boost::shared_ptr<art::Presence>(art::PresenceFactory::get()->makePresence("MessageServicePresence").release());
  } catch(std::exception& e) {
    std::cerr << e.what() << std::endl;
    return;
  }

  sleep_secs_=0;
  std::cerr << "asyncRunAsync 2 event\n";
  driveAsyncTest(&testeventprocessor::asyncRunAsync,test_config_2);
  std::cerr << "asyncRunAsync 80k event\n";
  driveAsyncTest(&testeventprocessor::asyncRunAsync,test_config_80k);
  sleep_secs_=3;
  std::cerr << "asyncRunAsync 2 event with sleep 3\n";
  driveAsyncTest(&testeventprocessor::asyncRunAsync,test_config_2);
  sleep_secs_=0;
  std::cerr << "asyncRunTimeout 80k event\n";
  // cannot run the following test from scram because of a runtime
  // library error:
  // libgcc_s.so.1 must be installed for pthread_cancel to work
  // driveAsyncTest(&testeventprocessor::asyncRunTimeout,test_config_80k);
}

bool testeventprocessor::asyncRunAsync(art::EventProcessor& ep)
{
  for(int i=0;i<7;++i)
    {
      ep.setRunNumber(i+1);
      ep.runAsync();
      if(sleep_secs_>0) sleep(sleep_secs_);

      art::EventProcessor::StatusCode rc;
      if (i < 2) {
        rc = ep.waitTillDoneAsync(1000);
      }
      else if (i == 2) {
        rc = ep.stopAsync(1000);
      }
      else if (i == 3) {
        rc = ep.stopAsync(1000);
        rc = ep.waitTillDoneAsync(1000);
      }
      else if (i == 4) {
        rc = ep.waitTillDoneAsync(1000);
        rc = ep.stopAsync(1000);
      }
      else if (i == 5) {
        rc = ep.waitTillDoneAsync(1000);
        rc = ep.waitTillDoneAsync(1000);
      }
      else if (i == 6) {
        rc = ep.stopAsync(1000);
        rc = ep.stopAsync(1000);
      }
      std::cerr << " ep runAsync run " << i << " done\n";

      switch(rc)
	{
	case art::EventProcessor::epSuccess:
	case art::EventProcessor::epInputComplete:
	  break;
	case art::EventProcessor::epTimedOut:
	default:
	  {
	    std::cerr << "rc from run "<< i <<", doneAsync = " << rc << "\n";
	    CPPUNIT_ASSERT("Bad rc from doneAsync"==0);
	  }
	}
    }
  return true;
}

bool testeventprocessor::asyncRunTimeout(art::EventProcessor& ep)
{
  ep.setRunNumber(1);
  ep.runAsync();
  art::EventProcessor::StatusCode rc = ep.waitTillDoneAsync(1);
  std::cerr << " ep runAsync run " << 1 << " done\n";

  switch(rc)
    {
    case art::EventProcessor::epTimedOut:
      break;
    case art::EventProcessor::epSuccess:
    case art::EventProcessor::epInputComplete:
      break;
    default:
      {
	std::cerr << "rc from run "<< 1 <<", doneAsync = " << rc << "\n";
	CPPUNIT_ASSERT("Bad rc from doneAsync"==0);
      }
    }
  return false;
}

void testeventprocessor::driveAsyncTest( bool(testeventprocessor::*func)(art::EventProcessor& ep),const std::string& config_str )
{

  try {
    art::EventProcessor proc(config_str, true);
    proc.beginJob();
    if ((this->*func)(proc))
      proc.endJob();
    else
      {
	std::cerr << "event processor is in error state\n";
      }
  }
  catch(artZ::Exception& e)
    {
      std::cerr << "cms exception: " << e.explainSelf() << std::endl;
      CPPUNIT_ASSERT("cms exeption"==0);
    }
  catch(std::exception& e)
    {
      std::cerr << "std exception: " << e.what() << std::endl;
      CPPUNIT_ASSERT("std exeption"==0);
    }
  catch(...)
    {
      std::cerr << "unknown exception " << std::endl;
      CPPUNIT_ASSERT("unknown exeption"==0);
    }
  std::cerr << "*********************** driveAsyncTest ending ------\n";
}

void testeventprocessor::parseTest()
{
  int rc = -1;                // we should never return this value!
  try { work(); rc = 0;}
  catch (artZ::Exception& e) {
      std::cerr << "cms exception caught: "
		<< e.explainSelf() << std::endl;
      CPPUNIT_ASSERT("Caught artZ::Exception " == 0);
  }
  catch (std::exception& e) {
      std::cerr << "Standard library exception caught: "
		<< e.what() << std::endl;
      CPPUNIT_ASSERT("Caught std::exception " == 0);
  }
  catch (...) {
      CPPUNIT_ASSERT("Caught unknown exception " == 0);
  }
}

static int g_pre = 0;
static int g_post = 0;

static
void doPre(const art::EventID&, const art::Timestamp&)
{
  ++g_pre;
}

static
void doPost(const art::Event&, const art::EventSetup&)
{
  CPPUNIT_ASSERT(g_pre == ++g_post);
}

void testeventprocessor::prepostTest()
{
  std::string configuration(
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "  input = cms.untracked.int32(5))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDProducer('TestMod',\n"
      "   ivalue = cms.int32(-3))\n"
      "process.p1 = cms.Path(process.m1)\n");

  art::EventProcessor proc(configuration, true);

  proc.preProcessEventSignal().connect(&doPre);
  proc.postProcessEventSignal().connect(&doPost);
  proc.beginJob();
  proc.run();
  proc.endJob();
  CPPUNIT_ASSERT(5 == g_pre);
  CPPUNIT_ASSERT(5 == g_post);
  {
    art::EventProcessor const& crProc(proc);
    typedef std::vector<art::ModuleDescription const*> ModuleDescs;
    ModuleDescs  allModules = crProc.getAllModuleDescriptions();
    CPPUNIT_ASSERT(2 == allModules.size()); // TestMod and TriggerResultsInserter
    std::cout << "\nModuleDescriptions in testeventprocessor::prepostTest()---\n";
    for (ModuleDescs::const_iterator i = allModules.begin(),
	    e = allModules.end() ;
	  i != e ;
	  ++i)
      {
	CPPUNIT_ASSERT(*i != 0);
	std::cout << **i << '\n';
      }
    std::cout << "--- end of ModuleDescriptions\n";

    CPPUNIT_ASSERT(5 == crProc.totalEvents());
    CPPUNIT_ASSERT(5 == crProc.totalEventsPassed());
  }
}

void testeventprocessor::beginEndTest()
{
  std::string configuration(
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "    input = cms.untracked.int32(10))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDAnalyzer('TestBeginEndJobAnalyzer')\n"
      "process.p1 = cms.Path(process.m1)\n");
  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(0 == proc.totalEvents());

    proc.beginJob();

    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(0 == proc.totalEvents());

    proc.endJob();

    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(0 == proc.totalEvents());
  }
  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);
    proc.runToCompletion(false);

    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(10 == proc.totalEvents());
  }
  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);
    proc.beginJob();

    // Check that beginJob is not called again
    TestBeginEndJobAnalyzer::beginJobCalled = false;

    proc.runToCompletion(false);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(10 == proc.totalEvents());

    proc.endJob();

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(10 == proc.totalEvents());
  }
  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);
    proc.beginJob();

    // Check that beginJob is not called again
    TestBeginEndJobAnalyzer::beginJobCalled = false;

    proc.runEventCount(-1);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(10 == proc.totalEvents());
  }
  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);
    proc.runEventCount(2);

    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(2 == proc.totalEvents());

    // Check that these are not called again
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;

    proc.runEventCount(1);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(3 == proc.totalEvents());

    proc.runEventCount(100);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(10 == proc.totalEvents());

    proc.endJob();

    // Check that these are not called again
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(10 == proc.totalEvents());
  }
  CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
  CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);

  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);
    proc.runEventCount(5);

    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(5 == proc.totalEvents());

    // Check that these are not called again
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;

    proc.endJob();

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(5 == proc.totalEvents());

    // Check that these are not called again
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;
  }
  CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
  CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);

  {
    TestBeginEndJobAnalyzer::beginJobCalled = false;
    TestBeginEndJobAnalyzer::endJobCalled = false;
    TestBeginEndJobAnalyzer::beginRunCalled = false;
    TestBeginEndJobAnalyzer::endRunCalled = false;
    TestBeginEndJobAnalyzer::beginSubRunCalled = false;
    TestBeginEndJobAnalyzer::endSubRunCalled = false;

    art::EventProcessor proc(configuration, true);
    proc.runEventCount(4);

    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginJobCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endRunCalled);
    CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::beginSubRunCalled);
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endSubRunCalled);
    CPPUNIT_ASSERT(4 == proc.totalEvents());
  }
  CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::endJobCalled);
  CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endRunCalled);
  CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::endSubRunCalled);
}

void testeventprocessor::cleanupJobTest()
{
  std::string configuration(
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "    input = cms.untracked.int32(2))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDAnalyzer('TestBeginEndJobAnalyzer')\n"
      "process.p1 = cms.Path(process.m1)\n");
  {
    TestBeginEndJobAnalyzer::destructorCalled = false;
    art::EventProcessor proc(configuration, true);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::destructorCalled);
    proc.beginJob();
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::destructorCalled);
    proc.endJob();
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::destructorCalled);
  }
  CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::destructorCalled);
  {
    TestBeginEndJobAnalyzer::destructorCalled = false;
    art::EventProcessor proc(configuration, true);

    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::destructorCalled);
    proc.run(1);
    CPPUNIT_ASSERT(1 == proc.totalEvents());
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::destructorCalled);
    proc.run(1);
    CPPUNIT_ASSERT(2 == proc.totalEvents());
    CPPUNIT_ASSERT(!TestBeginEndJobAnalyzer::destructorCalled);

  }
  CPPUNIT_ASSERT(TestBeginEndJobAnalyzer::destructorCalled);
}

namespace {
  struct Listener{
    Listener(art::ActivityRegistry& iAR) :
      postBeginJob_(false),
      postEndJob_(false),
      preEventProcessing_(false),
      postEventProcessing_(false),
      preModule_(false),
      postModule_(false){
	iAR.watchPostBeginJob(this,&Listener::postBeginJob);
	iAR.watchPostEndJob(this,&Listener::postEndJob);

	iAR.watchPreProcessEvent(this,&Listener::preEventProcessing);
	iAR.watchPostProcessEvent(this,&Listener::postEventProcessing);

	iAR.watchPreModule(this, &Listener::preModule);
	iAR.watchPostModule(this, &Listener::postModule);
      }

    void postBeginJob() {postBeginJob_=true;}
    void postEndJob() {postEndJob_=true;}

    void preEventProcessing(const art::EventID&, const art::Timestamp&){
      preEventProcessing_=true;}
    void postEventProcessing(const art::Event&, const art::EventSetup&){
      postEventProcessing_=true;}

    void preModule(const art::ModuleDescription&){
      preModule_=true;
    }
    void postModule(const art::ModuleDescription&){
      postModule_=true;
    }

    bool allCalled() const {
      return postBeginJob_&&postEndJob_
	&&preEventProcessing_&&postEventProcessing_
	&&preModule_&&postModule_;
    }

    bool postBeginJob_;
    bool postEndJob_;
    bool preEventProcessing_;
    bool postEventProcessing_;
    bool preModule_;
    bool postModule_;
  };
}

void
testeventprocessor::activityRegistryTest()
{
  std::string configuration(
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "    input = cms.untracked.int32(5))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDProducer('TestMod',\n"
      "   ivalue = cms.int32(-3))\n"
      "process.p1 = cms.Path(process.m1)\n");

  boost::shared_ptr<art::ProcessDesc> processDesc = PythonProcessDesc(configuration).processDesc();

  //We don't want any services, we just want an ActivityRegistry to be created
  // We then use this ActivityRegistry to 'spy on' the signals being produced
  // inside the EventProcessor
  std::vector<art::ParameterSet> serviceConfigs;
  art::ServiceToken token = art::ServiceRegistry::createSet(serviceConfigs);

  art::ActivityRegistry ar;
  token.connect(ar);
  Listener listener(ar);

  art::EventProcessor proc(processDesc, token, art::serviceregistry::kOverlapIsError);

  proc.beginJob();
  proc.run();
  proc.endJob();

  CPPUNIT_ASSERT(listener.postBeginJob_);
  CPPUNIT_ASSERT(listener.postEndJob_);
  CPPUNIT_ASSERT(listener.preEventProcessing_);
  CPPUNIT_ASSERT(listener.postEventProcessing_);
  CPPUNIT_ASSERT(listener.preModule_);
  CPPUNIT_ASSERT(listener.postModule_);

  CPPUNIT_ASSERT(listener.allCalled());
}

static
bool
findModuleName(const std::string& iMessage) {
  static const boost::regex expr("TestFailuresAnalyzer");
  return regex_search(iMessage,expr);
}

void
testeventprocessor::moduleFailureTest()
{
  try {

    const std::string preC(
      "import FWCore.ParameterSet.Config as cms\n"
      "process = cms.Process('p')\n"
      "process.maxEvents = cms.untracked.PSet(\n"
      "    input = cms.untracked.int32(2))\n"
      "process.source = cms.Source('EmptySource')\n"
      "process.m1 = cms.EDAnalyzer('TestFailuresAnalyzer',\n"
      "    whichFailure = cms.int32(");

    const std::string postC(
      "))\n"
      "process.p1 = cms.Path(process.m1)\n");

    {
      const std::string configuration = preC +"0"+postC;
      bool threw = true;
      try {
	art::EventProcessor proc(configuration, true);
	threw = false;
      } catch(const artZ::Exception& iException){
	if(!findModuleName(iException.explainSelf())) {
	  std::cout <<iException.explainSelf()<<std::endl;
	  CPPUNIT_ASSERT(0 == "module name not in exception message");
	}
      }
      CPPUNIT_ASSERT(threw && 0 != "exception never thrown");
    }
    {
      const std::string configuration = preC +"1"+postC;
      bool threw = true;
      art::EventProcessor proc(configuration, true);

      try {
	proc.beginJob();
	threw = false;
      } catch(const artZ::Exception& iException){
	if(!findModuleName(iException.explainSelf())) {
	  std::cout <<iException.explainSelf()<<std::endl;
	  CPPUNIT_ASSERT(0 == "module name not in exception message");
	}
      }
      CPPUNIT_ASSERT(threw && 0 != "exception never thrown");
    }

    {
      const std::string configuration = preC +"2"+postC;
      bool threw = true;
      art::EventProcessor proc(configuration, true);

      proc.beginJob();
      try {
	proc.run(1);
	threw = false;
      } catch(const artZ::Exception& iException){
	if(!findModuleName(iException.explainSelf())) {
	  std::cout <<iException.explainSelf()<<std::endl;
	  CPPUNIT_ASSERT(0 == "module name not in exception message");
	}
      }
      CPPUNIT_ASSERT(threw && 0 != "exception never thrown");
      proc.endJob();
    }
    {
      const std::string configuration = preC +"3"+postC;
      bool threw = true;
      art::EventProcessor proc(configuration, true);

      proc.beginJob();
      try {
	proc.endJob();
	threw = false;
      } catch(const artZ::Exception& iException){
	if(!findModuleName(iException.explainSelf())) {
	  std::cout <<iException.explainSelf()<<std::endl;
	  CPPUNIT_ASSERT(0 == "module name not in exception message");
	}
      }
      CPPUNIT_ASSERT(threw && 0 != "exception never thrown");
    }
    ///
    {
      bool threw = true;
      try {
        std::string configuration(
          "import FWCore.ParameterSet.Config as cms\n"
          "process = cms.Process('p')\n"
          "process.maxEvents = cms.untracked.PSet(\n"
          "    input = cms.untracked.int32(2))\n"
          "process.source = cms.Source('EmptySource')\n"
          "process.p1 = cms.Path(process.m1)\n");
        art::EventProcessor proc(configuration, true);

	threw = false;
      } catch(const artZ::Exception& iException){
        static const boost::regex expr("m1");
	if(!regex_search(iException.explainSelf(),expr)) {
	  std::cout <<iException.explainSelf()<<std::endl;
	  CPPUNIT_ASSERT(0 == "module name not in exception message");
	}
      }
      CPPUNIT_ASSERT(threw && 0 != "exception never thrown");
    }

  } catch(const artZ::Exception& iException) {
    std::cout <<"Unexpected exception "<<iException.explainSelf()<<std::endl;
    throw;
  }
}

void
testeventprocessor::endpathTest()
{
}
