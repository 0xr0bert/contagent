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

#ifndef CONTAGENT_UUIDD_H
#define CONTAGENT_UUIDD_H

#include <boost/uuid/uuid.hpp>
#include <memory>

namespace contagent {
/// Something with a UUID.
/// \author Robert Greener
class UUIDd {
public:
  /// Create a new UUIDd.
  /// \param uuid The UUID.
  /// \author Robert Greener
  explicit UUIDd(boost::uuids::uuid uuid) noexcept;

  /// Create a new UUIDd with a randomly generated UUID.
  /// \author Robert Greener
  UUIDd() noexcept;

  /// Get the UUID.
  /// \return The UUID.
  /// \author Robert Greener
  [[nodiscard]] const boost::uuids::uuid &get_uuid() const noexcept;

private:
  /// The UUID
  /// \author Robert Greener
  const boost::uuids::uuid uuid_;
};

template <class U>
concept CheckUUIDd = std::is_base_of<contagent::UUIDd, U>::value;
} // namespace contagent

#endif // CONTAGENT_UUIDD_H
