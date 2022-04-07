#ifndef EQUIPMENTS_H
#define EQUIPMENTS_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>


class Armor {

    //Q_OBJECT

public:
    Armor(){updated_=false;}
    Armor(std::string name, const int armor, const int id);  // constructor
    int get_armor() const { return armor_; }  // inline member function
    int get_id() const{return id_;}
    std::string get_name() {return name_;}
    bool if_updated() const { return updated_; }
    //write operator so Armor objects can be placed into map and vector in an order
    bool operator==(const Armor &other) {
        return id_ == other.id_;
    }
    bool operator<(const Armor &other)  const {
        return id_ < other.id_;
    }


private:


protected:
    int armor_;
    bool updated_;
    int id_;
    std::string name_;

};  // class Armor

class Armor_Up: public Armor {

    //Q_OBJECT

public:
    Armor_Up(std::string name, const int armor, const int id);  // constructor

private:

protected:

};  // class Armor_Up

class Weapon {

    //Q_OBJECT

public:
    Weapon(){updated_=false;}
    Weapon(std::string name, const int attack, const int id);  // constructor
    int get_attack() const { return attack_; }  // inline member function
    int get_id() const{return id_;}
    std::string get_name() {return name_;}
    bool if_updated() const { return updated_; }
    //write operator so Armor objects can be placed into map and vector in an order
    bool operator==(const Weapon &other) {
        return id_ == other.id_;
    }
    bool operator<(const Weapon &other)  const {
        return id_ < other.id_;
    }


private:


protected:
    int attack_;
    bool updated_;
    int id_;
    std::string name_;

};  // class Weapon

class Weapon_Up: public Weapon {

    //Q_OBJECT

public:
    Weapon_Up(std::string name, const int attack, const int id);  // constructor


private:

protected:

};  // class Weapon_Up

#endif
