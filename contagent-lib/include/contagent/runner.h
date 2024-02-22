#ifndef CONTAGENT_RUNNER_H
#define CONTAGENT_RUNNER_H

#include <memory>


#include "configuration.h"
namespace contagent {
class Runner {
 public:
  explicit Runner(std::unique_ptr<Configuration> configuration);

 private:
  std::unique_ptr<Configuration> configuration_;

  void perceiveBeliefs(uint_fast32_t time);

  void performActions(uint_fast32_t time);

  void tick(uint_fast32_t time);

  void tickBetween(uint_fast32_t start_time, uint_fast32_t end_time);

  void run();
};
}

#endif  //CONTAGENT_RUNNER_H
