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
#ifndef CONTAGENT_TYPES_H
#define CONTAGENT_TYPES_H

#include "glib.h"
#include <stdint.h>

typedef struct _behaviour {
  char *name;
  unsigned char uuid[16];
} behaviour;

typedef struct _belief {
  char *name;
  unsigned char uuid[16];
  GHashTable *perceptions;
  GHashTable *relationships;
} belief;

typedef struct _agent {
  unsigned char uuid[16];
  uint_fast32_t n_days;
  GHashTable **activations;
  GHashTable *friends;
  behaviour **actions;
  GHashTable *deltas;
  GHashTable *performance_relationships;
} agent;

typedef struct _configuration {
  GArray *behaviours;
  GArray *beliefs;
  GArray *agents;
  uint_fast32_t start_time;
  uint_fast32_t end_time;
  unsigned char full_output;
} configuration;

typedef struct _summary_stats {
  uint_fast32_t n_days;
  GHashTable **mean_activation;
  GHashTable **sd_activation;
  GHashTable **median_activation;
  GHashTable **nonzero_activaton;
  GHashTable **n_performers;
} summary_stats;

#endif // CONTAGENT_TYPES_H
