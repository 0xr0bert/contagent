
#include "contagent/uuidd.h"
#include <boost/uuid/uuid_generators.hpp>

UUIDd::UUIDd(boost::uuids::uuid uuid) : uuid_(uuid) {}

UUIDd::UUIDd() : UUIDd(boost::uuids::random_generator_mt19937()()) {}

const boost::uuids::uuid& UUIDd::getUuid() const {
  return uuid_;
}
bool UUIDd::operator==(const UUIDd& rhs) const {
  return uuid_ == rhs.uuid_;
}
bool UUIDd::operator!=(const UUIDd& rhs) const {
  return !(rhs == *this);
}
bool UUIDd::operator<(const UUIDd& rhs) const {
  return uuid_ < rhs.uuid_;
}
bool UUIDd::operator>(const UUIDd& rhs) const {
  return rhs < *this;
}
bool UUIDd::operator<=(const UUIDd& rhs) const {
  return !(rhs < *this);
}
bool UUIDd::operator>=(const UUIDd& rhs) const {
  return !(*this < rhs);
}
