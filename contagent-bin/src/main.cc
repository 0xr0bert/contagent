#include "main.h"
#include "clipp.h"
#include <iostream>
#include <glog/logging.h>

using namespace clipp;

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  uint_fast32_t start_time;
  uint_fast32_t end_time;

  auto cli = (option("-s", "--start-time")
                  .set(start_time)
                  .doc("The start time of the simulation"),
              option("-e", "--end-time")
                  .set(end_time)
                  .doc("The end time of the simulation"));

  if (!parse(argc, argv, cli)) {
    std::cout << make_man_page(cli, argv[0]);
  }

  auto config = make_configuration();
  Runner runner(std::move(config));
  runner.run();
}

std::unique_ptr<Configuration> make_configuration() {
  std::vector<std::shared_ptr<Behaviour>> behaviours;
  std::vector<std::shared_ptr<Belief>> beliefs;
  std::vector<std::shared_ptr<Agent>> agents;
  uint_fast32_t start_time = 0;
  uint_fast32_t end_time = 10;
  std::unique_ptr<std::ostream> ostream =
      std::make_unique<std::ostream>(std::cout.rdbuf());
  bool full_output = false;
  std::unique_ptr<Configuration> config = std::make_unique<Configuration>(
      behaviours, beliefs, agents, start_time, end_time, std::move(ostream),
      full_output);
  return config;
}