#include <utility>

#include "contagent/belief.h"

namespace contagent {

Belief::Belief(std::string name) noexcept : UUIDd(), Named(std::move(name)) {}

Belief::Belief(boost::uuids::uuid uuid, std::string name) noexcept
    : UUIDd(uuid), Named(std::move(name)) {}
[[maybe_unused]] void Belief::setRelationship(std::weak_ptr<Belief>& belief,
                                    double_t relationship) noexcept {
  relationships_[belief] = relationship;
}
[[maybe_unused]] void Belief::setRelationships(
    std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>
        relationships) noexcept {
  relationships_ = std::move(relationships);
}
const std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>> &
Belief::getRelationships() const noexcept {
  return relationships_;
}
[[maybe_unused]] void Belief::setPerception(std::weak_ptr<Behaviour>& behaviour,
                                  double_t perception) noexcept {
  perceptions_[behaviour] = perception;
}
[[maybe_unused]] void Belief::setPerceptions(
    std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> perceptions) noexcept {
  perceptions_ = std::move(perceptions);
}
const std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> &
Belief::getPerceptions() const noexcept {
  return perceptions_;
}

}  // namespace contagent