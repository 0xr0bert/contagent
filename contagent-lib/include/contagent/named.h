#ifndef CONTAGENT_NAMED_H
#define CONTAGENT_NAMED_H

#include <string>
namespace contagent {
class Named {
 public:
  explicit Named(std::string  name);

  [[nodiscard]] const std::string& getName() const;

 private:
  std::string name_;
};
}  // namespace contagent

#endif  //CONTAGENT_NAMED_H
