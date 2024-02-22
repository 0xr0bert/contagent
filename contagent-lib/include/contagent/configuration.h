#ifndef CONTAGENT_CONFIGURATION_H
#define CONTAGENT_CONFIGURATION_H

#include <memory>
#include <vector>
#include "agent.h"
#include "behaviour.h"
#include "belief.h"
namespace contagent {

class Configuration {
 public:
  Configuration(const std::vector<std::shared_ptr<Behaviour>>& behaviours,
                const std::vector<std::shared_ptr<Belief>>& beliefs,
                const std::vector<std::shared_ptr<Agent>>& agents,
                uint_fast32_t start_time, uint_fast32_t end_time,
                std::unique_ptr<std::ostream> output_stream, bool full_output);

  [[nodiscard]] const std::vector<std::shared_ptr<Behaviour>>& getBehaviours()
      const;
  [[nodiscard]] const std::vector<std::shared_ptr<Belief>>& getBeliefs() const;
  [[nodiscard]] const std::vector<std::shared_ptr<Agent>>& getAgents() const;
  [[nodiscard]] uint_fast32_t getStartTime() const;
  [[nodiscard]] uint_fast32_t getEndTime() const;
  [[nodiscard]] const std::unique_ptr<std::ostream>& getOutputStream() const;
  [[nodiscard]] bool getFullOutput() const;

 private:
  const std::vector<std::shared_ptr<Behaviour>> behaviours_;
  const std::vector<std::shared_ptr<Belief>> beliefs_;
  const std::vector<std::shared_ptr<Agent>> agents_;
  const uint_fast32_t start_time_;
  const uint_fast32_t end_time_;
  const std::unique_ptr<std::ostream> output_stream_;
  const bool full_output_;
};

}  // namespace contagent

#endif  //CONTAGENT_CONFIGURATION_H
