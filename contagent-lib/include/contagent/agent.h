#ifndef CONTAGENT_AGENT_H
#define CONTAGENT_AGENT_H

#include <map>
#include <memory>
#include "belief.h"
#include "uuidd.h"
namespace contagent {

class Agent : public UUIDd {
 public:
  Agent(uint_fast32_t n_days, boost::uuids::uuid uuid,
        std::vector<std::map<std::shared_ptr<Belief>, double_t>> activations);

  Agent(
      std::vector<std::map<std::shared_ptr<Belief>, double_t>> activations,
      uint_fast32_t n_days);

  Agent(uint_fast32_t n_days, boost::uuids::uuid uuid);

  explicit Agent(uint_fast32_t n_days);

  [[maybe_unused]] [[nodiscard]] const std::map<std::shared_ptr<Belief>, double_t>&
  getActivationsForDay(std::size_t day);

  [[maybe_unused]] [[nodiscard]] const std::map<std::weak_ptr<Agent>, double_t,
                               std::owner_less<>>&
  getFriends() const;
  [[maybe_unused]] void setFriends(
      std::map<std::weak_ptr<Agent>, double_t, std::owner_less<>> friends);

  [[nodiscard]] const std::vector<std::shared_ptr<Behaviour>>& getActions()
      const;
  [[maybe_unused]] void setActions(std::vector<std::shared_ptr<Behaviour>> actions);

  [[maybe_unused]] [[nodiscard]] const std::unordered_map<std::shared_ptr<Belief>, double_t>&
  getDeltas() const;
  [[maybe_unused]] void setDeltas(std::unordered_map<std::shared_ptr<Belief>, double_t> deltas);

  [[maybe_unused]] [[nodiscard]] const std::unordered_map<
      std::shared_ptr<Belief>,
      std::unordered_map<std::shared_ptr<Behaviour>, double_t>>&
  getPerformanceRelationships() const;

  [[maybe_unused]] void setPerformanceRelationships(
      std::unordered_map<
          std::shared_ptr<Belief>,
          std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
          performanceRelationships);

  [[nodiscard]] double_t weightedRelationship(
      uint_fast32_t sim_time, const std::shared_ptr<Belief>& b1,
      const std::shared_ptr<Belief>& b2) const;

  [[nodiscard]] double_t contextualize(
      uint_fast32_t sim_time, const std::shared_ptr<Belief>& b,
      const std::vector<std::shared_ptr<Belief>>& beliefs) const;

  [[nodiscard]] std::unique_ptr<
      std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
  getActionsOfFriends(uint_fast32_t sim_time) const;

  [[nodiscard]] static double_t pressure(
      const std::shared_ptr<Belief>& b,
      const std::unordered_map<std::shared_ptr<Behaviour>, double_t>&
          actions_of_friends);

  [[nodiscard]] double_t activationChange(
      uint_fast32_t sim_time, const std::shared_ptr<Belief>& belief,
      const std::vector<std::shared_ptr<Belief>>& beliefs,
      const std::unordered_map<std::shared_ptr<Behaviour>, double_t>&
          actions_of_friends) const;

  void updateActivation(uint_fast32_t sim_time,
                        const std::shared_ptr<Belief>& belief,
                        const std::vector<std::shared_ptr<Belief>>& beliefs,
                        const std::unordered_map<std::shared_ptr<Behaviour>,
                                                 double_t>& actions_of_friends);

  void updateActivationsForAllBeliefs(
      uint_fast32_t sim_time,
      const std::vector<std::shared_ptr<Belief>>& beliefs);

  void performAction(uint_fast32_t sim_time,
                     const std::vector<std::shared_ptr<Behaviour>>& behaviours,
                     const std::vector<std::shared_ptr<Belief>>& beliefs);

 private:
  std::vector<std::map<std::shared_ptr<Belief>, double_t>> activations_;

  std::map<std::weak_ptr<Agent>, double_t, std::owner_less<>> friends_;

  std::vector<std::shared_ptr<Behaviour>> actions_;

  std::unordered_map<std::shared_ptr<Belief>, double_t> deltas_;

  std::unordered_map<std::shared_ptr<Belief>,
                     std::unordered_map<std::shared_ptr<Behaviour>, double_t>>
      performance_relationships_;

  void initialize(uint_fast32_t n_days);
};
}  // namespace contagent

#endif  //CONTAGENT_AGENT_H
