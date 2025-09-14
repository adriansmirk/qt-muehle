#include "communicator.h"
#include "client.h"

Client::Client() {
    socket = new QTcpSocket();
}

void Client::connectToServer(const QString &host, quint16 port) {
    socket->connectToHost(host, port);
    if(socket -> waitForConnected(3000)) {
        inNout.setDevice(socket);
        connect(socket, &QTcpSocket::readyRead, dynamic_cast<Communicator*>(this), &Communicator::extractByteArray);
        qDebug() << "Connected to Server!";
        //emit nwStartRequest();
    } else {
        qDebug() << "Failed to connect to server!";
    }
}

void Client::connectionRequest() {
    quint8 cmd = 0x01;
    assembleBytes(cmd);
}

void Client::onGameStart(quint8 groupNumber) {
    quint8 cmd = 0x03;
    assembleBytes(cmd, groupNumber);
    emit acceptGameStart("Server has started the Game & the Client accepted.");
}


