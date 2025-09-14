/**
 * @class Server
 * @brief Diese Klasse implementiert alle Funktionen, die ein Server haben muss.
 *        Server erbt von der Communicatorklasse.
 * @author Arpad Pittino
 * @date 3.4.2025
 */
#ifndef SERVER_H
#define SERVER_H

#include "communicator.h"

class Server : public Communicator{
    Q_OBJECT
    public:
    /**
     * @brief Server ist der Konstruktor der Klasse und erstellt einen neuen QTCPServer.
     */
    Server();
        /**
         * @brief startServer ist die Funktion, die den Server startet und auf eine einkommende Verbindung wartet.
         * @param port gibt den Port an, an dem der Server gestartet werden soll.
         */
        void startServer(quint16 port);
        public slots:
            /**
             * @brief handleNewConnection ist ein Slot, der aufgerufen wird, sobald ein Client sich mit dem Server verbindet.
             * Es wird ebenfalls ein Socket erstellt mit dem der Server Daten erhalten kann.
             */
            void handleNewConnection();
            /**
             * @brief onResGameStart ist die Antwort auf den Client, um das Spiel starten zu können.
             * @param groupNumber gibt die Gruppennummer an.
             */
            void onResGameStart(quint8 groupNumber);
            /**
             * @brief startGame startet das Spiel.
             */
            void startGame();
        signals:
            /**
             * @brief printGameStart ist ein Signal an die GUI, um in einem Textbrowset den Start des Spiels zu verkünden.
             */
            void printGameStart(QString);
        private:
            /**
            * @brief server ist der QTcpServer, der im Konstruktor erstellt wird.
            */
        QTcpServer *server;
};




#endif // SERVER_H
