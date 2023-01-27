#include <iostream>
#include <utility>
#include "../lib/sharp_lib.cpp"

const int WIDTH = 10;
const int HEIGHT = 10;

auto main(void) -> int {
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT);
  sharp::Pixel pix = {10, 10, 10};
  sharp::render::line(c, std::make_pair(1, 1), std::make_pair(4, 4), pix);
  sharp::render::color(c, std::make_pair(2, 2), pix);
  sharp::PrintCanvas(c);
  return 0;
}
