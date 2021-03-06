#ifndef art_Persistency_Provenance_BranchIDListHelper_h
#define art_Persistency_Provenance_BranchIDListHelper_h
// vim: set sw=2:

#include "canvas/Persistency/Provenance/BranchIDList.h"
#include "canvas/Persistency/Provenance/BranchListIndex.h"
#include "canvas/Persistency/Provenance/ProductID.h"
#include "canvas/Persistency/Provenance/ProvenanceFwd.h"

#include <map>
#include <utility>

namespace art {

class MasterProductRegistry;

class BranchIDListHelper {
public:

  using IndexPair = std::pair<BranchListIndex, ProductIndex>;
  using BranchIDToIndexMap = std::map<BranchID, IndexPair>;

  static void updateFromInput(BranchIDLists const& file_bidlists,
                              std::string const& fileName);
  static void updateRegistries(MasterProductRegistry const& reg);
  static void clearRegistries();
  static void generate_branchIDToIndexMap();

  BranchIDToIndexMap const& branchIDToIndexMap() const
  {
    return branchIDToIndexMap_;
  }

private:
  BranchIDToIndexMap branchIDToIndexMap_;
};

} // namespace art

// Local Variables:
// mode: c++
// End:
#endif /* art_Persistency_Provenance_BranchIDListHelper_h */
