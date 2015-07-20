#include "art/Framework/Art/DebugOptionsHandler.h"

#include "art/Utilities/Exception.h"
#include "fhiclcpp/coding.h"
#include "fhiclcpp/extended_value.h"
#include "fhiclcpp/intermediate_table.h"
#include "fhiclcpp/parse.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std::string_literals;

art::DebugOptionsHandler::
DebugOptionsHandler(bpo::options_description & desc,
                    std::string const & basename,
                    bool rethrowDefault)
:
  rethrowDefault_(rethrowDefault)
{
  desc.add_options()
    ("trace", "Activate tracing.")
    ("notrace", "Deactivate tracing.")
    ("memcheck", "Activate monitoring of memory use.")
    ("memcheck-db", bpo::value<std::string>(), "Output memory use data to SQLite3 database with name <db-file>.")
    ("nomemcheck", "Deactivate monitoring of memory use.")
    ("default-exceptions", "Some exceptions may be handled differently by default (e.g. ProductNotFound).")
    ("rethrow-default", "All exceptions default to rethrow.")
    ("rethrow-all", "All exceptions overridden to rethrow (cf rethrow-default).")
    ("SIGINT-is-not-error", "A signal received from the user yields an art return code of 0.")
    ("debug-config", bpo::value<std::string>(),
     ("Output post-processed configuration to <file> and exit. Equivalent to env ART_DEBUG_CONFIG=<file> "s + basename + " ..."s).c_str())
    ("config-out", bpo::value<std::string>(),
     "Output post-processed configuration to <file> and continue with job.");
}

int
art::DebugOptionsHandler::
doCheckOptions(bpo::variables_map const & vm)
{
  if ((vm.count("rethrow-all") +
       vm.count("rethrow-default") +
       vm.count("no-rethrow-default")) > 1) {
    throw Exception(errors::Configuration)
        << "Options --default-exceptions, --rethrow-all and --rethrow-default \n"
        << "are mutually incompatible.\n";
  }
  if (vm.count("trace") == 1 &&
      vm.count("notrace") == 1) {
    throw Exception(errors::Configuration)
        << "Options --trace and --notrace are incompatible.\n";
  }
  if (vm.count("memcheck") == 1 &&
      vm.count("nomemcheck") == 1) {
    throw Exception(errors::Configuration)
        << "Options --memcheck and --nomemcheck are incompatible.\n";
  }
  if (vm.count("memcheck-db") == 1 &&
      vm.count("nomemcheck") == 1) {
    throw Exception(errors::Configuration)
      << "Options --memcheck-db and --nomemcheck are incompatible.\n";
  }
  return 0;
}

int
art::DebugOptionsHandler::
doProcessOptions(bpo::variables_map const & vm,
                 fhicl::intermediate_table & raw_config)
{
  if (vm.count("debug-config")) {
    setenv("ART_DEBUG_CONFIG",
           vm["debug-config"].as<std::string>().c_str(),
           true /* overwrite */);
  } else if (vm.count("config-out")) {
    raw_config.put("services.scheduler.configOut",
                   vm["config-out"].as<std::string>().c_str());
  }
  if (vm.count("trace")) {
    raw_config.put("services.scheduler.wantTracer", true);
  }
  else if (vm.count("notrace")) {
    raw_config.put("services.scheduler.wantTracer", false);
  }
  auto const memdb = vm.count("memcheck-db");
  if (vm.count("memcheck") || memdb) {
    raw_config.putEmptyTable("services.MemoryTracker");
    if ( memdb )
      raw_config.put("services.MemoryTracker.filename",
                     vm["memcheck-db"].as<std::string>().data());
  }
  else if (vm.count("nomemcheck")) {
    raw_config.erase("services.SimpleMemoryCheck");
    raw_config.erase("services.MemoryTracker");
  }
  if (vm.count("rethrow-all") == 1 ||
      vm.count("rethrow-default") == 1 ||
      (rethrowDefault_ && vm.count("default-exceptions") == 0) ) {
    raw_config.put("services.scheduler.defaultExceptions", false);
    if (vm.count("rethrow-all") == 1) {
      raw_config.putEmptySequence("services.scheduler.IgnoreCompletely");
      raw_config.putEmptySequence("services.scheduler.SkipEvent");
      raw_config.putEmptySequence("services.scheduler.FaileModule");
      raw_config.putEmptySequence("services.scheduler.FailPath");
    }
  }
  if (vm.count("SIGINT-is-not-error") == 1) {
    raw_config.put("services.scheduler.SIGINTisNotError", true );
  }
  return 0;
}
