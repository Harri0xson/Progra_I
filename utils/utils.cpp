#include "utils.h"

int rollDice(int pLimit) {
    // Generamos un número aleatorio entre 1 y pLimit
    return (rand() % pLimit) + 1;
}



