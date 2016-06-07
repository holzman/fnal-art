if (ALT_CMAKE)
include(altCMakeLists.cmake)
else(ALT_CMAKE)

art_make(SUBDIRS detail
  LIB_LIBRARIES
  art_Persistency_RootDB
  art_Utilities
  canvas_Persistency_Provenance
  canvas_Utilities
  ${Boost_THREAD_LIBRARY}
  MF_MessageLogger
	)

install_headers(SUBDIRS detail)
install_source(SUBDIRS detail)

endif(ALT_CMAKE)
