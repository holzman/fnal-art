if (PURE_CMAKE) 
include(altCMakeLists.txt)
else()
art_dictionary(DICTIONARY_LIBRARIES ${CLHEP})

install_source()
endif()
