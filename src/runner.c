/*
Copyright (c) 2024, Robert Greener

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
*/
#include "contagent/runner.h"

#include "contagent/agent.h"
#include "contagent/util.h"

void runner_perceive_beliefs(configuration *c, uint_fast32_t day) {
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    agent_update_activation_for_all_beliefs(a, day, c->beliefs);
  }
}

void runner_perform_actions(configuration *c, uint_fast32_t day) {
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    agent_perform_action(a, day, c->behaviours, c->beliefs);
  }
}

void runner_tick(configuration *c, uint_fast32_t day) {
  logger(day, "Perceiving beliefs");
  runner_perceive_beliefs(c, day);
  logger(day, "Performing actions");
  runner_perform_actions(c, day);
}

void runner_tick_between(configuration *c, uint_fast32_t start,
                         uint_fast32_t end) {
  for (uint_fast32_t i = start; i < end; ++i) {
    runner_tick(c, i);
  }
}

void runner_run(configuration *c) {
  char *template = "Starting simulation: {"
                   "\"start\":%lu, "
                   "\"end\":%lu, "
                   "\"n_beliefs\":%lu, "
                   "\"n_behaviours\":%lu, "
                   "\"n_agents\":%lu";
  size_t len = snprintf(NULL, 0, template, c->start_time, c->end_time,
                        c->beliefs->len, c->behaviours->len, c->agents->len) +
               1;

  char *log_str = malloc(sizeof(char) * len);
  snprintf(log_str, len, template, c->start_time, c->end_time, c->beliefs->len,
           c->behaviours->len, c->agents->len);

  logger(0, log_str);
  free(log_str);

  runner_tick_between(c, c->start_time, c->end_time);

  logger(c->end_time, "Simulation complete; serializing output");

  if (c->full_output) {
    // TODO: Serialize full output.
  } else {
    // TODO: Serialize summary output.
  }
}
