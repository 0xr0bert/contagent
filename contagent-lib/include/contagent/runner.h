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

#ifndef CONTAGENT_RUNNER_H
#define CONTAGENT_RUNNER_H

#include <memory>

#include "configuration.h"
namespace contagent {
/// Runner does as it suggests -- runs the simulation.
/// \author Robert Greener
class Runner {
public:
  /// Create a new Runner with a supplied Configuration, which the Runner owns.
  /// \author Robert Greener
  explicit Runner(std::unique_ptr<Configuration> configuration);

  /// For every Agent in the Configuration call
  /// Agent::updateActivationsForAllBeliefs.
  /// \author Robert Greener
  void perceiveBeliefs(uint_fast32_t time);

  /// For every Agent in the Configuration call Agent::performAction.
  /// \author Robert Greener
  void performActions(uint_fast32_t time);

  /// Tick for a given time, calls ::perceiveBeliefs followed by
  /// ::performActions.
  /// \author Robert Greener
  void tick(uint_fast32_t time);

  /// Tick between the start_time (inclusive) and end_time (exclusive), calling
  /// ::tick.
  /// \author Robert Greener
  void tickBetween(uint_fast32_t start_time, uint_fast32_t end_time);

  /// Tick between the Configuration::getStartTime (inclusive) and
  /// Configuration::getEndTime (exclusive), serializing the output at the end.
  /// \author Robert Greener
  void run();

private:
  /// The Configuration of the simulation run.
  std::unique_ptr<Configuration> configuration_;
};
} // namespace contagent

#endif // CONTAGENT_RUNNER_H
