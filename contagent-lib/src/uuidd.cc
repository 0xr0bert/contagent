
#include "contagent/uuidd.h"
#include <boost/uuid/uuid_generators.hpp>

UUIDd::UUIDd(boost::uuids::uuid uuid) noexcept : uuid_(uuid) {}

UUIDd::UUIDd() noexcept : UUIDd(boost::uuids::random_generator_mt19937()()) {}

const boost::uuids::uuid &UUIDd::getUuid() const noexcept { return uuid_; }
