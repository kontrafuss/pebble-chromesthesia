#include "utils.h"

GRect centered_square(GRect r) {
  int d = r.size.w - r.size.h;
  if (d > 0) {
    return (GRect) {
      .origin = GPoint(d/2, 0),
      .size = GSize(r.size.h, r.size.h)
    };
  } else {
    return (GRect) {
      .origin = GPoint(0, d/-2),
      .size = GSize(r.size.w, r.size.w)
    };
  }
}
