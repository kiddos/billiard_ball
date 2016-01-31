#ifndef TABLE_H
#define TABLE_H

#include "commons.h"

extern const double TABLE_RATIO;
extern const double TABLE_HOLE_RATIO;
extern const double TABLE_BORDER_WIDTH_RATIO;

typedef struct table_t {
  double sx, sy, width, height;
  double hole_radius, border_width;
} table, table_t;

table *table_init(size window_size);
void table_draw(table *t);
void table_resize(table *t, size new_window_size);

#endif /* end of include guard: TABLE_H */
