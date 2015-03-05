
# add_definitions(-DART_NO_MIX_PTRVECTOR)

# Make sure tests have correct environment settings.
include(CetTest)
# If you explicitly include CetTest in a subdirectory, you will need to
# re-initialize the test environment.
cet_test_env("FHICL_FILE_PATH=.")

# - Need these cause art doesn't know about itself
cet_test_env("PATH=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}:$ENV{PATH}")
if(APPLE)
  cet_test_env("DYLD_LIBRARY_PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}:$ENV{DYLD_LIBRARY_PATH}")
elseif(UNIX)
  cet_test_env("LD_LIBRARY_PATH=${CMAKE_LIBRARY_OUTPUT_DIRECTORY}:$ENV{LD_LIBRARY_PATH}")
endif()

# build Persistency libraries


add_subdirectory(Framework/Art)
add_subdirectory(Framework/Core)

# NEEDS FIX FOT STATEMACHINCE.sh
add_subdirectory(Framework/EventProcessor)

add_subdirectory(Framework/IO)
#add_subdirectory(Framework/IO/Root)
#add_subdirectory(Framework/Principal)

# New 'detail' subdir under this with what look like extra test
# tools for clients of art to use...
add_subdirectory(Framework/Services/Optional)
add_subdirectory(Framework/Services/Registry)
add_subdirectory(Framework/Services/System)
#add_subdirectory(Integration)
add_subdirectory(Persistency/Provenance)
add_subdirectory(Persistency/RootDB)
add_subdirectory(TestObjects)
add_subdirectory(Version)
add_subdirectory(Utilities)
add_subdirectory(tbb)
add_subdirectory(Ntuple)

