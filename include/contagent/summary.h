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
/// Summary statistics for the contagent simulation library.
/// @file summary.h
/// @author Robert Greener
#ifndef CONTAGENT_SUMMARY_H
#define CONTAGENT_SUMMARY_H

#include "contagent/types.h"
#include <math.h>

void summary_sum_activation(void *key, void *data, void *acc);

void summary_divide_activation(__attribute__((unused)) void *key, void *data,
                               void *div);

/// @brief Calculate the mean activation towards each belief at the time.
/// @param c The configuration, which the caller has ownership of.
/// @param time The time.
/// @return A GHashTable from belief* to double*, which the caller takes
/// ownership of. A delete function exists on the values of the GHashTable.
/// @author Robert Greener
GHashTable *summary_calculate_mean_activation(configuration *c,
                                              uint_fast32_t time);

/// @brief The accumulator for calculating the standard deviation of belief
/// activation.
/// @author Robert Greener
typedef struct _summary_sd_acc {
  /// @brief A GHashTable * from belief * to the current summed squared
  /// difference from the mean (double *).
  GHashTable *sd_sums;

  /// @brief A GHashTable * from belief * to the mean activation (*double).
  /// This can be calculated using
  /// summary_calculate_mean_activation(configuration *, uint_fast32_ time).
  GHashTable *means;
} summary_sd_acc;

/// @brief Sum the squared difference from the mean for belief activation.
/// @param key A belief *.
/// @param data The double * activation of the belief.
/// @param acc The summary_sd_acc;
void summary_sum_activation_sq_diff(void *key, void *data, void *acc);

void calculate_sd_for_each(__attribute__((unused)) void *key, void *data,
                           void *n_v);

// Get the sample standard deviation activation towards each belief at the time.
//
// mean_activations is a map from belief * to double * with the mean activation
// of that belief. This can be calculated using
// summary_calculate_mean_activation(configuration *, uint_fast32_ time).
//
// Returns a GHashTable from belief* to double*, which the caller
// takes ownership of. A delete function exists on the values of
// the GHashTable.
//
// The caller has ownership of all pointers.
GHashTable *summary_calculate_sd_activation(configuration *c,
                                            uint_fast32_t time,
                                            GHashTable *mean_activations);

void summary_add_to_activations_by_belief(void *key, void *data, void *acc);

// Get the median activation towards each belief at the time.
//
// Returns a GHashTable from belief* to double*, which the caller
// takes ownership of. A delete function exists on the values of
// the GHashTable.
//
// The caller has ownership of all pointers.
GHashTable *summary_calculate_median_activation(configuration *c,
                                                uint_fast32_t time);

void summary_add_to_nonzero_activation(void *key, void *data, void *acc);

// Get the number of agents who had non-zero activation towards each belief at
// the time.
//
// Returns a GHashTable from belief* to uint_fast32_t*, which the caller
// takes ownership of. A delete function exists on the values of
// the GHashTable.
//
// The caller has ownership of all pointers.
GHashTable *summary_calculate_nonzero_activation(configuration *c,
                                                 uint_fast32_t time);

// Get the number of agents who performed each behaviour at the time.
//
// Returns a GHashTable from behaviour* to uint_fast32_t*, which the caller
// takes ownership of. A delete function exists on the values of
// the GHashTable.
//
// The caller has ownership of all pointers.
GHashTable *summary_calculate_n_performers(configuration *c,
                                           uint_fast32_t time);

summary_stats *summary_stats_new(uint_fast32_t n_days);

summary_stats *summary_calculate_stats(configuration *c, uint_fast32_t n_days);

void free_summary_stats(summary_stats *s);

#endif
