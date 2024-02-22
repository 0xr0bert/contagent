
#include "contagent/configuration.h"

namespace contagent {
Configuration::Configuration(
    const std::vector<std::shared_ptr<Behaviour>>& behaviours,
    const std::vector<std::shared_ptr<Belief>>& beliefs,
    const std::vector<std::shared_ptr<Agent>>& agents,
    const uint_fast32_t start_time, const uint_fast32_t end_time,
    std::unique_ptr<std::ostream> output_stream, const bool full_output)
    : behaviours_(behaviours),
      beliefs_(beliefs),
      agents_(agents),
      start_time_(start_time),
      end_time_(end_time),
      output_stream_(std::move(output_stream)),
      full_output_(full_output) {}
const std::vector<std::shared_ptr<Behaviour>>& Configuration::getBehaviours()
    const {
  return behaviours_;
}
const std::vector<std::shared_ptr<Belief>>& Configuration::getBeliefs() const {
  return beliefs_;
}
const std::vector<std::shared_ptr<Agent>>& Configuration::getAgents() const {
  return agents_;
}
uint_fast32_t Configuration::getStartTime() const {
  return start_time_;
}
uint_fast32_t Configuration::getEndTime() const {
  return end_time_;
}
const std::unique_ptr<std::ostream>& Configuration::getOutputStream() const {
  return output_stream_;
}
bool Configuration::getFullOutput() const {
  return full_output_;
}
}  // namespace contagent