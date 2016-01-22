#include "message.h"

void regular_message(const char* msg) {
#ifdef DEBUG
  time_t current_time;
  struct tm *timeinfo;
  char time_str[128];

  time(&current_time);
  timeinfo = localtime(&current_time);
  strftime(time_str, 128, "%A %B %d %Y %X", timeinfo);

  fprintf(stdout, "[%s]: %s\n", time_str, msg);
#endif
}

void warning_message(const char* msg) {
#ifdef DEBUG
  fprintf(stdout, "WARNING: %s\n", msg);
#endif
}

void error_message(const char* msg) {
#ifdef DEBUG
  fprintf(stderr, "ERROR: %s\n", msg);
#endif
}
