#include <iostream>
#include <utility>
#include "../lib/sharp_lib.cpp"

const int WIDTH = 900;
const int HEIGHT = 900;

auto main(void) -> int
{
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT, sharp::Colors.White);
  sharp::Pixel Japan_Red = {238, 0, 0};
  sharp::render::circle_filled(c, sharp::Point(450, 450), 100, Japan_Red);
  sharp::SaveCanvas(c);
  return 0;
}
