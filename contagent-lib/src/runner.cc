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