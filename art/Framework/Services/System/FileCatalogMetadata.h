#ifndef art_Framework_Services_System_FileCatalogMetadata_h
#define art_Framework_Services_System_FileCatalogMetadata_h

#include "art/Framework/Services/Registry/ServiceMacros.h"
#include "art/Framework/Services/Registry/ServiceTable.h"
#include "art/Utilities/SAMMetadataTranslators.h"
#include "cetlib/canonical_string.h"
#include "cetlib/container_algorithms.h"
#include "fhiclcpp/types/Atom.h"
#include "fhiclcpp/types/OptionalAtom.h"
#include "fhiclcpp/types/Sequence.h"

#include <iterator>
#include <string>
#include <unordered_map>
#include <vector>

namespace art {
  class FileCatalogMetadata;
  class ActivityRegistry;
}

class art::FileCatalogMetadata {
public:
  using collection_type = std::vector<std::pair<std::string, std::string>>;
  using value_type = typename collection_type::value_type;

  struct Config {
    fhicl::Atom<bool> checkSyntax { fhicl::Name("checkSyntax"), false };
    fhicl::OptionalAtom<std::string> applicationFamily  { fhicl::Name("applicationFamily" ) };
    fhicl::OptionalAtom<std::string> applicationVersion { fhicl::Name("applicationVersion") };
    fhicl::OptionalAtom<std::string> group     { fhicl::Name("group") };
    fhicl::OptionalAtom<std::string> processID { fhicl::Name("processID") };

    fhicl::Sequence<std::string> metadataFromInput {
      fhicl::Name("metadataFromInput"),
      fhicl::Comment("This list specifies the metadata that is inherited\n"
                     "from the input file.  Currently only the run type and\n"
                     "file type metadata can be inherited.  The default list is empty."),
      std::vector<std::string>{}
    };

    bool inMetadataList(std::string const& name) const
    {
      return cet::search_all(metadataFromInput(), name);
    }

    fhicl::Atom<std::string> fileType  { fhicl::Name("fileType"),
        fhicl::Comment("Can specify 'fileType' only if it is not specified\n"
                       "in the 'metadataFromInput' list."),
        [this](){ return !inMetadataList("fileType"); },
        "unknown"
    };

    fhicl::OptionalAtom<std::string> runType   { fhicl::Name("runType"),
        fhicl::Comment("Can specify 'runType' only if it is not specified\n"
                       "in the 'metadataFromInput' list."),
        [this](){ return !inMetadataList("runType"); }
    };
  };

  using Parameters = ServiceTable<Config>;
  FileCatalogMetadata(Parameters const & config, ActivityRegistry &);

  // Add a new value to the metadata store.
  void addMetadata(std::string const & key, std::string const & value);
  // Ensure the value is a canonical string representation.
  void addMetadataString(std::string const & key, std::string const & value);

  void getMetadata(collection_type & coll) const; // Dump stored metadata into the provided container.

  // RootInput can set the run-type and file-type parameters
  void setMetadataFromInput(collection_type const& coll);
  bool inheritedFromInput(std::string const& name);

  // Ascertain whether JSON syntax checking is desired.
  bool wantCheckSyntax() const { return checkSyntax_; }

private:
  bool const checkSyntax_;
  collection_type md_;

  class InheritedMetadata {
  public:

    InheritedMetadata(std::vector<std::string> const& sortedMdToInherit,
                      collection_type const& coll)
    {
      NewToOld const translator;
      for(auto const& pr : coll) {
        if (cet::search_all(sortedMdToInherit, translator(pr.first))) {
          inputmd_.insert(pr);
          orderedmd_.emplace_back(pr);
        }
      }
    }

    auto const& entries() const { return orderedmd_; }

    void check_values(collection_type const& fromInput) const
    {
      using namespace std;
      using namespace cet::container_helpers;
      for (auto const& pr : fromInput) {
        auto it = inputmd_.find(pr.first);
        if ( it == cend(inputmd_) ) {
          throw Exception(errors::LogicError)
            << "Metadata key " << pr.first
            << " missing from list of metadata to inherit from input files.\n";
        }
        else if (it->second != pr.second) {
          throw Exception(errors::MismatchedInputFiles)
            << "The value for '" << pr.first << "' for the current file is: " << pr.second
            << ", which conflicts with the value from the first input file (\""
            << it->second << "\").\n";
        }
      }
    }

  private:
    collection_type orderedmd_;
    std::unordered_map<std::string, std::string> inputmd_;
  };

  std::unique_ptr<InheritedMetadata> imd_;
  std::vector<std::string> mdToInherit_;
};

inline
void
art::FileCatalogMetadata::
addMetadataString(std::string const & key, std::string const & value)
{
  addMetadata(key, cet::canonical_string(value));
}

inline
void
art::FileCatalogMetadata::
setMetadataFromInput(collection_type const& mdFromInput)
{
  if (mdToInherit_.empty()) return;

  if (!imd_)
    imd_ = std::make_unique<InheritedMetadata>(mdToInherit_, mdFromInput);
  else
    imd_->check_values(mdFromInput);

  OldToNew const translator;
  for (auto const& pr : imd_->entries()) {
    addMetadataString(translator(pr.first), pr.second);
  }
}

inline
void
art::FileCatalogMetadata::
getMetadata(collection_type & coll) const
{
  cet::copy_all(md_, std::back_inserter(coll));
}

DECLARE_ART_SERVICE(art::FileCatalogMetadata, LEGACY)
#endif /* art_Framework_Services_System_FileCatalogMetadata_h */

// Local Variables:
// mode: c++
// End:
