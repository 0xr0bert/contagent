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

#include <random>
#include <ranges>
#include <utility>

#include "contagent/agent.h"
#include "contagent/belief.h"

namespace contagent {
Agent::Agent(const uint_fast32_t n_days, boost::uuids::uuid uuid)
    : UUIDd(uuid) {
  initialize(n_days);
}

Agent::Agent(const uint_fast32_t n_days) {
  initialize(n_days);
}

[[maybe_unused]] const std::map<std::shared_ptr<Belief>, double_t>&
Agent::getActivationsForDay(const std::size_t day) {
  return activations_.at(day);
}

Agent::Agent(
    const uint_fast32_t n_days, boost::uuids::uuid uuid,
    std::vector<std::map<std::shared_ptr<Belief>, double_t>> activations)
    : UUIDd(uuid), activations_(std::move(activations)) {
  initialize(n_days);
}

Agent::Agent(
    std::vector<std::map<std::shared_ptr<Belief>, double_t>> activations,
    const uint_fast32_t n_days)
    : activations_(std::move(activations)) {
  initialize(n_days);
}

[[maybe_unused]] const std::map<std::weak_ptr<Agent>, double_t, std::owner_less<>>&
Agent::getFriends() const {
  return friends_;
}

[[maybe_unused]] void Agent::setFriends(
    std::map<std::weak_ptr<Agent>, double_t, std::owner_less<>> friends) {
  friends_ = std::move(friends);
}

const std::vector<std::shared_ptr<Behaviour>>& Agent::getActions()
    const {
  return actions_;
}

[[maybe_unused]] void Agent::setActions(std::vector<std::shared_ptr<Behaviour>> actions) {
  actions_ = std::move(actions);
}

[[maybe_unused]] const std::unordered_map<std::shared_ptr<Belief>, double_t>&
Agent::getDeltas() const {
  return deltas_;
}

[[maybe_unused]] void Agent::setDeltas(
    std::unordered_map<std::shared_ptr<Belief>, double_t> deltas) {
  deltas_ = std::move(deltas);
}

double_t Agent::weightedRelationship(const uint_fast32_t sim_time,
                                     const std::shared_ptr<Belief>& b1,
                                     const std::shared_ptr<Belief>& b2) const {
  auto& acts = activations_[sim_time];
  if (acts.contains(b1)) {
    auto& relationships = b1->getRelationships();
    if (relationships.contains(b2)) {
      return acts.at(b1) * relationships.at(b2);
    }
  } else {
    return 0.0;
  }
  return 0.0;
}

double_t Agent::contextualize(
    const uint_fast32_t sim_time, const std::shared_ptr<Belief>& b,
    const std::vector<std::shared_ptr<Belief>>& beliefs) const {
  const uint_fast32_t size = beliefs.size();

  if (size == 0) {
    return 0.0;
  } else {
    double_t context = 0;
    for (auto& b2 : beliefs) {
      context += weightedRelationship(sim_time, b, b2);
    }

    return context / size; // NOLINT(*-narrowing-conversions)
  }
}
std::unique_ptr<std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
Agent::getActionsOfFriends(uint_fast32_t sim_time) const {
  auto map = std::make_unique<
      std::unordered_map<std::shared_ptr<Behaviour>, double_t>>();

  for (auto const& [weak_friend, w] : friends_) {
    if (auto shared_friend = weak_friend.lock()) {
      auto& action = shared_friend->getActions().at(sim_time);
      map->insert(std::pair<std::shared_ptr<Behaviour>, double_t>(action, w));
    }
  }

  return map;
}

double_t Agent::pressure(
    const std::shared_ptr<Belief>& b,
    const std::unordered_map<std::shared_ptr<Behaviour>, double_t>&
        actions_of_friends) {
  const uint_fast32_t size = actions_of_friends.size();

  if (size == 0) {
    return 0.0;
  } else {
    double_t pressure = 0.0;

    for (auto const& [behaviour, w] : actions_of_friends) {
      pressure += b->getPerceptions().at(behaviour) * w;
    }

    return pressure / size; // NOLINT(*-narrowing-conversions)
  }
}
double_t Agent::activationChange(
    const uint_fast32_t sim_time, const std::shared_ptr<Belief>& belief,
    const std::vector<std::shared_ptr<Belief>>& beliefs,
    const std::unordered_map<std::shared_ptr<Behaviour>, double_t>&
        actions_of_friends) const {
  double_t pressure = contagent::Agent::pressure(belief, actions_of_friends);

  if (pressure > 0.0) {
    return (1.0 + this->contextualize(sim_time, belief, beliefs)) / 2.0 *
           pressure;
  } else {
    return (1.0 - this->contextualize(sim_time, belief, beliefs)) / 2.0 *
           pressure;
  }
}
void Agent::updateActivation(
    const uint_fast32_t sim_time, const std::shared_ptr<Belief>& belief,
    const std::vector<std::shared_ptr<Belief>>& beliefs,
    const std::unordered_map<std::shared_ptr<Behaviour>, double_t>&
        actions_of_friends) {
  const double_t delta = deltas_.at(belief);
  const auto& activations = activations_.at(sim_time - 1);
  const double_t activation = activations.at(belief);

  const double_t activation_change =
      activationChange(sim_time - 1, belief, beliefs, actions_of_friends);

  const double_t new_activation =
      std::max(-1.0, std::min(1.0, delta * activation + activation_change));

  if (activations_.size() < sim_time + 1) {
    activations_.resize(sim_time + 1);
  }

  activations_.at(sim_time).at(belief) = new_activation;
}
void Agent::updateActivationsForAllBeliefs(
    const uint_fast32_t sim_time,
    const std::vector<std::shared_ptr<Belief>>& beliefs) {
  const auto& actions_of_friends = getActionsOfFriends(sim_time - 1);
  for (const auto& belief : beliefs) {
    updateActivation(sim_time, belief, beliefs, *actions_of_friends);
  }
}
[[maybe_unused]] const std::unordered_map<
    std::shared_ptr<Belief>,
    std::unordered_map<std::shared_ptr<Behaviour>, double_t>>&
Agent::getPerformanceRelationships() const {
  return performance_relationships_;
}
[[maybe_unused]] void Agent::setPerformanceRelationships(
    std::unordered_map<std::shared_ptr<Belief>,
                       std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
        performanceRelationships) {
  performance_relationships_ = std::move(performanceRelationships);
}

void Agent::performAction(
    const uint_fast32_t sim_time,
    const std::vector<std::shared_ptr<Behaviour>>& behaviours,
    const std::vector<std::shared_ptr<Belief>>& beliefs) {
  std::vector<std::pair<std::shared_ptr<Behaviour>, double_t>>
      unnormalized_probabilities;
  unnormalized_probabilities.reserve(behaviours.size());

  for (const auto& behaviour : behaviours) {
    double_t value = 0.0;

    for (const auto& belief : beliefs) {
      double_t prs = performance_relationships_.at(belief).at(behaviour);
      double_t activation = activations_.at(sim_time).at(belief);
      value += prs * activation;
    }

    unnormalized_probabilities.emplace_back(behaviour, value);
  }

  std::sort(
      unnormalized_probabilities.begin(), unnormalized_probabilities.end(),
      [](const auto& p1, const auto& p2) { return p1.second < p2.second; });

  const auto& last_elem = unnormalized_probabilities.back();

  if (last_elem.second < 0.0) {
    actions_.at(sim_time) = last_elem.first;
  } else {
    std::vector<std::pair<std::shared_ptr<Behaviour>, double_t>>
        filtered_probabilities;
    std::copy_if(unnormalized_probabilities.begin(),
                 unnormalized_probabilities.end(),
                 std::back_inserter(filtered_probabilities),
                 [](const auto& pair) { return pair.second >= 0.0; });
    if (filtered_probabilities.size() == 1) {
      actions_.at(sim_time) = filtered_probabilities[0].first;
    } else {
      double_t normalizing_factor;

      for (const auto& [_, v] : filtered_probabilities) {
        normalizing_factor += v;
      }

      for (auto &[_, v] : filtered_probabilities) {
        v /= normalizing_factor;
      }

      std::shared_ptr<Behaviour>& chosenBehaviour = filtered_probabilities.back().first;

      std::random_device random_device;
      std::mt19937 rng{random_device()};
      std::uniform_real_distribution<> distribution(0, std::nextafter(1.0, std::numeric_limits<double_t>::max()));

      double_t rv = distribution(rng);

      for (auto &[behaviour, v] : filtered_probabilities) {
        rv -= v;

        if (rv <= 0.0) {
          chosenBehaviour = behaviour;
          break;
        }
      }

      actions_.at(sim_time) = chosenBehaviour;
    }
  }
}

void Agent::initialize(uint_fast32_t n_days) {
  actions_.reserve(n_days);
  activations_.reserve(n_days);
}
void Agent::setActivations(
    std::vector<std::unordered_map<std::shared_ptr<Belief>, double_t>>
        activations) {
  activations_ = std::move(activations);
}
}  // namespace contagent