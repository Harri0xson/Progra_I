#ifndef DUNGEON_H
#define DUNGEON_H

#include <vector>
#include <string>
#include <iostream>
#include "../../utils/utils.h"

// Definimos una estructura para representar una celda en el calabozo
struct Cell {
    bool hasEnemy;  // Indica si la celda tiene un enemigo
    bool hasTreasure;  // Indica si la celda tiene un tesoro
    bool hasCheckPoint;    // Indica que la celda es un punto de salvado
    bool hasTavern;     // Indica si tiene taberna para reclutar miembros al party
};

class Dungeon {
private:
    std::vector<std::vector<Cell>> floors;

public:
    Dungeon();  // Constructor
    ~Dungeon();  // Destructor

    void generateDungeon();  // Método para generar el calabozo
    void printDungeon();  // Método para imprimir el calabozo

    void serialize(std::ostream& stream);

};

#endif // DUNGEON_H
