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

#ifndef CONTAGENT_MAIN_H
#define CONTAGENT_MAIN_H

#include "contagent/contagent.h"
#include <memory>

using namespace contagent;

int main(int argc, char *argv[]);

std::unique_ptr<Configuration>
make_configuration(const uint_fast32_t start_time, const uint_fast32_t end_time,
                   const std::vector<std::shared_ptr<Behaviour>> &behaviours,
                   const std::vector<std::shared_ptr<Belief>> &beliefs,
                   const std::vector<std::shared_ptr<Agent>> &agents,
                   const bool full_output,
                   std::unique_ptr<std::ostream> output);

std::vector<std::shared_ptr<Behaviour>>
load_behaviours(const std::string &file_path);

std::vector<std::shared_ptr<Belief>>
load_beliefs(const std::string &file_path,
             const std::vector<std::shared_ptr<Behaviour>> &behaviours);

std::vector<std::shared_ptr<Agent>>
load_agents(std::istream &is,
            const std::vector<std::shared_ptr<Behaviour>> &behaviours,
            const std::vector<std::shared_ptr<Belief>> &beliefs,
            const uint_fast32_t n_days);

template <class U>
concept CheckUUIDd = std::is_base_of<UUIDd, U>::value;

template <class T>
  requires CheckUUIDd<T>
std::map<boost::uuids::uuid, std::shared_ptr<T>>
vector_to_uuid_map(const std::vector<std::shared_ptr<T>> &vec);

#endif // CONTAGENT_MAIN_H
