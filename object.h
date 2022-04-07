#ifndef OBJECT_H
#define OBJECT_H

#include <QColor>
#include <QGraphicsItem>
#include <math.h>
#include "equipments.h"

class Object : public QObject, public QGraphicsItem {

    Q_OBJECT

public:
    Object(){color_=QColor(0,0,0);}
    Object(QColor color, const int x, const int y);  // constructor

    int get_x() const { return x_; }  // inline member function
    int get_y() const { return y_; }  // inline member function
    int get_id() const { return id_; }
    QColor get_color() const { return color_; }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static int get_width() { return width_; }

    virtual void set_condition(int id);

    //return some information of the object. Now have been edited for checking treasure and player's hp, armor, attack only
    virtual std::vector<int> get_information(){return {-1};}

    void update_condition(int x, int y, int id, QColor color);

    std::map<Armor,int> get_armor(){return armor_list_;}

    std::map<Weapon,int> get_weapon(){return weapon_list_;}

    //equip the armor get from treasure, and return the updated armor value
    int equip_armor(std::map<Armor,int> armor_list);

    int equip_weapon(std::map<Weapon,int> weapon_list);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:

  friend bool operator==(const Object &first, const Object &other);

protected:
    //define the following parameters as protected so the child class can also access them
    int x_;
    int y_;
    QColor color_;
    // all our Cells will be 20 wide and tall
    static const int width_=20;
    //0:player, 1:empty, 2:treasure, 3:wall
    int id_;
    //armor types
    Armor wood={"wood",3,0};
    Armor bronze={"bronze",8,1};
    Armor silver={"silver",13,2};
    Armor_Up wood_up={"wood_up",8,3};
    Armor_Up bronze_up={"bronze_up",18,4};
    Armor_Up silver_up={"silver_up",28,5};
    //storage for each object
    //list of armor, each of them as an armor class, with int values that increases the armor_ value for the player
    //could also have some other effects
    std::map<Armor,int> armor_list_={{wood,0},{bronze,0},{silver,0},{wood_up,0},{bronze_up,0},{silver_up,0}};

    //weapon types
    Weapon bow={"bow",5,0};
    Weapon sword={"sword",10,1};
    Weapon gun={"gun",14,2};
    Weapon_Up bow_up={"bow_up",12,3};
    Weapon_Up sword_up={"dark sword",22,4};
    Weapon_Up gun_up={"gun_up",30,5};
    //storage for each object
    //list of weapon, each of them as an weapon class, with int values that increases the attack_ value for the player
    //could also have some other effects
    std::map<Weapon,int> weapon_list_={{bow,0},{sword,0},{gun,0},{bow_up,0},{sword_up,0},{gun_up,0}};

};  // class Object


class Player : public Object {

    Q_OBJECT

public:
    Player(QColor color, const int x, const int y);  // constructor
    std::vector<int> get_information(){return {hp_, armor_, attack_};}
    void update_status(int hp, int armor, int attack);
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    //set some default conditions for each player
    int hp_=10;
    int armor_=0;
    int attack_=5;

protected:


};  // class Player

class Treasure : public Object {

    Q_OBJECT

public:
    Treasure(QColor color, const int x, const int y);  // constructor
    //individual set_condition method that assign this treasure with different value of hp that could give to player
    void set_condition(int hp);
    std::vector<int> get_information(){return {hp_};}
    void store_armor(int id);
    void store_weapon(int id);

private:
    //hp stored for player to use
    int hp_;

protected:


};  // class Treasure

//use Factory as design pattern to generate different types of objects
class ObjectFactory{
public:
    static Object* GetWall(int i, int j);
    static Object* GetEmpty(int i, int j);
    static Object* GetTreasure(int i, int j);
    static Player* GetPlayer(int i, int j);
};


#endif
