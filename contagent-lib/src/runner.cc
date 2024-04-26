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
#include "contagent/json/agent_spec.h"
#include "contagent/json/summary_spec.h"
#include "contagent/summary.h"
#include <glog/logging.h>
#include <iostream>

namespace contagent {
Runner::Runner(std::unique_ptr<Configuration> configuration)
    : configuration_(std::move(configuration)) {}

void Runner::perceive_beliefs(const uint_fast32_t time) {
  for (auto &a : configuration_->get_agents()) {
    a->update_activation_for_all_beliefs(time, configuration_->get_beliefs());
  }
}

void Runner::perform_actions(const uint_fast32_t time) {
  for (auto &a : configuration_->get_agents()) {
    a->perform_action(time, configuration_->get_behaviours(),
                      configuration_->get_beliefs());
  }
}
void Runner::tick(const uint_fast32_t time) {
  LOG(INFO) << "[time=" << time << "] Perceiving beliefs";
  perceive_beliefs(time);
  LOG(INFO) << "[time=" << time << "] Performing actions";
  perform_actions(time);
}
void Runner::tick_between(const uint_fast32_t start_time,
                          const uint_fast32_t end_time) {
  for (uint_fast32_t i = start_time; i < end_time; ++i) {
    tick(i);
  }
}
void Runner::run() {
  LOG(INFO) << "Starting simulation {\"start\":"
            << configuration_->get_start_time()
            << ",\"end\":" << configuration_->get_end_time()
            << ",\"nBeliefs\":" << configuration_->get_beliefs().size()
            << ",\"nBehaviours\":" << configuration_->get_behaviours().size()
            << ",\"nAgents\":" << configuration_->get_agents().size() << "}";
  tick_between(configuration_->get_start_time(),
               configuration_->get_end_time());
  LOG(INFO) << "Simulation complete; serializing output";

  if (configuration_->get_full_output()) {
    serialize_and_output_full();
  } else {
    serialize_and_output_summary();
  }
  LOG(INFO) << "Output successfully serialized";
}

void Runner::serialize_and_output_summary() {
  std::vector<contagent::json::SummarySpec> summary_stats(
      configuration_->get_end_time());
  for (uint_fast32_t i = configuration_->get_start_time();
       i < configuration_->get_end_time(); ++i) {
    auto stats =
        contagent::summary::calculate_summary_stats(*configuration_, i);
    summary_stats[i] = json::SummarySpec(*stats);
  }

  nlohmann::json stats = summary_stats;
  *configuration_->get_output_stream() << stats;
}

void Runner::serialize_and_output_full() {
  std::vector<contagent::json::AgentSpec> agent_specs(
      configuration_->get_end_time());

  for (uint_fast32_t i = configuration_->get_start_time();
       i < configuration_->get_end_time(); ++i) {
    agent_specs[i] = json::AgentSpec(*configuration_->get_agents()[i]);
  }

  nlohmann::json j = agent_specs;
  *configuration_->get_output_stream() << j;
}
} // namespace contagent
