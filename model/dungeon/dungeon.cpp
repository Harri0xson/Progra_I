#include "Dungeon.h"

Dungeon::Dungeon() {
    // Inicializamos los pisos del calabozo con celdas vacías
    floors = std::vector<std::vector<std::vector<Cell>>>(10, std::vector<std::vector<Cell>>(10, std::vector<Cell>(10)));
    generateDungeon();
}

Dungeon::~Dungeon() {}

void Dungeon::generateDungeon() {
    for (int floor = 0; floor < 10; floor++) {
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; col++) {
                Cell cell;
                cell.hasEnemy = false;
                cell.hasTreasure = false;
                cell.hasCheckPoint = false;
                cell.hasTavern = false;
                // BUG falta asegurar que tenga 10 enemigos minimo
                int diceRoll = rollDice(4);
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
                floors[floor][row][col] = cell;
            }
        }
    }
}

void Dungeon::printDungeon() {
    // Método para imprimir el calabozo completo
    for (int floor = 0; floor < 10; floor++) {
        printFloor(floor, 10, 10, true);
    }
}

void Dungeon::printFloor(int floor, int characterX, int characterY, bool showAll){
    // Método para imprimir un piso del calabozo, junto con la posición del jugador, enemigos, tesoros, tabernas y puntos de salvado
    if (showAll){
        std::cout << "Floor " << floor + 1 << ":" << std::endl;
        for (int row = 0; row < 10; row++) {
            std::cout << "  Row " << row + 1 << ": \t\t";
            for (int col = 0; col < 10; col++) {
                if (row == characterY && col == characterX) {
                    std::cout << "Px";
                }
                std::cout << (floors[floor][row][col].hasEnemy ? "E" : "");
                std::cout << (floors[floor][row][col].hasTreasure ? "T" : "");
                std::cout << (floors[floor][row][col].hasCheckPoint ? "C" : "");
                std::cout << (floors[floor][row][col].hasTavern ? "V" : "");
                std::cout << " ";
            }
            std::cout << std::endl;
        }
    } else {
        std::cout << "Floor " << floor + 1 << ":" << std::endl;
        for (int row = 0; row < 10; row++) {
            std::cout << "  Row " << row + 1 << ": \t\t";
            for (int col = 0; col < 10; col++) {
                if (row == characterY && col == characterX) {
                    std::cout << "Px";
                } else {
                    std::cout << "  ";
                }
                std::cout << " x ";
            }
            std::cout << std::endl;
        }
    }
}

int Dungeon::checkCell(int floor, int x, int y){
    // Retorna del 1 al 4 dependiendo de si la celda tiene enemigo, tesoro, punto de salvado o taberna
    if (floors[floor][y][x].hasEnemy) {
        std::cout << "Un enemigo aparece!" << std::endl;
        return 1;
    } else if (floors[floor][y][x].hasTreasure) {
        std::cout << "Has encontrado un tesoro!" << std::endl;
        return 2;
    } else if (floors[floor][y][x].hasCheckPoint) {
        std::cout << "Has encontrado un punto de salvado!" << std::endl;
        return 3;
    } 
    std::cout << "Has encontrado una taberna!" << std::endl;
    return 4;
}

void Dungeon::serializeJSON(std::ostream& stream) {
    /*
    Esta funcion permite serializar la clase entera
    Serializar es basicamente "traducir" los datos a un formato
    que podamos escribir en algun archivo y guardar los datos.
    stream nos permite ir digamos que acumulando estos datos tal cual string,
    pero con la particularidad de que son binarizados, por eso la linea 73 tiene
    el casteo y el sizeof.
    */
    stream << "\"Dungeon\": {";
    for (int floor = 0; floor < 10; floor++) {
        stream << "\"Floor " << floor + 1 << "\": [";
        for (int row = 0; row < 10; row++) {
            stream << "{";
            for (int col = 0; col < 10; col++) {
                stream << "\"Cell " << col + 1 << "\": {";
                stream << "\"hasEnemy\": " << (floors[floor][row][col].hasEnemy ? "true" : "false") << ",";
                stream << "\"hasTreasure\": " << (floors[floor][row][col].hasTreasure ? "true" : "false") << ",";
                stream << "\"hasCheckPoint\": " << (floors[floor][row][col].hasCheckPoint ? "true" : "false") << ",";
                stream << "\"hasTavern\": " << (floors[floor][row][col].hasTavern ? "true" : "false");
                stream << "}";
                if (col != 9) {
                    stream << ",";
                }
            }
            stream << "}";
            if (row != 9) {
                stream << ",";
            }
        }
        stream << "]";
        if (floor != 9) {
            stream << ",";
        }
    }
    stream << "}";
}

void Dungeon::deserializeJSON(std::string jsonString) {
    nlohmann::json parsedData = nlohmann::json::parse(jsonString);
    std::cout << "\nDeserializando calabozo..." << std::endl;
    // Deserializar el calabozo
    for (int floor = 0; floor < 10; floor++) {
        for (int row = 0; row < 10; row++) {
            for (int col = 0; col < 10; col++) {
                floors[floor][row][col].hasEnemy = parsedData["Dungeon"]["Floor " + std::to_string(floor + 1)][row]["Cell " + std::to_string(col + 1)]["hasEnemy"];
                floors[floor][row][col].hasTreasure = parsedData["Dungeon"]["Floor " + std::to_string(floor + 1)][row]["Cell " + std::to_string(col + 1)]["hasTreasure"];
                floors[floor][row][col].hasCheckPoint = parsedData["Dungeon"]["Floor " + std::to_string(floor + 1)][row]["Cell " + std::to_string(col + 1)]["hasCheckPoint"];
                floors[floor][row][col].hasTavern = parsedData["Dungeon"]["Floor " + std::to_string(floor + 1)][row]["Cell " + std::to_string(col + 1)]["hasTavern"];
            }
        }
    }
}
