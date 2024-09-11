
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "blobs.h"

struct Position_t {
  int x;
  int y;
};

struct Position_t Position_Buffer[POPULATION][STEPS + 1];

// used for neuron IDs
int Total_Neuron_count = 0;
