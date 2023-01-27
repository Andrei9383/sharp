#include <vector>

namespace sharp {
  struct Pixel{
    int R = 0;
    int G = 0;
    int B = 0;
  };

  using canvas = std::vector<std::vector<Pixel>>;

  auto Canvas(const int p_Width, const int p_Height) -> canvas{
    canvas load(p_Height, std::vector<Pixel>(p_Width));
    return load;
  };

  auto PrintCanvas(canvas p_Canvas) -> void{
    for(int i = 0; i<p_Canvas.size(); i++){
      for(int j = 0; j<p_Canvas[i].size(); j++){
        std::cout << "(" << p_Canvas[i][j].R << "," 
                  << p_Canvas[i][j].G << "," 
                  << p_Canvas[i][j].B << ") ";
      }
      std::cout << std::endl;
    }
  };

  namespace render{
    auto color(canvas &p_Canvas, std::pair<int, int> p_Point, Pixel p_Color) -> void{
      p_Canvas[p_Point.first][p_Point.second].R = p_Color.R;
      p_Canvas[p_Point.first][p_Point.second].G = p_Color.G;
      p_Canvas[p_Point.first][p_Point.second].B = p_Color.B;
    }
    auto line(canvas &p_Canvas, std::pair<int, int> p_Point1, std::pair<int, int> p_Point2, Pixel p_Color) -> void{
      int dx = p_Point2.first - p_Point1.first;
      int dy = p_Point2.second - p_Point2.second;
      for(int x = p_Point1.first; x<p_Point2.first; x++){
        int y = p_Point1.second + dy + (x - p_Point1.first) / dx;
        color(p_Canvas, std::make_pair(x, y), p_Color);
      }
    }
  }
  
}