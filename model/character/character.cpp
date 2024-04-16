#include "character.h"

Character::Character(int pPs, int pPa, std::string pNombre) {
    ps = pPs; // Initial health points
    pa = pPa; // Initial attack points
    nombre = pNombre; // Name
    party = new std::vector<Character*>();
}

Character::~Character() {}

void Character::attack() {
    std::cout << nombre << " ataca con " << pa << " puntos de ataque." << std::endl;
}

int Character::receiveAttack(int enemyAttackPoints) {
    ps -= enemyAttackPoints;
    if (ps <= 0) {
        std::cout << nombre << " fue derrotado.\nHas muerto..." << std::endl;
        return 0;
    }
    return ps;
}

void Character::addPartyMember(Character* pPartyMember){
    party->push_back(pPartyMember);
    std::cout << "Se unio: " << pPartyMember->nombre << " al grupo!" << std::endl;
}

void Character::serialize(std::ostream& stream) {
    /*
    Esta funcion permite serializar la clase entera
    Serializar es basicamente "traducir" los datos a un formato
    que podamos escribir en algun archivo y guardar los datos.
    stream nos permite ir digamos que acumulando estos datos tal cual string,
    pero con la particularidad de que son binarizados, por eso tantos casteos y sizeof's
    */
    stream.write(reinterpret_cast<const char*>(&ps), sizeof(int));
    stream.write(reinterpret_cast<const char*>(&pa), sizeof(int));
    stream << nombre << '\n';
    size_t partySize = party->size();
    stream.write(reinterpret_cast<const char*>(&partySize), sizeof(size_t));
    for (Character* member : *party) {
        stream.write(reinterpret_cast<const char*>(&member), sizeof(Character*));
    }
}
