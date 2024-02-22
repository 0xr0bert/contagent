#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <utility>
#include "named.h"
#include "uuidd.h"

namespace contagent {

class Behaviour : public UUIDd, public Named {
 public:
  Behaviour(boost::uuids::uuid uuid, std::string name)
      : UUIDd(uuid), Named(std::move(name)) {}

  explicit Behaviour(std::string name)
      : UUIDd(), Named(std::move(name)) {
  }
};

}  // namespace contagent

#endif  //BEHAVIOUR_H
