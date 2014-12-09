#pragma once

#include <cycles/cycle.h>
template <class Matrix, class Vector>
class V_Cycle {
  public:
  inline V_Cycle(AMG_Level<Matrix,Vector> *next, const Vector& b, Vector &x) {
    next->cycle(V_CYCLE,b,x);
  }
};

