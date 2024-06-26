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

#include <utility>

#include "contagent/belief.h"

namespace contagent {

Belief::Belief(std::string name) noexcept : UUIDd(), Named(std::move(name)) {}

Belief::Belief(boost::uuids::uuid uuid, std::string name) noexcept
    : UUIDd(uuid), Named(std::move(name)) {}
[[maybe_unused]] void Belief::set_relationship(std::weak_ptr<Belief> &belief,
                                               double_t relationship) noexcept {
  relationships_[belief] = relationship;
}
[[maybe_unused]] void Belief::set_relationships(
    std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>
        relationships) noexcept {
  relationships_ = std::move(relationships);
}
const std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>> &
Belief::get_relationships() const noexcept {
  return relationships_;
}
[[maybe_unused]] void
Belief::set_perception(std::weak_ptr<Behaviour> &behaviour,
                       double_t perception) noexcept {
  perceptions_[behaviour] = perception;
}
[[maybe_unused]] void Belief::set_perceptions(
    std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>>
        perceptions) noexcept {
  perceptions_ = std::move(perceptions);
}
const std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> &
Belief::get_perceptions() const noexcept {
  return perceptions_;
}

} // namespace contagent