#ifndef art_Persistency_RootDB_MetaDataAccess_h
#define art_Persistency_RootDB_MetaDataAccess_h
// Provide access to the One True meta data database handle.

#include "art/Persistency/RootDB/SQLite3Wrapper.h"

#include "boost/noncopyable.hpp"

namespace art {
  class MetaDataAccess;
}

class art::MetaDataAccess : private boost::noncopyable {
public:
  static MetaDataAccess & instance();

  SQLite3Wrapper const & dbHandle() const { return dbHandle_; };
  SQLite3Wrapper & dbHandle() { return dbHandle_; };

  bool isTracing() const { return tracing_; }
  void setTracing(bool onOff = true);

private:
  MetaDataAccess();
  ~MetaDataAccess();

  SQLite3Wrapper dbHandle_;
  bool tracing_;
};

#endif /* art_Persistency_RootDB_MetaDataAccess_h */

// Local Variables:
// mode: c++
// End: