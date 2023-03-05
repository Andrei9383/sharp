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

  sharp::application::rendering sharp_render;
  auto function = [&]() {
    sharp::render::circle_filled(c, sharp::Point(450, 450), 300, sharp::Colors.Red);
    sharp::ClearCanvas(c);
    sharp::render::circle_filled(c, sharp::Point(450, 450), 300, sharp::Colors.Black);
  };
  sharp_render.create_sdl_window("Sample app");
  sharp_render.draw_canvas(c, function);
  return 0;
}
