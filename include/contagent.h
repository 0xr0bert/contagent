#ifndef CONTAGENT_H
#define CONTAGENT_H
#include "glib.h"

typedef unsigned char uuid[16];

typedef struct behaviour {
  char *name;
  uuid uuid;
} behaviour;

typedef struct belief {
  char *name;
  uuid uuid;
  GHashTable *perceptions;
  GHashTable *relationships;
} belief;

typedef struct agent {
  uuid uuid;
  unsigned int n_days;
  GHashTable **activations;
  GHashTable *friends;
  behaviour **actions;
  GHashTable *deltas;
  GHashTable *performance_relationships;
} agent;

typedef struct configuration {
  GArray *behaviours;
  GArray *beliefs;
  GArray *agents;
  unsigned int start_time;
  unsigned int end_time;
  unsigned char full_output;
} configuration;

void logger(const unsigned int day, const char *message);

// The weighted relationship is the relationship between two beliefs
// weighted by the activatino of the firs belief. It is used to describe
// the probability of  adopting b2 given that you already hold b1.
//
// The caller has ownership of all pointers.
double agent_weighted_relationship(const agent *a, const unsigned int day,
                                   belief *b1, belief *b2);

// The context of adopting b given your activation of all the beliefs.
//
// This is the average of agent_weighted_relationship.
//
// beliefs is a GArray of belief*.
//
// The caller has ownership of all pointers.
double agent_contextualize(const agent *a, const unsigned int day, belief *b,
                           const GArray *beliefs);

// Get the sum of the weights of friends, grouped by the actions they
// performed.
//
// Returns a GHashTable from behaviour* to double*, which the caller
// takes ownership of. A delete function exists on the values of
// the GHashTable.
//
// The caller has ownership of all pointers.
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

void perceive_beliefs(configuration *c, unsigned int day);

void perform_actions(configuration *c, unsigned int day);

void tick(configuration *c, unsigned int day);

void tick_between(configuration *c, unsigned int start, unsigned int end);

#endif // CONTAGENT_H