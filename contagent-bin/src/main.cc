#include "main.h"
#include "clipp.h"
#include "contagent/json/json.h"
#include <fstream>
#include <glog/logging.h>
#include <iostream>

using namespace clipp;

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  uint_fast32_t start_time;
  uint_fast32_t end_time;
  std::string agents_path;
  std::string beliefs_path;
  std::string behaviours_path;
  std::string output_path;
  bool full_output = false;

  auto cli =
      (value("start-time", start_time).doc("The start time of the simulation"),
       value("end-time", end_time).doc("The end time of the simulation"),
       value("agents", agents_path).doc("The path to agents.json.zst"),
       value("beliefs", beliefs_path).doc("The path to beliefs.json"),
       value("behaviours", behaviours_path).doc("The path to behaviours.json"),
       value("output", output_path).doc("The output.json.zst path"),
       option("-f", "--full-output")
           .set(full_output)
           .doc("Whether to fully serialize the agents as the output"));

  if (!parse(argc, argv, cli)) {
    std::cout << make_man_page(cli, "contagentsim");
  }

  auto behaviours = load_behaviours(behaviours_path);
  auto config = make_configuration(start_time, end_time, behaviours);
  Runner runner(std::move(config));
  runner.run();
}

std::unique_ptr<Configuration>
make_configuration(const uint_fast32_t start_time, const uint_fast32_t end_time,
                   const std::vector<std::shared_ptr<Behaviour>> &behaviours) {
  std::vector<std::shared_ptr<Belief>> beliefs;
  std::vector<std::shared_ptr<Agent>> agents;
  std::unique_ptr<std::ostream> ostream =
      std::make_unique<std::ostream>(std::cout.rdbuf());
  bool full_output = false;
  std::unique_ptr<Configuration> config = std::make_unique<Configuration>(
      behaviours, beliefs, agents, start_time, end_time, std::move(ostream),
      full_output);
  return config;
}
std::vector<std::shared_ptr<Behaviour>>
load_behaviours(const std::string &file_path) {
  std::ifstream file(file_path);
  try {
    nlohmann::json data = nlohmann::json::parse(file);
    auto specs =
        data.template get<std::vector<contagent::json::BehaviourSpec>>();

    std::vector<std::shared_ptr<Behaviour>> behaviours;
    std::transform(specs.begin(), specs.end(), std::back_inserter(behaviours),
                   [](const contagent::json::BehaviourSpec &spec) {
                     return spec.toBehaviour();
                   });

    return behaviours;
  } catch (const nlohmann::detail::parse_error &e) {
    LOG(FATAL) << "Error reading behaviours JSON " << e.what();
  }
}
