
#include "visio.c"

typedef struct {
  uint32_t id;
  uint16_t type;
} Neuron_t;

struct Connection_t {
  Neuron_t *From;
  Neuron_t *To;
  int Weight;
};

typedef struct {
  Neuron_t **neurons;
  struct Connection_t **connections;
  int neuron_count;
  int connection_count;
} Neural_Network_t;

struct Blob_t {
  struct Position_t *pos;
  Neural_Network_t *Neural_Network;
};

struct GameState_t {
  struct Blob_t **Blobs;
  int count_Blobs;
  uint8_t **grid;
  int pop_Count;
  int steps;
};

void print_Grid(struct GameState_t *GameState) {

  printf("\e[1;1H\e[2J");
  for (int i = 0; i < GRID_SIZE + 2; i++) {
    printf("#");
  }
  printf("\n");
  for (int i = 0; i < GRID_SIZE; i++) {
    printf("#");
    for (int j = 0; j < GRID_SIZE; j++) {
      printf(GameState->grid[i][j] == 1 ? "#" : " ");
    }
    printf("#\n");
  }

  for (int i = 0; i < GRID_SIZE + 2; i++) {
    printf("#");
  }
  printf("\n");
}

int init_Blob(struct Blob_t **Blob) {
  printf("initing husk\n");
  *Blob = (struct Blob_t *)malloc(sizeof(struct Blob_t));
  (*Blob)->pos = (struct Position_t *)malloc(sizeof(struct Position_t));
  (*Blob)->pos->x = *(int *)malloc(sizeof(int));
  (*Blob)->pos->y = *(int *)malloc(sizeof(int));
  (*Blob)->Neural_Network =
      (Neural_Network_t *)malloc(sizeof(Neural_Network_t));
  return 0;
}

int init_GameState(struct GameState_t **GameState) {

  *GameState = (struct GameState_t *)malloc(sizeof(struct GameState_t));

  (*GameState)->grid = (uint8_t **)malloc(GRID_SIZE * sizeof(uint8_t *));
  if (!(*GameState)->grid) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < GRID_SIZE; i++) {
    (*GameState)->grid[i] = (uint8_t *)malloc(GRID_SIZE * sizeof(uint8_t));
    if (!(*GameState)->grid[i]) {
      fprintf(stderr, "malloc failed\n");
      exit(EXIT_FAILURE);
    }
  }

  (*GameState)->Blobs =
      (struct Blob_t **)malloc(POPULATION * sizeof(struct Blob_t));
  if (!(*GameState)->Blobs) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}

int generate_random_neuron(Neuron_t *neuron) {
  neuron->id = Total_Neuron_count++;
  neuron->type = rand() % 15 + 1;
  return 0;
}

int add_connection(Neural_Network_t *network, Neuron_t *source,
                   Neuron_t *dest) {
  struct Connection_t *connection =
      (struct Connection_t *)malloc(sizeof(struct Connection_t));
  if (!connection) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }
  connection->From = source;
  connection->To = dest;
  connection->Weight = (rand() % 2001 - 1000) / 1000.0;
  network->connections[network->connection_count++] = connection;
  return 0;
}

int create_neural_net(Neural_Network_t *network) {
  network->neuron_count =
      rand() % (MAX_NEURONS - MIN_NEURONS + 1) + MIN_NEURONS;
  network->neurons =
      (Neuron_t **)malloc(network->neuron_count * sizeof(Neuron_t));
  network->connections = (struct Connection_t **)malloc(
      network->neuron_count * network->neuron_count *
      sizeof(struct Connection_t));
  network->connection_count = 0;

  for (int i = 0; i < network->neuron_count; ++i) {
    generate_random_neuron(network->neurons[i]);
  }

  for (int i = 0; i < network->neuron_count; ++i) {
    for (int j = 0; j < network->neuron_count; ++j) {
      if (network->neurons[i]->type >= BORDERDISTANCE_UP &&
          network->neurons[i]->type <= HAS_NEIGHBOR) {
        if (rand() % 2 && (network->neurons[j]->type >= MOVE_UP &&
                           network->neurons[j]->type <= THRESHOLD)) {
          add_connection(network, network->neurons[i], network->neurons[j]);
        }
      } else if (network->neurons[j]->type >= MOVE_UP &&
                 network->neurons[j]->type <= THRESHOLD) {
        if (rand() % 2) {
          add_connection(network, network->neurons[i], network->neurons[j]);
        }
      }
    }
  }
  return 0;
}

int spawn_Husk(struct GameState_t **GameState, struct Blob_t **Blob) {
  printf("entered spawn husk\n");
  init_Blob(Blob);
  if (!Blob) {
    fprintf(stderr, "malloc failed\n");
    exit(EXIT_FAILURE);
  }
  int x = rand() % GRID_SIZE;
  int y = rand() % GRID_SIZE;
  (*Blob)->pos->x = x;
  (*Blob)->pos->y = y;

  (*GameState)->Blobs[(*GameState)->count_Blobs] = *Blob;
  (*GameState)->grid[(*Blob)->pos->x][(*Blob)->pos->y] = 1;
  return 0;
}

int breathe_Life_Into_Husk(struct GameState_t *GameState, struct Blob_t *Husk) {

  create_neural_net(Husk->Neural_Network);

  return 0;
}

int breathe_Life(struct GameState_t *GameState) {

  for (int i = 0; i < POPULATION; i++) {
    breathe_Life_Into_Husk(GameState, GameState->Blobs[i]);
  }

  return 0;
}

int populate_Husks(struct GameState_t **GameState) {
  for (int i = 0; i < POPULATION; i++) {
    printf("spawning husk\n");

    spawn_Husk(GameState, &(*GameState)->Blobs[i]);
    printf("husk spawned at: %d,%d\n", (*GameState)->Blobs[i]->pos->x,
           (*GameState)->Blobs[i]->pos->y);
  }
  return 0;
}

int repopulate_World() { return 0; }

int move_Blob_To(struct Blob_t *Blob, struct GameState_t *gamestate, int x,
                 int y) {
  if (x >= GRID_SIZE || y >= GRID_SIZE || x < 0 || y < 0) {
    return 1;
  }
  if (gamestate->grid[x][y] == 1) {
    return 1;
  }

  gamestate->grid[Blob->pos->x][Blob->pos->y] = 0;
  gamestate->grid[x][y] = 1;
  Blob->pos->x = x;
  Blob->pos->y = y;

  return 0;
}

void move_Blob(struct GameState_t *GameState, struct Blob_t *Blob, int Action) {
  switch (Action) {
  case DIRECTION_UP:
    move_Blob_To(Blob, GameState, Blob->pos->x, Blob->pos->y + 1);
    break;
  case DIRECTION_DOWN:
    move_Blob_To(Blob, GameState, Blob->pos->x, Blob->pos->y - 1);
    break;
  case DIRECTION_LEFT:
    move_Blob_To(Blob, GameState, Blob->pos->x - 1, Blob->pos->y);
    break;
  case DIRECTION_RIGHT:
    move_Blob_To(Blob, GameState, Blob->pos->x + 1, Blob->pos->y);
    break;
  }
}

int has_Neighbor(struct GameState_t *GameState, struct Blob_t *Blob) {
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      if (dx == 0 && dy == 0) {
        continue;
      }
      if (GameState->grid[Blob->pos->x + dx][Blob->pos->y + dy] == 1) {
        return 0;
      }
    }
  }
  return -1;
}

int handle_Blob_Behaviour(struct GameState_t *GameState, struct Blob_t *Blob) {

  int *neuron_Values = (int *)malloc(sizeof(int) * MAX_NEURONS);
  int action = 11;

  for (int i = 0; i < Blob->Neural_Network->neuron_count; i++) {
    int neuron_Type = Blob->Neural_Network->neurons[i]->type;
    int neuron_id = Blob->Neural_Network->neurons[i]->id;
    if (neuron_Type == 1) {
      neuron_Values[neuron_id] = Blob->pos->y;
    }
    if (neuron_Type == 2) {
      neuron_Values[neuron_id] = GRID_SIZE - Blob->pos->y;
    }
    if (neuron_Type == 3) {
      neuron_Values[neuron_id] = GRID_SIZE - Blob->pos->x;
    }
    if (neuron_Type == 4) {
      neuron_Values[neuron_id] = Blob->pos->x;
    }
    if (neuron_Type == 5) {
      neuron_Values[neuron_id] = has_Neighbor(GameState, Blob);
    }
  }

  for (int i = 0; i < Blob->Neural_Network->connection_count; i++) {
    Neuron_t *from = Blob->Neural_Network->connections[i]->From;
    Neuron_t *to = Blob->Neural_Network->connections[i]->To;
    if (to->type == 12) {
      neuron_Values[to->id] = neuron_Values[from->id] * 2.0;
      continue;
    }
    if (to->type == 13) {
      neuron_Values[to->id] = neuron_Values[from->id] * 0.5;
      continue;
    }
    neuron_Values[to->id] = neuron_Values[from->id];
    // if(to->type == 14){
    //   neuron_Values[to->id] = neuron_Values[from->id] * 2;
    // }
  }

  int max_Neuron_Value = neuron_Values[0];
  for (int i = 1; i < MAX_NEURONS; i++) {
    int neuron_Type = Blob->Neural_Network->neurons[i]->type;
    int neuron_id = Blob->Neural_Network->neurons[i]->id;
    if (!(neuron_Type < 6 || neuron_Type > 11)) {
      continue;
    }
    if (neuron_Values[neuron_id] > max_Neuron_Value) {
      max_Neuron_Value = neuron_Values[neuron_id];
      action = neuron_Type;
    }
  }
  move_Blob(GameState, Blob, action);
  return 0;
}

int update_Pos_Buf(struct GameState_t **GameState) {
  for (int i = 0; i < POPULATION; i++) {
    Position_Buffer[i][(*GameState)->steps] = *(*GameState)->Blobs[i]->pos;
  }
  return 0;
}

int run_GameState() { return 0; }

int start_Simulation() {

  struct GameState_t *GameState;
  init_GameState(&GameState);

  populate_Husks(&GameState);
  breathe_Life(GameState);
  print_Grid(GameState);

  return 0;
}

int main() {

  srand(time(NULL));
  start_Simulation();

  return 0;
}
