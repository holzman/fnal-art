if (PURE_CMAKE) 
include(altCMakeLists.txt)
else()
art_dictionary()

install_source()
endif()
