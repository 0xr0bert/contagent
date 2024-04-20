#ifndef CONTAGENT_H
#define CONTAGENT_H
#include "glib.h"

typedef struct behaviour {
  char *name;
  unsigned char uuid[16];
} behaviour;

typedef struct belief {
  char *name;
  unsigned char uuid[16];
  GHashTable *perceptions;
  GHashTable *relationships;
} belief;

typedef struct agent {
  unsigned char uuid[16];
  unsigned int n_days;
  GHashTable **activations;
  GHashTable *friends;
  behaviour **actions;
  GHashTable *deltas;
  GHashTable *performance_relationships;
} agent;

double agent_weighted_relationship(const agent *a, const unsigned int day,
                                   belief *b1, belief *b2);

double agent_contextualize(const agent *a, const unsigned int day, belief *b,
                           const GArray *beliefs);

GHashTable *agent_get_actions_of_friends(const agent *a,
                                         const unsigned int day);

double agent_pressure(const agent *a, belief *b,
                      GHashTable *actions_of_friends);

double agent_activation_change(const agent *a, const unsigned int day,
                               belief *b, const GArray *beliefs,
                               GHashTable *actions_of_friends);

void agent_update_activation(agent *a, const unsigned int day, belief *b,
                             const GArray *beliefs,
                             GHashTable *actions_of_friends);

void agent_update_activation_for_all_beliefs(agent *a, const unsigned int day,
                                             const GArray *beliefs);

void agent_perform_action(const agent *a, unsigned int day, GArray *behaviours,
                          GArray *beliefs);
#endif