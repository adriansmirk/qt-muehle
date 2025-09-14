#ifndef MUEHLE_H
#define MUEHLE_H

#include "ui_muehle.h"
#include "board.h"
#include "network/server.h"
#include "network/client.h"
#include "server.h"
#include "client.h"
#include <QWidget>
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <QTcpServer>
#include <QByteArray>
#include <QPixmap>
#include <QStyle>
#include <QHostAddress>
#include <QDir>

#define private public


QT_BEGIN_NAMESPACE
namespace Ui { class muehle; }
QT_END_NAMESPACE

class muehle : public QWidget
{
    Q_OBJECT

public:
    muehle(QWidget *parent = nullptr);
    ~muehle();
    Ui::muehle *ui;
    Board myBoard;
    Server myServer;
    Client myClient;
public slots:
    void startGame();
    void updateColor(int pos, gridState color);
    void networkSelection(int state);
    void printGame(QString action);
    void whosTurnSlot(QString who);

private slots:
    void fadeNextPlayerIcon(int index);
    void showRemovedChip(bool byWhite);

signals:
    void startGameServer();

private:
    int fadeIndex = 0;  // access fade index over multiple calls so it doesnt reset everytime
    void gameIsOver();

};
#endif // MUEHLE_H

