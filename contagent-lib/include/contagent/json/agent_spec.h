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

#ifndef CONTAGENT_AGENT_SPEC_H

#include "contagent/agent.h"
#include "contagent/behaviour.h"
#include "contagent/belief.h"
#include "nlohmann/json.hpp"
#include <string>

namespace contagent::json {
class AgentSpec {
public:
  AgentSpec() = default;
  [[maybe_unused]] explicit AgentSpec(const contagent::Agent &agent);

  std::string uuid;
  std::vector<std::string> actions;
  std::vector<std::unordered_map<std::string, double_t>> activations;
  std::unordered_map<std::string, double_t> deltas;
  std::unordered_map<std::string, double_t> friends;
  std::unordered_map<std::string, std::unordered_map<std::string, double_t>>
      performance_relationships;

  [[nodiscard]] std::shared_ptr<contagent::Agent>
  toUnlinkedAgent(uint_fast32_t n_days,
                  const std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>>
                      &behaviours,
                  const std::map<boost::uuids::uuid, std::shared_ptr<Belief>>
                      &beliefs) const;

  void linkAgents(
      const std::map<boost::uuids::uuid, std::shared_ptr<Agent>> &agents) const;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AgentSpec, uuid, actions, activations,
                                   deltas, friends, performance_relationships)
} // namespace contagent::json

#define CONTAGENT_AGENT_SPEC_H

#endif // CONTAGENT_AGENT_SPEC_H
