//
// Created by rober on 22/02/2024.
//

#ifndef CONTAGENT_MAIN_H
#define CONTAGENT_MAIN_H

#include "contagent/contagent.h"
#include <memory>

using namespace contagent;

int main(int argc, char *argv[]);
std::unique_ptr<Configuration>
make_configuration(uint_fast32_t start_time, uint_fast32_t end_time,
                   const std::vector<std::shared_ptr<Behaviour>> &behaviours);

std::vector<std::shared_ptr<Behaviour>>
load_behaviours(const std::string &file_path);

#endif // CONTAGENT_MAIN_H
