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
#ifndef CONTAGENT_UTIL_H
#define CONTAGENT_UTIL_H

#include <stdlib.h>
#include <stdio.h>

// Log a message for a given day.
//
// The caller has ownership of all pointers.
static inline void logger(const uint_fast32_t day, const char *message) {
  printf("[time: %lu]: %s\n", day, message);
}

// Free a double pointer.
//
// This takes ownership of *v.
static inline void free_double(void *v) { free((double *)v); }

// Free a uint_fast32_t pointer.
//
// This takes ownership of *v.
static inline void free_uint_fast32_t(void *v) { free((uint_fast32_t *)v); }

// Free a garray by decreasing it's reference count by one.
//
// This takes ownership of *v.
static inline void free_garray(void *v) {
  GArray *a = (GArray *)v;
  g_array_unref(a);
}

static inline int double_cmp(const void *d1_v, const void *d2_v) {
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

// Generate a random uuid v4, saving it to uuid_to_set.
//
// The caller has ownership of all pointers.
static inline void random_uuid_v4(unsigned char *uuid_to_set) {
  for (uint_fast8_t i = 0; i < 16; ++i) {
    uuid_to_set[i] = rand() & 0xFF; // Get last byte of random number
  }

  // Set 4 most significant bits of time_hi_and_version: 0100 (4)
  uuid_to_set[6] = (uuid_to_set[6] & 0x0F) | 0x4F;

  // Set 2 most significant bits of clock_seq_hi_and_reserved to 0 and 1
  // respectively.
  uuid_to_set[8] = (uuid_to_set[8] & 0x3F) | 0x7F;
}

#endif // CONTAGENT_UTIL_H
