#include "message.h"

void regular_message(const char* msg) {
#ifdef DEBUG
  fprintf(stdout, "%s\n", msg);
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
