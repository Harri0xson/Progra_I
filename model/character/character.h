#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <iostream>
#include <vector>
#include "../../nlohmann/json.hpp"

class Character {
private:
    int ps; // Health points
    int pa; // Attack points
    int floor; // Z position
    int x; // X position
    int y; // Y position
    std::string nombre; // Name
    std::vector<Character*>* party;  // El party con los miembros que se unan

public:
    Character(int pPs, int pPa, std::string pNombre);
    ~Character();

    int getPs();
    void setPs(int pPs);
    int getPa();
    void setPa(int pPa);
    int getFloor();
    void setFloor(int pFloor);
    int getX();
    void setX(int pX);
    int getY();
    void setY(int pY);
    std::vector<Character*>* getParty();
    std::string getName();
    void setName(std::string pNombre);

    int attack();
    void takeDamage(int damage);
    
    void increaseAttackPower(int attackPower);
    void increaseHealth();
    void recoverHealth();

    void addPartyMember(Character* pPartyMember);
    void printCharacter();

    bool moveUp();
    bool moveDown();
    bool moveLeft();
    bool moveRight();

    bool raiseFloor();

    void serializeJSON(std::ostream& stream);
    void deserializeJSON(std::string characterJSON);
};

#endif // CHARACTER_H
