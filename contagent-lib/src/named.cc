
#include <utility>

#include "contagent/named.h"

contagent::Named::Named(std::string name) noexcept: name_(std::move(name)) {}
const std::string& contagent::Named::getName() const noexcept { return name_; }