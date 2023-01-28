#include <cmath>

namespace math {
    bool closeTo(int p_A, int p_B, int p_Error){
        if(abs(p_A - p_B) <= p_Error)
            return true;
        return false;
    }
};