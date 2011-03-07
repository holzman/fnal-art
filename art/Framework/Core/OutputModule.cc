#include "art/Framework/Core/OutputModule.h"

#include "art/Framework/Core/CPCSentry.h"
#include "art/Framework/Services/System/ConstProductRegistry.h"
#include "art/Framework/Core/CurrentProcessingContext.h"
#include "art/Framework/Core/Event.h"
#include "art/Framework/Core/EventPrincipal.h"
#include "art/Framework/Services/System/TriggerNamesService.h"
#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "art/Persistency/Common/Handle.h"
#include "art/Persistency/Provenance/BranchDescription.h"
#include "art/Persistency/Provenance/ParentageRegistry.h"
#include "art/Utilities/DebugMacros.h"


using fhicl::ParameterSet;
using std::vector;
using std::string;


namespace art {
  // This grotesque little function exists just to allow calling of
  // ConstProductRegistry::allBranchDescriptions in the context of
  // OutputModule's initialization list, rather than in the body of
  // the constructor.

  vector<art::BranchDescription const*>
  getAllBranchDescriptions() {
    art::ServiceHandle<art::ConstProductRegistry> reg;
    return reg->allBranchDescriptions();
  }

  vector<std::string> const& getAllTriggerNames() {
    art::ServiceHandle<art::TriggerNamesService> tns;
    return tns->getTrigPaths();
  }
}


namespace {

  //--------------------------------------------------------
  // Remove whitespace (spaces and tabs) from a string.
  void remove_whitespace(std::string& s) {
    s.erase(remove(s.begin(), s.end(), ' '), s.end());
    s.erase(remove(s.begin(), s.end(), '\t'), s.end());
  }

  void test_remove_whitespace() {
    string a("noblanks");
    string b("\t   no   blanks    \t");

    remove_whitespace(b);
    assert(a == b);
  }

  //--------------------------------------------------------
  // Given a path-spec (string of the form "a:b", where the ":b" is
  // optional), return a parsed_path_spec_t containing "a" and "b".

  typedef std::pair<string,string> parsed_path_spec_t;
  void parse_path_spec(std::string const& path_spec,
                       parsed_path_spec_t& output) {
    string trimmed_path_spec(path_spec);
    remove_whitespace(trimmed_path_spec);

    string::size_type colon = trimmed_path_spec.find(":");
    if (colon == string::npos) {
        output.first = trimmed_path_spec;
    } else {
        output.first  = trimmed_path_spec.substr(0, colon);
        output.second = trimmed_path_spec.substr(colon+1,
                                                 trimmed_path_spec.size());
    }
  }

  void test_parse_path_spec() {
    vector<std::string> paths;
    paths.push_back("a:p1");
    paths.push_back("b:p2");
    paths.push_back("  c");
    paths.push_back("ddd\t:p3");
    paths.push_back("eee:  p4  ");

    vector<parsed_path_spec_t> parsed(paths.size());
    for (size_t i = 0; i < paths.size(); ++i)
      parse_path_spec(paths[i], parsed[i]);

    assert(parsed[0].first  == "a");
    assert(parsed[0].second == "p1");
    assert(parsed[1].first  == "b");
    assert(parsed[1].second == "p2");
    assert(parsed[2].first  == "c");
    assert(parsed[2].second == "");
    assert(parsed[3].first  == "ddd");
    assert(parsed[3].second == "p3");
    assert(parsed[4].first  == "eee");
    assert(parsed[4].second == "p4");
  }
}


namespace art {

  namespace test {

    void run_all_output_module_tests() {
      test_remove_whitespace();
      test_parse_path_spec();
    }

  }  // namespace


  // -------------------------------------------------------
  OutputModule::OutputModule(ParameterSet const& pset) :
    maxEvents_(-1),
    remainingEvents_(maxEvents_),
    keptProducts_(),
    hasNewlyDroppedBranch_(),
    process_name_(),
    groupSelectorRules_(pset, "outputCommands", "OutputModule"),
    groupSelector_(),
    moduleDescription_(),
    current_context_(0),
    prodsValid_(false),
    wantAllEvents_(false),
    selectors_(),
    selector_config_id_(),
    branchParents_(),
    branchChildren_()
  {
    hasNewlyDroppedBranch_.assign(false);

    art::ServiceHandle<art::TriggerNamesService> tns;
    process_name_ = tns->getProcessName();

    ParameterSet selectevents =
      pset.get<fhicl::ParameterSet>("SelectEvents", ParameterSet());

    selector_config_id_ = selectevents.id();
    // If selectevents is an emtpy ParameterSet, then we are to write
    // all events, or one which contains a vstrig 'SelectEvents' that
    // is empty, we are to write all events. We have no need for any
    // EventSelectors.
    if (selectevents.is_empty()) {
        wantAllEvents_ = true;
        selectors_.setupDefault(getAllTriggerNames());
        return;
    }

    vector<std::string> path_specs =
      selectevents.get<std::vector<std::string> >("SelectEvents");

    if (path_specs.empty()) {
        wantAllEvents_ = true;
        selectors_.setupDefault(getAllTriggerNames());
        return;
    }

    // If we get here, we have the possibility of having to deal with
    // path_specs that look at more than one process.
    vector<parsed_path_spec_t> parsed_paths(path_specs.size());
    for (size_t i = 0; i < path_specs.size(); ++i)
      parse_path_spec(path_specs[i], parsed_paths[i]);

    selectors_.setup(parsed_paths, getAllTriggerNames(), process_name_);
  }

  void OutputModule::configure(OutputModuleDescription const& desc) {
    remainingEvents_ = maxEvents_ = desc.maxEvents_;
  }

  void OutputModule::selectProducts() {
    if (groupSelector_.initialized()) return;
    groupSelector_.initialize(groupSelectorRules_, getAllBranchDescriptions());
    ServiceHandle<ConstProductRegistry> reg;

    // TODO: See if we can collapse keptProducts_ and groupSelector_ into a
    // single object. See the notes in the header for GroupSelector
    // for more information.

    ProductRegistry::ProductList::const_iterator it  =
      reg->productList().begin();
    ProductRegistry::ProductList::const_iterator end =
      reg->productList().end();

    for (; it != end; ++it) {
      BranchDescription const& desc = it->second;
      if(desc.transient()) {
        // if the class of the branch is marked transient, output nothing
      } else if(!desc.present() && !desc.produced()) {
        // else if the branch containing the product has been previously dropped,
        // output nothing
      } else if (selected(desc)) {
        // else if the branch has been selected, put it in the list of selected branches
        keptProducts_[desc.branchType()].push_back(&desc);
      } else {
        // otherwise, output nothing,
        // and mark the fact that there is a newly dropped branch of this type.
        hasNewlyDroppedBranch_[desc.branchType()] = true;
      }
    }
  }

  OutputModule::~OutputModule() { }

  void 
  OutputModule::reconfigure(ParameterSet const&) {
     mf::LogError("FeatureNotImplemented")
        << "This module is not reconfigurable."
        << "\n";
   }

  void OutputModule::doBeginJob() {
    selectProducts();
    beginJob();
  }

  void OutputModule::doEndJob() {
    endJob();
  }


  Trig OutputModule::getTriggerResults(Event const& ev) const {
    return selectors_.getOneTriggerResults(ev);
  }

  Trig OutputModule::getTriggerResults(EventPrincipal const& ep) const {
    // This is bad, because we're returning handles into an Event that
    // is destructed before the return. It might not fail, because the
    // actual EventPrincipal is not destroyed, but it still needs to
    // be cleaned up.
    Event ev(const_cast<EventPrincipal&>(ep),
             *current_context_->moduleDescription());
    return getTriggerResults(ev);
  }

   namespace {
     class  PVSentry {
     public:
       PVSentry (detail::CachedProducts& prods, bool& valid) : p(prods), v(valid) {}
       ~PVSentry() { p.clear(); v=false; }
     private:
       detail::CachedProducts& p;
       bool&           v;

       PVSentry(PVSentry const&);  // not implemented
       PVSentry& operator=(PVSentry const&); // not implemented
     };
   }

  bool
  OutputModule::doEvent(EventPrincipal const& ep,
                        CurrentProcessingContext const* cpc) {
    detail::CPCSentry sentry(current_context_, cpc);
    PVSentry          products_sentry(selectors_, prodsValid_);

    FDEBUG(2) << "writeEvent called\n";

    // This ugly little bit is here to prevent making the Event
    // if we don't need it.
    if (!wantAllEvents_) {
      // use module description and const_cast unless interface to
      // event is changed to just take a const EventPrincipal
      Event e(const_cast<EventPrincipal &>(ep), moduleDescription_);
      if (!selectors_.wantEvent(e)) {
        return true;
      }
    }
    write(ep);
    updateBranchParents(ep);
    if (remainingEvents_ > 0) {
      --remainingEvents_;
    }
    return true;
  }

  bool
  OutputModule::doBeginRun(RunPrincipal const& rp,
                                CurrentProcessingContext const* cpc) {
    detail::CPCSentry sentry(current_context_, cpc);
    FDEBUG(2) << "beginRun called\n";
    beginRun(rp);
    return true;
  }

  bool
  OutputModule::doEndRun(RunPrincipal const& rp,
                              CurrentProcessingContext const* cpc) {
    detail::CPCSentry sentry(current_context_, cpc);
    FDEBUG(2) << "endRun called\n";
    endRun(rp);
    return true;
  }

  void
  OutputModule::doWriteRun(RunPrincipal const& rp) {
    FDEBUG(2) << "writeRun called\n";
    writeRun(rp);
  }

  bool
  OutputModule::doBeginSubRun(SubRunPrincipal const& srp,
                                            CurrentProcessingContext const* cpc) {
    detail::CPCSentry sentry(current_context_, cpc);
    FDEBUG(2) << "beginSubRun called\n";
    beginSubRun(srp);
    return true;
  }

  bool
  OutputModule::doEndSubRun(SubRunPrincipal const& srp,
                                          CurrentProcessingContext const* cpc) {
    detail::CPCSentry sentry(current_context_, cpc);
    FDEBUG(2) << "endSubRun called\n";
    endSubRun(srp);
    return true;
  }

  void OutputModule::doWriteSubRun(SubRunPrincipal const& srp) {
    FDEBUG(2) << "writeSubRun called\n";
    writeSubRun(srp);
  }

  void OutputModule::doOpenFile(FileBlock const& fb) {
    openFile(fb);
  }

  void OutputModule::doRespondToOpenInputFile(FileBlock const& fb) {
    respondToOpenInputFile(fb);
  }

  void OutputModule::doRespondToCloseInputFile(FileBlock const& fb) {
    respondToCloseInputFile(fb);
  }

  void OutputModule::doRespondToOpenOutputFiles(FileBlock const& fb) {
    respondToOpenOutputFiles(fb);
  }

  void OutputModule::doRespondToCloseOutputFiles(FileBlock const& fb) {
    respondToCloseOutputFiles(fb);
  }

  void OutputModule::maybeOpenFile() {
    if (!isFileOpen()) doOpenFile();
  }

  void OutputModule::doCloseFile() {
    if (isFileOpen()) reallyCloseFile();
  }

  void OutputModule::reallyCloseFile() {
    fillDependencyGraph();
    startEndFile();
    writeFileFormatVersion();
    writeFileIdentifier();
    writeFileIndex();
    writeEventHistory();
    writeProcessConfigurationRegistry();
    writeProcessHistoryRegistry();
    writeParameterSetRegistry();
    writeProductDescriptionRegistry();
    writeParentageRegistry();
    writeBranchIDListRegistry();
    writeProductDependencies();
    writeBranchMapper();
    finishEndFile();
    branchParents_.clear();
    branchChildren_.clear();
  }

  CurrentProcessingContext const*
  OutputModule::currentContext() const {
    return current_context_;
  }

  ModuleDescription const&
  OutputModule::description() const {
    return moduleDescription_;
  }

  bool OutputModule::selected(BranchDescription const& desc) const {
    return groupSelector_.selected(desc);
  }

  void
  OutputModule::updateBranchParents(EventPrincipal const& ep) {
    for (EventPrincipal::const_iterator i = ep.begin(), iEnd = ep.end(); i != iEnd; ++i) {
      if (i->second->productProvenancePtr() != 0) {
        BranchID const& bid = i->first;
        BranchParents::iterator it = branchParents_.find(bid);
        if (it == branchParents_.end()) {
           it = branchParents_.insert(std::make_pair(bid, std::set<ParentageID>())).first;
        }
        it->second.insert(i->second->productProvenancePtr()->parentageID());
        branchChildren_.insertEmpty(bid);
      }
    }
  }

  void
  OutputModule::fillDependencyGraph() {
    for (BranchParents::const_iterator i = branchParents_.begin(), iEnd = branchParents_.end();
        i != iEnd; ++i) {
      BranchID const& child = i->first;
      std::set<ParentageID> const& eIds = i->second;
      for (std::set<ParentageID>::const_iterator it = eIds.begin(), itEnd = eIds.end();
          it != itEnd; ++it) {
        Parentage entryDesc;
        ParentageRegistry::get(*it, entryDesc);
        std::vector<BranchID> const& parents = entryDesc.parents();
        for (std::vector<BranchID>::const_iterator j = parents.begin(), jEnd = parents.end();
          j != jEnd; ++j) {
          branchChildren_.insertChild(*j, child);
        }
      }
    }
  }

}  // art
