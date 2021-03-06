#ifndef art_Framework_Art_detail_get_MetadataSummary_h
#define art_Framework_Art_detail_get_MetadataSummary_h

#include "art/Framework/Art/detail/MetadataSummary.h"
#include "art/Utilities/PluginSuffixes.h"

#include <memory>

namespace art {
  namespace detail {

    std::unique_ptr<MetadataSummary> 
    get_MetadataSummary(suffix_type st,
                        LibraryInfoCollection const& coll);

  }
}

#endif /* art_Framework_Art_detail_get_MetadataSummary_h */

// Local variables:
// mode: c++
// End:
