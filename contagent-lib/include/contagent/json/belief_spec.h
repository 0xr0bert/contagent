//
// Created by rober on 25/02/2024.
//

#ifndef CONTAGENT_BELIEF_SPEC_H
#define CONTAGENT_BELIEF_SPEC_H

#include "contagent/belief.h"
#include "nlohmann/json.hpp"
#include <boost/uuid/uuid.hpp>

namespace contagent::json {
class BeliefSpec {
public:
  BeliefSpec() = default;

  explicit BeliefSpec(const contagent::Belief &belief);

  std::string uuid;
  std::string name;
  std::unordered_map<std::string, double_t> relationships;
  std::unordered_map<std::string, double_t> perceptions;

  [[nodiscard]] std::shared_ptr<contagent::Belief> toUnlinkedBelief(
      const std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>>
          &behaviours) const;

  void linkBeliefs(const std::map<boost::uuids::uuid, std::shared_ptr<Belief>>
                       &beliefs) const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BeliefSpec, uuid, name, relationships,
                                   perceptions);
} // namespace contagent::json

#endif // CONTAGENT_BELIEF_SPEC_H
