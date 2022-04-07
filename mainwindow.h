#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include "cell.h"
#include "object.h"

#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void print_board();
    bool move(Player *from, Object *to);
    int  get_position_i() const;
    int  get_position_j() const;
    void handle_direction(std::string direction);

    void check_treasure(Player *from, Object *to);
    void fight(Player *from, Player *to);
    void win();

private slots:
    void on_downButton_1_pressed();

    void on_upButton_pressed();

    void on_rightButton_pressed();

    void on_leftButton_pressed();

    void on_instructionButton_pressed();

signals:
    void emit_direction(std::string direction);

private:
    Ui::MainWindow *ui;
    //for play board view
    QGraphicsScene *scene;
    //for icon display view
    QGraphicsScene *scene2;

    std::vector<std::vector<Object*>> game_board;

    static const int rows_=20;

    static const int columns_=20;

    int turn_count=0;

    //check who's playing at the current turn
    int current_player=0;

    Player *player_1=ObjectFactory::GetPlayer(0,0);

    Player *player_2=ObjectFactory::GetPlayer(columns_-1,rows_-1);
};
#endif // MAINWINDOW_H
