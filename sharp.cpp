#include <iostream>
#include <utility>
#include "../lib/sharp_lib.cpp"

const int WIDTH = 20;
const int HEIGHT = 20;

auto main(void) -> int
{
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT);
  sharp::Pixel pix = {255, 255, 255};
  // sharp::render::line(c, std::make_pair(1, 1), std::make_pair(4, 4), pix);
  // sharp::render::color(c, std::make_pair(2, 2), pix);
  // sharp::render::triangle(c, sharp::Point(0, 0), sharp::Point(5, 0), sharp::Point(5, 9), pix);
  // sharp::render::line(c, sharp::Point(1, 1), sharp::Point(8, 8), pix);
  sharp::render::rect(c, sharp::Point(0, 0), sharp::Point(5, 0), sharp::Point(5, 5), sharp::Point(0, 5), pix);
  // sharp::PrintCanvas(c);
  sharp::SaveCanvas(c);
  return 0;
}
