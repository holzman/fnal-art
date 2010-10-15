#include "test/Framework/Services/Message/UnitTestClient_B.h"
#include "art/MessageLogger/MessageLogger.h"
#include "art/Framework/Core/MakerMacros.h"

#include <iostream>
#include <string>

namespace edmtest
{

int  UnitTestClient_B::nevent = 0;

void
  UnitTestClient_B::analyze( art::Event      const & e
                           , art::EventSetup const & /*unused*/
                              )
{
  nevent++;
  for (int i = 0; i < nevent; ++i) {
    art::LogError  ("cat_A")   << "LogError was used to send this message";
  }
  art::LogError  ("cat_B")   << "LogError was used to send this other message";
  art::LogStatistics();
}  // MessageLoggerClient::analyze()


}  // namespace edmtest


using edmtest::UnitTestClient_B;
DEFINE_FWK_MODULE(UnitTestClient_B);
