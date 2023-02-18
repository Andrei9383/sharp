#include <iostream>
#include <utility>
#include "./sharp_lib.cpp"

extern const int WIDTH = 900;
extern const int HEIGHT = 900;

auto main(void) -> int
{
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT, sharp::Colors.Red);
  sharp::render::circle_empty(c, sharp::Point(450, 450), 300, sharp::Colors.Black);
  sharp::SaveToPng(c);
  return 0;
}
