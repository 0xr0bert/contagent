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
