
#ifndef CONTAGENT_UUIDD_H
#define CONTAGENT_UUIDD_H

#include <boost/uuid/uuid.hpp>

/// Something with a UUID.
/// \author Robert Greener
class UUIDd {
 public:
  /// Create a new UUIDd.
  /// \param uuid The UUID.
  /// \author Robert Greener
  explicit UUIDd(boost::uuids::uuid uuid);

  /// Create a new UUIDd with a randomly generated UUID.
  /// \author Robert Greener
  UUIDd();

  /// Get the UUID.
  /// \return The UUID.
  /// \author Robert Greener
  [[nodiscard]] const boost::uuids::uuid& getUuid() const;

 private:
  const boost::uuids::uuid uuid_;
};

#endif  //CONTAGENT_UUIDD_H
