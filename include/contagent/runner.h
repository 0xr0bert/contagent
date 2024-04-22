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
#ifndef CONTAGENT_RUNNER_H
#define CONTAGENT_RUNNER_H

#include "contagent/types.h"

// Perceive beliefs for all agents.
//
// The caller has ownership of all pointers.
void runner_perceive_beliefs(configuration *c, uint_fast32_t day);

// Perform actions for all agents.
//
// The caller has ownership of all pointers.
void runner_perform_actions(configuration *c, uint_fast32_t day);

// Tick for the time.
//
// Perceives beliefs then performs actions.
//
// The caller has ownership of all pointers.
void runner_tick(configuration *c, uint_fast32_t day);

// Tick between the start time (inclusive) and end time (exclusive).
//
// The caller has ownership of all pointers.
void runner_tick_between(configuration *c, uint_fast32_t start,
                         uint_fast32_t end);

// Run the simulation given some configuration.
void runner_run(configuration *c);

#endif // CONTAGENT_RUNNER_H
