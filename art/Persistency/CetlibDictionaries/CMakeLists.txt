if (PURE_CMAKE) 
include(altCMakeLists.txt)
else()
# project name
project(art_Persistency_CetlibDictionaries)

art_dictionary(DICTIONARY_LIBRARIES cetlib)

install_source()
endif()
