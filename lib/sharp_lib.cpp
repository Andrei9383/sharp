#include <vector>
#include <fstream>
#include <cstdint>

struct BmpHeader
{
  char bitmapSignatureBytes[2] = {'B', 'M'};
  uint32_t sizeOfBitmapFile = 54 + 786432;
  uint32_t reservedBytes = 0;
  uint32_t pixelDataOffset = 54;
} bmpHeader;

struct BmpInfoHeader
{
  uint32_t sizeOfThisHeader = 40;
  int32_t width = 512;              // in pixels
  int32_t height = 512;             // in pixels
  uint16_t numberOfColorPlanes = 1; // must be 1
  uint16_t colorDepth = 24;
  uint32_t compressionMethod = 0;
  uint32_t rawBitmapDataSize = 0;      // generally ignored
  int32_t horizontalResolution = 3780; // in pixel per meter
  int32_t verticalResolution = 3780;   // in pixel per meter
  uint32_t colorTableEntries = 0;
  uint32_t importantColors = 0;
  BmpInfoHeader(int32_t p_Width, int32_t p_Height) : width(p_Width), height(p_Height) {}
};

namespace sharp
{
  struct Pixel
  {
    uint8_t R = 0;
    uint8_t G = 0;
    uint8_t B = 0;
  };
  struct Point
  {
    int x = 0;
    int y = 0;
    Point(int a, int b) : x(a), y(b){};
  };

  using canvas = std::vector<std::vector<Pixel>>;

  auto Canvas(const int p_Width, const int p_Height) -> canvas
  {
    canvas load(p_Height, std::vector<Pixel>(p_Width));
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

  void SaveCanvas(sharp::canvas p_Canvas)
  {
    std::ofstream fout("output.bmp", std::ios::binary);
    auto header = BmpInfoHeader(p_Canvas.size(), p_Canvas[0].size());
    fout.write((char *)&bmpHeader, 14);
    fout.write((char *)&header, 40);

    for (int i = 0; i < p_Canvas.size(); i++)
    {
      for (int j = 0; j < p_Canvas[i].size(); j++)
      {
        fout.write((char *)&p_Canvas[i][j], 3);
      }
    }
    fout.close();
  }

  namespace render
  {

    auto color(canvas &p_Canvas, std::pair<int, int> p_Point, Pixel p_Color) -> void
    {
      p_Canvas[p_Point.first][p_Point.second].R = p_Color.R;
      p_Canvas[p_Point.first][p_Point.second].G = p_Color.G;
      p_Canvas[p_Point.first][p_Point.second].B = p_Color.B;
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
        color(p_Canvas, std::make_pair(x, y), p_Color);
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
        color(p_Canvas, std::make_pair(x, y), p_Color);
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
  }

}

// TODO: Make sure line coords are useful
// TODO: Make sure triangle coords are useful