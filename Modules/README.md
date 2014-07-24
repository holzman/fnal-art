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
"artmod" from cetpkgsupport (that is simple code generation, so may not
be critical, though should be in art itself!!).

Usage in Art Components
=======================
art/Version
-----------
Calls `art_make_library`, `install_headers` and `install_source`.

art/Utilities
-------------
Calls `art_make`, `install_headers` and `install_source`.

Location of CMake Functions/Macros
==================================
- `install_headers` : `CBT::InstallSource.cmake`
  - Globbing with hardcoding, relies of directory structure to function,
    nothing more than wrapper around `install(FILES ...)`.
- `install_sources` : `CBT::InstallSource.cmake`
  - Basically performs the task that a basic CPack setup should do.
    May install generated sources as well, but no real need to do
    this.
- `art_make_library` : `ArtMake.cmake`
  - Basically a wrapper around `cet_make_library`.
  - Can supply a name for the library, otherwise derived from current
    directory path from cmake (not project!) source dir.
  - Has to be supplied with source code list
  - Can supply a variable that gets set to the library name in the calling
    scope. Does not appear to be used within art.
  - Calls `cet_make_library` with the library name, source list,
    LIBRARIES list (if it was supplied) plus all other arguments.
- `art_make` : `ArtMake.cmake`
  - Basically a wrapper around `art_make_library`, `_art_simple_plugin`
    and `art_dictionary`.
  - Begins by globbing for library/plugin sources...
    - Appears that plugin sources must be named `<something>_<type>.cc`
      where `<type>` is `source`, `module` or `service` (possibly others,
      as list is appended via some unclear calculations).
    - Several globs over everything, then more specific patterns, then
      subdirectories.
    - Then filters out specific from general...
    - Also has an `EXCLUDES` argument to filter out user specified.
    - Almost certainly easier and less error prone to move to explicit
      listing...
  - Some processing of file lists occurs, but seems overly complex
    - Appears that a nested loop is used when a simple list set/copy
      would suffice (see `art_file_list` and `art_make_library_src`
      variables).
  - Need to build library indicated by non-empty `art_file_list` and
    `art_make_library_src`
    - Calls down to `art_make_library` with source list being the
      `art_make_library_src` variable, which appears equal to the
      `art_file_list` variable.
  - Plugin build activated if `NO_PLUGINS` is false and the `plugin_files`
    list is non empty.
    - Each file has the plugin type extracted and then passed to
      `_art_simple_plugin` with the type and and type specific libs.
 - Check for dictionary performed by globbing for `classes.h` and
   `classes_def.xml` files.
   - If these exist, it appends them to `art_file_list`
   - If a library was built, it's appended to `art_make_dict_libraries`,
     followed by any further `DICT_LIBRARIES`.
   - A call is then made to `art_dictionary`, with
     `art_make_dict_libraries` if it exists.
- `cet_make_library` : `CBT::CetMake.cmake`
  - Basically a wrapper around `add_library/target_link_libraries/install`
  - Plus a flag to handle NO_INSTALL (i.e. alocal/test library)
  - Plus a flag to handle adding a static library as well.
  - Takes a source list, so no globbing.
  - It uses some command `find_tbb_offloads`. Appears that this checks
    sources for something, then modifies LINK_FLAGS property of target
    if check suceeds. Cannot find command to check what it does though...
  - Some odd processing of the LIBRARIES list (what it gets linked to).
    All libs are added, but some are converted to upper case names. Not
    at all clear why this is done.
  - Calls `cet_add_to_library_list` with library name as argument.
    That just adds the name to an internally cached list variable.
  - If installed, destination is set to "product" based paths.


