//
// Created by rober on 24/02/2024.
//

#ifndef CONTAGENT_BEHAVIOUR_SPEC_H
#define CONTAGENT_BEHAVIOUR_SPEC_H

#include "contagent/behaviour.h"
#include "nlohmann/json.hpp"
#include <string>

using json = nlohmann::json;

namespace contagent::json {
class BehaviourSpec {
public:
  BehaviourSpec() = default;
  explicit BehaviourSpec(const contagent::Behaviour &behaviour);

  std::string uuid;
  std::string name;

  [[nodiscard]] std::shared_ptr<contagent::Behaviour> toBehaviour() const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BehaviourSpec, uuid, name)
} // namespace contagent::json

#endif // CONTAGENT_BEHAVIOUR_SPEC_H
