#include "muehle.h"
#include "gui/custombutton.h"
#include "ui_muehle.h"
#include "board.h"
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QString>
#include <QDebug>
#include <QTcpServer>
#include <QByteArray>

#include "network/communicator.h"
#include "network/server.h"
#include "network/client.h"
#include <map>
#include <functional>
#include "custombutton.h"
#include <QStyle>
#include <QHostAddress>
#include <QDir>
#include <array>
#include <string>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

muehle::muehle(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::muehle)
{
    ui->setupUi(this);

    // Hide all removed chip buttons initially
    for (int i = 0; i < 9; ++i) {
        QString whiteName = QString("removedWhite_%1").arg(i);
        QString blackName = QString("removedBlack_%1").arg(i);

        if (QPushButton* w = findChild<QPushButton*>(whiteName))
            w->setVisible(false);
        if (QPushButton* b = findChild<QPushButton*>(blackName))
            b->setVisible(false);
    }

    QString homeDir = QDir::homePath();
    QPixmap pix(":/new/prefix1/Spielbrett.jpeg");
    ui->_board->setPixmap(pix);
    ui->_tbWhosTurn->setAlignment(Qt::AlignCenter);
    ui->_tbWhosTurn->setStyleSheet("font-size: 42px;");
    ui->_tbWhosTurn->setText("Der Server beginnt.");
}
muehle::~muehle()
{
    delete ui;
}

void muehle::updateColor(int pos, gridState color) {
    QPushButton* but[24] = { ui->_pb0, ui->_pb1, ui->_pb2, ui->_pb3, ui->_pb4, ui->_pb5, ui->_pb6, ui->_pb7, ui->_pb8, ui->_pb9, ui->_pb10, ui->_pb11, ui->_pb12, ui->_pb13, ui->_pb14, ui->_pb15, ui->_pb16, ui->_pb17, ui->_pb18, ui->_pb19, ui->_pb20, ui->_pb21, ui->_pb22, ui->_pb23 };

    if(color == gridState::white) {
        but[pos]->setStyleSheet("QPushButton { background-color: beige; "
                                "color: white; "
                                "border: 2px solid black; "
                                "border-radius: 20px; "
                                "min-width: 40px; "
                                "min-height: 40px;"
                                "max-width: 40px;"
                                "max-height: 40px;}"
                                "QPushButton:pressed { background-color: grey; }"
                                "QPushButton:hover { background-color:  lightgrey; }");
    } else if(color == gridState::black){
        but[pos]->setStyleSheet("QPushButton { background-color: brown; "
                                "color: white; "
                                "border: 2px solid black; "
                                "border-radius: 20px; "
                                "min-width: 40px; "
                                "min-height: 40px;"
                                "max-width: 40px;"
                                "max-height: 40px;}"
                                "QPushButton:pressed { background-color: grey; }"
                                "QPushButton:hover { background-color:  lightgrey; }");
    } else if (color == gridState::empty) {
        but[pos]->setStyleSheet("QPushButton { background-color: black; "
                                "color: white; "
                                "border: 2px solid black; "
                                "border-radius: 20px; "
                                "min-width: 40px; "
                                "min-height: 40px;"
                                "max-width: 40px;"
                                "max-height: 40px;}"
                                "QPushButton:pressed { background-color: grey; }"
                                "QPushButton:hover { background-color:  lightgrey; }");
    }
}

void muehle::gameIsOver(){
    ui->_gbMuehle->setDisabled(true);
}

void muehle::networkSelection(int state) {
        if(state == 0) {
            qDebug() << "Server ausgewählt: " << state;
            //ui->_leAdress->setText("localhost");
            ui->_lePort->setText("1234");
            quint16 port = ui->_lePort->text().toShort();
            myServer.startServer(port);
            myServer.isActive = true;
            myBoard._isServer = true;

        } else {
            qDebug() << "Client ausgewählt: " << state;
            //ui->_leAdress->setText("localhost");
            QString ipAdress = ui->_leAdress->text();
            ui->_lePort->setText("1234");
            quint16 port = ui->_lePort->text().toShort();
            qDebug() << ipAdress << port;
            myClient.connectToServer(ipAdress, port);
            myClient.isActive = true;
            myBoard._isServer = false;
        }
}

void muehle::fadeNextPlayerIcon(int index) {
    if (index < 0 || index >= 18) {
        qDebug() << "Invalid index:" << index;
        return;
    }

    QPushButton* buttons[18] = {
        ui->pushButton_0, ui->pushButton_1, ui->pushButton_2,
        ui->pushButton_3, ui->pushButton_4, ui->pushButton_5,
        ui->pushButton_6, ui->pushButton_7, ui->pushButton_8,
        ui->pushButton_9, ui->pushButton_10, ui->pushButton_11,
        ui->pushButton_12, ui->pushButton_13, ui->pushButton_14,
        ui->pushButton_15, ui->pushButton_16, ui->pushButton_17
    };

    QPushButton* button = buttons[index];
    if (!button) {
        qDebug() << "Button is null at index:" << index;
        return;
    }

    QGraphicsOpacityEffect* effect = new QGraphicsOpacityEffect(button);
    button->setGraphicsEffect(effect);

    QPropertyAnimation* anim = new QPropertyAnimation(effect, "opacity");
    anim->setDuration(500);
    anim->setStartValue(1.0);
    anim->setEndValue(0.0);
    anim->start(QAbstractAnimation::DeleteWhenStopped);

    button->setEnabled(false);
    qDebug() << "Fading icon at index:" << index;
}


void muehle::showRemovedChip(bool byWhite) {
    static int whiteRemovedIndex = 0;
    static int blackRemovedIndex = 0;

    QPushButton* removedIconsWhite[9] = {
        ui->removedWhite_0, ui->removedWhite_1, ui->removedWhite_2,
        ui->removedWhite_3, ui->removedWhite_4, ui->removedWhite_5,
        ui->removedWhite_6, ui->removedWhite_7, ui->removedWhite_8
    };

    QPushButton* removedIconsBlack[9] = {
        ui->removedBlack_0, ui->removedBlack_1, ui->removedBlack_2,
        ui->removedBlack_3, ui->removedBlack_4, ui->removedBlack_5,
        ui->removedBlack_6, ui->removedBlack_7, ui->removedBlack_8
    };

    if (byWhite && blackRemovedIndex < 9) {
        QPushButton* btn = removedIconsBlack[blackRemovedIndex++];
        btn->setVisible(true);
    } else if (!byWhite && whiteRemovedIndex < 9) {
        QPushButton* btn = removedIconsWhite[whiteRemovedIndex++];
        btn->setVisible(true);
    }

}

void muehle::startGame() {
    emit startGameServer();
}

void muehle::printGame(QString action) {
    ui->_tbGame->setText(action);
    ui->_gbMuehle->setEnabled(true);

}


void muehle::whosTurnSlot(QString who) {
    ui->_tbWhosTurn->setText(who);
}
