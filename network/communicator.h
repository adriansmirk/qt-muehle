/**
 * @class Communicator
 * @brief Diese Klasse ist eine "abstrakte" Klasse, die die gemeinsamen Funktionen von Server und Client beinhaltet.
 *        Also ist diese Klasse eine übergeordnete Klasse zur Steuerung der Netzwerkkommunikation.
 * @author Arpad Pittino
 * @date 3.4.2025
 */

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H
#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QDataStream>
#include <map>
#include <QTcpServer>
#include <QDataStream>
#include <functional>


using cmd_func_map = std::map<char,std::function<void(QByteArray)>>;

class Communicator : public QTcpServer  {
Q_OBJECT
    public:
    /**
     * @brief Konstruktor der Klasse Communicator.
     *
     * Initialisiert die Klasse und erstellt eine Map, die als Key das zu übergebende Kommando enthält.
     * Der Value ist eine Lambdafunktion, die die zugehörigen Signale emittiert.
     */
        Communicator();
        /**
        * @brief Setzt Bytes in einen Array und schreibt diesen in ein QDatastream, das zwischen Server und Client verbunden ist.
        * @param cmd ist das Kommando, das bestimmt welches Signal in der im Konstruktor erstellten Map emittiert wird.
        * @param Aufgrund der Implementation mit dem Template kann die Anzahl der übrigen Parameter variieren.
        */
        template<typename... N>
        void assembleBytes(char cmd,N ...n){
            if(isActive){
                QByteArray writeArray;
                writeArray.append(cmd);
                (writeArray.append(std::forward<N>(n)), ...);
                inNout << writeArray;
                qDebug() << "Daten gesendet:" << writeArray;
            }
        }
        /**
         * @brief isActive überprüft ob Server oder Client aktiv ist.
         */
        bool isActive = false;
    signals:
        /**
         * @brief nwStartRequest ist ein Signal das beim Start vom Client emittiert wird.
         */
        void nwStartRequest();
        /**
         * @brief nwGameStart ist ein Signal, das vom Server gesendet wird, um das Spiel zu beginnen
         * @param starter bestimmt wer beginnt.
         * @param groupNumber gibt die Gruppennummer aus.
         */
        void nwGameStart(quint8 starter, quint8 groupNumber);
        /**
         * @brief nwResGameStart ist ein Signal, das vom Client gesendet wird und auf die Anfrage vom Server antwortet.
         * @param state gibt den Status zurück, ob der Zug in Ordnung ist.
         */
        void nwResGameStart(quint8 state);
        /**
         * @brief nwSetChip ist ein Signal, das genutzt wird um einen Stein auf das Feld zu setzen.
         * @param direction gibt die Himmelsrichtung(1-8) an, um zu bestimmen wo der Stein gesetzt werden soll.
         * @param square gibt an im welchen Quadrat der Stein gesetzt werden soll.
         * @param chipCount gibt an, wie viele Steine pro Spieler noch zu legen sind.
         * @param isMill gibt an, ob der Stein in eine Mühle gesetzt wird oder nicht.
         */
        void nwSetChip(quint8 direction,quint8 square,quint8 chipCount,quint8 isMill);
        /**
         * @brief nwMoveChip ist ein Signal, das genutzt wird um einen Stein von einem zum nächsten zu bewegen.
         * @param oldDirection gibt die alte Himmelsrichtung an.
         * @param oldSquare gibt das alte Quadrat an.
         * @param newDirection gibt die neue Himmelsrichtung an.
         * @param newSquare gibt das neue Quadrat an.
         * @param isMill gibt an, ob der Stein in eine Mühle bewegt wird oder nicht.
         */
        void nwMoveChip(quint8 oldDirection, quint8 oldSquare, quint8 newDirection,quint8 newSquare, quint8 isMill);
        /**
         * @brief nwJumpChip ist ein Signal, das genutzt wird um mit einem Stein in der Endphase springen zu können.
         * @param oldDirection gibt die alte Himmelsrichtung an.
         * @param oldSquare gibt das alte Quadrat an.
         * @param newDirection gibt die neue Himmelsrichtung an.
         * @param newSquare gibt das neue Quadrat an.
         * @param isMill gibt an, ob der Stein in eine Mühle bewegt wird oder nicht.
         */
        void nwJumpChip(quint8 oldDirection, quint8 oldSquare, quint8 newDirection,quint8 newSquare, quint8 isMill);
        /**
         * @brief nwResChipAction ist das Signal zur Bestätigung vom setzen, bewegen oder springen.
         * @param state gibt den Status zurück, ob der Zug in Ordnung ist.
         */
        void nwResChipAction(quint8 state);
        /**
         * @brief nwRemoveChip ist das Signal, um einen Stein zu entfernen.
         * @param direction gibt die Himmelsrichtung(1-8) an, um zu bestimmen wo der Stein entfernt werden soll.
         * @param square gibt an im welchen Quadrat der Stein entfernt werden soll.
         */
        void nwRemoveChip(quint8 direction, quint8 square);
        /**
         * @brief nwResRemoveChip ist das Signal zur Bestätigung vom entfernen.
         * @param state gibt den Status zurück, ob der Zug in Ordnung ist.
         * @param chipsRemoved gibt die Anzahl, der entfernten Steine zurück.
         */
        void nwResRemoveChip(quint8 state, quint8 chipsRemoved);

    public slots:
        /**
         * @brief extractByteArray liest den Bytearray aus dem QDataStream aus und ruft die Map mit den zugehörigen Signalen auf.
         */
        void extractByteArray();
        /**
         * @brief setChip ist ein Slot der das Kommando setzt, um einen Stein zu setzen.
         * @param direction gibt die Himmelsrichtung(1-8) an, um zu bestimmen wo der Stein gesetzt werden soll.
         * @param square gibt an im welchen Quadrat der Stein gesetzt werden soll.
         * @param chipCount gibt an, wie viele Steine pro Spieler noch zu legen sind.
         * @param isMill gibt an, ob der Stein in eine Mühle gesetzt wird oder nicht.
         */
        void setChip(quint8 direction, quint8 square, quint8 chipCount, quint8 isMill);
        /**
         * @brief moveChip ist ein Slot der das Kommando setzt, um einen Stein zu bewegen.
         * @param oldDirection gibt die alte Himmelsrichtung an.
         * @param oldSquare gibt das alte Quadrat an.
         * @param newDirection gibt die neue Himmelsrichtung an.
         * @param newSquare gibt das neue Quadrat an.
         * @param isMill gibt an, ob der Stein in eine Mühle bewegt wird oder nicht.
         */
        void moveChip(quint8 oldDirection, quint8 oldSquare, quint8 newDirection, quint8 newSquare, quint8 isMill);
        /**
         * @brief jumpChip ist ein Slot der das Kommando setzt, um mit einem Stein zu springen.
         * @param oldDirection gibt die alte Himmelsrichtung an.
         * @param oldSquare gibt das alte Quadrat an.
         * @param newDirection gibt die neue Himmelsrichtung an.
         * @param newSquare gibt das neue Quadrat an.
         * @param isMill gibt an, ob der Stein in eine Mühle bewegt wird oder nicht.
         */
        void jumpChip(quint8 oldDirection, quint8 oldSquare, quint8 newDirection, quint8 newSquare, quint8 isMill);
        /**
         * @brief removeChip ist ein Slot der das Kommando setzt, um einen Stein zu entfernen.
         * @param direction gibt die Himmelsrichtung(1-8) an, um zu bestimmen wo der Stein entfernt werden soll.
         * @param square gibt an im welchen Quadrat der Stein entfernt werden soll.
         */
        void removeChip(quint8 direction, quint8 square);
        /**
         * @brief resChipAction ist ein Slot für die Antwort auf das Setzen, Bewegen oder Springen eines Steins.
         * @param state gibt den Status zurück, ob der Zug in Ordnung ist.
         */
        void resChipAction(quint8 state);
        /**
         * @brief resRemoveChip ist ein Slot für die Antwort auf das Entfernen eines Steins.
         * @param state gibt den Status zurück, ob der Zug in Ordnung ist.
         * @param count gibt die Anzahl entfernter Steine an.
         */
        void resRemoveChip(quint8 state, quint8 count);

    protected:
        /**
         * @brief inNout ist der Datastream, der zur Netzwerkkommunikation hergenommen wird.
         */
        QDataStream inNout;
        /**
         * @brief socket ist ein Socket, der vom Client genutzt wird, um Daten vom Server empfangen zu können
         */
        QTcpSocket *socket;
        /**
         * @brief map_command_to_function ist eine Map, die einen char als Key hat.
         * Dieser entspricht dem Kommando.
         * Der Value ist eine std::function, da in der Implementierung eine Lambdafunktion zum emittieren des Signals.
         */
        cmd_func_map map_command_to_function;
};

#endif // COMMUNICATOR_H

