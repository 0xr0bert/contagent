
#include <utility>

#include "contagent/named.h"

contagent::Named::Named(std::string  name) : name_(std::move(name)) {}
const std::string& contagent::Named::getName() const { return name_; }