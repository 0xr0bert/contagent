#include <iostream>
#include "contagent/contagent.h"
#include "clipp.h"

using namespace clipp;
int main(int argc, char* argv[]) {
  uint_fast32_t start_time;
  uint_fast32_t end_time;

  auto cli = (
      option("-s", "--start-time").set(start_time).doc("The start time of the simulation"),
      option("-e", "--end-time").set(end_time).doc("The end time of the simulation")
      );

  if (!parse(argc, argv, cli) || true) {
    std::cout << make_man_page(cli, argv[0]);
  }
}