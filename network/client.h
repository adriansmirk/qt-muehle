/**
 * @class Client
 * @brief Diese Klasse implementiert die Funktionen eines Clients.
 *        Client erbt von der Communicatorklasse.
 * @author Arpad Pittino
 * @date 3.4.2025
 */
#ifndef CLIENT_H
#define CLIENT_H

#include "communicator.h"


class Client : public Communicator {
    Q_OBJECT
    public:
    /**
     * @brief Client Konstruktor der Klasse Client.
     * Dieser erstellt einen neuen QTCPSocket.
     */
    Client();
        /**
         * @brief connectToServer ist eine Funktion, um eine Verbindung mit einem Server herzustellen.
         * @param host gibt die Ip-Adresse an.
         * @param port gibt den benötigten Port an.
         */
        void connectToServer(const QString &host, quint16 port);
    public slots:
        /**
         * @brief connectionRequest ist ein Slot, der eine Verbindungsanfrage an den Server schickt.
         */
        void connectionRequest();
        /**
         * @brief onGameStart ist ein Slot der das Spiel startet.
         * @param groupNumber gibt die Gruppennummer an.
         */
        void onGameStart(quint8 groupNumber);
    signals:
        /**
         * @brief acceptGameStart ist ein Slot, um den Spielstart, der vom Server ausgeführt wird, zu bestätigen.
         */
        void acceptGameStart(QString);
};

#endif // CLIENT_H
