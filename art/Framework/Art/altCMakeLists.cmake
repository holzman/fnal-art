# - Build art_Framework_Art and main applications

# - check_libs app
configure_file(check_libs.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/check_libs.cc @ONLY
  )
add_executable(check_libs ${CMAKE_CURRENT_BINARY_DIR}/check_libs.cc)
target_link_libraries(check_libs
  art_Utilities
  cetlib::cetlib fhiclcpp::fhiclcpp
  )

# - art_Framework_Art library
# Configure for desired default exception handling.
set(ART_MAIN_FUNC artapp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/artapp.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/${ART_MAIN_FUNC}.h @ONLY
  )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/artapp.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${ART_MAIN_FUNC}.cc @ONLY
  )

set(ART_MAIN_FUNC mu2eapp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/artapp.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/${ART_MAIN_FUNC}.h @ONLY
  )

set(ART_RETHROW_DEFAULT TRUE)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/artapp.cc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${ART_MAIN_FUNC}.cc @ONLY
  )

set(art_Framework_Art_HEADERS
BasicOutputOptionsHandler.h
OptionsHandler.h
OptionsHandlers.h
InitRootHandlers.h
DebugOptionsHandler.h
BasicPostProcessor.h
find_config.h
BasicOptionsHandler.h
BasicSourceOptionsHandler.h
run_art.h
FileCatalogOptionsHandler.h
${CMAKE_CURRENT_BINARY_DIR}/artapp.h
${CMAKE_CURRENT_BINARY_DIR}/mu2eapp.h
)

set(art_Framework_Art_detail_HEADERS
detail/get_LibraryInfoCollection.h
detail/DebugOutput.h
detail/PluginSymbolResolvers.h
detail/PluginMetadata.h
detail/PrintPluginMetadata.h
detail/MetadataRegexHelpers.h
detail/ServiceNames.h
detail/fillSourceList.h
detail/get_MetadataSummary.h
detail/LibraryInfo.h
detail/get_MetadataCollector.h
detail/MetadataSummary.h
detail/exists_outside_prolog.h
detail/LibraryInfoCollection.h
detail/MetadataCollector.h
detail/PrintFormatting.h
)

set(art_Framework_Art_detail_md-summary_HEADERS
detail/md-summary/MetadataSummaryForPlugin.h
detail/md-summary/MetadataSummaryForService.h
detail/md-summary/MetadataSummaryForModule.h
detail/md-summary/MetadataSummaryForSource.h
)

set(art_Framework_Art_detail_md-collector_HEADERS
detail/md-collector/MetadataCollectorForService.h
detail/md-collector/MetadataCollectorForModule.h
detail/md-collector/MetadataCollectorForPlugin.h
detail/md-collector/MetadataCollectorForSource.h
)


add_library(art_Framework_Art SHARED
  BasicOptionsHandler.cc
  BasicPostProcessor.cc
  BasicSourceOptionsHandler.cc
  BasicOutputOptionsHandler.cc
  DebugOptionsHandler.cc
  FileCatalogOptionsHandler.cc
  InitRootHandlers.cc
  OptionsHandler.cc
  ${CMAKE_CURRENT_BINARY_DIR}/artapp.cc
  ${CMAKE_CURRENT_BINARY_DIR}/mu2eapp.cc
  find_config.cc
  run_art.cc
  detail/MetadataRegexHelpers.cc
  detail/PrintPluginMetadata.cc
  detail/ServiceNames.cc
  detail/fillSourceList.cc
  detail/get_LibraryInfoCollection.cc
  detail/get_MetadataCollector.cc
  detail/get_MetadataSummary.cc
  )

# Describe library link interface - all Public for now
target_link_libraries(art_Framework_Art
  ${Boost_PROGRAM_OPTIONS_LIBRARY}
  art_Framework_IO_Root
  art_Framework_EventProcessor
  art_Framework_Core
  art_Framework_Services_Registry
  art_Persistency_Common
  art_Persistency_Provenance
  canvas::canvas_Persistency_Provenance
  art_Utilities
  canvas::canvas_Utilities
  ${ROOT_Hist_LIBRARY}
  ${ROOT_Matrix_LIBRARY}
  )

# Set any additional properties
set_target_properties(art_Framework_Art
  PROPERTIES
   VERSION ${art_VERSION}
   SOVERSION ${art_SOVERSION}
  )

set (ART_EXEC_ROOT_LIBS
  ${ROOT_CINTEX_LIBRARY}
  ${ROOT_PHYSICS_LIBRARY}
  ${ROOTSYS}/lib/libGraf.so
  ${ROOT_TREE_LIBRARY}
  ${ROOT_HIST_LIBRARY}
  ${ROOT_MATRIX_LIBRARY}
  ${ROOT_NET_LIBRARY}
  ${ROOT_MATHCORE_LIBRARY}
  ${ROOT_RIO_LIBRARY}
  ${ROOT_THREAD_LIBRARY}
  ${ROOT_CORE_LIBRARY}
  ${ROOT_CINT_LIBRARY}
  ${ROOT_REFLEX_LIBRARY}
)


# Build an art exec.
macro(art_exec TARGET_STEM IN_STEM MAIN_FUNC)
  set(ART_MAIN_FUNC ${MAIN_FUNC})
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${IN_STEM}.cc.in
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_STEM}.cc @ONLY
    )
  add_executable(${TARGET_STEM}
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_STEM}.cc
    )
  target_link_libraries(${TARGET_STEM}
    art_Framework_Art
    # ROOT libraries added for use convenience ONLY -- they will go away
    # eventually.
    ${ART_EXEC_ROOT_LIBS}
    messagefacility::MF_MessageLogger
    )
endmacro()

# Standard execs
art_exec(art art artapp)
art_exec(gm2 art artapp)
art_exec(lar art artapp)
art_exec(mu2e art mu2eapp)
art_exec(nova art artapp)

# Execs with Boost unit testing enabled for modules.
art_exec(art_ut art_ut artapp LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG})
art_exec(gm2_ut art_ut artapp LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG})
art_exec(lar_ut art_ut artapp LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG})
art_exec(mu2e_ut art_ut mu2eapp LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG})
art_exec(nova_ut art_ut artapp LIBRARIES ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY_DEBUG})

# - Install the targets
install(TARGETS
  check_libs
  art_Framework_Art
  art
  art_ut
  mu2e
  mu2e_ut
  lar
  lar_ut
  gm2
  gm2_ut
  nova
  nova_ut
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )

install(FILES ${art_Framework_Art_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Art
  COMPONENT Development
  )

install(FILES ${art_Framework_Art_detail_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Art/detail
  COMPONENT Development
  )

install(FILES ${art_Framework_Art_detail_md-collector_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Art/detail/md-collector
  COMPONENT Development
  )

install(FILES ${art_Framework_Art_detail_md-summary_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Framework/Art/detail/md-summary
  COMPONENT Development
  )
