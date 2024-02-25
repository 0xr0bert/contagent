#ifndef CONTAGENT_BELIEF_H
#define CONTAGENT_BELIEF_H

#include "behaviour.h"
#include "named.h"
#include "uuidd.h"
#include <map>
#include <memory>
namespace contagent {

class Belief : public UUIDd, public Named {
public:
  explicit Belief(std::string name) noexcept;

  Belief(boost::uuids::uuid uuid, std::string name) noexcept;

  [[maybe_unused]] void setRelationship(std::weak_ptr<Belief> &belief,
                                        double_t relationship) noexcept;

  [[maybe_unused]] void
  setRelationships(std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>
                       relationships) noexcept;

  [[nodiscard]] const std::map<std::weak_ptr<Belief>, double_t,
                               std::owner_less<>> &
  getRelationships() const noexcept;

  [[maybe_unused]] void setPerception(std::weak_ptr<Behaviour> &behaviour,
                                      double_t perception) noexcept;

  [[maybe_unused]] void
  setPerceptions(std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>>
                     perceptions) noexcept;

  [[nodiscard]] const std::map<std::weak_ptr<Behaviour>, double_t,
                               std::owner_less<>> &
  getPerceptions() const noexcept;

private:
  std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>> relationships_;
  std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> perceptions_;
};
} // namespace contagent

#endif // CONTAGENT_BELIEF_H
