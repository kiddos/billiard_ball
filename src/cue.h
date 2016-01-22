#ifndef CUE_H
#define CUE_H

#include "commons.h"

typedef struct cue_t {
  ALLEGRO_BITMAP *bitmap;
  double x, y, cx, cy;
  double length;
  double rotation;
} cue_t, cue;

#endif /* end of include guard: CUE_H */
