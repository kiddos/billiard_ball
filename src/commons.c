#include "commons.h"

/* math constants */
const double PI = 3.1415926535;

/* window rendering constants */
const double DEFAULT_WINDOW_WIDTH = 800;
const double DEFAULT_WINDOW_HEIGHT = 600;
const double FPS = 66.0;

/* basic type */
point point_init(double x, double y, double z) {
  point p;
  p.x = x;
  p.y = y;
  p.z = z;
  return p;
}

double point_get_x(point p) {
  return p.x;
}

double point_get_y(point p) {
  return p.y;
}

double point_get_z(point p) {
  return p.z;
}

rect rect_init(double x, double y, double z,
    double w, double h, double d) {
  rect r;
  r.x = x;
  r.y = y;
  r.z = z;
  r.width = w;
  r.height = h;
  r.depth = d;
  return r;
}

double rect_get_x(rect r) {
  return r.x;
}

double rect_get_y(rect r) {
  return r.y;
}

double rect_get_z(rect r) {
  return r.z;
}

double rect_get_w(rect r) {
  return r.width;
}

double rect_get_h(rect r) {
  return r.height;
}

double rect_get_d(rect r) {
  return r.depth;
}

