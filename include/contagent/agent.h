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
#ifndef CONTAGENT_AGENT_H
#define CONTAGENT_AGENT_H
#include "contagent/types.h"

// The weighted relationship is the relationship between two beliefs
// weighted by the activatino of the firs belief. It is used to describe
// the probability of  adopting b2 given that you already hold b1.
//
// The caller has ownership of all pointers.
double agent_weighted_relationship(const agent *a, const uint_fast32_t day,
                                   belief *b1, belief *b2);

// The context of adopting b given your activation of all the beliefs.
//
// This is the average of agent_weighted_relationship.
//
// beliefs is a GArray of belief*.
//
// The caller has ownership of all pointers.
double agent_contextualize(const agent *a, const uint_fast32_t day, belief *b,
                           const GArray *beliefs);

typedef struct _agent_action_acc {
  GHashTable *actions;
  uint_fast32_t day;
} agent_action_acc;

void agent_add_to_actions_acc(void *friend_v, void *weight_v, void *acc_v);

// Get the sum of the weights of friends, grouped by the actions they
// performed.
//
// Returns a GHashTable from behaviour* to double*, which the caller
// takes ownership of. A delete function exists on the values of
// the GHashTable.
//
// The caller has ownership of all pointers.
GHashTable *agent_get_actions_of_friends(const agent *a,
                                         const uint_fast32_t day);

typedef struct _agent_pressure_acc {
  double pressure;
  belief *belief;
} agent_pressure_acc;

void agent_add_to_pressure_acc(void *action_v, void *w_v, void *acc_v);

// The pressure towards adopting b based upon the actions of friends.
//
// This is the perception based upon their behaviour, multiplied by
// friend weights.
//
// actions_of_friends is a GHashTable from behaviour* to double*,
// which the caller has ownership of. This can be calculated using
// agent_get_actions_of_friends.
//
// The caller has ownership of all pointers.
double agent_pressure(belief *b, GHashTable *actions_of_friends);

// Calculate the change in activation towards some belief b at a given time
// and all the beliefs in the model and the actions of the friends.
//
// If the agent_pressure is negative, then this is 1 + agent_contextualise,
// otherwise it is 1 - agent_contextualise..
//
// beliefs is a GArray storing belief*.
//
// actions_of_friends is a GHashTable from behaviour* to double*,
// which the caller has ownership of. This can be calculated using
// agent_get_actions_of_friends.
//
// The caller has ownership of all pointers.
double agent_activation_change(const agent *a, const uint_fast32_t day,
                               belief *b, const GArray *beliefs,
                               GHashTable *actions_of_friends);

// Update the activation towards a belief b as the
// delta * old activation + agent_activation_change, capped at -1 and +1.
//
// beliefs is a GArray storing belief*.
//
// actions_of_friends is a GHashTable from behaviour* to double*,
// which the caller has ownership of. This can be calculated using
// agent_get_actions_of_friends.
//
// The caller has ownership of all pointers.
void agent_update_activation(agent *a, const uint_fast32_t day, belief *b,
                             const GArray *beliefs,
                             GHashTable *actions_of_friends);

// Update the activation for all the supplied beliefs for the given time.
//
// beliefs is a GArray storing belief*.
//
// The caller has ownership of all pointers.
void agent_update_activation_for_all_beliefs(agent *a, const uint_fast32_t day,
                                             const GArray *beliefs);

gboolean
agent_filter_negative_probabilities(__attribute__((unused)) void *key,
                                    void *value_v,
                                    __attribute__((unused)) void *data);

void agent_calculate_normalizing_factor(__attribute__((unused)) void *key,
                                        void *value_v, void *acc_v);

typedef struct _agent_chosen_behaviour_acc {
  behaviour *behaviour;
  double rv;
  double weight;
} agent_chosen_behaviour_acc;

void agent_choose_behaviour_acc(void *beh_v, void *prob_v, void *acc_v);

// Perform an action at a given thime, given all the beliefs and behaviours in
// the simulation.
//
// If no potential behaviour is felt positively about, the least-bad option is
// chosen.
//
// If one potential behaviour is felt positively about, then this is chosen.
//
// If there are multiple behavioours which are felt positively, then this is
// chosen randomly proportional to how positively they are felt.
//
// behaviours is a GArray storing behaviour*.
//
// beliefs is a GArray storing belief*.
//
// The caller has ownership of all pointers.
void agent_perform_action(const agent *a, uint_fast32_t day, GArray *behaviours,
                          GArray *beliefs);
void agent_calculate_probablities(const uint_fast32_t n_behaviours,
                                  GArray *behaviours,
                                  const uint_fast32_t n_beliefs,
                                  GArray *beliefs, const agent *a,
                                  uint_fast32_t day, double *max_v,
                                  behaviour **max_beh);
#endif // CONTAGENT_AGENT_H
