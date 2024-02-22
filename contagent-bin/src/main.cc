#include <iostream>
#include "contagent/contagent.h"

int main() {
  contagent::Named named("hi");
  std::cout << named.getName() << std::endl;
}