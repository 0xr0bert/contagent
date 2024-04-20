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

#include "contagent.h"

#include <stdio.h>
#include <stdlib.h>

void logger(const unsigned int day, const char *message) {
  printf("[time: %u]: %s\n", day, message);
}

double agent_weighted_relationship(const agent *a, const unsigned int day,
                                   belief *b1, belief *b2) {
  GHashTable *activations = a->activations[day];

  if (activations == NULL) {
    return 0.0;
  } else {
    double *relationship = g_hash_table_lookup(b1->relationships, b2);
    if (relationship == NULL) {
      return 0.0;
    } else {
      double *activation = g_hash_table_lookup(activations, b1);
      if (activation == NULL) {
        return 0.0;
      } else {
        return *activation * *relationship;
      }
    }
  }
}

double agent_contextualize(const agent *a, const unsigned int day, belief *b,
                           const GArray *beliefs) {
  const int n_beliefs = beliefs->len;
  if (n_beliefs == 0) {
    return 0.0;
  } else {
    double context = 0.0;
    for (int i = 0; i < n_beliefs; ++i) {
      belief *b2 = g_array_index(beliefs, belief *, i);
      context += agent_weighted_relationship(a, day, b, b2);
    }
    return context;
  }
}

typedef struct action_acc {
  GHashTable *actions;
  unsigned int day;
} action_acc;

void add_to_actions(void *friend_v, void *weight_v, void *acc_v) {
  const agent *friend = (agent *)friend_v;
  const double *weight = (double *)weight_v;
  action_acc *acc = (action_acc *)acc_v;
  behaviour *action = friend->actions[acc->day];
  const double *old_val = g_hash_table_lookup(acc->actions, action);
  double *new_val = malloc(sizeof(double));
  *new_val = (old_val == NULL ? 0.0 : *old_val) + *weight;
  g_hash_table_replace(acc->actions, action, new_val);
}

void free_double(void *v) { free((double *)v); }

GHashTable *agent_get_actions_of_friends(const agent *a,
                                         const unsigned int day) {
  GHashTable *actions = g_hash_table_new_full(NULL, NULL, NULL, free_double);

  action_acc acc;
  acc.actions = actions;
  acc.day = day;

  g_hash_table_foreach(a->friends, add_to_actions, &acc);

  return actions;
}

typedef struct pressure_acc {
  double pressure;
  belief *belief;
} pressure_acc;

void add_to_pressure(void *action_v, void *w_v, void *acc_v) {
  behaviour *action = (behaviour *)action_v;
  double *w = (double *)w_v;
  pressure_acc *acc = (pressure_acc *)acc_v;

  double *perception = g_hash_table_lookup(acc->belief->perceptions, action);
  if (perception == NULL) {
    exit(-1);
  }

  acc->pressure += *perception * *w;
}

double agent_pressure(const agent *a, belief *b,
                      GHashTable *actions_of_friends) {
  unsigned int size = g_hash_table_size(actions_of_friends);

  pressure_acc acc;
  acc.pressure = 0.0;
  acc.belief = b;

  if (size == 0) {
    return 0.0;
  } else {
    g_hash_table_foreach(actions_of_friends, add_to_pressure, &acc);
  }

  return acc.pressure;
}

double agent_activation_change(const agent *a, const unsigned int day,
                               belief *b, const GArray *beliefs,
                               GHashTable *actions_of_friends) {
  double pressure = agent_pressure(a, b, actions_of_friends);

  if (pressure >= 0.0) {
    return 1.0 + agent_contextualize(a, day, b, beliefs);
  } else {
    return 1.0 - agent_contextualize(a, day, b, beliefs);
  }
}

void agent_update_activation(agent *a, const unsigned int day, belief *b,
                             const GArray *beliefs,
                             GHashTable *actions_of_friends) {
  double *delta = g_hash_table_lookup(a->deltas, b);
  if (delta == NULL) {
    exit(-2);
  }
  GHashTable *activations = a->activations[day - 1];
  if (activations == NULL) {
    exit(-3);
  }
  double *activation = g_hash_table_lookup(activations, b);
  if (activation == NULL) {
    exit(-4);
  }

  double activation_change =
      agent_activation_change(a, day, b, beliefs, actions_of_friends);
  double *new_activation = malloc(sizeof(double));
  *new_activation =
      MAX(-1.0, MIN(1.0, *delta * *activation + activation_change));

  if (a->activations[day] == NULL) {
    a->activations[day] = g_hash_table_new_full(NULL, NULL, NULL, free_double);
  }

  g_hash_table_replace(a->activations[day], b, new_activation);
}

void agent_update_activation_for_all_beliefs(agent *a, const unsigned int day,
                                             const GArray *beliefs) {
  GHashTable *actions_of_friends = agent_get_actions_of_friends(a, day);
  const unsigned int n_beliefs = beliefs->len;
  for (int i = 0; i < n_beliefs; ++i) {
    agent_update_activation(a, day, g_array_index(beliefs, belief *, i),
                            beliefs, actions_of_friends);
  }
  g_hash_table_unref(actions_of_friends);
}

gboolean filter_negative_probabilities(void *key, void *value_v, void *data) {
  double *value = (double *)value_v;

  return *value >= 0.0;
}

void calculate_normalizing_factor(void *key, void *value_v, void *acc_v) {
  double *acc = (double *)acc_v;
  double *value = (double *)value_v;
  *acc += *value;
}

typedef struct chosen_behaviour_acc {
  behaviour *behaviour;
  double rv;
  double weight;
} chosen_behaviour_acc;

void choose_behaviour(void *beh_v, void *prob_v, void *acc_v) {
  behaviour *beh = (behaviour *)beh_v;
  double *prob = (double *)prob_v;
  if (prob == NULL) {
    exit(-9);
  } else {
    chosen_behaviour_acc *acc = (chosen_behaviour_acc *)acc_v;
    if (acc->behaviour == NULL) {
      return;
    } else {
      acc->rv -= *prob / acc->weight;

      if (acc->rv <= 0.0) {
        acc->behaviour = beh;
      }
    }
  }
}

void agent_perform_action(const agent *a, unsigned int day, GArray *behaviours,
                          GArray *beliefs) {
  GHashTable *probabilities =
      g_hash_table_new_full(NULL, NULL, NULL, free_double);

  behaviour *max_beh = NULL;
  double max_v = DBL_MIN;
  const unsigned int n_behaviours = behaviours->len;
  const unsigned int n_beliefs = beliefs->len;

  for (int i = 0; i < n_behaviours; i++) {
    behaviour *beh = g_array_index(behaviours, behaviour *, i);
    double v = 0;
    for (int j = 0; j < n_beliefs; j++) {
      belief *bel = g_array_index(beliefs, belief *, i);
      GHashTable *prs_for_belief =
          g_hash_table_lookup(a->performance_relationships, bel);
      if (prs_for_belief == NULL) {
        exit(-5);
      } else {
        double *prs = g_hash_table_lookup(prs_for_belief, beh);
        if (prs == NULL) {
          exit(-6);
        } else {
          GHashTable *acts = a->activations[day];
          if (acts == NULL) {
            exit(-7);
          } else {
            double *act = g_hash_table_lookup(acts, bel);
            if (act == NULL) {
              exit(-8);
            } else {
              v += *act * *prs;
            }
          }
        }
      }
    }

    if (v > max_v || max_beh == NULL) {
      max_beh = beh;
      max_v = v;
    }
  }

  if (max_v < 0.0) {
    a->actions[day] = max_beh;
  } else {
    g_hash_table_foreach_remove(probabilities, filter_negative_probabilities,
                                NULL);

    if (g_hash_table_size(probabilities) == 1) {
      behaviour **chosen =
          (behaviour **)g_hash_table_get_keys_as_array(probabilities, NULL);
      a->actions[day] = chosen[0];
      free(chosen);
    } else {
      double acc = 0.0;

      g_hash_table_foreach(probabilities, calculate_normalizing_factor, &acc);

      chosen_behaviour_acc beh_acc;
      beh_acc.behaviour = NULL;
      beh_acc.rv = rand();
      beh_acc.weight = acc;
      g_hash_table_foreach(probabilities, choose_behaviour, &beh_acc);
      a->actions[day] = beh_acc.behaviour;
    }
  }
}

void perceive_beliefs(configuration *c, unsigned int day) {
  for (unsigned int i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    agent_update_activation_for_all_beliefs(a, day, c->beliefs);
  }
}

void perform_actions(configuration *c, unsigned int day) {
  for (unsigned int i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    agent_perform_action(a, day, c->behaviours, c->beliefs);
  }
}

void tick(configuration *c, unsigned int day) {
  logger(day, "Perceiving beliefs");
  perceive_beliefs(c, day);
  logger(day, "Performing actions");
  perform_actions(c, day);
}

void tick_between(configuration *c, unsigned int start, unsigned int end) {
  for (unsigned int i = start; i < end; ++i) {
    tick(c, i);
  }
}