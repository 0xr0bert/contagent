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

#ifndef CONTAGENT_BELIEF_H
#define CONTAGENT_BELIEF_H

#include "behaviour.h"
#include "named.h"
#include "uuidd.h"
#include <map>
#include <memory>
namespace contagent {

/// Belief represents a belief in the simulation that an Agent may hold. It is
/// both Named and UUIDd.
/// \author Robert Greener
class Belief : public UUIDd, public Named {
public:
  /// Create a new Belief with a supplied name and a randomly generated UUID.
  /// \param name The name.
  /// \author Robert Greener
  explicit Belief(std::string name) noexcept;

  /// Create a new Belief with a supplied name and UUID.
  /// \param uuid The UUID.
  /// \param name The name.
  /// \author Robert Greener
  Belief(boost::uuids::uuid uuid, std::string name) noexcept;

  /// Set the relationship to some new value. This is the value that describes
  /// how adopting the other belief is affected by holding this belief. 1 is no
  /// effect. < 1 is a negative influence. > 1 is a positive influence.
  /// \brief Set the relationship to some new value.
  /// \param belief The belief related to this.
  /// \param relationship The new relationship weight.
  /// \author Robert Greener
  [[maybe_unused]] void set_relationship(std::weak_ptr<Belief> &belief,
                                         double_t relationship) noexcept;

  /// Set all the relationships, overriding the existing ones. This is the value
  /// that describes how adopting the other belief is affected by holding this
  /// belief. 1 is no effect. < 1 is a negative influence. > 1 is a positive
  /// influence.
  /// \brief Set all the relationships.
  /// \param relationships The new relationships.
  /// \author Robert Greener
  [[maybe_unused]] void
  set_relationships(std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>
                        relationships) noexcept;

  /// Get all the relationships. This is the value
  /// that describes how adopting the other belief is affected by holding this
  /// belief. 1 is no effect. < 1 is a negative influence. > 1 is a positive
  /// influence.
  /// \brief Get all the relationships.
  /// \return The relationships.
  /// \author Robert Greener
  [[nodiscard]] const std::map<std::weak_ptr<Belief>, double_t,
                               std::owner_less<>> &
  get_relationships() const noexcept;

  /// Set the perception. This is the amount that when observing a Behaviour,
  /// they can be assumes to be driven by this Belief. In a range of [-1,+1].
  /// \brief Set the perception.
  /// \param behaviour The Behaviour that could be observed.
  /// \param perception The new perception. In a range of [-1,+1].
  /// \author Robert Greener
  [[maybe_unused]] void set_perception(std::weak_ptr<Behaviour> &behaviour,
                                      double_t perception) noexcept;

  /// Sets all the perceptions. This overwrites all the old perceptions. This is
  /// the amount that when observing a Behaviour, they can be assumes to be
  /// driven by this Belief. In a range of [-1,+1].
  /// \brief Set all the perceptions.
  /// \param perceptions The new perceptions. In a range of [-1,+1].
  /// \author Robert Greener
  [[maybe_unused]] void
  set_perceptions(std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>>
                     perceptions) noexcept;

  /// Gets all the perceptions. This is the amount that when observing a
  /// Behaviour, they can be assumes to be driven by this Belief. In a range of
  /// [-1,+1].
  /// \brief Gets all the perceptions.
  /// \return The perceptions.
  /// \author Robert Greener
  [[nodiscard]] const std::map<std::weak_ptr<Behaviour>, double_t,
                               std::owner_less<>> &
  get_perceptions() const noexcept;

private:
  /// This is the value
  /// that describes how adopting the other belief is affected by holding this
  /// belief. 1 is no effect. < 1 is a negative influence. > 1 is a positive
  /// influence.
  std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>> relationships_;

  /// Perceptions is the amount that when observing a Behaviour,
  /// they can be assumes to be driven by this Belief. In a range of [-1,+1].
  std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> perceptions_;
};
} // namespace contagent

#endif // CONTAGENT_BELIEF_H
