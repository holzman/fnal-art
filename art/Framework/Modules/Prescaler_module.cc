// ======================================================================
//
// Prescaler_plugin
//
// ======================================================================


#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/Frameworkfwd.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "fhiclcpp/types/Atom.h"

namespace art {
  class Prescaler;
}
using namespace fhicl;
using art::Prescaler;

// ======================================================================

class art::Prescaler
  : public EDFilter
{
public:

  struct Config {
    Atom<int> prescaleFactor { Name("prescaleFactor") };
    Atom<int> prescaleOffset { Name("prescaleOffset") };
  };

  using Parameters = EDFilter::Table<Config>;
  explicit Prescaler( Parameters const & );

  bool filter( Event & ) override;
  void endJob() override;

private:
  int count_;
  int n_;      // accept one in n
  int offset_; // with offset,
               // i.e. sequence of events does not have to start at first event

};  // Prescaler

// ======================================================================

Prescaler::Prescaler( Parameters const & config )
  : EDFilter( )
  , count_ ( 0 )
  , n_     ( config().prescaleFactor() )
  , offset_( config().prescaleOffset() )
{ }

bool
  Prescaler::filter( Event & /* unused */ )
{
  return ++count_ % n_ == offset_;
}

void
  Prescaler::endJob()
{ }

// ======================================================================

DEFINE_ART_MODULE(Prescaler)

// ======================================================================
