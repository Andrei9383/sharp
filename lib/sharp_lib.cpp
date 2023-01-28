#include <vector>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include "./file_lib.cpp"
#include "./math_lib.cpp"

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
    Pixel Black = {0, 0 ,0};
    Pixel Red = {255, 0, 0};
    Pixel Green = {0, 255, 0};
    Pixel Blue = {0, 0, 255};
  }Colors;

  using canvas = std::vector<std::vector<Pixel>>;

  auto Canvas(const int p_Width, const int p_Height, Pixel p_Color) -> canvas
  {
    canvas load(p_Height, std::vector<Pixel>(p_Width));
    std::vector<Pixel> widthVector(p_Width);
    fill(widthVector.begin(), widthVector.end(), p_Color);
    std::fill(load.begin(), load.end(), widthVector);
    return load;
  };

  auto PrintCanvas(canvas p_Canvas) -> void
  {
    for (int i = 0; i < p_Canvas.size(); i++)
    {
      for (int j = 0; j < p_Canvas[i].size(); j++)
      {
        std::cout << "(" << p_Canvas[i][j].R << ","
                  << p_Canvas[i][j].G << ","
                  << p_Canvas[i][j].B << ") ";
      }
      std::cout << std::endl;
    }
  };

  void SaveCanvas(sharp::canvas p_Canvas){
    BYTE* buf = new BYTE[p_Canvas.size() * 3 * p_Canvas[0].size()];
    int c = 0;
    for(int i = 0; i<p_Canvas.size(); i++){
      for(int j = 0; j<p_Canvas[i].size(); j++){
        buf[c + 0] = (BYTE) p_Canvas[i][j].B;
        buf[c + 1] = (BYTE) p_Canvas[i][j].G;
        buf[c + 2] = (BYTE) p_Canvas[i][j].R;
        c += 3;
      }
    }
    SaveBitmapToFile((BYTE*)buf, p_Canvas.size(), p_Canvas[0].size(), 24, 0, "C:\\Users\\andre\\Desktop\\output.bmp");
    delete [] buf;
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
    auto triangle(canvas &p_Canvas, Point p_Point1, Point p_Point2, Point p_Point3, Pixel p_Color) -> void
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
       for(int i = p_Center.x - p_Radius; i <= p_Center.x + p_Radius; i++)
       {
        for(int j = p_Center.y - p_Radius; j <= p_Center.y + p_Radius; j++)
        {
          if(math::closeTo((i - p_Center.x) * (i - p_Center.x) + (j - p_Center.y) * (j - p_Center.y), p_Radius * p_Radius, p_Radius))
          {
            color(p_Canvas, Point(i, j), p_Color);
          }
        }
       }
    }

    auto circle_filled(canvas &p_Canvas, Point p_Center, int p_Radius, Pixel p_Color) -> void 
    {
       for(int i = p_Center.x - p_Radius; i <= p_Center.x + p_Radius; i++)
       {
        for(int j = p_Center.y - p_Radius; j <= p_Center.y + p_Radius; j++)
        {
          if((i - p_Center.x) * (i - p_Center.x) + (j - p_Center.y) * (j - p_Center.y) <= p_Radius * p_Radius)
          {
            color(p_Canvas, Point(i, j), p_Color);
          }
        }
       }
    }
  }

}