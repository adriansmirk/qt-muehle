#include "board.h"
#include <QWidget>


Board::Board(): _isWhiteTurn(1){
    for(int i = 0; i < 24; ++i){
        _grid[i]=gridState::empty;

    }
    _muehlen = {{
                 { {0, 1, 2} },
                 { {3, 4, 5} },
                 { {6, 7, 8} },
                 { {0, 9, 21} },
                 { {3, 10, 18} },
                 { {6, 11, 15} },
                 { {8, 12, 17} },
                 { {5, 13, 20} },
                 { {2, 14, 23} },
                 { {9, 10, 11} },
                 { {12, 13, 14} },
                 { {15, 16, 17} },
                 { {18, 19, 20} },
                 { {21, 22, 23} },
                 { {16, 19, 22} },
                 { {1, 4, 7} },
                 }};

    _neighbour = {{
        {1, 9},          // Nachbarn von Position 0
        {0, 2, 4},       // Nachbarn von Position 1
        {1, 14},         // Nachbarn von Position 2
        {4, 10},         // Nachbarn von Position 3
        {1, 3, 5, 7},    // Nachbarn von Position 4
        {4, 13},         // Nachbarn von Position 5
        {7, 11},         // Nachbarn von Position 6
        {4, 6, 8},       // Nachbarn von Position 7
        {7, 12},         // Nachbarn von Position 8
        {0, 21, 10},     // Nachbarn von Position 9
        {9, 11, 18, 3},  // Nachbarn von Position 10
        {6, 10, 15},     // Nachbarn von Position 11
        {8, 13, 17},     // Nachbarn von Position 12
        {5, 12, 14, 20},     // Nachbarn von Position 13
        {2, 13, 23},     // Nachbarn von Position 14
        {11, 16},        // Nachbarn von Position 15
        {15, 17, 19},    // Nachbarn von Position 16
        {12, 16},        // Nachbarn von Position 17
        {10, 19},        // Nachbarn von Position 18
        {16, 18, 20, 22},// Nachbarn von Position 19
        {13, 19},        // Nachbarn von Position 20
        {9, 22},         // Nachbarn von Position 21
        {19, 21, 23},    // Nachbarn von Position 22
        {14, 22}         // Nachbarn von Position 23

    }};
    _convert ={
                {0, {8, 1}},
                {1, {1, 1}},
                {2, {2, 1}},
                {3, {8, 2}},
                {4, {1, 2}},
                {5, {2, 2}},
                {6, {8, 3}},
                {7, {1, 3}},
                {8, {2, 3}},
                {9, {7, 1}},
                {10, {7, 2}},
                {11, {7, 3}},
                {12, {3, 3}},
                {13, {3, 2}},
                {14, {3, 1}},
                {15, {6, 3}},
                {16, {5, 3}},
                {17, {4, 3}},
                {18, {6, 2}},
                {19, {5, 2}},
                {20, {4, 2}},
                {21, {6, 1}},
                {22, {5, 1}},
                {23, {4, 1}},
                };

}

void Board::sendNetworkSignalAddChip(int pos) {
    quint8 isMill;
    if(_grid[pos] == gridState::black || _grid[pos] == gridState::white){
        isMill = 0;
    } else{
        isMill = 1;
    }
    if(_numTurns % 2 == 0) {
        _wChipsToPlace--;
        emit writeAddChip(convert(pos).first, convert(pos).second, _wChipsToPlace, isMill);
    } else {
        _bChipsToPlace--;
        emit writeAddChip(convert(pos).first, convert(pos).second, _bChipsToPlace, isMill);
    }
}

void Board::sendNetworkSignalMove(int oldPos, int newPos) {
    quint8 isMill;
    if(_grid[newPos] == gridState::black || _grid[newPos] == gridState::white){
        isMill = 0;
    } else{
        isMill = 1;
    }
    emit writeMoveChip(convert(oldPos).first, convert(oldPos).second,convert(newPos).first, convert(newPos).second, isMill);
}

void Board::sendNetWorkJump(int oldPos, int newPos){
    quint8 isMill ;
    if(_grid[newPos] == gridState::black || _grid[newPos] == gridState::white){
        isMill = 0;
    } else{
        isMill = 1;
    }
    emit signalJumpChip(convert(oldPos).first, convert(oldPos).second,convert(newPos).first, convert(newPos).second, isMill);
}

void Board::sendNetWorkRemove(int pos){
    emit signalRemoveChip(convert(pos).first, convert(pos).second);
}

int Board::invert(quint8 direction,quint8 square){
    for (auto it : _convert) {
        std::pair<quint8, quint8> tmp = {direction, square};
        if (tmp == it.second) {
            return it.first;
        }
    }
    return -1;
}

std::pair<quint8, quint8> Board::convert(int pos){
    for (auto it : _convert) {
        if (pos == it.first) {
            return it.second;
        }
    }
    return std::pair<quint8,quint8>(-1,-1);
}

void Board::setChip(quint8 direction,quint8 square,quint8 chipCount,quint8 isMill) {
    buttonPressed(invert(direction, square), fromWhom::network);
    if(_isWhiteTurn) {
        //_wChipCount = chipCount;
        if(isMill == 1){
            _grid[invert(direction,square)] = gridState::mill_white;
        }
    } else {
        //_bChipCount = chipCount;
        if(isMill == 1){
            _grid[invert(direction,square)] = gridState::mill_black;
        }
        emit nwResChipAction(state);

    }

}

void Board::moveChipNW(quint8 oldDirection,quint8 oldSquare,quint8 newDirection,quint8 newSquare, quint8 isMill) {
    buttonPressed(invert(oldDirection, oldSquare), fromWhom::network);
    buttonPressed(invert(newDirection, newSquare), fromWhom::network);
    if(_isWhiteTurn) {
        if(isMill == 1){
            _grid[invert(newDirection,newSquare)] = gridState::mill_white;
        }
    } else {
        if(isMill == 1){
            _grid[invert(newDirection,newSquare)] = gridState::mill_black;
        }
        emit nwResChipAction(state);
    }
}

void Board::jumpChipNW(quint8 oldDirection,quint8 oldSquare,quint8 newDirection,quint8 newSquare, quint8 isMill) {
    buttonPressed(invert(oldDirection, oldSquare), fromWhom::network);
    buttonPressed(invert(newDirection, newSquare), fromWhom::network);
    if(_isWhiteTurn) {
        if(isMill == 1){
            _grid[invert(newDirection,newSquare)] = gridState::mill_white;
        }
    } else {
        if(isMill == 1){
            _grid[invert(newDirection,newSquare)] = gridState::mill_black;
        }
        emit nwResChipAction(state);
    }
}


void Board::removeChipNW(quint8 direction, quint8 square){
    buttonPressed(invert(direction, square), fromWhom::network);
    if(!_isWhiteTurn) {
        quint8 removedPieces = 9 - _wChipCount;
        emit nwResRemoveChip(state,removedPieces);
    }
}

void Board::callOutDeadLock(){
    if(wDeadlock){
        _wChipCount = 2;
        callOutGameOver();
    }

    if(bDeadlock){
        _bChipCount = 2;
        callOutGameOver();
    }
}

void Board::canMoveChips(){
    if(_numTurns >= 2){
        wDeadlock = true;
        bDeadlock = true;
    }

    for(int i = 0; i < 24 ; i++){
        if((_grid[i] == gridState::white || _grid[i] == gridState::mill_white) && _numTurns >= 2){
            for (int neighbor : _neighbour[i]) {
                if (_grid[neighbor] == gridState::empty) {
                    wDeadlock = false;
                    break;
                }
            }

        }
        if((_grid[i] == gridState::black || _grid[i] == gridState::mill_black) && _numTurns >= 2){
            for (int neighbor : _neighbour[i]) {
                if (_grid[neighbor] == gridState::empty) {
                    bDeadlock = false;
                    break;
                }
            }

        }
    }

    callOutDeadLock();

}


void Board::buttonPressed(int pos,fromWhom who) {

    switch(whichPhase()) {

    case phase::SetzPhase:
        if(haveToRemoveChip){
            canRemove(pos, who);
            break;
        }
        addChip(pos, who);
        callZugOutPhase();
        whosTurn();
        break;
    case phase::ZugPhase:
        if(!isLegalInput(who)) {
            state = 0x13;
            break;
        }
        static std::function<void(int)> moveStorage;
        if(haveToRemoveChip){
            canRemove(pos, who);
            break;
        }
        waitForSecondInputInMove(moveStorage, pos, isFirstInput,who);
        canMoveChips();
        break;
    case phase::EndPhase:
        if(!isLegalInput(who)) {
            state = 0x13;
            break;
        }
        static std::function<void(int)> jumpStorage;
        if(haveToRemoveChip){
            canRemove(pos, who);
            break;
        }
        waitForSecondInputInJump(jumpStorage, pos, isFirstInput, who);
        break;
    }

}

bool Board::isLegalInput(fromWhom who) {
    if(_isServer) {
        return (_isWhiteTurn && who == fromWhom::gui) || (!_isWhiteTurn && who == fromWhom::network);
    } else {
        return (_isWhiteTurn && who == fromWhom::network) || (!_isWhiteTurn && who == fromWhom::gui);
    }
}

void Board::addChipHelper(int pos, gridState color, bool canSend){
    if(_grid[pos] == gridState::empty){
        _grid[pos] = color;
        int index = _numTurns;
        emit fadeIcon(index);
        emit gameActionSignal("Ein Stein wurde gesetzt");
        emit updateColor(pos, color);
        _numTurns++;
        state = 0x00;
        newMill();
        if(canSend){
            sendNetworkSignalAddChip(pos);
        }
        if(canToggle){
            toggleTurn();
        }
        canToggle = true;
    } else {
        state = 0x12;
        std::cout<<"Error in addChip"<<std::endl;
        emit gameActionSignal("Error in addChip\n");
    }
}

void Board::addChip(int pos, fromWhom who){
    if(_isServer) { // network is black
        if(_isWhiteTurn && who == fromWhom::gui) {
            addChipHelper(pos, gridState::white, true);
        } else if(!_isWhiteTurn && who ==fromWhom::network) {
            addChipHelper(pos, gridState::black, false);
        }else{
            state = 0x13;
        }
    } else { // network is white
        if(!_isWhiteTurn && who == fromWhom::gui) {
            addChipHelper(pos, gridState::black, true);
        } else if(_isWhiteTurn && who == fromWhom::network) {
            addChipHelper(pos, gridState::white, false);
        } else{
            state = 0x13;
        }
    }

}

void Board::moveChipHelper(int oldPos, int newPos, gridState color, gridState colormill, bool canSend){
    if((_grid[oldPos] == color || _grid[oldPos] == colormill) && _grid[newPos] == gridState::empty){
        if (std::find(_neighbour[oldPos].begin(), _neighbour[oldPos].end(), newPos) != _neighbour[oldPos].end()) {
            _grid[oldPos] = gridState::empty;
            emit updateColor(oldPos, gridState::empty);
            _grid[newPos] = color;
            emit updateColor(newPos, color);
            if(canSend){
                sendNetworkSignalMove(oldPos, newPos);
            }
        }
    } else if(_grid[newPos] != gridState::empty || _grid[oldPos] != color || _grid[oldPos] != colormill){
        std::cout<<"Error in moveChip"<<oldPos<<newPos<<std::endl;
        emit gameActionSignal("error in moveChip\n");
        canToggle = false;
        illegalMove = true;
    }
}

void Board::moveChip(int oldPos, int newPos, fromWhom who){
    if(_isServer) { // network is black
        if(_isWhiteTurn && who == fromWhom::gui) {
            moveChipHelper(oldPos, newPos, gridState::white, gridState::mill_white, true);
        } else if(!_isWhiteTurn && who == fromWhom::network) {
            moveChipHelper(oldPos, newPos, gridState::black, gridState::mill_black,false);
        }  else{
            illegalMove = true;
        }
    } else if(!_isServer) { // network is white
        if(!_isWhiteTurn && who == fromWhom::gui) {
            moveChipHelper(oldPos, newPos, gridState::black, gridState::mill_black, true);
        } else if(_isWhiteTurn && who == fromWhom::network) {
            moveChipHelper(oldPos, newPos, gridState::white, gridState::mill_white, false);
        } else{
            illegalMove = true;
        }
    }
}

void Board::jumpChipHelper(int oldPos, int newPos, gridState color, gridState colormill, bool canSend){
    if((_grid[oldPos] == color || _grid[oldPos] == colormill) && _grid[newPos] == gridState::empty){
        _grid[oldPos] = gridState::empty;
        _grid[newPos] = color;
        emit updateColor(oldPos, gridState::empty);
        emit updateColor(newPos, color);
        if(canSend){
            sendNetWorkJump(oldPos, newPos);
        }
    }else if(_grid[newPos] != gridState::empty || _grid[oldPos] != color || _grid[oldPos] != colormill){
        std::cout<<"error in jumpChip"<<std::endl;
        emit gameActionSignal("error in jumpChip\n");
        illegalMove = true;
        canToggle = false;
    }

}

void Board::jumpChip(int oldPos, int newPos, fromWhom who){
    if(_isServer) { // network is black
        if(_isWhiteTurn && who == fromWhom::gui) {
            jumpChipHelper(oldPos, newPos, gridState::white, gridState::mill_white, true);
        } else if(!_isWhiteTurn && who == fromWhom::network) {
            jumpChipHelper(oldPos, newPos, gridState::black, gridState::mill_black,false);
        }  else{
            illegalMove = true;
        }
    } else if(!_isServer) { // network is white
        if(!_isWhiteTurn && who == fromWhom::gui) {
            jumpChipHelper(oldPos, newPos, gridState::black, gridState::mill_black, true);
        } else if(_isWhiteTurn && who == fromWhom::network) {
            jumpChipHelper(oldPos, newPos, gridState::white, gridState::mill_white, false);
        } else{
            illegalMove = true;
        }
    }
}

void Board::removeChipHelper(int pos, gridState color, bool canSend){
    bool onlyMills = true;
    for(int i = 0; i < 24; ++i){
        if(_isWhiteTurn && _grid[i] == gridState::black){
            onlyMills = false;
        }else if(!_isWhiteTurn && _grid[i] == gridState::white){
            onlyMills = false;
        }
    }
    if(_grid[pos] == color || onlyMills){
        _grid[pos] = gridState::empty;
        emit updateColor(pos, gridState::empty);
        if(canSend){
            sendNetWorkRemove(pos);
        }
        if(_isWhiteTurn){
            _bChipCount--;
            emit gameActionSignal(QString("Schwarz hat noch %1 Chips\n").arg(_bChipCount));
            emit showRemovedChip(true);  // white removed black chip
        }
        if(!_isWhiteTurn){
            _wChipCount--;
            emit gameActionSignal(QString("Weiß hat noch %1 Chips\n").arg(_wChipCount));
            emit showRemovedChip(false); // black removed white chip
        }
        callOutEndPhase();
        callOutGameOver();
    } else{

        std::cout<<"Error in removeChip"<<std::endl;
        emit gameActionSignal("Error in removeChip\n");
        errorInRemoveChip = true;
    }
}

void Board::removeChip(int pos, fromWhom who){
    if(_isServer) { // network is black
        if(_isWhiteTurn && who == fromWhom::gui) {
            removeChipHelper(pos, gridState::black, true);
        } else if(!_isWhiteTurn && who == fromWhom::network) {
            removeChipHelper(pos, gridState::white, false);
        }else{
            errorInRemoveChip = true;
            state = 0x21;
        }
    } else{ // network is white
        if(!_isWhiteTurn && who == fromWhom::gui) {
            removeChipHelper(pos, gridState::white, true);
        } else if(_isWhiteTurn && who == fromWhom::network) {
            removeChipHelper(pos, gridState::black, false);
        } else{
            errorInRemoveChip = true;
            state = 0x21;
        }
    }
}

void Board::newMillHelper(gridState color, gridState millColor){
    for(auto mills : _muehlen){
        int count = 0;
        for (int pos : mills) {
            if (_grid[pos] == color) {
                ++count;
            }
            else if (_grid[pos] != millColor) {
                count = -1;
                break;
            }
        }
        if (count > 0) {
            _grid[mills[0]] = millColor;
            _grid[mills[1]] = millColor;
            _grid[mills[2]] = millColor;
            if(_isWhiteTurn){
                emit gameActionSignal("----------------------- \n"
                                      "neue Mühle gefunden für weiß\n"
                                      "----------------------- \n"
                                      "Wähle einen Stein zum entfernen\n");
                haveToRemoveChip = true;
                canToggle = false;
                state = 0x01;
            } else{
                emit gameActionSignal("----------------------- \n"
                                      "neue Mühle gefunden für schwarz\n"
                                      "----------------------- \n"
                                      "Wähle einen Stein zum entfernen\n");
                haveToRemoveChip = true;
                canToggle = false;
                state = 0x01;
            }

        }

    }

}

void Board::newMill(){
    if(_isWhiteTurn){
        newMillHelper(gridState::white, gridState::mill_white);
    } else{
        newMillHelper(gridState::black, gridState::mill_black);
    }
}

void Board::fixMill(){
    for(int i = 0; i < 24; i++){
        if(_grid[i] == gridState::mill_black){
            _grid[i] = gridState::black;
        } else if(_grid[i] == gridState::mill_white){
            _grid[i] = gridState::white;
        }
    }
    for(auto &mills : _muehlen){
        if((_grid[mills[0]] == gridState::white || _grid[mills[0]] == gridState::mill_white) &&
            (_grid[mills[1]] == gridState::white || _grid[mills[1]] == gridState::mill_white) &&
            (_grid[mills[2]] == gridState::white || _grid[mills[2]] == gridState::mill_white)){
            _grid[mills[0]] = gridState::mill_white;
            _grid[mills[1]] = gridState::mill_white;
            _grid[mills[2]] = gridState::mill_white;
        }

    }
    for(auto &mills : _muehlen){
        if((_grid[mills[0]] == gridState::black || _grid[mills[0]] == gridState::mill_black) &&
            (_grid[mills[1]] == gridState::black || _grid[mills[1]] == gridState::mill_black) &&
            (_grid[mills[2]] == gridState::black || _grid[mills[2]] == gridState::mill_black)){
            _grid[mills[0]] = gridState::mill_black;
            _grid[mills[1]] = gridState::mill_black;
            _grid[mills[2]] = gridState::mill_black;
        }

    }

}

void Board::toggleTurn(){
    _isWhiteTurn = !_isWhiteTurn;
}

void Board::whosTurn(){
    if(_isWhiteTurn){
        emit whosTurnSignal("Weiß ist am Zug");
    } else{
        emit whosTurnSignal("Schwarz ist am Zug");
    }
}

void Board::canRemove(int pos,fromWhom who){
    if(haveToRemoveChip){
        fixMill();
        removeChip(pos, who);
        if(!errorInRemoveChip){
            haveToRemoveChip = false;
            toggleTurn();
            whosTurn();
        }
        if(errorInRemoveChip){
            emit gameActionSignal("Illegaler Move wähle ein anderes Feld\n");
            errorInRemoveChip = false;
        }

    }

}

void Board::callZugOutPhase(){
    if(_numTurns == 18){
        emit gameActionSignal("-----------------------\n"
                              "Zugphase\n"
                              "-----------------------\n"
                              "Wähle ein Chip zum moven\n");
    }
}

void Board::callOutEndPhase(){
    static bool wBlock = false;
    static bool bBlock = false;
    if(_wChipCount == 3 && !wBlock){
        emit gameActionSignal("-----------------------\n"
                              "Weiß ist in der Endphase\n"
                              "-----------------------");
        wBlock = true;
    }
    if(_bChipCount == 3 && !bBlock){
        emit gameActionSignal("-----------------------\n"
                              "Schwarz ist in der Endphase\n"
                              "-----------------------");
        bBlock = true;
    }
}

void Board::callOutGameOver(){
    if(_wChipCount == 2){
        emit whosTurnSignal("Schwarz hat gewonnen\n");
        emit gameActionSignal("Schwarz hat gewonnen\n");
        emit signalGameIsOver();
    } else if (_bChipCount == 2){
        emit whosTurnSignal("Weiß hat gewonnen\n");
        emit gameActionSignal("Weiß hat gewonnen\n");
        emit signalGameIsOver();
    }
}

phase Board::whichPhase(){
    if(_numTurns < 18){
        return phase::SetzPhase;
    }else if(_isWhiteTurn){
        if(_wChipCount == 3){
            wInEndPhase = true;
            return phase::EndPhase;
        }
    } else if (!_isWhiteTurn){
        if(_bChipCount == 3){
            bInEndPhase = true;
            return phase::EndPhase;
        }
    }
    return phase::ZugPhase;

}

void Board::waitForSecondInputInMove(std::function<void(int)> &moveStorage, int pos, bool &isFirstInput, fromWhom who){
    if(isFirstInput) {
        if(_numTurns != 18){
            emit gameActionSignal("Wähle ein Chip zum moven\n");
        }
        moveStorage = std::bind(&Board::moveChip, this, pos, std::placeholders::_1, who);
        emit gameActionSignal("Du hast einen Chip gewählt\n");
        emit gameActionSignal("Wähle eine neue Position\n");
        isFirstInput = false;
        _numTurns++;
    } else {
        moveStorage(pos);
        if(!illegalMove){
            emit gameActionSignal("Ein Chip wurde bewegt\n");
        }
        if(illegalMove){
            canToggle = false;
        }
        isFirstInput = true;
        newMill();
        if(canToggle){
            toggleTurn();
        }
        canToggle = true;
        illegalMove = false;
        whosTurn();
    }
}

void Board::waitForSecondInputInJump(std::function<void(int)> &jumpStorage, int pos, bool &isFirstInput, fromWhom who){
    if(isFirstInput) {
        emit gameActionSignal("Wähle ein Chip zum jumpen\n");
        jumpStorage = std::bind(&Board::jumpChip, this, pos, std::placeholders::_1, who);
        emit gameActionSignal("Du hast einen Chip gewählt\n");
        emit gameActionSignal("Wähle eine neue Position\n");
        isFirstInput = false;
        _numTurns++;
    }else {
        jumpStorage(pos);
        if(!illegalMove){
            emit gameActionSignal("Ein Chip wurde bewegt\n");
        }
        if(illegalMove){
            canToggle = false;
        }
        isFirstInput = true;
        newMill();
        if(canToggle){
            toggleTurn();
        }
        canToggle = true;
        illegalMove = false;
        whosTurn();
    }
}

BUTTONPRESSED(0)
BUTTONPRESSED(1)
BUTTONPRESSED(2)
BUTTONPRESSED(3)
BUTTONPRESSED(4)
BUTTONPRESSED(5)
BUTTONPRESSED(6)
BUTTONPRESSED(7)
BUTTONPRESSED(8)
BUTTONPRESSED(9)
BUTTONPRESSED(10)
BUTTONPRESSED(11)
BUTTONPRESSED(12)
BUTTONPRESSED(13)
BUTTONPRESSED(14)
BUTTONPRESSED(15)
BUTTONPRESSED(16)
BUTTONPRESSED(17)
BUTTONPRESSED(18)
BUTTONPRESSED(19)
BUTTONPRESSED(20)
BUTTONPRESSED(21)
BUTTONPRESSED(22)
BUTTONPRESSED(23)

