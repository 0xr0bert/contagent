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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void random_uuid_v4(unsigned char *uuid_to_set) {
  for (uint_fast8_t i = 0; i < 16; ++i) {
    uuid_to_set[i] = rand() & 0xFF; // Get last byte of random number
  }

  // Set 4 most significant bits of time_hi_and_version: 0100 (4)
  uuid_to_set[6] = (uuid_to_set[6] & 0x0F) | 0x4F;

  // Set 2 most significant bits of clock_seq_hi_and_reserved to 0 and 1
  // respectively.
  uuid_to_set[8] = (uuid_to_set[8] & 0x3F) | 0x7F;
}

void logger(const uint_fast32_t day, const char *message) {
  printf("[time: %lu]: %s\n", day, message);
}

double agent_weighted_relationship(const agent *a, const uint_fast32_t day,
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

double agent_contextualize(const agent *a, const uint_fast32_t day, belief *b,
                           const GArray *beliefs) {
  const uint32_t n_beliefs = beliefs->len;
  if (n_beliefs == 0) {
    return 0.0;
  } else {
    double context = 0.0;
    for (uint_fast32_t i = 0; i < n_beliefs; ++i) {
      belief *b2 = g_array_index(beliefs, belief *, i);
      context += agent_weighted_relationship(a, day, b, b2);
    }
    return context;
  }
}

typedef struct _action_acc {
  GHashTable *actions;
  uint_fast32_t day;
} action_acc;

void add_to_actions(void *friend_v, void *weight_v, void *acc_v) {
  const agent *friend = (agent *)friend_v;
  const double *weight = (double *)weight_v;
  action_acc *acc = (action_acc *)acc_v;
  behaviour *action = friend->actions[acc->day];
  double *val = g_hash_table_lookup(acc->actions, action);
  if (val == NULL) {
    val = malloc(sizeof(double));
    *val = 0.0;
  }
  *val += *weight;
}

static void free_double(void *v) { free((double *)v); }

GHashTable *agent_get_actions_of_friends(const agent *a,
                                         const uint_fast32_t day) {
  GHashTable *actions = g_hash_table_new_full(NULL, NULL, NULL, free_double);

  action_acc acc;
  acc.actions = actions;
  acc.day = day;

  g_hash_table_foreach(a->friends, add_to_actions, &acc);

  return actions;
}

typedef struct _pressure_acc {
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

double agent_pressure(belief *b, GHashTable *actions_of_friends) {
  uint_fast32_t size = g_hash_table_size(actions_of_friends);

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

double agent_activation_change(const agent *a, const uint_fast32_t day,
                               belief *b, const GArray *beliefs,
                               GHashTable *actions_of_friends) {
  double pressure = agent_pressure(b, actions_of_friends);

  if (pressure >= 0.0) {
    return 1.0 + agent_contextualize(a, day, b, beliefs);
  } else {
    return 1.0 - agent_contextualize(a, day, b, beliefs);
  }
}

void agent_update_activation(agent *a, const uint_fast32_t day, belief *b,
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

void agent_update_activation_for_all_beliefs(agent *a, const uint_fast32_t day,
                                             const GArray *beliefs) {
  GHashTable *actions_of_friends = agent_get_actions_of_friends(a, day);
  const uint_fast32_t n_beliefs = beliefs->len;
  for (uint_fast32_t i = 0; i < n_beliefs; ++i) {
    agent_update_activation(a, day, g_array_index(beliefs, belief *, i),
                            beliefs, actions_of_friends);
  }
  g_hash_table_unref(actions_of_friends);
}

gboolean filter_negative_probabilities(__attribute__((unused)) void *key,
                                       void *value_v,
                                       __attribute__((unused)) void *data) {
  double *value = (double *)value_v;

  return *value >= 0.0;
}

void calculate_normalizing_factor(__attribute__((unused)) void *key,
                                  void *value_v, void *acc_v) {
  double *acc = (double *)acc_v;
  double *value = (double *)value_v;
  *acc += *value;
}

typedef struct _chosen_behaviour_acc {
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

void agent_perform_action(const agent *a, uint_fast32_t day, GArray *behaviours,
                          GArray *beliefs) {
  GHashTable *probabilities =
      g_hash_table_new_full(NULL, NULL, NULL, free_double);

  behaviour *max_beh = NULL;
  double max_v = DBL_MIN;
  const uint_fast32_t n_behaviours = behaviours->len;
  const uint_fast32_t n_beliefs = beliefs->len;

  for (uint_fast32_t i = 0; i < n_behaviours; i++) {
    behaviour *beh = g_array_index(behaviours, behaviour *, i);
    double v = 0;
    for (uint_fast32_t j = 0; j < n_beliefs; j++) {
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

void perceive_beliefs(configuration *c, uint_fast32_t day) {
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    agent_update_activation_for_all_beliefs(a, day, c->beliefs);
  }
}

void perform_actions(configuration *c, uint_fast32_t day) {
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    agent_perform_action(a, day, c->behaviours, c->beliefs);
  }
}

void tick(configuration *c, uint_fast32_t day) {
  logger(day, "Perceiving beliefs");
  perceive_beliefs(c, day);
  logger(day, "Performing actions");
  perform_actions(c, day);
}

void tick_between(configuration *c, uint_fast32_t start, uint_fast32_t end) {
  for (uint_fast32_t i = start; i < end; ++i) {
    tick(c, i);
  }
}

void run(configuration *c) {
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

  tick_between(c, c->start_time, c->end_time);

  logger(c->end_time, "Simulation complete; serializing output");

  if (c->full_output) {
    // TODO: Serialize full output.
  } else {
    // TODO: Serialize summary output.
  }
}

static void free_uint_fast32_t(void *v) { free((uint_fast32_t *)v); }

static void sum_activation(void *key, void *data, void *acc) {
  belief *b = (belief *)key;
  double *act = (double *)data;
  GHashTable *summed_act_ght = (GHashTable *)acc;
  double *summed_act = g_hash_table_lookup(summed_act_ght, b);

  if (summed_act == NULL) {
    summed_act = malloc(sizeof(double));
    *summed_act = 0.0;
    g_hash_table_replace(summed_act_ght, b, summed_act);
  }

  if (act != NULL && *act != 0.0) {
    *summed_act += *act;
  }
}

static void divide_activation(__attribute__((unused)) void *key, void *data,
                              void *div) {
  double *act = (double *)data;
  uint_fast32_t *divisor = (uint_fast32_t *)div;

  if (act != NULL && *act != 0.0) {
    *act /= *divisor;
  }
}

GHashTable *config_calculate_mean_activation(configuration *c,
                                             uint_fast32_t time) {
  GHashTable *ght = g_hash_table_new_full(NULL, NULL, NULL, free_double);
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *acts = a->activations[time];
    g_hash_table_foreach(acts, sum_activation, ght);
  }

  g_hash_table_foreach(ght, divide_activation, &c->agents->len);

  return ght;
}

typedef struct _sd_acc {
  GHashTable *sd_sums;
  GHashTable *means;
} sd_acc;

static void sum_activation_sq_diff(void *key, void *data, void *acc) {
  belief *b = (belief *)key;
  double *act = (double *)data;
  sd_acc *s = (sd_acc *)acc;
  double *summed_sq_diff = g_hash_table_lookup(s->sd_sums, b);

  if (summed_sq_diff == NULL) {
    summed_sq_diff = malloc(sizeof(double));
    *summed_sq_diff = 0.0;
    g_hash_table_replace(s->sd_sums, b, summed_sq_diff);
  }

  double *mean = g_hash_table_lookup(s->means, b);
  double mean_v = mean == NULL ? 0.0 : *mean;

  double act_v = act == NULL ? 0.0 : *act;
  *summed_sq_diff += pow((act_v - mean_v), 2);
}

static void calculate_sd_for_each(__attribute__((unused)) void *key, void *data,
                                  void *n_v) {
  double *v = data;
  uint_fast64_t *n = n_v;

  *v = sqrt(*v / (*n - 1));
}

GHashTable *config_calculate_sd_activation(configuration *c, uint_fast32_t time,
                                           GHashTable *mean_activations) {
  GHashTable *sds = g_hash_table_new_full(NULL, NULL, NULL, free_double);

  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *acts = a->activations[time];
    sd_acc acc;
    acc.sd_sums = sds;
    acc.means = mean_activations;
    g_hash_table_foreach(acts, sum_activation_sq_diff, &acc);
  }

  g_hash_table_foreach(sds, calculate_sd_for_each, &c->agents->len);

  return sds;
}

static void free_garray(void *v) {
  GArray *a = (GArray *)v;
  g_array_unref(a);
}

static void add_to_activations_by_belief(void *key, void *data, void *acc) {
  belief *b = (belief *)key;
  double *act = (double *)data;
  GHashTable *acts_by_beliefs = (GHashTable *)acc;
  double actual_act = act == NULL ? 0.0 : *act;

  GArray *acts = g_hash_table_lookup(acts_by_beliefs, b);

  if (acts == NULL) {
    exit(-10);
  } else {
    g_array_append_val(acts, actual_act);
  }
}

static int double_cmp(const void *d1_v, const void *d2_v) {
  double *d1 = (double *)d1_v;
  double *d2 = (double *)d2_v;
  if (d1 < d2) {
    return -1;
  } else if (d1 == d2) {
    return 0;
  } else {
    return 1;
  }
}

GHashTable *config_calculate_median_activation(configuration *c,
                                               uint_fast32_t time) {
  GHashTable *activations_by_belief =
      g_hash_table_new_full(NULL, NULL, NULL, free_garray);
  for (uint_fast32_t i = 0; i < c->beliefs->len; ++i) {
    belief *b = g_array_index(c->beliefs, belief *, i);
    GArray *acts =
        g_array_sized_new(FALSE, TRUE, sizeof(double), c->agents->len);
    g_hash_table_replace(activations_by_belief, b, acts);
  }

  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *activations = a->activations[time];
    g_hash_table_foreach(activations, add_to_activations_by_belief,
                         activations_by_belief);
  }

  GHashTable *median_activations =
      g_hash_table_new_full(NULL, NULL, NULL, free_double);

  uint_fast32_t ix = c->agents->len / 2;

  for (uint_fast32_t i = 0; i < c->beliefs->len; ++i) {
    belief *b = g_array_index(c->beliefs, belief *, i);
    GArray *acts = g_hash_table_lookup(activations_by_belief, b);
    g_array_sort(acts, double_cmp);

    double median = g_array_index(acts, double, ix);

    if (c->agents->len % 2 == 0) {
      median = (median + g_array_index(acts, double, ix + 1)) / 2.0;
    }

    double *median_heaped = malloc(sizeof(double));
    *median_heaped = median;
    g_hash_table_replace(median_activations, b, median_heaped);
  }

  g_hash_table_unref(activations_by_belief);
  return median_activations;
}

static void add_to_nonzero_activation(void *key, void *data, void *acc) {
  belief *b = (belief *)key;
  double *act = (double *)data;
  GHashTable *nonzero_m = (GHashTable *)acc;

  if (act != NULL && *act != 0.0) {
    uint_fast32_t *v = g_hash_table_lookup(nonzero_m, b);
    if (v == NULL) {
      v = malloc(sizeof(uint_fast32_t));
      *v = 0;
      g_hash_table_replace(nonzero_m, b, v);
    }
    ++(*v);
  }
}

GHashTable *config_calculate_nonzero_activation(configuration *c,
                                                uint_fast32_t time) {
  GHashTable *ght = g_hash_table_new_full(NULL, NULL, NULL, free_uint_fast32_t);
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *activations = a->activations[time];
    g_hash_table_foreach(activations, add_to_nonzero_activation, ght);
  }

  return ght;
}

GHashTable *config_calculate_n_performers(configuration *c,
                                          uint_fast32_t time) {
  GHashTable *ght = g_hash_table_new_full(NULL, NULL, NULL, free_uint_fast32_t);
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    uint_fast32_t *val = g_hash_table_lookup(ght, a->actions[time]);
    if (val == NULL) {
      val = malloc(sizeof(uint_fast32_t));
      *val = 0;
      g_hash_table_replace(ght, a->actions[time], val);
    }
    ++(*val);
  }

  return ght;
}
