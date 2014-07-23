Art's Custom CMake Modules
==========================
Notes on Art's custom cmake modules to help in decoding what these do
(or more specifically, the functions and macros they provide).

Document this in a separate file to keep things in one place and not
to modify the core files. There will be some reference to the source
tree under art/ because it's from here that we'll find uses of the
functions and macros.

Inclusion Order/Dependencies
============================
Art use cetbuildtools, so often calls into that, with usage marked by
"CBT::"

- ArtDictionary.cmake
  - CBT::BuildDictionary
  - CheckClassVersion.cmake
- ArtMake.cmake
  - ArtDictionary.cmake
  - CBT::CetMake
  - CBT::CetParseArgs
  - CBT::InstallSource
- BuildPlugins.cmake
  - CBT::BasicPlugin.cmake
- CheckClassVersion.cmake
  - CBT::CetParseArgs.cmake

Will need to check for other usage of CBT/Cetpkgsupport modules (e.g.
"artmod" from cetpkgsupport.

Usage in Art Components
=======================
art/Version
-----------
Calls `art_make_library`, `install_headers` and `install_sources`.

art/Utilities
-------------
Calls `art_make`, `install_headers` and `install_sources`.


