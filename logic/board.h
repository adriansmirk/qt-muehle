#ifndef BOARD_H
#define BOARD_H
#include <string>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <QObject>
#include <iostream>
#include <QDebug>
#include <utility>
enum class fromWhom{gui = 0,/**< Repräsentiert ein Iput von der Gui. */
                      network = 1/**< Repräsentiert ein Iput vom Netzwerk. */};
#define CONNECT_UI_BUTTON(id) QObject::connect(w.ui->_pb##id, &custombutton::clicked, &w.myBoard, &Board::button##id##Pressed)
#define DECLARE_BUTTONPRESSED(id) void button##id##Pressed()
#define BUTTONPRESSED(id) void Board::button##id##Pressed() {       \
buttonPressed(id,fromWhom::gui);                                    \
}
enum class gridState { empty = 0, white = 1, black = 2, mill_white = 3, mill_black = 4};
enum class phase { SetzPhase = 0, ZugPhase = 1, EndPhase = 2};
class Board : public QObject{
    Q_OBJECT


public:
    Board();
    ~Board(){};
    void addChip(int pos, fromWhom who);
    void addChipHelper(int pos, gridState color, bool canSend);
    void moveChip(int oldPos, int newPos, fromWhom who);
    void moveChipHelper(int oldPos, int newPos, gridState color, gridState colormill,bool canSend);
    void jumpChip(int oldPos, int newPos, fromWhom who);
    void jumpChipHelper(int oldPos, int newPos, gridState color, gridState colormill, bool canSend);
    void removeChip(int pos, fromWhom who);
    void removeChipHelper(int pos, gridState color, bool canSend);
    void newMill();
    void newMillHelper(gridState color, gridState millColor);
    void fixMill();
    void toggleTurn();
    void whosTurn();
    void canRemove(int pos, fromWhom who);
    void callZugOutPhase();
    void callOutEndPhase();
    void callOutGameOver();
    void canMoveChips();
    void callOutDeadLock();
    phase whichPhase();
    bool isLegalInput(fromWhom who);
    std::pair<quint8, quint8> convert(int pos);
    int invert(quint8 direction,quint8 square);
    void waitForSecondInputInMove(std::function<void(int)> &moveStorage, int pos, bool &isFirstInput, fromWhom who);
    void waitForSecondInputInJump(std::function<void(int)> &jumpStorage, int pos, bool &isFirstInput, fromWhom who);
    void sendNetworkSignalAddChip(int pos);
    void sendNetworkSignalMove(int oldPos, int newPos);
    void sendNetWorkRemove(int pos);
    void sendNetWorkJump(int oldPos, int newPos);
    bool _isServer;

public slots:
    void buttonPressed(int pos, fromWhom who);
    void setChip(quint8 direction,quint8 square,quint8 chipCount,quint8 isMill);
    void moveChipNW(quint8 oldDirection,quint8 oldSquare,quint8 newDirection,quint8 newSquare, quint8 isMill);
    void removeChipNW(quint8 direction, quint8 square);
    void jumpChipNW(quint8 oldDirection,quint8 oldSquare,quint8 newDirection,quint8 newSquare, quint8 isMill);
    DECLARE_BUTTONPRESSED(0);
    DECLARE_BUTTONPRESSED(1);
    DECLARE_BUTTONPRESSED(2);
    DECLARE_BUTTONPRESSED(3);
    DECLARE_BUTTONPRESSED(4);
    DECLARE_BUTTONPRESSED(5);
    DECLARE_BUTTONPRESSED(6);
    DECLARE_BUTTONPRESSED(7);
    DECLARE_BUTTONPRESSED(8);
    DECLARE_BUTTONPRESSED(9);
    DECLARE_BUTTONPRESSED(10);
    DECLARE_BUTTONPRESSED(11);
    DECLARE_BUTTONPRESSED(12);
    DECLARE_BUTTONPRESSED(13);
    DECLARE_BUTTONPRESSED(14);
    DECLARE_BUTTONPRESSED(15);
    DECLARE_BUTTONPRESSED(16);
    DECLARE_BUTTONPRESSED(17);
    DECLARE_BUTTONPRESSED(18);
    DECLARE_BUTTONPRESSED(19);
    DECLARE_BUTTONPRESSED(20);
    DECLARE_BUTTONPRESSED(21);
    DECLARE_BUTTONPRESSED(22);
    DECLARE_BUTTONPRESSED(23);
signals:
    void updateColor(int pos, gridState color);
    void writeAddChip(quint8 direction, quint8 square, quint8 chipsToPlace, quint8 millState);
    void writeMoveChip(quint8 oldDirection, quint8 oldSquare,quint8 newDirection, quint8 newSquare,quint8 millState);
    void resWriteAddChip(quint8 state);
    void signalGameIsOver();
    void nwResChipAction(quint8);
    void nwResRemoveChip(quint8,quint8);
    void signalRemoveChip(quint8, quint8);
    void nwResJumpChip(quint8,quint8, quint8, quint8, quint8);
    void signalJumpChip(quint8, quint8, quint8, quint8, quint8);
    void gameActionSignal(QString);
    void whosTurnSignal(QString);
    void fadeIcon(int Index);
    void showRemovedChip(bool byWhite);
private:

    std::map<int, enum gridState> _grid;
    std::array<std::array<int, 3> ,16> _muehlen;
    std::array<std::vector<int>, 24> _neighbour;
    std::map<int,std::pair<quint8,quint8>> _convert;
    bool _isWhiteTurn;
    int _wChipCount = 9;
    int _bChipCount = 9;
    int _numTurns = 0;
    bool illegalMove = false;
    bool haveToRemoveChip = false;
    bool canToggle = true;
    bool errorInRemoveChip = false;
    bool isFirstInput = true;
    bool wInEndPhase = false;
    bool bInEndPhase = false;
    bool wDeadlock = false;
    bool bDeadlock = false;
    quint8 _wChipsToPlace = 9;
    quint8 _bChipsToPlace = 9;
    quint8 state = 0;
};

#endif // BOARD_H

