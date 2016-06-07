set(art_UTILITIES_HEADERS
BasicHelperMacros.h
BasicPluginMacros.h
ConfigTable.h
ensureTable.h
ExceptionMessages.h
FirstAbsoluteOrLookupWithDotPolicy.h
fwd.h
GetReleaseVersion.h
HRRealTime.h
JobMode.h
MallocOpts.h
OutputFileInfo.h
parent_path.h
PluginSuffixes.h
pointersEqual.h
RegexMatch.h
RootHandlers.h
SAMMetadataTranslators.h
ScheduleID.h
ThreadSafeIndexedRegistry.h
unique_filename.h
UnixSignalHandlers.h
Verbosity.h
  )

set(art_UTILITIES_DETAIL_HEADERS
detail/math_private.h
detail/serviceConfigLocation.h  
  )

set(art_UTILITIES_SOURCES
ensureTable.cc
ExceptionMessages.cc
FirstAbsoluteOrLookupWithDotPolicy.cc
MallocOpts.cc
parent_path.cc
PluginSuffixes.cc
RegexMatch.cc
RootHandlers.cc
unique_filename.cc
UnixSignalHandlers.cc
detail/serviceConfigLocation.cc
  )


add_library(art_Utilities
  SHARED
  ${art_UTILITIES_SOURCES}
  )

set_target_properties(art_Utilities
  PROPERTIES
    VERSION ${art_VERSION}
    SOVERSION ${art_SOVERSION}
  )

# No usage requirements for Root dirs, so add them here,
# but due to Root's broken CMake config file, this won't
# work directly.
target_include_directories(art_Utilities
  PRIVATE ${ROOT_INCLUDE_DIRS}
  )

target_link_libraries(art_Utilities
  LINK_PUBLIC
   cetlib::cetlib
   fhiclcpp::fhiclcpp
   messagefacility::MF_MessageLogger
   canvas::canvas_Utilities
   ${Boost_FILESYSTEM_LIBRARY}
   ${Boost_SYSTEM_LIBRARY}
  LINK_PRIVATE
   ${ROOT_Reflex_LIBRARY}
  )

install(TARGETS art_Utilities
  EXPORT ArtLibraries
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Runtime
  )
install(FILES ${art_UTILITIES_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Utilities
  COMPONENT Development
  )
install(FILES ${art_UTILITIES_DETAIL_HEADERS}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/art/Utilities/detail
  COMPONENT Development
  )

