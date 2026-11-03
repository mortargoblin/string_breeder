#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<stdio.h>

typedef struct {
  int len; // len of target string
  int fitness;
  char *genes; // char array, trying to find target
} Dna;

typedef struct {
  int size;
  Dna *current;
  Dna *next;
} Population;

typedef struct {
  int cap;
  int count;
  Dna **items;
} MatingPool;

// returns char between ' ' (32) and '~' (126)
char random_char(void) {
  return ' ' + rand() % 95;
}

Dna dna_alloc(int len) {
  Dna dna = {0};
  dna.len = len;
  dna.fitness = 0;
  dna.genes = malloc(len + 1);
  dna.genes[len] = '\0';

  return dna;
}

void dna_randomize(Dna *dna) {
  for (int i = 0; i < dna->len; i++)
    dna->genes[i] = random_char();
}

void dna_fitness(Dna *dna, const char *target) {
  int score = 0;
  for (int i = 0; i < dna->len; i++)
    if (dna->genes[i] == target[i])
      score++;
  dna->fitness = score;
}

void pool_fill(MatingPool *pool, Population *pop) {
  pool->count = 0;
  for (int i = 0; i < pop->size; i++)
    for(int j = 0; j < pop->current[i].fitness; j++) {
      pool->items[i] = &(pop->current[i]);
      pool->count++;
    }
}

void pop_free(Population *pop) {
  for (int i = 0; i < pop->size; i++) {
    free(pop->current[i].genes);
    free(pop->next[i].genes);
  }
  free(pop->current);
  free(pop->next);
}

void pool_free(MatingPool *pool) {
  for (int i = 0; i < pool->count; i++) {
    free(pool->items[i]);
  }
}

int main(int argc, char **argv) {
  srand(time(NULL));

  char *target_string = "target string";
  if (argc > 1) {
    target_string = argv[1];
  }

  int generation = 0;
  const float mutation_rate = 0.01;
  const int target_length = strlen(target_string);

  // init population
  Population pop = {0};
  pop.size = 40;
  pop.current = malloc(sizeof(Dna) * pop.size);
  pop.next = malloc(sizeof(Dna) * pop.size);

  // init dna
  for (int i = 0; i < pop.size; i++)
    pop.current[i] = dna_alloc(target_length);
  for (int i = 0; i < pop.size; i++)
    dna_randomize(&pop.current[i]);

  // init mating pool 
  MatingPool pool = {0};
  pool.cap = pop.size * (target_length + 1);
  pool.items = malloc(sizeof(Dna*) * pool.cap);

  // loop
  int found = 0;
  while (!found) {
    for (int i = 0; i < pop.size; i++) {
      dna_fitness(&pop.current[i], target_string);
      printf("%s, score: %d\n", pop.current[i].genes, pop.current[i].fitness);
      if (pop.current->fitness == target_length)
        found = 1337; // perfect match found
    }
  }

  printf("genes: %s\n", pop.current[0].genes);

  pop_free(&pop);
  pool_free(&pool);

  return 0;
}
