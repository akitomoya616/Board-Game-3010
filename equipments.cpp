#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <random>
#include <math.h>
#include "equipments.h"

//assign armor and weapons based on the armor and attack value. Also check their updated status

Armor::Armor(std::string name, const int armor, const int id) {
    armor_=armor;
    id_=id;
    name_=name;
    updated_=false;
}

Armor_Up::Armor_Up(std::string name, const int armor, const int id) {
    armor_=armor;
    id_=id;
    name_=name;
    updated_=true;
}

Weapon::Weapon(std::string name, const int attack, const int id) {
    attack_=attack;
    id_=id;
    name_=name;
    updated_=false;
}

Weapon_Up::Weapon_Up(std::string name, const int attack, const int id) {
    attack_=attack;
    id_=id;
    name_=name;
    updated_=true;
}
