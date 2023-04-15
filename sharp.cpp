#include <iostream>
#include <utility>
#include "./sharp_lib.cpp"

extern const int WIDTH = 900;
extern const int HEIGHT = 900;

auto main(void) -> int
{
  sharp::canvas c = sharp::Canvas(WIDTH, HEIGHT, sharp::Colors.White);
  //sharp::render::circle_filled(c, sharp::Point(450, 450), 300, sharp::Colors.Black);
  //sharp::SaveToPng(c);
  sharp::Pixel clr = {255, 0, 0};
  sharp::application::rendering sharp_render;
  int size = 300;
  auto function = [&]() {
    sharp::ClearCanvas(c);
    sharp::render::circle_filled(c, sharp::Point(450, 450), size, clr);
    if(size > 0) {
      size--;
    }
    else {
      size = 300;
    }
    if(clr.R > 0) {
      clr.R --;
      clr.G++;
    }
  };
  sharp_render.create_sdl_window("Sample app");
  sharp_render.draw_canvas(c, function);
  return 0;
}
