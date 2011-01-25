#ifndef NovaConfigPostProcessor_h
#define NovaConfigPostProcessor_h

#include <string>
#include <vector>

namespace fhicl {
   class intermediate_table;
}

class NovaConfigPostProcessor {
 public:
   NovaConfigPostProcessor();
   void apply(fhicl::intermediate_table &raw_config) const;

   void sources(std::vector<std::string> const &sources);
   void tFileName(std::string const &tFileName);
   void output(std::string const &output);
   void nevts(int nevts) {nevts_ = nevts; wantNevts_ = true; }
   void startEvt(unsigned long startEvt) {startEvt_ = startEvt; wantStartEvt_ = true; }
   void skipEvts(unsigned long skipEvts) {skipEvts_ = skipEvts; wantSkipEvts_ = true; }
   void trace(bool trace = true) {trace_ = trace; wantTrace_ = true; }
 private:

   void applySource(fhicl::intermediate_table &raw_config) const;
   void applyOutput(fhicl::intermediate_table &raw_config) const;
   void applyTFileName(fhicl::intermediate_table &raw_config) const;
   void applyTrace(fhicl::intermediate_table &raw_config) const;

   std::vector<std::string> sources_;
   std::string tFileName_;
   std::string output_;
   int nevts_;
   unsigned long startEvt_;
   unsigned long skipEvts_;
   bool trace_;
   bool wantNevts_;
   bool wantStartEvt_;
   bool wantSkipEvts_;
   bool wantTrace_;
};
#endif
