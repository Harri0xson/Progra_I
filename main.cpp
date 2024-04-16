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

//Aqui haremos los includes de las estructuras de datos y clases
#include "model/character/character.h"
#include "model/dungeon/dungeon.h"
#include "utils/utils.h"

void createCheckpoint(Dungeon& dungeon, Character& character) {
    // Genera el nombre del archivo basado en la marca de tiempo
    std::stringstream filenameStream;
    filenameStream << "CheckPoint_" << std::time(nullptr) << ".saved";
    std::string filename = filenameStream.str();

    // Abre el archivo en modo binario
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    // Serializa y escribe la instancia de Dungeon
    dungeon.serialize(file);

    // Serializa y escribe la instancia de Character
    character.serialize(file);

    // Cierra el archivo
    file.close();

    std::cout << "Checkpoint creado exitosamente en " << filename << std::endl;
}

void loadCheckpoint(Dungeon& dungeon, Character*& personajePrincipal) {
    /*
    Carga un checkpoint ya previamente guardado, recreando el Dungeon y el personaje principal
    Hardcoded, hay que hacer que el usuario vea sus checkpoints disponibles y pueda cargar alguno de todos
    Idealmente, cargar los nombres de archivos de Checkpoint, mostrarlos en pantalla y que el usuario
    elija cual de todos desea cargar al juego.
    Si no hay checkpoints, se le avisa y que empiece un juego nuevo
    */

    std::ifstream checkpointFile("CheckPoint_1713229588.saved", std::ios::binary);
    if (!checkpointFile.is_open()) {
        std::cerr << "Error al abrir el archivo de checkpoint." << std::endl;
        return;
    }
    checkpointFile.read(reinterpret_cast<char*>(&dungeon), sizeof(Dungeon));
    personajePrincipal = new Character(0, 0, "");  // Crear un personaje temporal
    checkpointFile.read(reinterpret_cast<char*>(personajePrincipal), sizeof(Character));
    int partySize;
    checkpointFile.read(reinterpret_cast<char*>(&partySize), sizeof(int));
    for (int i = 0; i < partySize; ++i) {
        Character* partyMember = new Character(0, 0, "");  // Crear un miembro temporal
        checkpointFile.read(reinterpret_cast<char*>(partyMember), sizeof(Character));
        personajePrincipal->addPartyMember(partyMember);
    }
    checkpointFile.close();
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
    int textSpeed = 25;
    printSlow("\n\t>> El juego presenta un calabozo de 10 pisos <<\n", textSpeed);
    printSlow("\n1- El objetivo del jugador es avanzar por todos los pisos de calabazo con \n", textSpeed);
    printSlow("el objetivo de asesinar al Arcangel que se encuentra en el decimo piso.\n", textSpeed);
    printSlow("\n2- En cada piso el jugador podra encontrar enemigos que intentaran lastimarlo, tabernas con mercenarios ", textSpeed);
    printSlow("que se podran unir al equipo de jugador, puntos de guardado y cofres con sorpresas.\n\n", textSpeed);

}


int main() {
    /*
    Aqui empieza la ejecucion y se llaman todas las funciones que componen el juego.
    Tambien se instancian las estructuras de datos necesarias para jugar.
    */

    welcome();
    printRules();
    
    Character* personajePrincipal = new Character(3, 3, "Erresire");
    Character* party1 = new Character(2, 4, "Fjoldir");
    Character* party2 = new Character(3, 2, "Mike");
    Character* enemy = new Character(2, 6, "Ydrassil");
    Character* archangel = new Character(3, 12, "Archangel");
    personajePrincipal->addPartyMember(party1);
    personajePrincipal->addPartyMember(party2);
    Dungeon* dungeon = new Dungeon();
    createCheckpoint(*dungeon, *personajePrincipal);

    Dungeon dungeonCheckpoint;
    Character* personajePrincipalCheckpoint = nullptr;

    std::cout << "Ahora vamos a cargar el checkpoint" << std::endl;
    loadCheckpoint(dungeonCheckpoint, personajePrincipalCheckpoint);

    std::cout << "Ahora veremos lo que teniamos cargado antes en Dungeon" << std::endl;
    dungeonCheckpoint.printDungeon();

    return 0;
}

