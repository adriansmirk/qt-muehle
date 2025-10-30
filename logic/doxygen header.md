```c++
/**
 * @file board.h
 * @brief Hier wird jede Methode und jede Member-Variable von der Klasse Board deklariert.
 * @author Adrian Wüllner
 * @date 03.04.2025
 */

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
/**
 * @enum fromWhom
 * @brief Diese Enumeration definiert ob beispielsweise ein Button von gui oder netzwerk gedrückt wurde.
 * @detail Der Enum steht vor dem Makro im code, da dieser von Makor verwendet wird
 */
enum class fromWhom{gui = 0,/**< Repräsentiert ein Iput von der Gui. */
                      network = 1/**< Repräsentiert ein Iput vom Netzwerk. */};
/**
 * @def CONNECT_UI_BUTTON(id)
 * @brief Ein Makro um "connect" einfacher lesbar zu machen.
 * @param w.ui->_pb##id Welches Objekt Pushbutton sendet das Signal.
 * @param &custombutton::clicked Das Signal, welches gesendet wird.
 * @param &w.myBoard Das Objekt, welches das Signal erhält.
 * @param &Board::button##id##Pressed Der Slot der aufgerufen wird.
 */
#define CONNECT_UI_BUTTON(id) QObject::connect(w.ui->_pb##id, &custombutton::clicked, &w.myBoard, &Board::button##id##Pressed)
/**
 * @def DECLARE_BUTTONPRESSED(id)
 * @brief Ein Makro um den Slot void button##id##Pressed() zu deklarieren.
 */
#define DECLARE_BUTTONPRESSED(id) void button##id##Pressed()
/**
 * @def BUTTONPRESSED(id)
 * @brief Ein Makro um den Slot void button##id##Pressed() aufzurufen und besser leserlich zu machen.
 */
#define BUTTONPRESSED(id) void Board::button##id##Pressed() {       \
buttonPressed(id,fromWhom::gui);                                    \
}
/**
 * @enum gridState
 * @brief Diese Enumeration definiert die Zustände welche das Spielbrett haben können.
 * @detail Ich habe mich dazu entschlossen Mühlen direkt in den Zustand des Spielbretts zu schreiben, daher gibt es auch die Zustände mill_white und mill_black.
 */
enum class gridState { empty = 0, white = 1, black = 2, mill_white = 3, mill_black = 4};
/**
 * @enum phase
 * @brief Diese Enumeration definiert die Zustände in denen sich Spiel befinden kann.
 */
enum class phase { SetzPhase = 0, ZugPhase = 1, EndPhase = 2};
/**
 * @class Board
 * @brief Hier findet die Logik statt, welche im Hintergrund das Spiel Mühle steuert.
 */
class Board : public QObject{
    Q_OBJECT


public:
    /**
 * @brief Konstruktor für die Klasse Board.
 * @details Initialisiert wird meine Klasse mit keinem Parameter, da kein Member const ist. Es werden jedoch vier Member-Variablen im Funktionsrumpf zugewiesen.
 */
    Board();
    ~Board(){};
    /**
 * @brief Eine Methode die nach Fallunterscheidungen die addChipHelper Methode aufruft und nur zur Setzphase verwendet wird.
 * @param name Der Name, der der Instanz zugewiesen wird.
 * @param pos An welche Position soll ein Chip gelegt werden.
 * @param who Von wem kommt der Input.
 */
    void addChip(int pos, fromWhom who);
    /**
 * @brief Eine Methode welche einen Stein setzt oder eine Fehlermeldung zurückgibt.
 * @param pos An welche Position soll ein Chip gelegt werden.
 * @param who Von wem kommt der Input.
 * @param canSend Es wird überprüft ob der Zug gesendet werden soll oder nicht.
 */
    void addChipHelper(int pos, gridState color, bool canSend);
    /**
 * @brief Eine Methode die nach Fallunterscheidungen die moveChipHelper Methode aufruft und nur zur Zugphase verwendet wird.
 * @param oldPos An welcher Position soll ein Chip bewegt werden.
 * @param newPos Wohin soll der Chip gelegt werden.
 * @param who Von wem kommt der Input.
 */
    void moveChip(int oldPos, int newPos, fromWhom who);
    /**
 * @brief Eine Methode welche einen Stein von einer zur nächsten Positon bewegt oder eine Fehlermeldung zurückgibt.
 * @param oldPos An welcher Position soll ein Chip bewegt werden.
 * @param newPos Wohin soll der Chip gelegt werden.
 * @param color Welche Farbe hat der zubewegende Chip.
 * @param colormill Welche Farbe hat der zubewegende Chip in einer Mühle.
 * @param who Von wem kommt der Input.
 */
    void moveChipHelper(int oldPos, int newPos, gridState color, gridState colormill,bool canSend);
    /**
 * @brief Eine Methode die nach Fallunterscheidungen die jumpChipHelper Methode aufruft und nur zur Endphase verwendet wird.
 * @param oldPos An welcher Position soll ein Chip bewegt werden.
 * @param newPos Wohin soll der Chip gelegt werden.
 * @param who Von wem kommt der Input.
 */
    void jumpChip(int oldPos, int newPos, fromWhom who);
    /**
 * @brief Eine Methode welche einen Stein von einer zu einer freien Positon bewegt oder eine Fehlermeldung zurückgibt.
 * @param oldPos An welcher Position soll ein Chip bewegt werden.
 * @param newPos Wohin soll der Chip gelegt werden.
 * @param color Welche Farbe hat der zubewegende Chip.
 * @param colormill Welche Farbe hat der zubewegende Chip in einer Mühle.
 * @param canSend Es wird überprüft ob der Zug gesendet werden soll oder nicht.
 */
    void jumpChipHelper(int oldPos, int newPos, gridState color, gridState colormill, bool canSend);
    /**
 * @brief Eine Methode die nach Fallunterscheidungen die removeChipHelper Methode aufruft.
 * @param pos An welcher Position soll ein Chip entfernt werden.
 * @param who Von wem kommt der Input.
 */
    void removeChip(int pos, fromWhom who);
    /**
 * @brief Eine Methode welche einen Chip entfert oder eine Fehlermeldung zurückgibt.
 * @param pos An welcher Position soll ein Chip entfernt werden.
 * @param color Welche Farbe zu entfernen ist.
 * @param canSend Es wird überprüft ob der Zug gesendet werden soll oder nicht.
 */
    void removeChipHelper(int pos, gridState color, bool canSend);
    /**
 * @brief Eine Methode die nach Fallunterscheidungen die newMillHelper Methode aufruft.
 */
    void newMill();
    /**
 * @brief Überprüft ob eine neue Mühle gefunden wurde.
 */
    void newMillHelper(gridState color, gridState millColor);
    /**
 * @brief Durch die Wahl eines Feld mit Zustand entstehen Probleme mit Mühlen zuweisungen welche fixMill löst.
 */
    void fixMill();
    /**
 * @brief Ändert wer am Zug ist.
 */
    void toggleTurn();
    /**
 * @brief Gibt an wer am Zug ist.
 */
    void whosTurn();
    /**
 * @brief Eine Hilfsfuntion, welche Felher beim Entfernen behandeln kann.
 */
    void canRemove(int pos, fromWhom who);
    /**
 * @brief Verkündet die Zugsphase.
 */
    void callZugOutPhase();
    /**
 * @brief Verkündet die EndPhase.
 */
    void callOutEndPhase();
    /**
 * @brief Verkündet dass das Spiel zuende ist.
 */
    void callOutGameOver();
    /**
 * @brief Überprüft ob ein Spieler sich nicht mehr Bewegen kann.
 */
    void canMoveChips();
    /**
 * @brief Verkündet, dass ein Spieler sich nicht mehr Bewegen kann.
 */
    void callOutDeadLock();
    /**
 * @brief Gibt zurück in welcher Phase das Spiel ist.
 */
    phase whichPhase();
    /**
 * @brief Konvertiert die Logikposition in eine Netzwerkposition.
 * @param pos Welche Position umgewandelt werden soll.
 */
    std::pair<quint8, quint8> convert(int pos);
    /**
 * @brief Konvertiert die Netzwerkposition in eine Logikposition.
 * @param direction Gibt einer Zahl zwischen 0 und 8 acht.
 * @param square Gibt an ob die Position im ersten, zweiten oder dritten Quadrat ist.
 *
 */
    int invert(quint8 direction,quint8 square);
    /**
 * @brief Eine Funktion die einen Callback mit einer std::function macht um einen zweiten Buttoninput zu kriegen.
 * @param moveStorage Eine std::function für den callback
 * @param pos Die übermittelte Position auf dem Feld
 * @param isFirstInput Überprüft ob es der erste Input ist
 * @param who Von wem kommt der Input.
 */
    void waitForSecondInputInMove(std::function<void(int)> &moveStorage, int pos, bool &isFirstInput, fromWhom who);
    /**
 * @brief Die Funtion ist analog zu waitForSecondInputInMove.
 */
    void waitForSecondInputInJump(std::function<void(int)> &jumpStorage, int pos, bool &isFirstInput, fromWhom who);
    /**
 * @brief Sendet ein addChip-Signal an das Netzwerk.
 */
    void sendNetworkSignalAddChip(int pos);
    /**
 * @brief Sendet ein moveChip-Signal an das Netzwerk.
 */
    void sendNetworkSignalMove(int oldPos, int newPos);
    /**
 * @brief Sendet ein removeChip-Signal an das Netzwerk.
 */
    void sendNetWorkRemove(int pos);
    /**
 * @brief Sendet ein jumpChip-Signal an das Netzwerk.
 */
    void sendNetWorkJump(int oldPos, int newPos);
    /**
 * @brief Eine boolean zur überprüfung ob der Server etwas ausführt.
 */
    bool _isServer;

public slots:
    /**
 * @brief Der Slot für das Signal wenn ein Knopf gedrückt wird.
 * @param pos An welcher Position wurde der Knopf gedrückt.
 * @param who Von wem wird der Knopf gedrückt.
 */
    void buttonPressed(int pos, fromWhom who);
    /**
 * @brief Der Slot für das Signal wenn ein Chip gesetzt wurde.
 * @param direction Zahl 1 bis 8 für die Richtung im Netzwerk.
 * @param square Quadrat 1 bis 3 aus dem Netzwerk.
 * @param chipCount Wie viele Chips sind noch zu plazieren.
 * @param isMill Ist der plazierte Chip in einer Mühle.
 */
    void setChip(quint8 direction,quint8 square,quint8 chipCount,quint8 isMill);
    /**
 * @brief Dieser Slot ist analog zu addChip, jedoch werden zwei Positionen übergeben.
 */
    void moveChipNW(quint8 oldDirection,quint8 oldSquare,quint8 newDirection,quint8 newSquare, quint8 isMill);
    /**
 * @brief Dieser Slot ist zum entfernen eines Chips und erhält nur eine Position.
 */
    void removeChipNW(quint8 direction, quint8 square);
    /**
 * @brief Dieser Slot ist analog zu addChip, jedoch werden zwei Positionen übergeben.
 */
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
    /**
     * @brief updateColor ist das Signal, um die Farbe des Buttons auf dem Spielfeld zu ändern.
     * @param pos Gibt die Position des Steins an.
     * @param color gibt an in welche Farbe das Feld gefärbt wird.
     */
    void updateColor(int pos, gridState color);
    /**
     * @brief writeAddChip ist das Signal, welches einen Button setzt.
     */
    void writeAddChip(quint8 direction, quint8 square, quint8 chipsToPlace, quint8 millState);
    /**
     * @brief writeMoveChip ist das Signal, welches einen Button bewegt.
     */
    void writeMoveChip(quint8 oldDirection, quint8 oldSquare,quint8 newDirection, quint8 newSquare,quint8 millState);
    /**
     * @brief resWriteAddChip ist das Signal, welches die Antwort auf das Button setzen ist.
     */
    void resWriteAddChip(quint8 state);
    /**
     * @brief signalGameIsOver ist das Signal, welches das Spiel beendet.
     */
    void signalGameIsOver();
    /**
     * @brief nwResChipAction ist das Signal, welches die Antwort auf das Button bewegen ist.
     */
    void nwResChipAction(quint8);
    /**
     * @brief nwResRemoveChip ist das Signal, welches die Antwort auf das Button entfernen ist.
     */
    void nwResRemoveChip(quint8,quint8);
    /**
     * @brief signalRemoveChip ist das Signal, welches den Button entfernen.
     */
    void signalRemoveChip(quint8, quint8);
    /**
     * @brief nwResJumpChip ist das Signal, welches die Antwort auf einen Chip der springt ist.
     */
    void nwResJumpChip(quint8,quint8, quint8, quint8, quint8);
    /**
     * @brief nwResJumpChip ist das Signal, damit ein Chip springt.
     */
    void signalJumpChip(quint8, quint8, quint8, quint8, quint8);
    /**
     * @brief gameActionSignal ist das Signal, welches den Spielverlauf anzeigt.
     */
    void gameActionSignal(QString);
    /**
     * @brief whosTurnSignal ist das Signal, welches ausgibt wer am Zug ist.
     */
    void whosTurnSignal(QString);
    /**
     * @brief fadeIcon ist das Signal, welches die Icons verschwinden lässt.
     */
    void fadeIcon(int Index);
    /**
     * @brief showRemovedChip zeigt die Steine an, die man dem Gegner geklaut hat.
     * @param byWhite gibt an wer klaut.
     */
    void showRemovedChip(bool byWhite);
private:

    /**
 * @brief _grid[] Diese Map stellt das Spielfeld dar, wobei der Key die Position und der Value der Zustant ist.
 */
    std::map<int, enum gridState> _grid;
    /**
     * @brief _muehlen[] Dieses Array speichert alle möglichen Mühlen, wobei das Mühlefeld Zeilenweise von oben nach unten abgezählt wird.
     */
    std::array<std::array<int, 3> ,16> _muehlen;
    /**
     * @brief _neighbour[] Dieses Array speichert jeden Nachbar, wobei der Index des Arrays der Betrachtungspunkt ist z.B. Nachbar von 0 : 2, 9.
     */
    std::array<std::vector<int>, 24> _neighbour;
    /**
     * @brief _convert[] Die ist eine lookup Map welche als Key die Position meiner Logik und als Value die korospondierende Netzwerk Position hat.
     */
    std::map<int,std::pair<quint8,quint8>> _convert;
    /**
     * @brief Ist Weiß am Zug.
     */
    bool _isWhiteTurn;
    /**
     * @brief Die Anzahl an Chips die noch nicht removed wurden für Weiß.
     */
    int _wChipCount = 9;
    /**
     * @brief Die Anzahl an Chips die noch nicht removed wurden für Schwarz.
     */
    int _bChipCount = 9;
    /**
     * @brief Wie viele Züge wurden von addChip ausgeführt.
     */
    int _numTurns = 0;
    /**
     * @brief Gibt eine Fehlermedlung an.
     */
    bool illegalMove = false;
    /**
     * @brief Gibt an ob ein Chip entfernt werden muss.
     */
    bool haveToRemoveChip = false;
    /**
     * @brief Gibt an ob ich den Zug wechseln darf.
     */
    bool canToggle = true;
    /**
     * @brief Gibt an ob ein fehler in removeChip vorliegt.
     */
    bool errorInRemoveChip = false;
    /**
     * @brief Gibt an ob es der erste Input ist.
     */
    bool isFirstInput = true;
    /**
     * @brief Gibt an ob Weiß in der Endphase ist.
     */
    bool wInEndPhase = false;
    /**
     * @brief Gibt an ob Schwarz in der Endphase ist.
     */
    bool bInEndPhase = false;
    /**
     * @brief Gibt an ob Schwarz bewegungsunfähig ist.
     */
    bool wDeadlock = false;
    /**
     * @brief Gibt an ob Schwarz bewegungsunfähig ist.
     */
    bool bDeadlock = false;
    /**
     * @brief Gibt an wie viel Chips gesetzt werden können für Weiß.
     */
    quint8 _wChipsToPlace = 9;
    /**
     * @brief Gibt an wie viel Chips gesetzt werden können für Schwarz.
     */
    quint8 _bChipsToPlace = 9;
    /**
     * @brief Gibt an ob ein Zug gültig ist.
     */
    quint8 state = 0;
};

#endif // BOARD_H


