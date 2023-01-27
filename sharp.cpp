#include <iostream>
#include "../lib/sharp_lib.cpp"

const int WIDTH = 100;
const int HEIGHT = 100;

int main(void) {
  auto c = sharp::Canvas(WIDTH, HEIGHT);
  sharp::PrintCanvas(c);
  return 0;
}
