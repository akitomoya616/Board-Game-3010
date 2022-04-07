#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>
#include <random>
#include <math.h>
#include "object.h"

/**
  Creates a new Point object with coordinates x and y
  @param x int x coordinate
  @param y int y coordinate
*/
Object::Object(QColor color, const int x, const int y) {
  this->color_ = color;
  x_ = x;
  y_ = y;
  //id pre-set to empty
  id_ = 1;
}

void Object::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        qDebug() << "left clicked!";
    }
    else if(event->button()==Qt::RightButton){
        qDebug() << "right clicked!";
    }
    update();
}

QRectF Object::boundingRect() const
{
    return QRectF(x_, y_, width_, width_);
}

QPainterPath Object::shape() const
{
    QPainterPath path;
    path.addRect(x_, y_, width_, width_);
    return path;
}


void Object::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    QBrush b = painter->brush();
    painter->setBrush(QBrush(color_));
    painter->drawRect(QRect(this->x_, this->y_, this->width_, this->width_));
    painter->setBrush(b);
}

//pre-set the condition for each cell while initializing the board
void Object::set_condition(int id){
    //use the id number to check the following cases
    if(id==1){
        //empty cell
        color_=QColor(255,255,255);
        id_=1;
    }
    else if(id==3){
        //wall
        color_=QColor(0,0,0);
        id_=3;
    }
}

//update condition on x, y value only to re-shape the cell with the right starting (x,y) coordinate in the board
void Object::update_condition(int x, int y, int id, QColor color){
    x_=x;
    y_=y;
    id_=id;
    color_=color;
    update();
}

int Object::equip_armor(std::map<Armor, int> armor_list){
    int armor_value=0;
    int id=-1;
    //go through the treasure's armor list and add value to player's treasure list
    for(std::pair<Armor, int> pair:armor_list){
        //qDebug()<<pair.first.get_id()<<" has "<<pair.first.get_armor()<<" armor value, and we have "<<pair.second;
        //update the amount of armor stored in the map
        armor_list_.find(pair.first)->second+=pair.second;
        if(armor_list_.find(pair.first)->second>1 && !armor_list_.find(pair.first)->first.if_updated()){
            qDebug()<<"need update for armor: "<<pair.first.get_id();
            //if now we have the same armor for more than one and it is not the updated form
            //clear them and add one to the updated version
            armor_list_.find(pair.first)->second=0;
            //store the id of it, then we know the updated_armor has id+3
            id=armor_list_.find(pair.first)->first.get_id();
        }
    }
    //now in player's treasure list, sum up the armor value and update it
    //still go through treasure's armor list cause otherwise the update in player's list will not be saved.
    //Cannot go through the same array and keep updating information in the member that is not the current value
    for(std::pair<Armor, int> pair_re:armor_list){
        if(id>=0 && pair_re.first.get_id()==id+3){
            armor_list_.find(pair_re.first)->second+=1;
            //pair_re.second+=1;
            //qDebug()<<"find it at id: "<<pair_re.first.get_id()<<" has "<<pair_re.first.get_armor()<<" armor value, and the value we have after updating "<<armor_list_.find(pair_re.first)->second;
        }
        qDebug()<<armor_list_.find(pair_re.first)->first.get_id()<<" has "<<armor_list_.find(pair_re.first)->first.get_armor()<<" armor value, and we have "<<armor_list_.find(pair_re.first)->second;
        armor_value+=armor_list_.find(pair_re.first)->first.get_armor()*armor_list_.find(pair_re.first)->second;
    }
    return armor_value;
}

int Object::equip_weapon(std::map<Weapon, int> weapon_list){
    int attack_value=0;
    int id=-1;
    //go through the treasure's armor list and add value to player's treasure list
    for(std::pair<Weapon, int> pair:weapon_list){
        //update the amount of armor stored in the map
        weapon_list_.find(pair.first)->second+=pair.second;
        if(weapon_list_.find(pair.first)->second>1 && !weapon_list_.find(pair.first)->first.if_updated()){
            //if now we have the same armor for more than one and it is not the updated form
            //clear them and add one to the updated version
            weapon_list_.find(pair.first)->second=0;
            //store the id of it, then we know the updated_armor has id+3
            id=weapon_list_.find(pair.first)->first.get_id();
        }
    }
    //now in player's treasure list, sum up the armor value and update it
    //still go through treasure's weapon list cause otherwise the update in player's list will not be saved.
    //Cannot go through the same array and keep updating information in the member that is not the current value
    for(std::pair<Weapon, int> pair_re:weapon_list){
        if(id>=0 && pair_re.first.get_id()==id+3){
            weapon_list_.find(pair_re.first)->second+=1;
            //pair_re.second+=1;
        }
        attack_value+=weapon_list_.find(pair_re.first)->first.get_attack()*weapon_list_.find(pair_re.first)->second;
    }
    return attack_value;
}

//assign each player with default id=0
Player::Player(QColor color, const int x, const int y) {
  this->color_ = color;
  x_ = x;
  y_ = y;
  id_ = 0;
}

//update hp, reassign armor and attack value
void Player::update_status(int hp, int armor, int attack){
    //update hp value
    hp_+=hp;
    //assign hp to 0 if the current hp is less than 0
    if(hp_<0){
        hp_=0;
    }
    //re-calculate the current armor value from the equipped armor
    armor_=armor;

    //extra step to check if armor now is 0, then player lose all armor
    if(armor==0){
        //qDebug()<<"lose all armor!";
        for(std::pair<Armor, int> pair:armor_list_){
            armor_list_.find(pair.first)->second=0;
        }
//        for(std::pair<Armor, int> pair:armor_list_){
//            qDebug()<<armor_list_.find(pair.first)->first.get_id()<<" has "<<armor_list_.find(pair.first)->first.get_armor()<<" armor value, and we have "<<armor_list_.find(pair.first)->second;
//        }
    }

    //re-calculate the attack value from the equipped weapon
    attack_=attack;
}

void Player::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        qDebug() << "left clicked! and it is a player!";
    }
    else if(event->button()==Qt::RightButton){
        qDebug() << "right clicked! and it is a player!";
    }
}

Treasure::Treasure(QColor color, const int x, const int y) {
  this->color_ = color;
  x_ = x;
  y_ = y;
  id_ = 2;
}

void Treasure::store_armor(int id){
    if(id==0){
        //add one to wood
        armor_list_.find(wood)->second++;
    }
    else if(id==1){
        //add one to bronze
        armor_list_.find(bronze)->second++;
    }
    else{
        //add one to silver
        armor_list_.find(silver)->second++;
    }
}

void Treasure::store_weapon(int id){
    if(id==0){
        //add one to bow
        weapon_list_.find(bow)->second++;
    }
    else if(id==1){
        //add one to sword
        weapon_list_.find(sword)->second++;
    }
    else{
        //add one to gun
        weapon_list_.find(gun)->second++;
    }
}

//pre-set the hp value, together with some weapon and armor objects in it
void Treasure::set_condition(int hp){
    hp_=hp;
    color_=QColor(155,155,255);
    id_=2;
    //then set some armors and weapons
    //generate random value of armor
    int random_armor=rand()%3;
    store_armor(random_armor);
    //generate random value of weapon
    int random_weapon=rand()%3;
    store_weapon(random_weapon);
}

/**
  Makes it so the == operator will have the behavior that you
  expect when comparing points.
  You can overload pretty much any operator in c++
  @param first Point left hand side of the expression
  @param other Point right hand side of the expression
*/
bool operator==(const Object &first, const Object &other) {
  return first.x_ == other.x_ && first.y_ == other.y_;
}

//the following functions are all for design pattern Factory
//simply create a pointer type of corresponding object and return it
Object* ObjectFactory::GetEmpty(int i, int j){
    QColor color1(255,255,255);
    Object *p1=new Object(color1,0+20*j,0+20*i);
    p1->set_condition(1);
    return p1;
}

Object* ObjectFactory::GetWall(int i, int j){
    QColor color1(255,255,255);
    Object *p1=new Object(color1,0+20*j,0+20*i);
    p1->set_condition(3);
    return p1;
}

Object* ObjectFactory::GetTreasure(int i, int j){
    QColor color1(255,255,255);
    Object *p1=new Treasure(color1,0+20*j,0+20*i);
    //assign a hp value for the treasure
    //hp will be in range [10,20] so it must be at least 10
    int hp=rand()%20+10;
    p1->set_condition(hp);
    return p1;
}

Player* ObjectFactory::GetPlayer(int i, int j){
    //for player 1 starts at the upper left corner, mark it as red
    if(i==0 && j==0){
        Player *p1=new Player(QColor(255,0,0),0,0);
        return p1;
    }
    //for player 2 starts at the bottom right corner, mark it as blue
    else{
        Player *p1=new Player(QColor(0,0,255),0+20*j,0+20*i);
        return p1;
    }
}


