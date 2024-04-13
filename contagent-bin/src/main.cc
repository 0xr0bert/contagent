// Copyright (c) 2024, Robert Greener
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

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

  if (end_time <= start_time) {
    throw std::invalid_argument("End time must be after start time");
  }

  auto behaviours = load_behaviours(behaviours_path);
  auto beliefs = load_beliefs(beliefs_path, behaviours);
  auto agents =
      load_agents(agents_path, behaviours, beliefs, end_time - start_time);
  auto config = make_configuration(start_time, end_time, behaviours, beliefs,
                                   agents, full_output);
  Runner runner(std::move(config));
  runner.run();
}

std::unique_ptr<Configuration>
make_configuration(const uint_fast32_t start_time, const uint_fast32_t end_time,
                   const std::vector<std::shared_ptr<Behaviour>> &behaviours,
                   const std::vector<std::shared_ptr<Belief>> &beliefs,
                   const std::vector<std::shared_ptr<Agent>> &agents,
                   const bool full_output) {
  std::unique_ptr<std::ostream> ostream =
      std::make_unique<std::ostream>(std::cout.rdbuf());
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
std::vector<std::shared_ptr<Belief>>
load_beliefs(const std::string &file_path,
             const std::vector<std::shared_ptr<Behaviour>> &behaviours) {
  std::ifstream file(file_path);
  try {
    nlohmann::json data = nlohmann::json::parse(file);
    auto specs = data.template get<std::vector<contagent::json::BeliefSpec>>();

    std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>> behaviour_map;

    std::transform(
        behaviours.begin(), behaviours.end(),
        std::inserter(behaviour_map, behaviour_map.end()),
        [](const std::shared_ptr<Behaviour> &behaviour) {
          return std::pair<boost::uuids::uuid, std::shared_ptr<Behaviour>>(
              behaviour->getUuid(), behaviour);
        });

    std::vector<std::shared_ptr<Belief>> beliefs;

    std::transform(specs.begin(), specs.end(), std::back_inserter(beliefs),
                   [behaviour_map](const contagent::json::BeliefSpec &spec) {
                     return spec.toUnlinkedBelief(behaviour_map);
                   });

    std::map<boost::uuids::uuid, std::shared_ptr<Belief>> belief_map;

    std::transform(
        beliefs.begin(), beliefs.end(),
        std::inserter(belief_map, belief_map.end()),
        [](const std::shared_ptr<Belief> &belief) {
          return std::pair<boost::uuids::uuid, std::shared_ptr<Belief>>(
              belief->getUuid(), belief);
        });

    for (auto &belief : specs) {
      belief.linkBeliefs(belief_map);
    }

    return beliefs;
  } catch (const std::exception &e) {
    LOG(FATAL) << "Error reading beliefs JSON " << e.what();
  }
}
std::vector<std::shared_ptr<Agent>>
load_agents(const std::string &file_path,
            const std::vector<std::shared_ptr<Behaviour>> &behaviours,
            const std::vector<std::shared_ptr<Belief>> &beliefs,
            const uint_fast32_t n_days) {
  std::ifstream file(file_path);
  try {
    nlohmann::json data = nlohmann::json::parse(file);
    auto specs = data.template get<std::vector<contagent::json::AgentSpec>>();

    std::map<boost::uuids::uuid, std::shared_ptr<Behaviour>> behaviour_map =
        vector_to_uuid_map(behaviours);

    std::map<boost::uuids::uuid, std::shared_ptr<Belief>> belief_map =
        vector_to_uuid_map(beliefs);

    std::vector<std::shared_ptr<Agent>> agents;

    std::transform(specs.begin(), specs.end(), std::back_inserter(agents),
                   [n_days, behaviour_map,
                    belief_map](const contagent::json::AgentSpec &spec) {
                     return spec.toUnlinkedAgent(n_days, behaviour_map,
                                                 belief_map);
                   });

    std::map<boost::uuids::uuid, std::shared_ptr<Agent>> agent_map =
        vector_to_uuid_map(agents);

    for (auto &agent : specs) {
      agent.linkAgents(agent_map);
    }

    return agents;
  } catch (const std::exception &e) {
    LOG(FATAL) << "Error reading agents JSON " << e.what();
  }
}
template <class T>
  requires CheckUUIDd<T>
std::map<boost::uuids::uuid, std::shared_ptr<T>>
vector_to_uuid_map(const std::vector<std::shared_ptr<T>> &vec) {
  std::map<boost::uuids::uuid, std::shared_ptr<T>> m;

  std::transform(vec.begin(), vec.end(), std::inserter(m, m.end()),
                 [](const std::shared_ptr<T> &elem) {
                   return std::pair<boost::uuids::uuid, std::shared_ptr<T>>(
                       elem->getUuid(), elem);
                 });

  return m;
}
