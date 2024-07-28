

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// defs

#define SIZE 200

#define POPULATION 50

#define MIN_NB_GENES 2
#define MAX_NB_GENES 5

// Neurons
#define BORDERDISTANCE_UP 0x01
#define BORDERDISTANCE_DOWN 0x02
#define BORDERDISTANCE_LEFT 0x03
#define BORDERDISTANCE_RIGHT 0x04
#define DISTANCE_NEIGHBOR 0x05

#define MOVE_UP 0x06
#define MOVE_DOWN 0x07
#define MOVE_LEFT 0x08
#define MOVE_RIGHT 0x09
#define MOVE_RANDOM 0x0A
#define HALT 0x0B

#define DOUBLEWEIGHT 0x0C
#define HALFWEIGHT 0x0D
#define THRESHOLD 0x0E

// Directions
#define DIRECTION_UP 8
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 4
#define DIRECTION_RIGHT 6

struct Neuron {
  uint32_t id;
  uint32_t value;
  uint32_t type;
};

struct Connection {
  struct Neuron *From;
  struct Neuron *To;
  double weight;
  uint8_t Is_Positive;
};

struct Bob {
  struct Neuron **Neurons;
  struct Connection **Connections;

  int Neuron_Count;
  int Connection_Count;

  int x;
  int y;
};

struct GameState {
  uint8_t **grid;
  struct Bob **bobs;
};

void init_Bob(struct Bob *bob) {}

void init_GameState(struct GameState *gamestate) {}

int move_Bob_To(struct Bob *bob, struct GameState *gamestate, int x, int y) {
  if (x >= SIZE || y >= SIZE) {
    return 1;
  }
  if (*(*(gamestate->grid + x) + y) == 1) {
    return 1;
  }

  *(*(gamestate->grid + bob->x) + bob->y) = 0;
  *(*(gamestate->grid + x) + y) = 1;

  return 0;
}

void move_Bob(struct Bob *bob, struct GameState *gamestate, int direction) {
  switch (direction) {
  case DIRECTION_UP:
    move_Bob_To(bob, gamestate, bob->x, bob->y + 1);
    break;

  case DIRECTION_DOWN:
    move_Bob_To(bob, gamestate, bob->x, bob->y - 1);
    break;

  case DIRECTION_LEFT:
    move_Bob_To(bob, gamestate, bob->x - 1, bob->y);
    break;

  case DIRECTION_RIGHT:
    move_Bob_To(bob, gamestate, bob->x + 1, bob->y);
    break;
  }
}

int main() {
  srand(time(NULL));

  return 0;
}
