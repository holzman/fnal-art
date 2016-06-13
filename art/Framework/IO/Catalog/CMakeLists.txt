if (ALT_CMAKE)
include(altCMakeLists.cmake)
else(ALT_CMAKE)


set(art_Framework_IO_Catalog_sources
  InputFileCatalog.cc
  )

art_make_library( LIBRARY_NAME art_Framework_IO_Catalog
                  SOURCE ${art_Framework_IO_Catalog_sources}
                  LIBRARIES art_Framework_Services_Optional_TrivialFileDelivery_service
  art_Framework_Services_Optional_TrivialFileTransfer_service
  art_Utilities canvas_Utilities
  ${Boost_FILESYSTEM_LIBRARY}
)

install_headers()
install_source()

endif(ALT_CMAKE)
