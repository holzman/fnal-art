#include "art/Persistency/Provenance/EntryDescription.h"
#include "art/Persistency/Provenance/ModuleDescriptionRegistry.h"
#include "art/Persistency/Provenance/EntryDescriptionRegistry.h"
#include <ostream>
#include <sstream>

/*----------------------------------------------------------------------

----------------------------------------------------------------------*/

namespace art {
  EntryDescription::EntryDescription() :
    parents_(),
    moduleDescriptionID_()
  { }

  EntryDescriptionID
  EntryDescription::id() const
  {
    // This implementation is ripe for optimization.
    std::ostringstream oss;
    oss << moduleDescriptionID_ << ' ';
    for (std::vector<ProductID>::const_iterator
	   i = parents_.begin(),
	   e = parents_.end();
	 i != e;
	 ++i)
      {
	oss << *i << ' ';
      }

    std::string stringrep = oss.str();
    art::Digest md5alg(stringrep);
    return EntryDescriptionID(md5alg.digest().toString());
  }


  void
  EntryDescription::write(std::ostream& os) const {
    // This is grossly inadequate, but it is not critical for the
    // first pass.
    os << "Module Description ID = " << moduleDescriptionID() << '\n';
  }

  bool
  operator==(EntryDescription const& a, EntryDescription const& b) {
    return
      a.parents() == b.parents()
      && a.moduleDescriptionID() == b.moduleDescriptionID();
  }
}
