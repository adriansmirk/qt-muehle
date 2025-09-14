#include "communicator.h"
#include "server.h"

Server::Server() {
    server = new QTcpServer();
}

void Server::startServer(quint16 port) {
    if (server->listen(QHostAddress::Any, port)) {
        qDebug() << "Server started on port" << port;
        // Verarbeite neue Verbindungen über das Signal "newConnection"
        connect(server, &QTcpServer::newConnection, this, &Server::handleNewConnection);
        //qDebug() << server->nextPendingConnection();
    } else {
        qDebug() << "Failed to start Server:" << server->errorString();
    }
}


void Server::handleNewConnection() {
    qDebug() << "called handleNewConnection \n";
    QTcpSocket *clientSocket = server->nextPendingConnection(); // Hole den Socket der neuen Verbindung
    if (clientSocket) {
        socket = clientSocket;
        inNout.setDevice(socket);
        connect(socket, SIGNAL(readyRead()),this, SLOT(extractByteArray()));
        qDebug() << "Server: New client connected" << socket;
    } else {
        qDebug() << "Server: Failed to retrieve client socket!";
    }
}


void Server::startGame() {
    quint8 cmd = 0x02;
    quint8 groupNumber = 0x09;
    if(isActive)
        assembleBytes(cmd, 0x00, groupNumber);
}

void Server::onResGameStart(quint8 groupNumber) {
    QString gN = QString::number(groupNumber);
    emit printGameStart("Server hast started the Game. G09");
}
