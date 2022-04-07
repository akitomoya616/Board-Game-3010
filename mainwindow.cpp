#include "mainwindow.h"
#include "equipments.h"
#include "object.h"
#include <QMessageBox>
#include "ui_mainwindow.h"
#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QTime>
#include <QTimer>
#include <math.h>
#include <QTableView>
#include <vector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // we need to set up the ui before we draw on our scene
    ui->setupUi(this);

    // the QGraphicsView is the UI element that contains the
    // scene that we will actually get to draw on.
    QGraphicsView * view = ui->graphicsView;

    // scene is a QGraphicsScene pointer field of the PlotWindow class
    // this makes our lives easier by letting us easily access it
    // from other functions in this class.
    scene = new QGraphicsScene;
    view->setScene(scene);
    // make the scene the same size as the view containing it
    view->setSceneRect(0,0,view->frameSize().width(),view->frameSize().height());

    //set up for icon view, no need to change the code through out the whole game
    //this view shows the sample cell that will appear on the board
    //so that user can recognize the meaning of each cell and what the current condition is like
    QGraphicsView * view2 = ui->icon_view;
    scene2 = new QGraphicsScene;
    view2->setScene(scene2);
    view2->setSceneRect(0,0,view2->frameSize().width(),view2->frameSize().height());
    QColor red(255,0,0);
    QColor green(0,255,0);
    QColor blue(0,0,255);
    QColor purple(155,155,255);
    QColor black(0,0,0);
    QColor white(255,255,255);
    Object *red_icon=new Object(red, 100, 2);
    Object *blue_icon=new Object(blue, 100, 32);
    //Object *green_icon=new Object(green, 100, 62);
    Object *purple_icon=new Object(purple, 100, 62);
    Object *black_icon=new Object(black, 100, 92);
    Object *white_icon=new Object(white, 100, 122);
    scene2->addItem(red_icon);
    scene2->addItem(blue_icon);
    //scene2->addItem(green_icon);
    scene2->addItem(purple_icon);
    scene2->addItem(black_icon);
    scene2->addItem(white_icon);
    QGraphicsTextItem *text1 = scene2->addText("Player 1");
    text1->setPos(0, 0);
    QGraphicsTextItem *text2 = scene2->addText("Player 2");
    text2->setPos(0, 30);
    QGraphicsTextItem *text3 = scene2->addText("Treasure");
    text3->setPos(0, 60);
    QGraphicsTextItem *text4 = scene2->addText("Wall");
    text4->setPos(0, 90);
    QGraphicsTextItem *text5 = scene2->addText("Empty");
    text5->setPos(0, 120);

    // we'll want to generate random numbers later so we're
    // going to seed our random number generator once
    qsrand(static_cast<unsigned>(QTime::currentTime().msec()));

    qDebug() << "Game starts";
    QColor color1(255,255,255);
    //creat a 2d vector of objects, including players
    for(int i=0;i<rows_;i++){
        game_board.push_back(std::vector<Object*>());
        for(int j=0;j<columns_;j++){
            //for each cell, set it as a character
            if(i==0 && j==0){
                //set player1
                game_board[i].push_back(player_1);
            }
            else if(i==(rows_-1) && j==(columns_-1)){
                //set player2
                game_board[i].push_back(player_2);
            }
            else{
                //set for any other condition
                int random=rand()%10;
                if(random<=2){
                    //empty space with 30% probability
                    Object *p1=ObjectFactory::GetEmpty(i,j);
                    game_board[i].push_back(p1);
                }
                else if(random<9){
                    //treasure with 60% probability
                    Object *p1=ObjectFactory::GetTreasure(i,j);
                    game_board[i].push_back(p1);
                }
                else{
                    //wall with 10% probability
                    Object *p1=ObjectFactory::GetWall(i,j);
                    game_board[i].push_back(p1);
                }
            }
        }
    }
    print_board();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//function that print the 2d board on the screen
void MainWindow::print_board() {
    for (int i=0;i<game_board.size();i++){
        for (int j=0;j<game_board[i].size();j++){
            //add cells on UI
            scene->addItem(game_board[i][j]);
        }
    }
}

//handle the direction, get from pointer and to pointer and give them to move function
void MainWindow::handle_direction(std::string direction){
    //initialize a text so we can use it later for showing different warning
    QString text="";
    //get the current player's position on board
    int current_i=get_position_i();
    int current_j=get_position_j();
    int next_i=0;
    int next_j=0;
    //get the next position on board
    if(direction=="up"){
        next_i=current_i-1;
        next_j=current_j;
    }
    else if(direction=="down"){
        next_i=current_i+1;
        next_j=current_j;
    }
    else if(direction=="left"){
        next_i=current_i;
        next_j=current_j-1;
    }
    else{
        next_i=current_i;
        next_j=current_j+1;
    }
    //check the condition of the next step based on the collected coordinates
    if(next_i>rows_-1||next_j>columns_-1||next_i<0||next_j<0){
        //out of bound
        QString text_update="Reach the boundary of the map";
        ui->updateLabel->setText(text_update);
        text="Warning: Invalid move! Out of bound!";
        ui->warningLabel->setText(text);
    }
    //then call move function to move the player to the next step
    //return the bool to check whether the movement was successful or not
    else{
        bool result=false;
        if(current_player==0){
            result=move(player_1,game_board[next_i][next_j]);
        }
        else{
            result=move(player_2,game_board[next_i][next_j]);
        }
        //if the movement itself successed
        if(result){
            //everything is fine, no warning should be made
            //clear the warning text
            text="Warning:";
            ui->warningLabel->setText(text);
            //change the next player's id

            if(current_player==0){
                current_player=1;
            }
            else{
                current_player=0;
            }

            //update the information to tell the user who's player the current turn
            //since this step happens after pressing the direction button and before changing player
            //the instruction is actually opposite to the player id
            QString player_text;
            if(current_player==1){
                player_text="Current Player: Player 2";
            }
            else{
                player_text="Current Player: Player 1";
            }
            ui->currentPlayerLabel->setText(player_text);
        }
        else{
            //if moving to another player, then we just go to next player's turn
            if(game_board[next_i][next_j]==player_2){
                current_player=1;
            }
            else if(game_board[next_i][next_j]==player_1){
                current_player=0;
            }
            //otherwise re-do the step
            else{
                text="Warning: Invalid move! Cannot go there!";
                ui->warningLabel->setText(text);
            }
        }
    }
}

void MainWindow::fight(Player *from, Player *to){
    //start fight between two players
    //from will attack to
    std::vector<int> from_status=from->get_information();
    std::vector<int> to_status=to->get_information();
    //0:hp, 1:armor, 2: attack
    //store to_player's information for later use
    int to_hp=to_status[0];
    int to_armor=to_status[1];
    int to_attack=to_status[2];
    //store from_player's information for later use
    int from_attack=from_status[2];
    int from_armor=from_status[1];
    int from_hp=from_status[0];
    //if the armor can take the attack
    if(to_armor>=from_attack){
        //simply update armor value by -attack value from another player
        to->update_status(0,to_armor-from_attack,to_attack);
    }
    //otherwise the armor value becomes 0 and the player starts losing hp
    //the armor will be lost. this process had been handled in update_status function
    else{
        //update hp, armor value
        to->update_status(-(from_attack-to_armor),0,to_attack);
    }
    //update the newest information in the corresponding player's label
    std::vector<int> new_from_status=from->get_information();
    std::vector<int> new_to_status=to->get_information();
    //0:hp, 1:armor, 2: attack
    to_hp=new_to_status[0];
    to_armor=new_to_status[1];
    to_attack=new_to_status[2];

    from_attack=new_from_status[2];
    from_armor=new_from_status[1];
    from_hp=new_from_status[0];

    QString text_hp_from="HP: "+QString::number(from_hp);
    QString text_armor_from="Armor: "+QString::number(from_armor);
    QString text_attack_from="Attack: "+QString::number(from_attack);

    QString text_hp_to="HP: "+QString::number(to_hp);
    QString text_armor_to="Armor: "+QString::number(to_armor);
    QString text_attack_to="Attack: "+QString::number(to_attack);
    if(current_player==0){
        ui->hp1Label->setText(text_hp_from);
        ui->armor1Label->setText(text_armor_from);
        ui->attack1Label->setText(text_attack_from);

        ui->hp2Label->setText(text_hp_to);
        ui->armor2Label->setText(text_armor_to);
        ui->attack2Label->setText(text_attack_to);
    }
    else{
        ui->hp2Label->setText(text_hp_from);
        ui->armor2Label->setText(text_armor_from);
        ui->attack2Label->setText(text_attack_from);

        ui->hp1Label->setText(text_hp_to);
        ui->armor1Label->setText(text_armor_to);
        ui->attack1Label->setText(text_attack_to);
    }
    //if to has hp<=0, then from_player win
    if(to_hp<=0){
        win();
    }
}

bool MainWindow::move(Player *from, Object *to){
    int goto_id=to->get_id();
    //0:player, 1:empty, 2:treasure, 3:wall
    //check on the status we are moving to
    if(goto_id==3){
        //if move to wall, then movement failed
        QString text="Go to a wall!";
        ui->updateLabel->setText(text);
        return false;
    }
    else if(goto_id==0){
        QString text="Meet with another player!";
        ui->updateLabel->setText(text);
        //if move to another player then fight
        if(current_player==0){
            fight(player_1, player_2);
            QString text="Current Player: Player 2";
            ui->currentPlayerLabel->setText(text);
        }
        else{
            fight(player_2,player_1);
            QString text="Current Player: Player 1";
            ui->currentPlayerLabel->setText(text);
        }
        return false;
    }
    else{
        //get the i,j value of the "from" object in the board
        int from_i=from->get_y()/20;//using y to get i, since y doesn't change in the row, just like i doesn't change in a board for a row
        int from_j=from->get_x()/20;
        QColor from_color=from->get_color();
        int from_id=from->get_id();
        //get the i,j value of the "to" object in the board
        int to_i=to->get_y()/20;
        int to_j=to->get_x()/20;
        //check if to_id is wall, player, or empty
        if(goto_id==1){
            //then it is empty
            //nothing will happen, just move to that position
            QString text="Nothing happened";
            ui->updateLabel->setText(text);
        }
        else if(goto_id==2){
            //then it is a treasure
            //check the treasure box and equip all the armors and weapons from the treasure box to the player
            QString text="Get treasure!";
            ui->updateLabel->setText(text);
            check_treasure(from, to);
        }
        //then do the moving process, since it is always runnable
        //now first swap from and to on board that are pointing to
        Object* temp=game_board[to_i][to_j];
        QColor empty_color=QColor(255,255,255);
        //move "from" to "to" by first setting the pointer on board at "to" to be "from"
        game_board[to_i][to_j]=game_board[from_i][from_j];
        //then assign from on board to "to" object
        game_board[from_i][from_j]=temp;
        //then re-assign the id and the color of it as well
        //and re-assign the information for the new "to"
        game_board[from_i][from_j]->update_condition(from_j*20,from_i*20, 1, empty_color);
        game_board[to_i][to_j]->update_condition(to_j*20,to_i*20, from_id, from_color);
    }
    return true;
}

void MainWindow::check_treasure(Player *from, Object *to){
    //process for checking the content in it
    std::vector<int> status=to->get_information();

    //get the armor list from the treasure box
    std::map<Armor, int> armor_list=to->get_armor();
    //get the weapon list from the treasure box
    std::map<Weapon, int> weapon_list=to->get_weapon();
    //equip them to player and get the added armor value
    int armor_value=from->equip_armor(armor_list);
    //equip them to player and get the added armor value
    int attack_value=from->equip_weapon(weapon_list);

    //update the hp, armor, attack status in player object
    //attack will be updated together with 5 fundemental attack value
    from->update_status(status[0],armor_value,attack_value+5);
    //then update the information on the labels as well
    QString text_hp="HP: "+QString::number(from->get_information()[0]);
    QString text_armor="Armor: "+QString::number(from->get_information()[1]);
    QString text_attack="Attack: "+QString::number(from->get_information()[2]);
    //update the hp information in the corresponding player's hp, armor, attack labels
    if(current_player==0){
        ui->hp1Label->setText(text_hp);
        ui->armor1Label->setText(text_armor);
        ui->attack1Label->setText(text_attack);
    }
    else{
        ui->hp2Label->setText(text_hp);
        ui->armor2Label->setText(text_armor);
        ui->attack2Label->setText(text_attack);
    }
}

//check who win the game
void MainWindow::win(){
    QString text;
    if(current_player==0){
        text="Player 1 wins!";

    }
    else{
        text="Player 2 wins!";
    }
    //show the information on warning lable
    ui->warningLabel->setText(text);
    //also disable all the play buttons
    ui->downButton_1->setEnabled(false);
    ui->upButton->setEnabled(false);
    ui->leftButton->setEnabled(false);
    ui->rightButton->setEnabled(false);
    //make a pop up window and show the information of the winner
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}

int MainWindow::get_position_i() const{
    if(current_player==0){
        return player_1->get_y()/20;
    }
    else{
        return player_2->get_y()/20;
    }
}

int MainWindow::get_position_j() const{
    if(current_player==0){
        return player_1->get_x()/20;
    }
    else{
        return player_2->get_x()/20;
    }
}


//press the following buttons to activate different condition for handle_direction function
void MainWindow::on_downButton_1_pressed()
{
    handle_direction("down");
}

void MainWindow::on_upButton_pressed()
{
    handle_direction("up");
}

void MainWindow::on_rightButton_pressed()
{
    handle_direction("right");
}

void MainWindow::on_leftButton_pressed()
{
    handle_direction("left");
}

void MainWindow::on_instructionButton_pressed()
{
    //press the instruction button to pop up a window that shows the rule of the game
    QString text="Welcome to the game!\n";
    text+="We have two players on the board\n";
    text+="and we need two users to control each of them.\n";
    text+="The game takes turns in loop\n";
    text+="and each player takes one turn by pressing on one of the four direction buttons to take a move.\n";
    text+="Once the player move to a treasure box, he/she will gain some hp value, armor, and weapon.\n";
    text+="\n";
    text+="The goal of the game is to let one player kill another player.\n";
    text+="When two players meets, the battle happens\n";
    text+="and the defenser's hp will be subtracted by the difference between the attacker's attack value and defenser's armor value.\n";
    text+="When the hp value goes to 0, the player dies.\n";
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.exec();
}
