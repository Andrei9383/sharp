#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stb_image_write.h"
#include <vector>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <string>
#include "./lib/math_lib.cpp"
#include "SDL.h"

extern const int WIDTH;
extern const int HEIGHT;

// TODO: Implement nobuild tool from rexim
// TODO: Triangle filled
// TODO: Rect filled
// TODO: SDL rendering

namespace sharp
{
  struct Pixel
  {
    int R = 0;
    int G = 0;
    int B = 0;
  };

  struct Point
  {
    int x = 0;
    int y = 0;
    Point(int a, int b) : x(a), y(b){};
  };

  struct Color
  {
    Pixel White = {255, 255, 255};
    Pixel Black = {0, 0, 0};
    Pixel Red = {255, 0, 0};
    Pixel Green = {0, 255, 0};
    Pixel Blue = {0, 0, 255};
    unsigned long createRGBA(int r, int g, int b, int a = 255)
    {
      return ((a & 0xff) << 24) + ((b & 0xff) << 16) + ((g & 0xff) << 8) + (r & 0xff);
    }
  } Colors;

  using canvas = std::vector<std::vector<Pixel>>;

  auto Canvas(const int p_Width, const int p_Height, Pixel p_Color) -> canvas
  {
    canvas load(p_Height, std::vector<Pixel>(p_Width));
    std::vector<Pixel> widthVector(p_Width);
    fill(widthVector.begin(), widthVector.end(), p_Color);
    std::fill(load.begin(), load.end(), widthVector);
    return load;
  };

  void SaveToPng(sharp::canvas p_Canvas)
  {
    auto height = p_Canvas.size();
    auto width = p_Canvas[0].size();

    uint32_t *pixels = new uint32_t[width * height * sizeof(uint32_t)];

    for (int y = 0; y < height; y++)
      for (int x = 0; x < width; x++)
        pixels[y * width + x] = Colors.createRGBA(p_Canvas[x][y].R, p_Canvas[x][y].G, p_Canvas[x][y].G);

    stbi_write_png("./old.png", width, height, 4, pixels, sizeof(uint32_t) * width);
    delete[] pixels;
  }

  namespace render
  {

    auto color(canvas &p_Canvas, Point p_Point, Pixel p_Color) -> void
    {
      p_Canvas[p_Point.x][p_Point.y].R = p_Color.R;
      p_Canvas[p_Point.x][p_Point.y].G = p_Color.G;
      p_Canvas[p_Point.x][p_Point.y].B = p_Color.B;
    }

    void PlotLineLow(canvas &p_Canvas, Point p_Point1, Point p_Point2, Pixel p_Color)
    {
      auto dx = p_Point2.x - p_Point1.x;
      auto dy = p_Point2.y - p_Point1.y;
      auto yi = 1;
      if (dy < 0)
      {
        yi = -1;
        dy = -dy;
      }
      auto D = (2 * dy) - dx;
      auto y = p_Point1.y;
      for (auto x = p_Point1.x; x <= p_Point2.x; x++)
      {
        color(p_Canvas, Point(x, y), p_Color);
        if (D > 0)
        {
          y = y + yi;
          D = D + (2 * (dy - dx));
        }
        else
        {
          D = D + 2 * dy;
        }
      }
    }

    void PlotLineHigh(canvas &p_Canvas, Point p_Point1, Point p_Point2, Pixel p_Color)
    {
      auto dx = p_Point2.x - p_Point1.x;
      auto dy = p_Point2.y - p_Point1.y;
      auto xi = 1;
      if (dx < 0)
      {
        xi = -1;
        dx = -dx;
      }
      auto D = (2 * dx) - dy;
      auto x = p_Point1.x;
      for (auto y = p_Point1.y; y <= p_Point2.y; y++)
      {
        color(p_Canvas, Point(x, y), p_Color);
        if (D > 0)
        {
          x = x + xi;
          D = D + (2 * (dx - dy));
        }
        else
        {
          D = D + 2 * dx;
        }
      }
    }

    auto line(canvas &p_Canvas, Point p_Point1, Point p_Point2, Pixel p_Color) -> void
    {
      if (abs(p_Point2.y - p_Point1.y) < abs(p_Point2.x - p_Point1.x))
      {
        if (p_Point1.x > p_Point2.x)
          PlotLineLow(p_Canvas, p_Point2, p_Point1, p_Color);
        else
          PlotLineLow(p_Canvas, p_Point1, p_Point2, p_Color);
      }
      else
      {
        if (p_Point1.y > p_Point2.y)
          PlotLineHigh(p_Canvas, p_Point2, p_Point1, p_Color);
        else
          PlotLineHigh(p_Canvas, p_Point1, p_Point2, p_Color);
      }
    }
    auto triangle_empty(canvas &p_Canvas, Point p_Point1, Point p_Point2, Point p_Point3, Pixel p_Color) -> void
    {
      line(p_Canvas, p_Point1, p_Point2, p_Color);
      line(p_Canvas, p_Point1, p_Point3, p_Color);
      line(p_Canvas, p_Point2, p_Point3, p_Color);
    }
    auto triangle_filled(canvas &p_Canvas, Point p_Point1, Point p_Point2, Point p_Point3, Pixel p_Color) -> void
    {
      line(p_Canvas, p_Point1, p_Point2, p_Color);
      line(p_Canvas, p_Point1, p_Point3, p_Color);
      line(p_Canvas, p_Point2, p_Point3, p_Color);
    }

    // p_Canvas, top-left, bottom-left, bottom-right, top-right, p_Color
    auto rect(canvas &p_Canvas, Point p_Point1, Point p_Point2, Point p_Point3, Point p_Point4, Pixel p_Color) -> void
    {
      line(p_Canvas, p_Point1, p_Point2, p_Color);
      line(p_Canvas, p_Point2, p_Point3, p_Color);
      line(p_Canvas, p_Point3, p_Point4, p_Color);
      line(p_Canvas, p_Point1, p_Point4, p_Color);
    }

    auto circle_empty(canvas &p_Canvas, Point p_Center, int p_Radius, Pixel p_Color) -> void
    {
      for (int i = p_Center.x - p_Radius; i <= p_Center.x + p_Radius; i++)
      {
        for (int j = p_Center.y - p_Radius; j <= p_Center.y + p_Radius; j++)
        {
          if (math::closeTo((i - p_Center.x) * (i - p_Center.x) + (j - p_Center.y) * (j - p_Center.y), p_Radius * p_Radius, p_Radius))
          {
            color(p_Canvas, Point(i, j), p_Color);
          }
        }
      }
    }

    auto circle_filled(canvas &p_Canvas, Point p_Center, int p_Radius, Pixel p_Color) -> void
    {
      for (int i = p_Center.x - p_Radius; i <= p_Center.x + p_Radius; i++)
      {
        for (int j = p_Center.y - p_Radius; j <= p_Center.y + p_Radius; j++)
        {
          if ((i - p_Center.x) * (i - p_Center.x) + (j - p_Center.y) * (j - p_Center.y) <= p_Radius * p_Radius)
          {
            color(p_Canvas, Point(i, j), p_Color);
          }
        }
      }
    }
  }
  namespace application {

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* surface;
    SDL_Event event;

    auto create_sdl_window(std::string p_Title="App")
    {
      if(SDL_Init(SDL_INIT_VIDEO) < 0)
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init SDL: %s", SDL_GetError());
        return 3;
      }

      if (SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer))
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
      }

      SDL_SetWindowResizable(window, SDL_FALSE);
      SDL_SetWindowTitle(window, p_Title.c_str());

   }
    auto draw_canvas(sharp::canvas p_Canvas)
    {
      auto height = p_Canvas.size();
      auto width = p_Canvas[0].size();

      SDL_Texture* theTexture;

      theTexture = SDL_CreateTexture( renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height );
      uint32_t *pixels = new uint32_t[width * height * sizeof(uint32_t)];

      for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
          pixels[y * width + x] = Colors.createRGBA(p_Canvas[x][y].R, p_Canvas[x][y].G, p_Canvas[x][y].G);

      while (1)
      {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
          break;

        SDL_RenderClear(renderer);
        SDL_UpdateTexture(theTexture, NULL, pixels, sizeof(uint32_t) * width);
        SDL_RenderCopy(renderer, theTexture, NULL, NULL);
        SDL_RenderPresent(renderer);
      }

      // Avoid memory leaks
      delete[] pixels;
      SDL_DestroyTexture(theTexture);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);

      SDL_Quit();

    }
  }
}
