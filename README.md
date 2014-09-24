FNAL Art Event Processing Framework
===================================
See the main [FNAL page](https://cdcvs.fnal.gov/redmine/projects/art) for
the authoratative upstream project.

This follow of the upstream project aims to make building Art easier
by removing the dependence on FNAL's custom build tools and configuration
management system. Whilst Art uses the [CMake](http://www.cmake.org)
build tool, the upstream FNAL CMake scripts make calls to the higher level
configuration management system,
[ups](https://cdcvs.fnal.gov/redmine/projects/ups)
and the [cetbuildtools](https://cdcvs.fnal.gov/redmine/projects/cetbuildtools).
This makes it impossible to build Art without both of these systems,
reducing the portability and usefulness of Art. It also prevents use
of any other configuration management system for supplying (for example)
locations of dependencies. Removing the coupling of the buildtool to the
configuration management system does not prevent usage of the latter if
so required. It simply notices that the buildtool is at a lower level than
the configuration management tool, and thus should not call up into the
higher level system.

This specific `remove-ups-1.11.3` branch removes calls to the `ups` system and
wrappings of other FNAL specific tools in the CMake scripts for Art.
It also makes use of the [FNALCore](https://github.com/LBNE/FNALCore)
project to simplify the use of Art's FNAL supplied dependencies.
This project is a work in progress, so some areas include reasonable
amounts of boilerplate CMake scripting that could be replaced by functions
or macros. This is intentional because we want to identify the key repeatable
areas before wrapping them into a coherent "ArtCMake" API for both developers
and clients of art. A problem with
Art's CET supplied CMake functions (and cetbuildtools) is that they provide
"kitchen sink" interfaces which are often overcomplicated and with significant
side effects.

Installation
============
Requirements
------------
- [CMake](http://www.cmake.org) 2.8.12 or above
- C++11 compliant compiler (GCC 4.8 or better)
  - Intent is to support any C++11 compliant compiler, but code from upstream
    is not completely portable at present.
  - NB: Full list of needed C++11 features not fully determined at present
  - Usage of FNALCore does not guarantee this
- [FNALCore](https://github.com/LBNE/FNALCore) library
  - Correct installation of this will also provide [Boost](http://www.boost.org)
  - art requires Boost 1.53 or better
- [GCCXML](http://gccxml.github.io/HTML/Index.html) 0.9.0 or higher
- [CLHEP](http://proj-clhep.web.cern.ch/proj-clhep/) 2.2.0.3 or higher
- [SQLite](http://www.sqlite.org/) 3.8.5 or higher
- [ROOT](http://root.cern.ch) 5.34.20 (or better 5 series)
- [Intel TBB](https://www.threadingbuildingblocks.org/) 4.1.0 or higher

**NB**: all C++ libraries must have been compiled against the *same*
C++ standard, specifically 0X or 11, and link to the *same* C++ Standard
Library.

How to Install
--------------
The build of Art provides a standard CMake system, so if you
have all the requirements installed and available directly in CMake's
search path(s), then you can simply do

```
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
$ make install
```

**TODO**: add detail on how to find packages when not in standard locations.


Status
======
Build Tree
----------
Here's a tree of the products in the art source tree, and the status
of their conversion. NB, some products, notably plugins and dictionary
libraries may be incomplete as it can be difficult to derive exactly what
is produced (and names may not be exact). The conversion to pure CMake tries
to add functionality from least to most dependent (an estimate of dependencies
based off of stated dependencies for cetbuildtools invocations is given
in [art/Framework/CMakeLists.txt](art's Framework CMake script).
Dictionary libraries are left until last.

```sh
+- art/
   +- Framework/
   |  +- Art/
   |  |  +- libart_Framework_Art.so                               (NO)
   |  |  +- art                                                   (NO) \
   |  |  +- gm2                                                   (NO) |
   |  |  +- lar                                                   (NO) |- only difference, mu2e has different
   |  |  +- mu2e                                                  (NO) |  exception handling
   |  |  +- nova                                                  (NO) /
   |  |  +- art_ut                                                (NO) \
   |  |  +- gm2_ut                                                (NO) |
   |  |  +- lar_ut                                                (NO) | - as above, but use boost::test
   |  |  +- mu2e_ut                                               (NO) |
   |  |  +- nova_ut                                               (NO) /
   |  +- Core/
   |  |  +- libart_Framework_Core.so                              (YES)
   |  +- EventProcessor/
   |  |  +- libart_Framework_EventProcessor.so                    (NO)
   |  +- IO/
   |  |  +- libart_Framework_IO.so                                (NO)
   |  |  +- Catalog/
   |  |  |  +- libart_Framework_IO_Catalog.so                     (NO)
   |  |  +- ProductMix/
   |  |  |  +- libart_Framework_IO_ProductMix.so                  (NO)
   |  |  |  +- libart_Framework_IO_ProductMix_dict.so             (NO)
   |  |  |  +- libart_Framework_IO_ProductMix_map.so              (NO)
   |  |  +- Root/
   |  |  |  +- libart_Framework_IO_Root.so                        (NO)
   |  |  |  +- libart_Framework_IO_RootVersion.so                 (NO)
   |  |  |  +- libRootInput_source.so                             (NO)
   |  |  |  +- libRootOutput_module.so                            (NO)
   |  |  |  +- config_dumper                                      (NO)
   |  |  |  +- sam_metadata_dumper                                (NO)
   |  |  +- Sources/
   |  |     +- libart_Framework_IO_Sources.so                     (NO)
   |  +- Modules/
   |  |  +- libart_Framework_Modules_dict.so                      (NO)
   |  |  +- libart_Framework_Modules_map.so                       (NO)
   |  |  +- libBlockingPrescaler_module.so                        (NO)
   |  |  +- libEmptyEvent_source.so                               (NO)
   |  |  +- libFileDumperOutput_module.so                         (NO)
   |  |  +- libPrescaler_module.so                                (NO)
   |  |  +- libProvenanceCheckerOutput_module.so                  (NO)
   |  |  +- libRandomNumberSaver_module.so                        (NO)
   |  +- Principal/
   |  |  +- libart_Framework_Principal.so                         (YES)
   |  +- Services/
   |     +- FileServiceInterfaces/
   |     |  +- libart_Framework_Services_FileServiceInterfaces.so (YES)
   |     +- Optional/
   |     |  +- libart_Framework_Services_Optional.so              (YES)
   |     |  +- libRandomNumberGenerator_service.so                (YES)
   |     |  +- libSimpleInteraction_service.so                    (YES)
   |     |  +- libSimpleMemoryCheck_service.so                    (YES)
   |     |  +- libTFileService_service.so                         (NO)
   |     |  +- libTiming_service.so                               (YES)
   |     |  +- libTracer_service.so                               (YES)
   |     |  +- libTrivialFileDelivery_service.so                  (YES)
   |     |  +- libTrivialFileTransfer_service.so                  (YES)
   |     +- Registry/
   |     |  +- libart_Framework_Services_Registry.so              (YES)
   |     +- System/
   |     |  +- libCurrentModule_service.so                        (YES)
   |     |  +- libFileCatalogMetadata_service.so                  (YES)
   |     |  +- libFloatingPointControl_service.so                 (YES)
   |     |  +- libPathSelection_service.so                        (NO)
   |     |  +- libScheduleContext_service.so                      (NO)
   |     |  +- libTriggerNamesService.so                          (YES)
   |     +- UserInteraction/
   |        +- libart_Framework_Services_UserInteraction.so       (YES)
   +- Ntuple/
   |  +- libart_Ntuple.so                                         (YES)
   +- Persistency/
   |  +- CetlibDictionaries/
   |  |  +- libart_Persistency_CetlibDictionaries_dict.so         (NO)
   |  |  +- libart_Persistency_CetlibDictionaries_map.so          (NO)
   |  +- CLHEPDictionaries/
   |  |  +- libart_Persistency_CLHEPDictionaries_dict.so          (NO)
   |  |  +- libart_Persistency_CLHEPDictionaries_map.so           (NO)
   |  +- Common/
   |  |  +- libart_Persistency_Common.so                          (YES)
   |  |  +- libart_Persistency_Common_dict.so                     (NO)
   |  |  +- libart_Persistency_Common_map.so                      (NO)
   |  +- FhiclCppDictionaries/
   |  |  +- libart_Persistency_FhiclCppDictionaries_dict.so       (NO)
   |  |  +- libart_Persistency_FhiclCppDictionaries_map.so        (NO)
   |  +- Provenance/
   |  |  +- libart_Persistency_Provenance.so                      (YES)
   |  |  +- libart_Persistency_Provenance_dict.so                 (NO)
   |  |  +- libart_Persistency_Provenance_map.so                  (NO)
   |  +- RootDB/
   |  |  +- libart_Persistency_RootDB.so                          (YES)
   |  +- StdDictionaries/
   |  |  +- libart_Persistency_StdDictionaries_dict.so            (NO)
   |  |  +- libart_Persistency_StdDictionaries_map.so             (NO)
   |  +- WrappedStdDictionaries/
   |     +- libart_Persistency_WrappedStdDictionaries_dict.so     (NO)
   |     +- libart_Persistency_WrappedStdDictionaries_map.so      (NO)
   +- Utilities/
   |  +- libart_Utilities.so                                      (YES)
   +- Version/
      +- libart_Version.so                                        (YES)
```

So, an estimated 72 products from around 520 source files

Notes
-----
- Add build product tree section above to track updates to available products
- Update sources to upstream art v1.11.3
- No dictionary libraries built yet
- Build of `art_Framework_Principal`
  - Discovered that this makes use of art/Persistency/Common/PtrVector.h
    which uses [ref-qualified](http://en.cppreference.com/w/cpp/language/member_functions) member functions. With GNU compilers, this is only
    supported with GCC 4.8 and higher.
- Build of `art_Framework_Services_Registry`
- Build of `art_Persistency_Common`
- Build of `art_Persistency_Provenance`
- Build of `art_Persistency_RootDB`
- Generate and install minimal CMake support files so that client projects
  can use Art via CMake's `find_package` command.
- Build of `art_Ntuple`
- Build of `art_Utilities`
- Build of `art_Version`
- FindXXX.cmake modules supplied for SQLite, TBB, and CPPUnit.

