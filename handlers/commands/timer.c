#include <time.h>

#include "../commands.h"

void start_timer(clock_t *start, clock_t *stop) {
  *start = clock();
  *stop = 0;
}

void stop_timer(clock_t *stop) { *stop = clock(); }

double time_timer(clock_t start, clock_t stop) {
  if (!start) return 0;
  if (stop) return (stop - start) / CLOCKS_PER_SEC * 1000;
  return (clock() - start) / CLOCKS_PER_SEC * 1000;
}