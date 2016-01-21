#include "message.h"

void regular_message(const char* msg) {
#ifdef DEBUG
  fprintf(stdout, "%s\n", msg);
#endif
}
