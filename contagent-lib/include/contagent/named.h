#ifndef CONTAGENT_NAMED_H
#define CONTAGENT_NAMED_H

#include <string>
namespace contagent {
/// Something with a name.
/// \author Robert Greener
class Named {
public:
  /// Create a new named.
  /// \param name The name.
  /// \author Robert Greener.
  explicit Named(std::string name) noexcept;

  /// Get the name of this object.
  /// \return The name.
  /// \author Robert Greener
  [[nodiscard]] const std::string &getName() const noexcept;

private:
  /// The name of this object
  /// \author Robert Greener
  const std::string name_;
};
} // namespace contagent

#endif // CONTAGENT_NAMED_H
