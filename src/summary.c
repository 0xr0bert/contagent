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
#include "contagent/summary.h"
#include "contagent/util.h"

#include <math.h>

void summary_sum_activation(void *key, void *data, void *acc) {
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

void summary_divide_activation(__attribute__((unused)) void *key, void *data,
                               void *div) {
  double *act = (double *)data;
  uint_fast32_t *divisor = (uint_fast32_t *)div;

  if (act != NULL && *act != 0.0) {
    *act /= *divisor;
  }
}

GHashTable *summary_calculate_mean_activation(configuration *c,
                                              uint_fast32_t time) {
  GHashTable *ght = g_hash_table_new_full(NULL, NULL, NULL, free_double);
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *acts = a->activations[time];
    g_hash_table_foreach(acts, summary_sum_activation, ght);
  }

  g_hash_table_foreach(ght, summary_divide_activation, &c->agents->len);

  return ght;
}

void summary_sum_activation_sq_diff(void *key, void *data, void *acc) {
  belief *b = (belief *)key;
  double *act = (double *)data;
  summary_sd_acc *s = (summary_sd_acc *)acc;
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

void calculate_sd_for_each(__attribute__((unused)) void *key, void *data,
                           void *n_v) {
  double *v = data;
  uint_fast64_t *n = n_v;

  *v = sqrt(*v / (*n - 1));
}

GHashTable *summary_calculate_sd_activation(configuration *c,
                                            uint_fast32_t time,
                                            GHashTable *mean_activations) {
  GHashTable *sds = g_hash_table_new_full(NULL, NULL, NULL, free_double);

  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *acts = a->activations[time];
    summary_sd_acc acc;
    acc.sd_sums = sds;
    acc.means = mean_activations;
    g_hash_table_foreach(acts, summary_sum_activation_sq_diff, &acc);
  }

  g_hash_table_foreach(sds, calculate_sd_for_each, &c->agents->len);

  return sds;
}

void summary_add_to_activations_by_belief(void *key, void *data, void *acc) {
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

GHashTable *summary_calculate_median_activation(configuration *c,
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
    g_hash_table_foreach(activations, summary_add_to_activations_by_belief,
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

void summary_add_to_nonzero_activation(void *key, void *data, void *acc) {
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

GHashTable *summary_calculate_nonzero_activation(configuration *c,
                                                 uint_fast32_t time) {
  GHashTable *ght = g_hash_table_new_full(NULL, NULL, NULL, free_uint_fast32_t);
  for (uint_fast32_t i = 0; i < c->agents->len; ++i) {
    agent *a = g_array_index(c->agents, agent *, i);
    GHashTable *activations = a->activations[time];
    g_hash_table_foreach(activations, summary_add_to_nonzero_activation, ght);
  }

  return ght;
}

GHashTable *summary_calculate_n_performers(configuration *c,
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

#define malloc_ght_array(store_loc, len)                                       \
  store_loc = malloc(sizeof(GHashTable *) * len);

summary_stats *summary_stats_new(uint_fast32_t n_days) {
  summary_stats *s = malloc(sizeof(summary_stats));
  s->n_days = n_days;
  malloc_ght_array(s->mean_activation, n_days);
  malloc_ght_array(s->sd_activation, n_days);
  malloc_ght_array(s->median_activation, n_days);
  malloc_ght_array(s->nonzero_activaton, n_days);
  malloc_ght_array(s->n_performers, n_days);
  return s;
}

summary_stats *summary_calculate_stats(configuration *c, uint_fast32_t n_days) {
  summary_stats *s = summary_stats_new(n_days);
  for (uint_fast32_t i = 0; i < n_days; ++i) {
    s->mean_activation[i] = summary_calculate_mean_activation(c, i);
    s->sd_activation[i] =
        summary_calculate_sd_activation(c, i, s->mean_activation[i]);
    s->median_activation[i] = summary_calculate_median_activation(c, i);
    s->nonzero_activaton[i] = summary_calculate_nonzero_activation(c, i);
    s->n_performers[i] = summary_calculate_n_performers(c, i);
  }
  return s;
}

void free_summary_stats(summary_stats *s) {
    for (uint_fast32_t i = 0; i < s->n_days; ++i) {
        g_hash_table_unref(s->mean_activation[i]);
        g_hash_table_unref(s->sd_activation[i]);
        g_hash_table_unref(s->median_activation[i]);
        g_hash_table_unref(s->nonzero_activaton[i]);
        g_hash_table_unref(s->n_performers[i]);
    }
    free(s->mean_activation);
    free(s->sd_activation);
    free(s->median_activation);
    free(s->nonzero_activaton);
    free(s->n_performers);
    free(s);
}
