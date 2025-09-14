#ifndef LOGIC_H
#define LOGIC_H
#include <string>
#include <map>
#include <vector>
#include <array>
#include <optional>

class Logic
{
public:
    Logic();
    class Board{
        std::map<int, int> _grid;
        std::array<std::array<int, 3> ,16> _muehlen;
        bool _isWhiteTurn;

    public:
        Board();
        void addChip(int position);
        void moveChip();
        void jumpChip();
        std::optional<std::array<int, 3>> checkMill();
        int getGridAt(int index);
        void setGridAt(int index, int val);
        bool invalidMove();
        void turns();

    };
    class Chip{
        const bool _white;
        int _position;
    public:
        Chip(bool white, int position);
        bool getColor();
        void freeChip(Chip myChip, Board myBoard);
        int getPosition();
        void setPosition(int position);

    };
    class Player{
        int _chips;
        int _remainingChips;
        const bool _isWhite;

    public:
        Player(bool isWhite);
        void placeChip(int position, Board myBoard, Player myPlayer);
        void moveChip(int position, Chip myChip, Board myBoard, Player myPlayer);
        void removeChip(int position, Board myBoard, Chip myChip, Player myPlayer);
        bool getColor();
        int phases();

    };

};

enum gridState {empty = 0, white = 1, black = 2, SetzPhase = 3, ZugPhase = 4, EndPhase = 5};

#endif // LOGIC_H
