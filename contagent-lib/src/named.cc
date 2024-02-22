
#include <utility>

#include "contagent/named.h"

contagent::Named::Named(std::string  name) : name_(std::move(name)) {}
const std::string& contagent::Named::getName() const {
  return name_;
}
bool contagent::Named::operator==(const contagent::Named& rhs) const {
  return name_ == rhs.name_;
}
bool contagent::Named::operator!=(const contagent::Named& rhs) const {
  return !(rhs == *this);
}
bool contagent::Named::operator<(const contagent::Named& rhs) const {
  return name_ < rhs.name_;
}
bool contagent::Named::operator>(const contagent::Named& rhs) const {
  return rhs < *this;
}
bool contagent::Named::operator<=(const contagent::Named& rhs) const {
  return !(rhs < *this);
}
bool contagent::Named::operator>=(const contagent::Named& rhs) const {
  return !(*this < rhs);
}
