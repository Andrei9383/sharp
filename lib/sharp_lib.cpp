#include <vector>

namespace sharp {
  typedef struct {
    int R = 0;
    int G = 0;
    int B = 0;
  }Pixel;

  using canvas = std::vector<std::vector<Pixel>>;

  canvas Canvas(const int p_Width, const int p_Height){
    canvas load(p_Height, std::vector<Pixel>(p_Width));
    return load;
  };

  void PrintCanvas(canvas p_Canvas){
    for(int i = 0; i<p_Canvas.size(); i++){
      for(int j = 0; j<p_Canvas[i].size(); j++){
        std::cout << "(" << p_Canvas[i][j].R << "," 
                  << p_Canvas[i][j].G << "," 
                  << p_Canvas[i][j].B << ") ";
      }
      std::cout << std::endl;
    }
  };
  
  
}