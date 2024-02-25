//
// Created by rober on 25/02/2024.
//

#include "contagent/json/belief_spec.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace contagent::json {

BeliefSpec::BeliefSpec(const contagent::Belief &belief) {
  uuid = boost::lexical_cast<std::string>(belief.getUuid());
  name = belief.getName();

  for (const auto &[b2, relationship] : belief.getRelationships()) {
    if (auto p = b2.lock()) {
      relationships.insert(std::pair(
          boost::lexical_cast<std::string>(p->getUuid()), relationship));
    } else {
      throw std::runtime_error("Unable to lock weak pointer");
    }
  }

  for (const auto &[behaviour, perception] : belief.getPerceptions()) {
    if (auto p = behaviour.lock()) {
      perceptions.insert(std::pair(
          boost::lexical_cast<std::string>(p->getUuid()), perception));
    } else {
      throw std::runtime_error("Unable to lock weak pointer");
    }
  }
}
std::shared_ptr<contagent::Belief> BeliefSpec::toUnlinkedBelief(
    const std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>> &behaviours)
    const {
  auto belief = std::make_shared<contagent::Belief>(
      boost::lexical_cast<boost::uuids::uuid>(uuid), name);

  for (const auto &[behaviour, perception] : perceptions) {
    auto behaviour_uuid = boost::lexical_cast<boost::uuids::uuid>(behaviour);

    const auto &actual_behaviour = behaviours.at(behaviour_uuid);
    std::weak_ptr<contagent::Behaviour> weak_behaviour = actual_behaviour;

    belief->setPerception(weak_behaviour, perception);
  }

  return belief;
}
void BeliefSpec::linkBeliefs(
    const std::map<boost::uuids::uuid, std::shared_ptr<Belief>> &beliefs)
    const {
  auto &belief = beliefs.at(boost::lexical_cast<boost::uuids::uuid>(uuid));

  for (const auto &[b2, relationship] : relationships) {
    const auto &actual_b2 =
        beliefs.at(boost::lexical_cast<boost::uuids::uuid>(b2));
    std::weak_ptr<contagent::Belief> weak_b2 = actual_b2;

    belief->setRelationship(weak_b2, relationship);
  }
}
} // namespace contagent::json