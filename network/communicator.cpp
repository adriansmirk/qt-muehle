#include "communicator.h"

//Is map for server or client
const std::map<char,int> SanityCheckMap{
    {0x01,0},
    {0x02,2},
    {0x03,1},
    {0x10,4},
    {0x11,5},
    {0x12,5},
    {0x13,1},
    {0x20,2},
    {0x21,2},
};

Communicator::Communicator() {
    map_command_to_function = {
                               {0x02, [this](QByteArray data) { emit nwGameStart(data.at(0), data.at(1));}}, //Server
                               {0x03, [this](QByteArray data) { emit nwResGameStart(data.at(0));}}, //Client
                               {0x10, [this](QByteArray data) { emit nwSetChip(data.at(0), data.at(1), data.at(2), data.at(3));}},
                               {0x11, [this](QByteArray data) { emit nwMoveChip(data.at(0), data.at(1), data.at(2), data.at(3), data.at(4));}},
                               {0x12, [this](QByteArray data) { emit nwJumpChip(data.at(0), data.at(1), data.at(2), data.at(3), data.at(4));}},
                               {0x13, [this](QByteArray data) { emit nwResChipAction(data.at(0));}},
                               {0x20, [this](QByteArray data) { emit nwRemoveChip(data.at(0), data.at(1));}},
                               {0x21, [this](QByteArray data) { emit nwResRemoveChip(data.at(0), data.at(1));}},
                               };
}

void Communicator::extractByteArray() {
    if (!socket->bytesAvailable()) {
        qDebug() << "Keine Daten verfügbar.";
        return;
    }
    QByteArray readOut;
    inNout.setDevice(socket);

    inNout.startTransaction();
    inNout >> readOut;

    if (!inNout.commitTransaction()) {
        qDebug() << "Fehler beim Lesen der Daten. Unvollständige Nachricht.";
        return;
    }

    if (readOut.isEmpty()) {
        qDebug() << "Leerer Datenstrom empfangen.";
        return;
    }
    qDebug() << "Data Print Out: "<<readOut;
    char cmd = readOut[0];
    readOut.remove(0, 1);
    if (SanityCheckMap.find(cmd) != SanityCheckMap.end() && SanityCheckMap.at(cmd) == readOut.size()) {
        if (map_command_to_function.find(cmd) != map_command_to_function.end()) {
            map_command_to_function.at(cmd)(readOut);
            qDebug() << "Daten erfolgreich extrahiert und verarbeitet.";
        } else {
            qDebug() << "Unbekannter Befehl:" << cmd;
        }
    } else {
        qDebug() << "Daten nicht korrekt oder unvollständig.";
    }
}


void Communicator::setChip(quint8 direction, quint8 square, quint8 chipCount, quint8 isMill) {
    quint8 cmd = 0x10;
    assembleBytes(cmd,direction,square,chipCount,isMill);
}


void Communicator::moveChip(quint8 oldDirection, quint8 oldSquare, quint8 newDirection, quint8 newSquare, quint8 isMill) {
    quint8 cmd = 0x11;
        assembleBytes(cmd,oldDirection,oldSquare,newDirection,newSquare,isMill);
}

void Communicator::jumpChip(quint8 oldDirection, quint8 oldSquare, quint8 newDirection, quint8 newSquare, quint8 isMill) {
    quint8 cmd = 0x12;
        assembleBytes(cmd,oldDirection,oldSquare,newDirection,newSquare,isMill);
}

void Communicator::removeChip(quint8 direction, quint8 square) {
    quint8 cmd = 0x20;
    assembleBytes(cmd,direction,square);
}

void Communicator::resChipAction(quint8 state) {
    quint8 cmd = 0x13;
    assembleBytes(cmd,state);
}

void Communicator::resRemoveChip(quint8 state, quint8 count) {
    quint8 cmd = 0x21;
    assembleBytes(cmd,state,count);
}

