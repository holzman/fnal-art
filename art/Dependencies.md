Art product by-hand dependencies
================================
Art libraries have, by stated links in old `cetbuildtools` style scripts,
a fairly complex dependency (link) graph. As we'd like to get linking
roughly right from the start, there's a need for an "order of battle".
This is simply the topologically sorted list of links, so start with the
rough digraph from `art_make` and the like (ignoring, for now, externals)
This may or may not result in overlinking, but at minimum we want complete
links so that an `ldd -u -r <binary>` on Linux results in no missing symbols.

Once CMake-ificiation is completed, then CMake's graphviz tool will be
able to help with this...

The file [Dependencies.dot](Dependencies.dot) holds the Digraph of the
art dependencies in the [Graphviz](http://www.graphviz.org) DOT language.
When topologically sorted, the sequence of most to least dependent
binaries is:

```
Framework_Art
Framework_EventProcessor
Framework_Services_System_FloatingPointControl_service
Framework_Services_System_PathSelection_service
Framework_Services_System_FileCatalogMetadata_service
Framework_IO_Root
Framework_IO_Catalog
Framework_Services_Optional_TrivialFileTransfer_service
Framework_Services_Optional_TrivialFileDelivery_service
Framework_IO_RootVersion
Framework_Services_System_ScheduleContext_service
Framework_Core
Framework_Services_Optional_RandomNumberGenerator_service
Framework_Services_System_TriggerNamesService_service
Framework_IO
Framework_Services_System_CurrentModule_service
Framework_Principal
Version
Framework_Services_Registry
Persistency_Common
Persistency_Provenance
Persistency_RootDB
Utilities
```
