if (ALT_CMAKE)
include(altCMakeLists.cmake)
else(ALT_CMAKE)


# project name
project(art_Framework_Modules)

simple_plugin(BlockingPrescaler       "module")
simple_plugin(EmptyEvent              "source")
simple_plugin(FileDumperOutput        "module")
simple_plugin(Prescaler               "module")
simple_plugin(ProvenanceCheckerOutput "module")
simple_plugin(RandomNumberSaver       "module")

install_headers(SUBDIRS detail)
install_source(SUBDIRS detail)
endif(ALT_CMAKE)
