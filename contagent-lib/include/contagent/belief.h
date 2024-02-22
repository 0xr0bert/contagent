#ifndef CONTAGENT_BELIEF_H
#define CONTAGENT_BELIEF_H

#include <map>
#include <memory>
#include "behaviour.h"
#include "named.h"
#include "uuidd.h"
namespace contagent {

class Belief : public UUIDd, public Named {
 public:
  explicit Belief(std::string name);

  Belief(boost::uuids::uuid uuid, std::string name);

  void setRelationship(std::weak_ptr<Belief>& belief, double_t relationship);

  void setRelationships(
      std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>>
          relationships);

  [[nodiscard]] const std::map<std::weak_ptr<Belief>, double_t,
                               std::owner_less<>>&
  getRelationships() const;

  void setPerception(std::weak_ptr<Behaviour>& behaviour, double_t perception);

  void setPerceptions(
      std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>>
          perceptions);

  [[nodiscard]] const std::map<std::weak_ptr<Behaviour>, double_t,
                               std::owner_less<>>&
  getPerceptions() const;

  bool operator<(const Belief& rhs) const;
  bool operator>(const Belief& rhs) const;
  bool operator<=(const Belief& rhs) const;
  bool operator>=(const Belief& rhs) const;

 private:
  std::map<std::weak_ptr<Belief>, double_t, std::owner_less<>> relationships_;
  std::map<std::weak_ptr<Behaviour>, double_t, std::owner_less<>> perceptions_;
};
}  // namespace contagent

#endif  //CONTAGENT_BELIEF_H
