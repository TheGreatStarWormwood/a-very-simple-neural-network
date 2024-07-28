

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
#define BorderDistanceUp 0x01
#define BorderDistanceDown 0x02
#define BorderDistanceLeft 0x03
#define BorderDistanceRight 0x04
#define DistanceNeighbor 0x05

#define MoveUp 0x06
#define MoveDown 0x07
#define MoveLeft 0x08
#define MoveRight 0x09
#define MoveRandom 0x0A
#define Halt 0x0B

#define DoubleWeight 0x0C
#define HalfWeight 0x0D
#define Threshold 0x0E

// Directions
// mapped like numpad
#define Direction_Up 8
#define Direction_Down 2
#define Direction_Left 4
#define Direction_Right 6

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
  case Direction_Up:
    move_Bob_To(bob, gamestate, bob->x, bob->y + 1);
    break;

  case Direction_Down:
    move_Bob_To(bob, gamestate, bob->x, bob->y - 1);
    break;

  case Direction_Left:
    move_Bob_To(bob, gamestate, bob->x - 1, bob->y);
    break;

  case Direction_Right:
    move_Bob_To(bob, gamestate, bob->x + 1, bob->y);
    break;
  }
}

int main() {
  srand(time(NULL));

  return 0;
}
