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

#include "contagent/json/agent_spec.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

contagent::json::AgentSpec::AgentSpec(const contagent::Agent &agent) {}

std::shared_ptr<contagent::Agent> contagent::json::AgentSpec::toUnlinkedAgent(
    uint_fast32_t n_days,
    const std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>> &behaviours,
    const std::map<boost::uuids::uuid, std::shared_ptr<Belief>> &beliefs)
    const {
  auto agent = std::make_shared<contagent::Agent>(
      n_days, boost::lexical_cast<boost::uuids::uuid>(uuid));

  std::vector<std::shared_ptr<contagent::Behaviour>> actions_proper;

  std::transform(actions.begin(), actions.end(),
                 std::back_inserter(actions_proper),
                 [behaviours](const std::string &action) {
                   auto u = boost::lexical_cast<boost::uuids::uuid>(action);
                   return behaviours.at(u);
                 });

  agent->setActions(actions_proper);

  std::vector<std::unordered_map<std::shared_ptr<contagent::Belief>, double_t>>
      activations_proper;

  std::transform(
      activations.begin(), activations.end(),
      std::back_inserter(activations_proper),
      [beliefs](const std::unordered_map<std::string, double_t>
                    &activations_at_time) {
        std::unordered_map<std::shared_ptr<contagent::Belief>, double_t>
            activations_at_time_proper;

        std::transform(
            activations_at_time.begin(), activations_at_time.end(),
            std::inserter(activations_at_time_proper,
                          activations_at_time_proper.end()),
            [beliefs](const auto &p) {
              std::pair<std::shared_ptr<contagent::Belief>, double_t> new_pair;
              new_pair.first =
                  beliefs.at(boost::lexical_cast<boost::uuids::uuid>(p.first));
              new_pair.second = p.second;
              return new_pair;
            });

        return activations_at_time_proper;
      });

  agent->setActivations(activations_proper);

  std::unordered_map<std::shared_ptr<contagent::Belief>, double_t>
      deltas_proper;

  std::transform(
      deltas.begin(), deltas.end(),
      std::inserter(deltas_proper, deltas_proper.end()),
      [beliefs](const auto &p) {
        std::pair<std::shared_ptr<contagent::Belief>, double_t> new_pair;
        new_pair.first =
            beliefs.at(boost::lexical_cast<boost::uuids::uuid>(p.first));
        new_pair.second = p.second;
        return new_pair;
      });

  agent->setDeltas(deltas_proper);

  std::unordered_map<
      std::shared_ptr<contagent::Belief>,
      std::unordered_map<std::shared_ptr<contagent::Behaviour>, double_t>>
      performance_relationships_proper;

  std::transform(
      performance_relationships.begin(), performance_relationships.end(),
      std::inserter(performance_relationships_proper,
                    performance_relationships_proper.end()),
      [beliefs, behaviours](const auto &p) {
        std::pair<
            std::shared_ptr<contagent::Belief>,
            std::unordered_map<std::shared_ptr<contagent::Behaviour>, double_t>>
            new_outer_pair;

        new_outer_pair.first =
            beliefs.at(boost::lexical_cast<boost::uuids::uuid>(p.first));

        std::unordered_map<std::shared_ptr<contagent::Behaviour>, double_t>
            inner_map;

        std::transform(
            p.second.begin(), p.second.end(),
            std::inserter(inner_map, inner_map.end()),
            [behaviours](const auto &inner_pair) {
              std::pair<std::shared_ptr<contagent::Behaviour>, double_t>
                  new_inner_pair;
              new_inner_pair.first = behaviours.at(
                  boost::lexical_cast<boost::uuids::uuid>(inner_pair.first));
              new_inner_pair.second = inner_pair.second;
              return new_inner_pair;
            });

        new_outer_pair.second = inner_map;

        return new_outer_pair;
      });

  agent->setPerformanceRelationships(performance_relationships_proper);

  return agent;
}

void contagent::json::AgentSpec::linkAgents(
    const std::map<boost::uuids::uuid, std::shared_ptr<Agent>> &agents) const {
  auto &agent = agents.at(boost::lexical_cast<boost::uuids::uuid>(uuid));

  std::map<std::weak_ptr<contagent::Agent>, double_t, std::owner_less<>>
      friends_proper;

  std::transform(
      friends.begin(), friends.end(),
      std::inserter(friends_proper, friends_proper.end()),
      [agents](const auto &p) {
        const auto &f =
            agents.at(boost::lexical_cast<boost::uuids::uuid>(p.first));
        std::weak_ptr<contagent::Agent> weak_friend = f;
        std::pair<std::weak_ptr<contagent::Agent>, double_t> new_pair(
            weak_friend, p.second);
        return new_pair;
      });

  agent->setFriends(friends_proper);
}
