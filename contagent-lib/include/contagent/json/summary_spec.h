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

#ifndef CONTAGENT_JSON_SUMMARY_SPEC_H
#define CONTAGENT_JSON_SUMMARY_SPEC_H

#include "contagent/summary.h"

#include <boost/uuid/uuid.hpp>
#include <nlohmann/json.hpp>
#include <string>

namespace contagent::json {
class SummarySpec {
public:
  SummarySpec() = default;
  explicit SummarySpec(const contagent::summary::SummaryStats &stats);

  std::map<std::string, double_t> mean_activations;
  std::map<std::string, double_t> sd_activations;
  std::map<std::string, double_t> median_activations;
  std::map<std::string, std::size_t> nonzero_activations;
  std::map<std::string, std::size_t> n_performers;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SummarySpec, mean_activations,
                                   sd_activations, median_activations,
                                   nonzero_activations, n_performers);
} // namespace contagent::json

#endif // CONTAGENT_JSON_SUMMARY_SPEC_H