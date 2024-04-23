// Copyright (c) 2024, Robert Greener
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "contagent/json/belief_spec.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace contagent::json {

BeliefSpec::BeliefSpec(const contagent::Belief &belief) {
  uuid = boost::lexical_cast<std::string>(belief.get_uuid());
  name = belief.get_name();

  for (const auto &[b2, relationship] : belief.get_relationships()) {
    if (auto p = b2.lock()) {
      relationships.insert(std::pair(
          boost::lexical_cast<std::string>(p->get_uuid()), relationship));
    } else {
      throw std::runtime_error("Unable to lock weak pointer");
    }
  }

  for (const auto &[behaviour, perception] : belief.get_perceptions()) {
    if (auto p = behaviour.lock()) {
      perceptions.insert(std::pair(
          boost::lexical_cast<std::string>(p->get_uuid()), perception));
    } else {
      throw std::runtime_error("Unable to lock weak pointer");
    }
  }
}
std::shared_ptr<contagent::Belief> BeliefSpec::to_unlinked_belief(
    const std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>> &behaviours)
    const {
  auto belief = std::make_shared<contagent::Belief>(
      boost::lexical_cast<boost::uuids::uuid>(uuid), name);

  for (const auto &[behaviour, perception] : perceptions) {
    auto behaviour_uuid = boost::lexical_cast<boost::uuids::uuid>(behaviour);

    const auto &actual_behaviour = behaviours.at(behaviour_uuid);
    std::weak_ptr<contagent::Behaviour> weak_behaviour = actual_behaviour;

    belief->set_perception(weak_behaviour, perception);
  }

  return belief;
}
void BeliefSpec::link_beliefs(
    const std::map<boost::uuids::uuid, std::shared_ptr<Belief>> &beliefs)
    const {
  auto &belief = beliefs.at(boost::lexical_cast<boost::uuids::uuid>(uuid));

  for (const auto &[b2, relationship] : relationships) {
    const auto &actual_b2 =
        beliefs.at(boost::lexical_cast<boost::uuids::uuid>(b2));
    std::weak_ptr<contagent::Belief> weak_b2 = actual_b2;

    belief->set_relationship(weak_b2, relationship);
  }
}
} // namespace contagent::json