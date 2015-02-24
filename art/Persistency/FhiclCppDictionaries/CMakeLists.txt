if (PURE_CMAKE) 
include(altCMakeLists.txt)
else()
# project name
project(art_Persistency_FhiclCppDictionaries)

art_dictionary(DICTIONARY_LIBRARIES fhiclcpp)

install_source()
endif()
