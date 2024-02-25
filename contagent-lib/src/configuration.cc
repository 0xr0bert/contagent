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