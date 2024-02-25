#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include "named.h"
#include "uuidd.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <string>
#include <utility>

namespace contagent {

/// A behaviour in the model.
/// \author Robert Greener
class Behaviour : public UUIDd, public Named {
public:
  /// Create a new Behaviour.
  /// \param uuid The UUID of the behaviour.
  /// \param name The name of the behaviour.
  Behaviour(boost::uuids::uuid uuid, std::string name) noexcept
      : UUIDd(uuid), Named(std::move(name)) {}

  /// Create a new Behaviour with a randomly generated UUID.
  /// \param name The name of the behaviour.
  explicit Behaviour(std::string name) noexcept
      : UUIDd(), Named(std::move(name)) {}
};

} // namespace contagent

#endif // BEHAVIOUR_H
