//
// Created by rober on 22/02/2024.
//

#ifndef CONTAGENT_MAIN_H
#define CONTAGENT_MAIN_H

#include <memory>
#include "contagent/contagent.h"

using namespace contagent;

int main(int argc, char* argv[]);
std::unique_ptr<Configuration> make_configuration(uint_fast32_t start_time, uint_fast32_t end_time);

std::vector<std::unique_ptr<Behaviour>> load_behaviours(const std::string& file_path);

#endif // CONTAGENT_MAIN_H
