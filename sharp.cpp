#include <iostream>
#include <utility>
#include "../lib/sharp_lib.cpp"

const int WIDTH = 900;
const int HEIGHT = 900;

auto main(void) -> int
{
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT, sharp::Colors.Black);
  sharp::SaveCanvas(c);
  return 0;
}
