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
