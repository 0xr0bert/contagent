//
// Created by rober on 24/02/2024.
//

#include "contagent/json/behaviour_spec.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace contagent::json {
std::shared_ptr<contagent::Behaviour> BehaviourSpec::toBehaviour() const {
  auto uuid_v = boost::lexical_cast<boost::uuids::uuid>(uuid);
  return std::make_shared<contagent::Behaviour>(uuid_v, name);
}
BehaviourSpec::BehaviourSpec(const contagent::Behaviour &behaviour)
    : uuid(boost::lexical_cast<std::string>(behaviour.getUuid())),
      name(behaviour.getName()) {}
} // namespace contagent::json