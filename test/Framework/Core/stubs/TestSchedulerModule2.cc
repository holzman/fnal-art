/**
   \file
   Test Module for testScheduler

   \author Stefano ARGIRO
   \version
   \date 19 May 2005
*/

static const char CVSId[] = "";


#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Core/Event.h"
#include "art/Framework/Core/ModuleMacros.h"

#include "fhiclcpp/ParameterSet.h"
#include "test/TestObjects/ToyProducts.h"
#include <memory>
#include <string>

namespace art{

  class TestSchedulerModule2 : public EDProducer
  {
  public:
    explicit TestSchedulerModule2(ParameterSet const& p):pset_(p){
       produces<arttest::StringProduct>();
    }

    void produce(Event& e, EventSetup const&);

  private:
    ParameterSet pset_;
  };


  void TestSchedulerModule2::produce(Event& e, EventSetup const&)
  {

    std::string myname = pset_.get<std::string>("module_name");
    std::auto_ptr<arttest::StringProduct> product(new arttest::StringProduct(myname));
    e.put(product);

  }
}//namespace
using art::TestSchedulerModule2;
DEFINE_ART_MODULE(TestSchedulerModule2);

// Configure (x)emacs for this file ...
// Local Variables:
// mode:c++
// compile-command: "make -C .. -k"
// End:
