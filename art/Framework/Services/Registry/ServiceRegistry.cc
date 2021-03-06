// ======================================================================
//
// ServiceRegistry
//
// ======================================================================

#include "art/Framework/Services/Registry/ActivityRegistry.h"
#include "art/Framework/Services/Registry/ServiceRegistry.h"
#include "art/Utilities/PluginSuffixes.h"

#include "boost/thread/tss.hpp"

using fhicl::ParameterSet;
using art::ServiceRegistry;
using art::ServiceToken;

ServiceRegistry::ServiceRegistry()
  : lm_{ Suffixes::service() }
{ }

ServiceRegistry::~ServiceRegistry()
{ }

ServiceToken
  ServiceRegistry::setContext( ServiceToken const & iNewToken )
{
  ServiceToken result(manager_);
  manager_ = iNewToken.manager_;
  return result;
}

void
  ServiceRegistry::unsetContext( ServiceToken const & iOldToken )
{
  manager_ = iOldToken.manager_;
}

ServiceToken
  ServiceRegistry::presentToken() const
{
  return manager_;
}

ServiceToken
ServiceRegistry::createSet(ParameterSets const & iPS, ActivityRegistry & reg)
{
  auto result = std::make_shared<ServicesManager>( iPS,
                                                   ServiceRegistry::instance().lm_,
                                                   reg);
  return ServiceToken(result);
}

ServiceRegistry &
ServiceRegistry::instance()
{
  static boost::thread_specific_ptr<ServiceRegistry> s_registry;
  if( 0 == s_registry.get() ) {
    s_registry.reset(new ServiceRegistry);
  }
  return *s_registry;
}

// ======================================================================
