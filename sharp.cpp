#include <iostream>
#include <utility>
#include "./sharp_lib.cpp"

extern const int WIDTH = 900;
extern const int HEIGHT = 900;

auto main(void) -> int
{
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT, sharp::Colors.Black);
  sharp::SaveToBmp(c);
  return 0;
}
