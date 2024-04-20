/* 
Command to run 
g++ -o main.exe main.cpp model/character/character.cpp model/dungeon/dungeon.cpp utils/utils.cpp
./main.exe 
*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>
#include "nlohmann/json.hpp"

//Aqui haremos los includes de las estructuras de datos y clases
#include "model/character/character.h"
#include "model/dungeon/dungeon.h"
#include "utils/utils.h"


void createCheckpoint(Dungeon& dungeon, Character& character) {
    // Genera el nombre del archivo basado en la marca de tiempo
    std::stringstream filenameStream;
    filenameStream << "CheckPoint.json";
    std::string filename = filenameStream.str();

    // Abre el archivo en modo binario
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    file << "{";

    // Serializa y escribe la instancia de Dungeon
    dungeon.serializeJSON(file);
    file << ",";

    // Serializa y escribe la instancia de Character
    character.serializeJSON(file);

    file << "}";

    // Cierra el archivo
    file.close();

    std::cout << "Checkpoint creado exitosamente en " << filename << std::endl;
}

// Función para cargar un checkpoint desde un archivo JSON
void loadCheckpoint(Dungeon& dungeon, Character*& personajePrincipal) {
    
    // imprimir el mensaje de carga
    std::cout << "Cargando el checkpoint..." << std::endl;
    std::ifstream checkpointFile("CheckPoint.json");
    if (!checkpointFile.is_open()) {
        std::cerr << "Error al abrir el archivo de checkpoint." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << checkpointFile.rdbuf();
    std::string jsonString = buffer.str();
    checkpointFile.close();
    
    std::cout << "Deserializando dungeon..." << std::endl;
    dungeon.deserializeJSON(jsonString);

    std::cout << "Deserializando Character..." << std::endl;
    personajePrincipal = new Character(0, 0, "");
    personajePrincipal->deserializeJSON(jsonString);
}

void printSlow(const std::string& pText, int pSleepTime) {
    /*
    Una funcion de impresion linda caracter por caracter.
    Lo que hace es recorrer un string con el for, imprimir y luego esperar un tiempo dado.
    */
    for (char c : pText) {
        std::cout << c << std::flush;
        Sleep(pSleepTime);
    }
}

void welcome() {
    /*
    Solo imprime la bienvenida de manera bonita y espera a que el usuario le de a cualquier letra para 
    empezar el juego, luego de esto deberia dar las reglas en la funcion printRules()
    */
    std::cout << "\n\n\t******************************" << std::endl;
    std::cout << "\t*                            *" << std::endl;
    std::cout << "\t*  Bienvenido al Calabozo    *" << std::endl;
    std::cout << "\t*     del Arcangel           *" << std::endl;
    std::cout << "\t*                            *" << std::endl;
    std::cout << "\t******************************" << std::endl;
    std::cout << "\n\n>>> Toca cualquier tecla para empezar..." << std::endl;
    // Esto es para que el usuario ingrese una tecla y continue, simulando los juegos de verdad
    _getch();

}

void printRules() {
    /*
    Aqui se imprimen las reglas, un contexto para alguien que no tiene idea de que es esto
    Se usa la funcion printSlow que da ese toquecillo suave y progresivo de impresion,
    asi el juego a pesar de no tener interfaz, se ve bien y es entretenido.
    */
    int textSpeed = 1;
    printSlow("\n\t>> El juego presenta un calabozo de 10 pisos <<\n", textSpeed);
    printSlow("\n1- El objetivo del jugador es avanzar por todos los pisos de calabazo con \n", textSpeed);
    printSlow("el objetivo de asesinar al Arcangel que se encuentra en el decimo piso.\n", textSpeed);
    printSlow("\n2- En cada piso el jugador podra encontrar enemigos que intentaran lastimarlo, tabernas con mercenarios ", textSpeed);
    printSlow("que se podran unir al equipo de jugador, puntos de guardado y cofres con sorpresas.\n\n", textSpeed);

}

void combat(Character& character, Character& enemy) {
    std::cout << "Combate! " << character.getName() << " vs. " << enemy.getName() << std::endl;
    int damage = 0;
    while (character.getPs() > 0 && enemy.getPs() > 0) {

        // Elegir quien ataca primero de forma aleatoria
        int firstAttack = rollDice(2);
        if (firstAttack == 1) {
            std::cout << character.getName() << " ataca primero." << std::endl;
            damage = character.attack();
            enemy.takeDamage(damage);
            std::cout << character.getName() << " ataca a " << enemy.getName() << " por " << damage << " puntos de danio." << std::endl;
            if (enemy.getPs() <= 0) {
                std::cout << enemy.getName() << " ha sido derrotado." << std::endl;
                break;
            }
            damage = enemy.attack();
            // Si el party tiene miembros, se eligira uno aleatoriamente para recibir el ataque
            if (character.getParty()->size() > 0) {
                int partyMemberIndex = rollDice(character.getParty()->size()) - 1;
                Character* partyMember = character.getParty()->at(partyMemberIndex);
                partyMember->takeDamage(damage);
                std::cout << enemy.getName() << " ataca a " << partyMember->getName() << " por " << damage << " puntos de danio." << std::endl;
                if (partyMember->getPs() <= 0) {
                    character.getParty()->erase(character.getParty()->begin() + partyMemberIndex);
                    std::cout << partyMember->getName() << " ha sido derrotado." << std::endl;
                }
            }
            else {
                character.takeDamage(damage);
                std::cout << enemy.getName() << " ataca a " << character.getName() << " por " << damage << " puntos de danio." << std::endl;
            }
        } else {
            std::cout << enemy.getName() << " ataca primero." << std::endl;
            damage = enemy.attack();
            // Si el party tiene miembros, se eligira uno aleatoriamente para recibir el ataque
            if (character.getParty()->size() > 0) {
                int partyMemberIndex = rollDice(character.getParty()->size()) - 1;
                Character* partyMember = character.getParty()->at(partyMemberIndex);
                partyMember->takeDamage(damage);
                std::cout << enemy.getName() << " ataca a " << partyMember->getName() << " por " << damage << " puntos de danio." << std::endl;
                if (partyMember->getPs() <= 0) {
                    character.getParty()->erase(character.getParty()->begin() + partyMemberIndex);
                    std::cout << partyMember->getName() << " ha sido derrotado." << std::endl;
                }
            }
            else {
                character.takeDamage(damage);
                std::cout << enemy.getName() << " ataca a " << character.getName() << " por " << damage << " puntos de danio." << std::endl;
                if(character.getPs() <= 0){
                    std::cout << character.getName() << " ha sido derrotado por " << enemy.getName() << "." << std::endl;
                    printSlow("Erresire, el valiente aventurero, ha caido en batalla... \n", 50);
                    exit(0);
                }
                damage = character.attack();
                enemy.takeDamage(damage);
                std::cout << character.getName() << " ataca a " << enemy.getName() << " por " << damage << " puntos de danio." << std::endl;
                if (enemy.getPs() <= 0) {
                    std::cout << enemy.getName() << " ha sido derrotado." << std::endl;
                    break;
                }
            }
            std::cout << character.getName() << ": " << character.getPs() << " HP" << std::endl;
            for (Character* member : *character.getParty()) {
                std::cout << member->getName() << ": " << member->getPs() << " HP" << std::endl;
            }
            std::cout << enemy.getName() << ": " << enemy.getPs() << " HP" << std::endl;
        }
    }
}

void tavern(Character& character) {
    // Muestra el mensaje de la taberna
    std::cout << "Taberna! " << character.getName() << " ha encontrado una taberna." << std::endl;
    if (character.getParty()->size() >= 3) {
        std::cout << "Tu party está lleno! No puedes contratar a más mercenarios." << std::endl;
        return;
    }

    std::cout << "Deseas contratar a un mercenario? (s/n): ";
    char response = _getch();
    std::cout << response << std::endl;    

    if (response == 's') {
        Character* mercenary = new Character(character.getPs()-1, character.getPa()-1, "Mercenario");
        character.addPartyMember(mercenary);
        std::cout << "Bienvenido a la aventura, " << mercenary->getName() << "!" << std::endl;
    }
}

void treasure(Character& character) {
    int option = rollDice(3);
    int attackPower = 0;
    switch (option) {
        case 1:
            attackPower = rollDice(2);
            character.increaseAttackPower(attackPower);
            std::cout << "¡Tesoro! " << character.getName() << " ha encontrado un arma. + " << attackPower << " puntos de ataque." << std::endl;
            break;
        case 2:
            character.increaseHealth();
            std::cout << "¡Tesoro! " << character.getName() << " ha encontrado un aumento de salud." << std::endl;
            break;
        case 3:
            character.recoverHealth();
            std::cout << "¡Tesoro! " << character.getName() << " ha encontrado una recuperación de salud." << std::endl;
            break;
    }
}

void checkpoint(Character& character, Dungeon& dungeon) {
    std::cout << "¡Punto de guardado! " << character.getName() << " ha encontrado un punto de guardado." << std::endl;
    createCheckpoint(dungeon, character);
}

void moveCharacter(Character& character, Dungeon& dungeon) {
    // Ciclo hasta que el personaje este en la casilla final del piso
    int tiradas = 0;
    int dice1 = 0;
    int dice2 = 0;
    bool validDirection = false;
    char direction;
    while (true && tiradas < 15) {
        dungeon.printFloor(character.getFloor(), character.getX(), character.getY(), false);
        // Se lanzan dos veces los dados y se guardan la cantidad de movimientos
        dice1 = rollDice(6);
        dice2 = rollDice(6);
        // Imprmimos los resultados de los dados
        std::cout << "Dado 1: " << dice1 << std::endl;
        std::cout << "Dado 2: " << dice2 << std::endl;
        
        while(!validDirection){
            std::cout << "Mueve al personaje (w: arriba, a: izquierda, s: abajo, d: derecha): ";
            direction = _getch();
            // Si se ingresa Ctrl + C, se termina el juego
            if(direction == 3){
                std::cout << "\n\nJuego cancelado por comando del teclado." << std::endl;
                exit(0);
            }
            std::cout << direction << std::endl;
            if(direction == 'w' || direction == 'a' || direction == 's' || direction == 'd'){
                validDirection = true;
            }
            else{ 
                std::cout << "Movimiento invalido, intenta de nuevo" << std::endl;
                validDirection = false;
            }
        }        
        tiradas++;
        while(dice1 > 0 && tiradas < 15) {
            switch (direction) {
                case 'w':
                    // Mueve el personaje las dice1 veces
                    character.moveUp();
                    dice1--;
                    break;
                case 'a':
                    character.moveLeft();
                    dice1--;
                    break;
                case 's':
                    character.moveDown();
                    dice1--;
                    break;
                case 'd':
                    character.moveRight();
                    dice1--;
                    break;
                default:
                    break;
            }
            if (character.getX() == 9 && character.getY() == 9) {
                break;
            }
        }
        if (character.getX() == 9 && character.getY() == 9) {
            break;
        }
        validDirection = false;
        dungeon.printFloor(character.getFloor(), character.getX(), character.getY(), false);
        
        while(!validDirection){
            std::cout << "Mueve al personaje (w: arriba, a: izquierda, s: abajo, d: derecha): ";
            direction = _getch();
            if(direction == 3){
                std::cout << "\n\nJuego cancelado por comando del teclado." << std::endl;
                exit(0);
            }
            if(direction == 'w' || direction == 'a' || direction == 's' || direction == 'd'){
                validDirection = true;
            }
            else{ 
                std::cout << "Movimiento invalido, intenta de nuevo" << std::endl;
                validDirection = false;
            }
        }
        while(dice2 > 0 && tiradas < 15) {
            switch (direction) {
                case 'w':
                    // Mueve el personaje las dice2 veces
                    character.moveUp();
                    dice2--;
                    break;
                case 'a':
                    character.moveLeft();
                    dice2--;
                    break;
                case 's':
                    character.moveDown();
                    dice2--;
                    break;
                case 'd':
                    character.moveRight();
                    dice2--;
                    break;
                default:
                    break;
            }
            if (character.getX() == 9 && character.getY() == 9) {
                break;
            }
        }
        validDirection = false;
        if (character.getX() == 9 && character.getY() == 9) {
            break;
        }
        
        dungeon.printFloor(character.getFloor(), character.getX(), character.getY(), false);       
        int cellType = dungeon.checkCell(character.getFloor(), character.getX(), character.getY());
        Character* enemy = nullptr;
        switch (cellType) {
            case 1:
                enemy = new Character(character.getFloor() + 1, character.getFloor(), "Esbirro nivel " + std::to_string(character.getFloor() + 1));
                combat(character, *enemy);
                break;
            case 2:
                treasure(character);
                break;
            case 3:
                checkpoint(character, dungeon);
                break;
            case 4:
                tavern(character);
                break;
        }
    
        // Si el personaje ha llegado a la casilla final del piso, se rompe el ciclo
        if (character.getX() == 9 && character.getY() == 9) {
            break;
        }
    }
    if(tiradas == 15){
        std::cout << "¡Has llegado al limite de movimientos! " << character.getName() << " ha sido derrotado." << std::endl;
        return;
    }
}

void playGame(Character& character, Dungeon& dungeon) {
    // Muestra el mensaje de inicio
    std::cout << "Comienza la aventura! " << character.getName() << " se encuentra en el piso 1." << std::endl;

    // Mueve al personaje por los 10 pisos del calabozo
    while (character.getFloor() < 10) {
        
        if (character.getPs() <= 0) {
            std::cout << "¡Game Over! " << character.getName() << " ha sido derrotado." << std::endl;
            break;
        }
        if (character.getFloor() == 8) {
            printSlow("\n\nErresire ha llegado al piso 10... \n\n\nEs oscuro, solo se nota una pequena baldosa brillante, deseas tocarla? (s/n)", 50);
            char response = _getch();
            if (response == 's') {
                printSlow("\n\nErresire decide tocar la baldosa...\n", 50);
                printSlow("Cuando Erresire toca la baldosa, una luz cegadora lo envuelve...\n", 45);
                printSlow("Cuando la luz se disipa, se encuentra en un lugar desconocido, una voz le habla: \n", 40);
                printSlow("Bienvenido, valiente aventurero. \nHas llegado al piso final del calabozo. \n", 40);
                printSlow("Desde antiguos tiempos... ha existido una leyenda sobre un ser supremo que protege el calabozo. \n", 35);
                printSlow("El Arcangel, un ser de luz y oscuridad, que ha sido el guardian de este lugar por siglos. \n", 30);
                printSlow("Solo aquellos valientes que han logrado superar los desafios de los 9 pisos anteriores, pueden enfrentarlo. \n", 25);
                printSlow("Preparate para la batalla final! Que comience el combate! \n", 25);
            }
            else {
                std::cout << "Erresire decide no tocar la baldosa y se retira del calabozo... \n";
                std::cout << "Guardando partida..." << std::endl;
                Sleep(2000);
                checkpoint(character, dungeon);
                exit(0);
            }
            Character* archangel = new Character(11, 10, "Arcangel");
            combat(character, *archangel);
            break;
        }
        moveCharacter(character, dungeon);
        character.raiseFloor();
        std::cout << "Subiendo al piso " << character.getFloor() + 1 << std::endl;
    }
    std::cout << "\n\nFelicidades! " << character.getName() << " ha derrotado al Arcangel y ha completado el calabozo." << std::endl;
}

void TestSerialization(){
    Dungeon* dungeon = new Dungeon();
    Character* personajePrincipal = new Character(3, 3, "Erresire");
    Character* party1 = new Character(2, 4, "Fjoldir");
    Character* party2 = new Character(3, 2, "Mike");
    Character* enemy = new Character(2, 6, "Ydrassil");
    personajePrincipal->addPartyMember(party1);
    personajePrincipal->addPartyMember(party2);
    std::cout << "Creamos el dungeon" << std::endl;

    std::cout << "Creamos el checkpoint" << std::endl;
    createCheckpoint(*dungeon, *personajePrincipal);
    std::cout << "Creado" << std::endl;
    Dungeon dungeonCheckpoint;
    Character* personajePrincipalCheckpoint = nullptr;
    std::cout << "Ahora vamos a cargar el checkpoint antes creado con members Tony y Mike" << std::endl;
    loadCheckpoint(dungeonCheckpoint, personajePrincipalCheckpoint);
    std::cout << "Ahora veremos lo que teniamos cargado antes en Dungeon" << std::endl;
    //dungeonCheckpoint.printDungeon();
    personajePrincipalCheckpoint->printCharacter();
    std::cout << "Dungeon recuperado" << std::endl;
}

int main() {
    /* Aqui empieza la ejecucion y se llaman todas las funciones que componen el juego.
    Tambien se instancian las estructuras de datos necesarias para jugar. */
    welcome();
    printRules();

    Dungeon* dungeon = new Dungeon();
    Character* personajePrincipal = new Character(3, 3, "Erresire");

    // Preguntar si desea cargar el ultimo checkpoint
    std::cout << "Desea cargar el ultimo checkpoint? (s/n): ";
    char response = _getch();
    std::cout << response << std::endl;
    if (response == 's') {
        loadCheckpoint(*dungeon, personajePrincipal);
        // Imprimir mensaje de reanudando juego
        std::cout << "Reanudando el juego..." << std::endl;
        Sleep(2000);
    }
    else {
        std::cout << "Empezando nueva partida..." << std::endl;
        Sleep(2000);
    }
    playGame(*personajePrincipal, *dungeon);
    std::cout << "Gracias por jugar!" << std::endl;
    return 0;
}

