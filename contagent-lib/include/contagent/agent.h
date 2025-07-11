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

#ifndef CONTAGENT_AGENT_H
#define CONTAGENT_AGENT_H

#include "belief.h"
#include "uuidd.h"
#include <map>
#include <memory>
#include <optional>
#include <unordered_map>

namespace contagent {

/// An agent in the simulation.
/// \author Robert Greener
class Agent : public UUIDd {
public:
  /// Create a new agent.
  /// \param n_days The number of days in the simulation.
  /// \param uuid The UUID of the agent.
  /// \param activations The activations of the agent.
  /// \throws std::length_exception If n_days is greater than std::max_size().
  /// \author Robert Greener
  Agent(uint_fast32_t n_days, boost::uuids::uuid uuid,
        std::vector<std::unordered_map<std::shared_ptr<Belief>, double_t>>
            activations);

  Agent(std::vector<std::unordered_map<std::shared_ptr<Belief>, double_t>>
            activations,
        uint_fast32_t n_days);

  Agent(uint_fast32_t n_days, boost::uuids::uuid uuid);

  explicit Agent(uint_fast32_t n_days);

  [[maybe_unused]] [[nodiscard]] const std::unordered_map<
      std::shared_ptr<Belief>, double_t> &
  get_activations_for_day(std::size_t day);

  [[maybe_unused]] [[nodiscard]] const std::map<std::weak_ptr<Agent>, double_t,
                                                std::owner_less<>> &
  get_friends() const;

  [[nodiscard]] const std::vector<
      std::unordered_map<std::shared_ptr<Belief>, double_t>> &
  get_activations() const noexcept;

  void set_activations(
      std::vector<std::unordered_map<std::shared_ptr<Belief>, double_t>>
          activations);
  [[maybe_unused]] void set_friends(
      std::map<std::weak_ptr<Agent>, double_t, std::owner_less<>> friends);

  [[nodiscard]] const std::vector<std::shared_ptr<Behaviour>> &
  get_actions() const;
  [[maybe_unused]] void
  set_actions(std::vector<std::shared_ptr<Behaviour>> actions);

  [[maybe_unused]] [[nodiscard]] const std::unordered_map<
      std::shared_ptr<Belief>, double_t> &
  get_deltas() const;
  [[maybe_unused]] void
  set_deltas(std::unordered_map<std::shared_ptr<Belief>, double_t> deltas);

  [[maybe_unused]] [[nodiscard]] const std::unordered_map<
      std::shared_ptr<Belief>,
      std::unordered_map<std::shared_ptr<Behaviour>, double_t>> &
  get_performance_relationships() const;

  [[maybe_unused]] void set_performance_relationships(
      std::unordered_map<
          std::shared_ptr<Belief>,
          std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
          performanceRelationships);

  [[nodiscard]] double_t
  weighted_relationship(uint_fast32_t sim_time,
                        const std::shared_ptr<Belief> &b1,
                        const std::shared_ptr<Belief> &b2) const;

  [[nodiscard]] double_t
  contextualize(uint_fast32_t sim_time, const std::shared_ptr<Belief> &b,
                const std::vector<std::shared_ptr<Belief>> &beliefs) const;

  [[nodiscard]] std::unique_ptr<
      std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
  get_actions_of_friends(uint_fast32_t sim_time) const;

  [[nodiscard]] double_t
  pressure(const std::shared_ptr<Belief> &b,
           const std::unordered_map<std::shared_ptr<Behaviour>, double_t>
               &actions_of_friends) const;

  [[nodiscard]] double_t activation_change(
      uint_fast32_t sim_time, const std::shared_ptr<Belief> &belief,
      const std::vector<std::shared_ptr<Belief>> &beliefs,
      const std::unordered_map<std::shared_ptr<Behaviour>, double_t>
          &actions_of_friends) const;

  void update_activation(
      uint_fast32_t sim_time, const std::shared_ptr<Belief> &belief,
      const std::vector<std::shared_ptr<Belief>> &beliefs,
      const std::unordered_map<std::shared_ptr<Behaviour>, double_t>
          &actions_of_friends);

  void update_activation_for_all_beliefs(
      uint_fast32_t sim_time,
      const std::vector<std::shared_ptr<Belief>> &beliefs);

  void perform_action(uint_fast32_t sim_time,
                      const std::vector<std::shared_ptr<Behaviour>> &behaviours,
                      const std::vector<std::shared_ptr<Belief>> &beliefs);

private:
  std::vector<std::unordered_map<std::shared_ptr<Belief>, double_t>>
      activations_;

  std::map<std::weak_ptr<Agent>, double_t, std::owner_less<>> friends_;

  std::vector<std::shared_ptr<Behaviour>> actions_;

  std::unordered_map<std::shared_ptr<Belief>, double_t> deltas_;

  std::unordered_map<std::shared_ptr<Belief>,
                     std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
      performance_relationships_;

  /// Initialize the agent, reserving the space in the ::activations_ and
  /// ::actions_ vectors.
  /// \param n_days The number of days that the simulation will be run for.
  /// \throws std::length_exception If n_days is greater than std::max_size().
  void initialize(uint_fast32_t n_days);
};
} // namespace contagent

#endif // CONTAGENT_AGENT_H
