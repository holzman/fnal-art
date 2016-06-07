if (ALT_CMAKE)
include(altCMakeLists.cmake)
else(ALT_CMAKE)


# build art_Framework_Services libraries
add_subdirectory (FileServiceInterfaces)
add_subdirectory (Optional)
add_subdirectory (Registry)
add_subdirectory (System)
add_subdirectory (UserInteraction)


endif(ALT_CMAKE)
