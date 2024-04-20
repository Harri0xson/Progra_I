#include "character.h"

Character::Character(int pPs, int pPa, std::string pNombre) {
    ps = pPs; // Initial health points
    pa = pPa; // Initial attack points
    x = 0; // Initial X position
    y = 0; // Initial Y position
    floor = 0; // Initial Z position
    nombre = pNombre; // Name
    party = new std::vector<Character*>();
}

Character::~Character() {}

std::string Character::getName() { 
    return nombre; 
}

void Character::setName(std::string pNombre) { 
    nombre = pNombre; 
}

int Character::getPs() { 
    return ps; 
}

void Character::setPs(int pPs) { 
    ps = pPs; 
}

int Character::getPa() { 
    return pa; 
}

void Character::setPa(int pPa) { 
    pa = pPa; 
}

int Character::getFloor() { 
    return floor; 
}

void Character::setFloor(int pFloor) { 
    floor = pFloor; 
}

int Character::getX() { 
    return x; 
}

void Character::setX(int pX) { 
    x = pX; 
}

int Character::getY() { 
    return y; 
}

void Character::setY(int pY) { 
    y = pY; 
}

std::vector<Character*>* Character::getParty() { 
    return party; 
}

void Character::increaseAttackPower(int attackPower) {
    pa += attackPower;
    std::cout << nombre << " aumento su poder de ataque a " << pa << " puntos." << std::endl;
}

void Character::increaseHealth() {
    ps += 1;
    std::cout << nombre << " aumento su salud a " << ps << " puntos." << std::endl;
}

void Character::recoverHealth() {
    //Recuperar el 10% de la vida, minimo se recupera 1 punto de vida
    int recovery = ps / 10;
    ps += recovery > 0 ? recovery : 1;
    std::cout << nombre << " recupero su salud a " << ps << " puntos." << std::endl;
}


int Character::attack() {
    return pa;
}

void Character::takeDamage(int damage) {
    ps -= damage;
}

void Character::addPartyMember(Character* pPartyMember){
    party->push_back(pPartyMember);
    std::cout << "Se unio: " << pPartyMember->nombre << " al grupo!" << std::endl;
}

void Character::printCharacter() {
    std::cout << "Character Name: " << nombre << std::endl;
    std::cout << "Health Points (ps): " << ps << std::endl;
    std::cout << "Attack Points (pa): " << pa << std::endl;

    if (party != nullptr && !party->empty()) {
        std::cout << "Party Members:" << std::endl;
        for (Character* member : *party) {
            std::cout << "- " << member->getName() << std::endl;
        }
    } else {
        std::cout << "Party: None" << std::endl;
    }
}

bool Character::moveUp(){
    // cheequea si aun puede moverse en esa direccion, son 10 casillas
    if (y > 0) {
        y--;
        return true;
    }
    return false;
}

bool Character::moveDown(){
    if (y < 9) {
        y++;
        return true;
    }
    return false;
}

bool Character::moveLeft(){
    if (x > 0) {
        x--;
        return true;
    }
    return false;
}

bool Character::moveRight(){
    if (x < 9) {
        x++;
        return true;
    }
    return false;
}

bool Character::raiseFloor(){
    if (floor < 9) {
        floor++;
        x = 0;
        y = 0;
        return true;
    }
    return false;
}

void Character::serializeJSON(std::ostream& stream){
    stream << "\"Character\": {";
    stream << "\"ps\": " << ps << ",";
    stream << "\"pa\": " << pa << ",";
    stream << "\"nombre\": \"" << nombre << "\",";
    stream << "\"x\": " << x << ",";
    stream << "\"y\": " << y << ",";
    stream << "\"floor\": " << floor << ",";
    stream << "\"party\": [";
    for (size_t i = 0; i < party->size(); i++) {
        // Serializar cada miembro del party sin usar el metodo serializeJSON
        stream << "{";
        stream << "\"ps\": " << party->at(i)->ps << ",";
        stream << "\"pa\": " << party->at(i)->pa << ",";
        stream << "\"nombre\": \"" << party->at(i)->nombre << "\"";
        stream << "}";
        if (i < party->size() - 1) {
            stream << ",";
        }    
    }
    stream << "]";
    stream << "}";
}

//Deserializar un personaje usando nlohmann/json
void Character::deserializeJSON(std::string jsonString){
    nlohmann::json parsedData = nlohmann::json::parse(jsonString);
    // Deserializar el personaje
    ps = parsedData["Character"]["ps"];
    pa = parsedData["Character"]["pa"];
    nombre = parsedData["Character"]["nombre"];
    x = parsedData["Character"]["x"];
    y = parsedData["Character"]["y"];
    floor = parsedData["Character"]["floor"];
    // Deserializar el party
    for (nlohmann::json partyMember : parsedData["Character"]["party"]) {
        Character* member = new Character(partyMember["ps"], partyMember["pa"], partyMember["nombre"]);
        party->push_back(member);
    }
}
