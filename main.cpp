// Command to run 
// g++ -o main.exe main.cpp 
// ./main.exe

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <chrono>
#include <thread>

void printSlow(const std::string& pText, int pSleepTime) {
    for (char c : pText) {
        std::cout << c << std::flush;
        Sleep(pSleepTime);
    }
}

void welcome() {
    std::cout << "\n\n\t******************************" << std::endl;
    std::cout << "\t*                            *" << std::endl;
    std::cout << "\t*  Bienvenido al Calabozo    *" << std::endl;
    std::cout << "\t*     del Arcangel           *" << std::endl;
    std::cout << "\t*                            *" << std::endl;
    std::cout << "\t******************************" << std::endl;

    std::cout << "\n\n>>> Toca cualquier tecla para empezar" << std::endl;
    std::cout << ">>> ";

    // Wait for user input
    _getch();

}

void printRules() {
    /*

    Imprimir las siguientes reglas, de ser posible, de forma lenta

    El juego presenta un calabozo de 10 pisos
    El objetivo del jugador es avanzar por todos los
    pisos de calabazo con el objetivo de asesinar al 
    Arcángel que se encuentra en el décimo piso.

    En cada piso el jugador podrá encontrar enemigos 
    que intentarán lastimarlo, tabernas con mercenarios 
    que sepodrán unir al equipo de jugador, puntos de guardado 
    y cofres con sorpresas.
    
    */

    printSlow("\n\t>> El juego presenta un calabozo de 10 pisos <<\n", 25);
    printSlow("\n1- El objetivo del jugador es avanzar por todos los pisos de calabazo con \n", 25);
    printSlow("el objetivo de asesinar al Arcangel que se encuentra en el decimo piso.\n", 25);
    printSlow("\n2- En cada piso el jugador podra encontrar enemigos que intentaran lastimarlo, tabernas con mercenarios\n", 25);
    printSlow("que se podran unir al equipo de jugador, puntos de guardado y cofres con sorpresas.\n\n", 25);



}


int main() {

    welcome();
    printRules();
    return 0;
}

