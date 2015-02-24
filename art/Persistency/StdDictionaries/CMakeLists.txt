if (PURE_CMAKE) 
include(altCMakeLists.txt)
else()
# project name
project(art_Persistency_StdDictionaries)

art_dictionary()

install_source()
endif()
