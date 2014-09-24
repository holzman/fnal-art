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
One way to topologically sort this is to use the [networkx](https://networkx.github.io) Python package (with support for graphviz):

```python
import networkx

art_graph = networkx.read_dot("./Dependencies.dot")
art_tsort = networkx.topological_sort(art_graph)
print(art_tsort)
```

The list returned by `networkx.topological_sort` is ordered
from most to least dependent. Note that all this means is that for
any edge `u->v` in the DAG (i.e. `u` depends on `v`), `u` appears
before `v` in the output list. Many other implementations of topological
sort are available, though `networkx` is one of the most widespread and
easy to use.

Using `networkx`, the topologically sorted list of art binaries is
as follows (together with their build status):

```
[yes] Framework_Services_Optional_TrivialFileTransfer_service
[yes] Utilities
[yes] Persistency_RootDB
[yes] Persistency_Provenance
[yes] Persistency_Common
[yes] Version
[yes] Framework_Services_Registry
[yes] Framework_Principal
[yes] Framework_Services_System_TriggerNamesService_service
[yes] Framework_Services_Optional_RandomNumberGenerator_service
[yes] Framework_Services_System_CurrentModule_service
[yes] Framework_Core
[yes] Framework_Services_UserInteraction
[yes] Framework_Services_Optional_SimpleInteraction_ervice
[yes] Framework_IO
[yes] Framework_Services_Optional_Tracer_service
[yes] Framework_Services_Optional
[yes] Framework_Services_Optional_TFileService_service
[yes] Framework_Services_FileServiceInterfaces
[yes] Framework_Services_Optional_Timing_service
[no]  Framework_IO_RootVersion
[yes] Framework_Services_Optional_TrivialFileDelivery_service
[no]  Framework_IO_Catalog
[no]  Framework_IO_Root
[no]  Framework_IO_Sources
[yes] Framework_Services_System_FileCatalogMetadata_service
[no]  Framework_IO_ProductMix
[no]  Framework_Services_System_PathSelection_service
[no]  Framework_Services_System_ScheduleContext_service
[yes] Framework_Services_System_FloatingPointControl_service
[no]  Framework_EventProcessor
[yes] Framework_Services_Optional_SimpleMemoryCheck_service
[no]  Framework_Art
[yes] Ntuple
```
