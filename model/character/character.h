#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <iostream>
#include <vector>

class Character {
private:
    int ps; // Health points
    int pa; // Attack points
    std::string nombre; // Name
    std::vector<Character*>* party;  // El party con los miembros que se unan

public:
    Character(int pPs, int pPa, std::string pNombre);
    ~Character();
    void attack();
    int receiveAttack(int enemyAttackPoints);
    void addPartyMember(Character* pPartyMember);

    void serialize(std::ostream& stream);
};

#endif // CHARACTER_H
