#include "Dungeon.h"

Dungeon::Dungeon() {
    // Inicializamos los pisos del calabozo con celdas vacías
    floors = std::vector<std::vector<Cell>>(10, std::vector<Cell>(10));
    generateDungeon();
}

Dungeon::~Dungeon() {}

void Dungeon::generateDungeon() {
    // Generamos el calabozo, marcando las celdas de inicio y salida en cada piso
    for (int floor = 0; floor < 10; floor++) {
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; col++) {
                Cell cell;
                cell.hasEnemy = false;
                cell.hasTreasure = false;
                cell.hasCheckPoint = false;
                cell.hasTavern = false;

                // Generamos un número aleatorio entre 1 y 4
                int diceRoll = rollDice(4);

                // Usamos un switch case para asignar true a un atributo según el resultado del dado
                switch (diceRoll) {
                    case 1:
                        cell.hasEnemy = true;
                        break;
                    case 2:
                        cell.hasTreasure = true;
                        break;
                    case 3:
                        cell.hasCheckPoint = true;
                        break;
                    case 4:
                        cell.hasTavern = true;
                        break;
                }

                floors[floor][col] = cell;
            }
        }
    }
}

void Dungeon::printDungeon() {
    // Imprimimos el calabozo
    for (int floor = 0; floor < 10; floor++) {
        std::cout << "Piso " << floor + 1 << ":\n";
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; col++) {
                std::cout << "Cell:\n" << floors[row][col].hasEnemy << "\t" << 
                floors[row][col].hasTreasure << "\t" <<
                floors[row][col].hasCheckPoint << "\t" <<
                floors[row][col].hasTavern << "\t" << std::endl;
            }
            std::cout << std::endl;
        }
    }
}

void Dungeon::serialize(std::ostream& stream) {
    /*
    Esta funcion permite serializar la clase entera
    Serializar es basicamente "traducir" los datos a un formato
    que podamos escribir en algun archivo y guardar los datos.
    stream nos permite ir digamos que acumulando estos datos tal cual string,
    pero con la particularidad de que son binarizados, por eso la linea 73 tiene
    el casteo y el sizeof.
    */
    for (const auto& floor : floors) {
        for (const Cell& cell : floor) {
            stream.write(reinterpret_cast<const char*>(&cell), sizeof(Cell));
        }
    }
}

