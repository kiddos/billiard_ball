#include "table.h"

const double TABLE_RATIO = 2.0;
const double TABLE_HOLE_RATIO = 20.0;
const double TABLE_BORDER_WIDTH_RATIO = 10.0;

table *table_init(size window_size) {
  table *t = (table *) malloc(sizeof(table));
  t->sx = 0;
  t->sy = 0;

  if (window_size.width / window_size.height <= TABLE_RATIO) {
    t->width = window_size.width;
    t->height = t->width / TABLE_RATIO;
  } else {
    t->height = t->height;
    t->width = t->height * TABLE_RATIO;
  }
  t->hole_radius = t->width / TABLE_HOLE_RATIO;
  return t;
}

void table_draw(table *t) {
}

void table_resize(table *t, size new_window_size) {
  t->width = new_window_size.width;
  t->height = new_window_size.height;
}
