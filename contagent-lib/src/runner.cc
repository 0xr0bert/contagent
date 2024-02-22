//
// Created by rober on 21/02/2024.
//

#include "contagent/runner.h"
#include <glog/logging.h>
#include <iostream>

namespace contagent {
Runner::Runner(std::unique_ptr<Configuration> configuration)
    : configuration_(std::move(configuration)) {}

void Runner::perceiveBeliefs(const uint_fast32_t time) {
  for (auto &a : configuration_->getAgents()) {
    a->updateActivationsForAllBeliefs(time, configuration_->getBeliefs());
  }
}

void Runner::performActions(const uint_fast32_t time) {
  for (auto &a : configuration_->getAgents()) {
    a->performAction(time, configuration_->getBehaviours(),
                     configuration_->getBeliefs());
  }
}
void Runner::tick(const uint_fast32_t time) {
  LOG(INFO) << "[time=" << time << "] Perceiving beliefs";
  perceiveBeliefs(time);
  LOG(INFO) << "[time=" << time << "] Performing actions";
  performActions(time);
}
void Runner::tickBetween(const uint_fast32_t start_time,
                         const uint_fast32_t end_time) {
  for (uint_fast32_t i = start_time; i < end_time; ++i) {
    tick(i);
  }
}
void Runner::run() {
  LOG(INFO) << "Starting simulation {\"start\":"
            << configuration_->getStartTime()
            << ",\"end\":" << configuration_->getEndTime()
            << ",\"nBeliefs\":" << configuration_->getBeliefs().size()
            << ",\"nBehaviours\":" << configuration_->getBehaviours().size()
            << ",\"nAgents\":" << configuration_->getAgents().size() << "}";
  tickBetween(configuration_->getStartTime(), configuration_->getEndTime());
  LOG(INFO) << "Simulation complete; serializing output";

  if (configuration_->getFullOutput()) {
    // TODO: serialize full output
  } else {
    // TODO: serialize summary output
  }
}
} // namespace contagent