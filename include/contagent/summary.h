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

/// @brief Sum activation into an accumulator.
/// @param key The belief * which the caller has ownership of.
/// @param data The activation (double *) which the caller has ownership of.
/// @param acc A GHashTable * from belief* to double*, which the caller has
/// ownership of. The value for the corresponding belief has *data added to it.
/// @author Robert Greener
void summary_sum_activation(void *key, void *data, void *acc);

/// @brief Divide the activation by some divisor.
/// @param key Unused belief * which the caller has ownership of.
/// @param data The (double *) summed activation which will be divided by div
/// which the caller has ownership of.
/// @param div The (double *) divisor which the caller has ownership of.
/// @author Robert Greener
void summary_divide_activation(__attribute__((unused)) void *key, void *data,
                               void *div);

/// @brief Calculate the mean activation towards each belief at the time.
/// @param c The configuration, which the caller has ownership of.
/// @param time The time.
/// @return A GHashTable * from belief* to double*, which the caller takes
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
/// @param key A belief * which the caller has ownership of.
/// @param data The double * activation of the belief which the caller has
/// ownership of.
/// @param acc The summary_sd_acc * which the caller has ownership of.
/// @author Robert Greener
void summary_sum_activation_sq_diff(void *key, void *data, void *acc);

/// @brief Calculate the standard deviation, by dividing the value by (n-1) and
/// sqrting.
/// @param key Unused belief * that the caller has ownership of.
/// @param data The summed squared differences as a double * which the caller
/// has ownership of.
/// @param n_v The number of agents as a uint_fast32_t * which the caller has
/// ownership of.
/// @author Robert Greener
void calculate_sd_for_each(__attribute__((unused)) void *key, void *data,
                           void *n_v);

/// @brief Get the sample standard deviation activation towards each belief at
/// the time.
/// @param c The configuration. The caller has ownership over this.
/// @param time The time.
/// @param mean_activations A map from belief * to double * with the mean
/// activation of that belief. This can be calculated using
/// summary_calculate_mean_activation(configuration *, uint_fast32_ time).
/// @return A GHashTable * from belief* to double*, which the caller
/// takes ownership of. A delete function exists on the values of
/// the GHashTable.
/// @author Robert Greener
GHashTable *summary_calculate_sd_activation(configuration *c,
                                            uint_fast32_t time,
                                            GHashTable *mean_activations);

/// @brief Add to the activations by belief in the acc
/// @param key The belief * which the caller has ownership of.
/// @param data The activation (double *) which the
/// caller has ownership of.
/// @param acc A GHashTable * from belief * to a GArray * of activations which
/// the caller has ownership of. The activation for the belief * is appended to
/// the appropriate GArray *.
/// @author Robert Greener
void summary_add_to_activations_by_belief(void *key, void *data, void *acc);

/// @brief Get the median activation towards each belief at the time.
/// @param c The configuration * which the caller has ownership of.
/// @param time The time.
/// @return A GHashTable * from belief* to double*, which the caller
/// takes ownership of. A delete function exists on the values of
/// the GHashTable.
/// @author Robert Greener
GHashTable *summary_calculate_median_activation(configuration *c,
                                                uint_fast32_t time);

/// @brief Add to nonzero activation
/// @param key The belief * which the caller has ownership of.
/// @param data The double * activation which the caller has ownership of.
/// @param acc A GHashTable * from belief* to uint_fast32_t* which is the
/// current count of non-zero activations, if *data is non-zero and non-NULL,
/// then this is incremented.
/// @author Robert Greener
void summary_add_to_nonzero_activation(void *key, void *data, void *acc);

/// @brief Get the number of agents who had non-zero activation towards each
/// belief at the time.
/// @param c The configuration * which the caller has ownership of.
/// @param time The time.
/// @return A GHashTable * from belief* to uint_fast32_t*, which the caller
/// takes ownership of. A delete function exists on the values of
/// the GHashTable.
/// @author Robert Greener
GHashTable *summary_calculate_nonzero_activation(configuration *c,
                                                 uint_fast32_t time);

/// @brief Get the number of agents who performed each behaviour at the time.
/// @param c The configuration * which the caller has ownership of.
/// @param time The time.
/// @return A GHashTable * from behaviour* to uint_fast32_t*, which the caller
/// takes ownership of. A delete function exists on the values of
/// the GHashTable.
/// @author Robert Greener
GHashTable *summary_calculate_n_performers(configuration *c,
                                           uint_fast32_t time);

/// @brief Create an empty summary_stats with all arrays being n_days long.
/// @param n_days The number of days.
/// @return A summary_stats * that the caller takes ownership of. This can be
/// freed using free_summary_stats.
/// @author Robert Greener
summary_stats *summary_stats_new(uint_fast32_t n_days);

/// @brief Create and calculate the summary_stats
/// @param c The configuration * which the caller has ownership of.
/// @param n_days The number of days.
/// @return A summary_stats * that the caller takes ownership of. This can be
/// freed using free_summary_stats.
/// @author Robert Greener
summary_stats *summary_calculate_stats(configuration *c, uint_fast32_t n_days);

/// @brief Recursively free summary stats, freeing all GHashTables inside.
/// @param s The summary_stats * which this takes ownership of.
/// @author Robert Greener
void free_summary_stats(summary_stats *s);

#endif
