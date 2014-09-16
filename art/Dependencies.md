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

```sh
digraph art {
  Framework_Art -> Framework_IO_Root;
  Framework_Art -> Framework_EventProcessor;
  Framework_Art -> Framework_Core;
  Framework_Art -> Framework_Services_Registry;
  Framework_Art -> Persistency_Common;
  Framework_Art -> Persistency_Provenance;
  Framework_Art -> Utilities;

  Framework_IO_Root -> Framework_Core;
  Framework_IO_Root -> Framework_IO;
  Framework_IO_Root -> Framework_IO_Catalog;
  Framework_IO_Root -> Framework_Principal;
  Framework_IO_Root -> Framework_Services_Registry;
  Framework_IO_Root -> Persistency_Common;
  Framework_IO_Root -> Persistency_Provenance;
  Framework_IO_Root -> Framework_IO_RootVersion;

  Framework_EventProcessor -> Framework_Services_System_CurrentModule_service;
  Framework_EventProcessor -> Framework_Services_System_FileCatalogMetadata_service;
  Framework_EventProcessor -> Framework_Services_System_FloatingPointControl_service;
  Framework_EventProcessor -> Framework_Services_System_PathSelection_service;
  Framework_EventProcessor -> Framework_Services_System_ScheduleContext_service;
  Framework_EventProcessor -> Framework_Services_System_TriggerNamesService_service;
  Framework_EventProcessor -> Framework_Core;
  Framework_EventProcessor -> Utilities;

  Framework_Core -> Framework_Services_System_CurrentModule_service;
  Framework_Core -> Framework_Services_System_TriggerNamesService_service;
  Framework_Core -> Framework_Services_Optional_RandomNumberGenerator_service;
  Framework_Core -> Framework_Principal;
  Framework_Core -> Persistency_Common;
  Framework_Core -> Persistency_Provenance;
  Framework_Core -> Framework_Services_Registry;
  Framework_Core -> Utilities;
  Framework_Core -> Version;

  Framework_Services_Registry -> Utilities;

  Persistency_Common -> Persistency_Provenance;
  Persistency_Common -> Utilities;

  Persistency_Provenance -> Persistency_RootDB;
  Persistency_Provenance -> Utilities;

  Persistency_RootDB -> Utilities;

  Utilities;

  Version;

  Framework_IO -> Persistency_Provenance;

  Framework_IO_Catalog -> Framework_Services_Optional_TrivialFileDelivery_service;
  Framework_IO_Catalog -> Framework_Services_Optional_TrivialFileTransfer_service;
  Framework_IO_Catalog -> Utilities;

  Framework_Principal -> Persistency_Provenance;
  Framework_Principal -> Persistency_Common;
  Framework_Principal -> Framework_Services_Registry;
  Framework_Principal -> Utilities;
  Framework_Principal -> Version;

  Framework_IO_RootVersion;

  Framework_Services_System_CurrentModule_service -> Persistency_Provenance;

  Framework_Services_System_FileCatalogMetadata_service;

  Framework_Services_System_FloatingPointControl_service;

  Framework_Services_System_PathSelection_service -> Framework_Core;

  Framework_Services_System_ScheduleContext_service -> Framework_Core;

  Framework_Services_System_TriggerNamesService_service;

  Framework_Services_Optional_RandomNumberGenerator_service -> Framework_Principal;
  Framework_Services_Optional_RandomNumberGenerator_service -> Persistency_Common;

  Framework_Services_Optional_TrivialFileDelivery_service;

  Framework_Services_Optional_TrivialFileTransfer_service;
}
```
