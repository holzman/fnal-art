#ifndef art_Framework_Principal_fwd_h
#define art_Framework_Principal_fwd_h

// For enums only.
#include "art/Framework/Principal/BranchActionType.h"
#include "art/Framework/Principal/ActionCodes.h"

namespace art {

  class ActionTable; // Action.h
  class CurrentProcessingContext;
  class DataViewImpl;
  class Event;
  class EventPrincipal;
  class NoDelayedReader;
  class Principal;
  class Run;
  class RunPrincipal;
  class ProcessNameSelector; // Selector.h
  class ProductInstanceNameSelector; // Selector.h
  class ModuleLabelSelector; // Selector.h
  class MatchAllSelector; // Selector.h
  template <class A, class B> class AndHelper; // Selector.h
  template <class A, class B> class OrHelper; // Selector.h
  template <class A> class NotHelper; // Selector.h
  template <class T> class ComposedSelectorWrapper; // Selector.h
  class RunStopwatch;
  class Selector;
  class SelectorBase;
  class SubRun;
  class SubRunPrincipal;
  template <class T> class View;
  class Worker;
  class WorkerParams;
}  // art

#endif /* art_Framework_Principal_fwd_h */

// Local Variables:
// mode: c++
// End: