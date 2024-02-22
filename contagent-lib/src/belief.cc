#include <utility>

#include "contagent/belief.h"

namespace contagent {

Belief::Belief(std::string name) : UUIDd(), Named(std::move(name)) {}

Belief::Belief(boost::uuids::uuid uuid, std::string name)
    : UUIDd(uuid), Named(std::move(name)) {}
bool Belief::operator<(const Belief& rhs) const {
  return std::tie(static_cast<const UUIDd&>(*this),
                  static_cast<const Named&>(*this)) <
         std::tie(static_cast<const UUIDd&>(rhs),
                  static_cast<const Named&>(rhs));
}
bool Belief::operator>(const Belief& rhs) const {
  return rhs < *this;
}
bool Belief::operator<=(const Belief& rhs) const {
  return !(rhs < *this);
}
bool Belief::operator>=(const Belief& rhs) const {
  return !(*this < rhs);
}
void Belief::setRelationship(std::weak_ptr<Belief>& belief,
                                    double_t relationship) {
  relationships_[belief] = relationship;
}
void Belief::setRelationships(
    std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>
        relationships) {
  relationships_ = std::move(relationships);
}
const std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>&
Belief::getRelationships() const {
  return relationships_;
}
void Belief::setPerception(std::weak_ptr<Behaviour>& behaviour,
                                  double_t perception) {
  perceptions_[behaviour] = perception;
}
void Belief::setPerceptions(
    std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> perceptions) {
  perceptions_ = std::move(perceptions);
}
const std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>>&
Belief::getPerceptions() const {
  return perceptions_;
}

}  // namespace contagent