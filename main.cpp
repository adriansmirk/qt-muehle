#include "muehle.h"
#include "gui/custombutton.h"
#include <QObject>
#include <functional>
#include "board.h"

#include <QApplication>
#include <QSplashScreen>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    muehle w;
    w.show();
    w.ui->_gbMuehle->setDisabled(true);

    //Gamestart
    QObject::connect(w.ui->_pbStartGame, &QPushButton::clicked, &w, &muehle::startGame);
    QObject::connect(&w, &muehle::startGameServer, &(w.myServer), &Server::startGame);
    QObject::connect(&w.myClient, &Communicator::nwGameStart, &(w.myClient), &Client::onGameStart);
    QObject::connect(&w.myServer, &Communicator::nwResGameStart, &(w.myServer), &Server::onResGameStart);
    QObject::connect(&w.myServer, &Server::printGameStart, &w, &muehle::printGame);
    QObject::connect(&w.myClient, &Client::acceptGameStart, &w, &muehle::printGame);
    QObject::connect(w.ui->_cbServerClient, qOverload<int>(&QComboBox::activated), &w, &muehle::networkSelection);

    //Gamehistory
    QObject::connect(&w.myBoard, &Board::gameActionSignal, &w, &muehle::printGame);
    QObject::connect(&w.myBoard, &Board::whosTurnSignal, &w, &muehle::whosTurnSlot);

    //add chip
    QObject::connect(&w.myBoard, &Board::writeAddChip, dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::setChip);
    QObject::connect(&w.myBoard, &Board::writeAddChip, dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::setChip);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::nwSetChip, &w.myBoard, &Board::setChip);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::nwSetChip, &w.myBoard, &Board::setChip);

    //move Chip
    QObject::connect(&w.myBoard, &Board::writeMoveChip, dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::moveChip);
    QObject::connect(&w.myBoard, &Board::writeMoveChip, dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::moveChip);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::nwMoveChip, &w.myBoard, &Board::moveChipNW);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::nwMoveChip, &w.myBoard, &Board::moveChipNW);

    //Response for add, move & jump Chip
    QObject::connect(&w.myBoard, &Board::nwResChipAction, dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::resChipAction);
    QObject::connect(&w.myBoard, &Board::nwResChipAction, dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::resChipAction);
    QObject::connect(&w.myBoard, &Board::nwResRemoveChip, dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::resRemoveChip);
    QObject::connect(&w.myBoard, &Board::nwResRemoveChip, dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::resRemoveChip);

    //removeChip
    QObject::connect(&w.myBoard, &Board::signalRemoveChip, dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::removeChip);
    QObject::connect(&w.myBoard, &Board::signalRemoveChip, dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::removeChip);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::nwRemoveChip, &w.myBoard, &Board::removeChipNW);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::nwRemoveChip, &w.myBoard, &Board::removeChipNW);

    //jumpChip
    QObject::connect(&w.myBoard, &Board::signalJumpChip, dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::jumpChip);
    QObject::connect(&w.myBoard, &Board::signalJumpChip, dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::jumpChip);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myServer)), &Communicator::nwJumpChip, &w.myBoard, &Board::jumpChipNW);
    QObject::connect(dynamic_cast<Communicator*>(&(w.myClient)), &Communicator::nwJumpChip, &w.myBoard, &Board::jumpChipNW);


    //Gameover
    QObject::connect(&w.myBoard, &Board::signalGameIsOver,&w,&muehle::gameIsOver);
    QObject::connect(&w.myBoard, &Board::signalGameIsOver,&w,&muehle::gameIsOver);
    QObject::connect(&w.myBoard, &Board::signalGameIsOver,&w,&muehle::gameIsOver);

    //GUI
    QObject::connect(&w.myBoard, &Board::updateColor, &w, &muehle::updateColor);
    QObject::connect(&w.myBoard, &Board::fadeIcon, &w, &muehle::fadeNextPlayerIcon);

    CONNECT_UI_BUTTON(0);
    CONNECT_UI_BUTTON(1);
    CONNECT_UI_BUTTON(2);
    CONNECT_UI_BUTTON(3);
    CONNECT_UI_BUTTON(4);
    CONNECT_UI_BUTTON(5);
    CONNECT_UI_BUTTON(6);
    CONNECT_UI_BUTTON(7);
    CONNECT_UI_BUTTON(8);
    CONNECT_UI_BUTTON(9);
    CONNECT_UI_BUTTON(10);
    CONNECT_UI_BUTTON(11);
    CONNECT_UI_BUTTON(12);
    CONNECT_UI_BUTTON(13);
    CONNECT_UI_BUTTON(14);
    CONNECT_UI_BUTTON(15);
    CONNECT_UI_BUTTON(16);
    CONNECT_UI_BUTTON(17);
    CONNECT_UI_BUTTON(18);
    CONNECT_UI_BUTTON(19);
    CONNECT_UI_BUTTON(20);
    CONNECT_UI_BUTTON(21);
    CONNECT_UI_BUTTON(22);
    CONNECT_UI_BUTTON(23);



    QObject::connect(&w.myBoard, &Board::showRemovedChip, &w, &muehle::showRemovedChip);

    return a.exec();
}
