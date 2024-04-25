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

#include "contagent/summary.h"

#include <cmath>

namespace contagent::summary {
std::unordered_map<std::shared_ptr<Belief>, double_t>
calculate_mean_activation(const Configuration &c, std::size_t time) {
  std::unordered_map<std::shared_ptr<Belief>, double_t> m;

  for (const std::shared_ptr<Agent> &a : c.get_agents()) {
    auto acts = a->get_activations_for_day(time);
    for (const auto &[b, act] : acts) {
      if (!m.contains(b)) {
        m[b] = 0.0;
      }
      m[b] += act;
    }
  }

  for (auto &[_b, act] : m) {
    act /= c.get_agents().size();
  }

  return m;
}
std::unordered_map<std::shared_ptr<Belief>, double_t> calculate_sd_activation(
    const Configuration &c, std::size_t time,
    std::unordered_map<std::shared_ptr<Belief>, double_t> &means) {
  std::unordered_map<std::shared_ptr<Belief>, double_t> m;

  for (const std::shared_ptr<Agent> &a : c.get_agents()) {
    auto acts = a->get_activations_for_day(time);
    for (const auto &[b, act] : acts) {
      if (!m.contains(b)) {
        m[b] = 0.0;
      }
      m[b] += std::pow(act - means.at(b), 2.0);
    }
  }

  for (auto &[_b, sqdiff] : m) {
    sqdiff = std::sqrt(sqdiff / (c.get_agents().size() - 1));
  }

  return m;
}

std::unordered_map<std::shared_ptr<Belief>, double_t>
calculate_median_activation(const Configuration &c, std::size_t time) {
  std::unordered_map<std::shared_ptr<Belief>, std::vector<double_t>> all_acts;

  for (const std::shared_ptr<Agent> &a : c.get_agents()) {
    auto acts = a->get_activations_for_day(time);
    for (const auto &[b, act] : acts) {
      all_acts[b].push_back(act);
    }
  }

  for (auto &[_b, v] : all_acts) {
    std::sort(v.begin(), v.end());
  }

  std::size_t ix = c.get_agents().size();
  bool is_even = c.get_agents().size() % 2 == 0;

  std::unordered_map<std::shared_ptr<Belief>, double_t> m;

  for (const auto &[b, v] : all_acts) {
    m[b] = is_even ? (v.at(ix) + v.at(ix + 1)) / 2 : v.at(ix);
  }

  return m;
}

std::unordered_map<std::shared_ptr<Belief>, std::size_t>
calculate_nonzero_activation(const Configuration &c, std::size_t time) {
  std::unordered_map<std::shared_ptr<Belief>, std::size_t> m;

  for (const std::shared_ptr<Agent> &a : c.get_agents()) {
    auto acts = a->get_activations_for_day(time);
    for (const auto &[b, act] : acts) {
      if (act != 0) {
        if (!m.contains(b)) {
          m[b] = 0;
        }
        ++m[b];
      }
    }
  }

  return m;
}

std::unordered_map<std::shared_ptr<Behaviour>, std::size_t>
calculate_n_performers(const Configuration &c, std::size_t time) {
  std::unordered_map<std::shared_ptr<Behaviour>, std::size_t> m;

  for (const std::shared_ptr<Agent> &a : c.get_agents()) {
    auto action = a->get_actions().at(time);
    if (!m.contains(action)) {
      m[action] = 0;
    }
    ++m[action];
  }

  return m;
}

[[nodiscard]] std::unique_ptr<SummaryStats>
calculate_summary_stats(const Configuration &c, std::size_t time) {
  auto s = std::make_unique<SummaryStats>();
  s->mean_activations = calculate_mean_activation(c, time);
  s->sd_activations = calculate_sd_activation(c, time, s->mean_activations);
  s->median_activations = calculate_median_activation(c, time);
  s->nonzero_activations = calculate_nonzero_activation(c, time);
  s->n_performers = calculate_n_performers(c, time);
  return s;
}

} // namespace contagent::summary