#include "chessboard.h"

/*!
    \class chessBoard
    \brief Klasa chessBoard odpowiada za prawidłowe wyświetlanie i działanie szachownicy.

    Klasa ta dziedziczy po QWidget i wyświetla pustą szachownicę o standardowych kolorach
    szary-biały.
*/

/*!
    \fn chessBoard::chessBoard(QWidget *parent)
    \brief Konstruktor klasy chessBoard

    Konstruktor klasy chessBoard tworzy pustą szachownicę składającą sie z obiektów typu chessSquare
    oraz ustawia domyślne kolory.
*/

chessBoard::chessBoard(QWidget *parent) : QWidget(parent)
{
    currentPlayer = 0;
    setFixedSize(64*8,64*8);
    QGridLayout *gridlayout = new QGridLayout();
    gridlayout->setSpacing(0);
    blackColor = "grey";
    whiteColor = "white";
    selectColor = "blue";
    attackColor = "red";

    chesssquares = new chessSquare**[8];
    for (int i=0; i<8; i++) {
        chesssquares[i] = new chessSquare*[8];
        for(int j=0; j<8;j++) {
            chesssquares[i][j] = new chessSquare(i,j);
            gridlayout->addWidget(chesssquares[i][j],8-i,j);
            connect(chesssquares[i][j],SIGNAL(clicked(int,int)),this,SLOT(validateClick(int,int)));
        }
    }
    setDefaultColors();

    this->setLayout(gridlayout);
}

/*!
    \fn void chessBoard::validateClick(int x, int y)
    \param x Współrzędna x kliknietego pola
    \param y Współrzędna x kliknietego pola

    Funkcja analizuje współrzędne pola na szachownicy (x, y). Wywoływana jest przez sygnał clicked() z klasy chessSquare.
    W przypadku gdy na danym polu znajduje się figura, funkcja rozpoznaje typ figury, a następnie aktywuje te pola
    na które dana figura może się przemieścić. W przypadku gdy na danym polu nie ma żadnej figury, wywoływana jest funkcja
    move().
*/

void chessBoard::validateClick(int x, int y)
{
    chessSquare *square = chesssquares[x][y];
    if (square->isPiece()==true && square->getPiece()->getPlayer()==currentPlayer) {
        updateSquares();
        square->setActive(selectColor);
        sx = x;
        sy = y;
        //Walidacja ruchów
        int i;
        switch (square->getPiece()->getSign()) {
        case 'K':
            //Król
            checkActive(x+1,y-1);
            checkActive(x+1,y);
            checkActive(x+1,y+1);
            checkActive(x,y+1);
            checkActive(x,y-1);
            checkActive(x-1,y+1);
            checkActive(x-1,y);
            checkActive(x-1,y+1);
            break;
        case 'B':
            //Goniec
            i=1;
            while (checkActive(x+i,y+i)) i++;
            i=1;
            while (checkActive(x+i,y-i)) i++;
            i=1;
            while (checkActive(x-i,y+i)) i++;
            i=1;
            while (checkActive(x-i,y-i)) i++;
            break;
        case 'N':
            //Koń
            checkActive(x+2,y+1);
            checkActive(x+2,y-1);
            checkActive(x-2,y+1);
            checkActive(x-2,y-1);
            checkActive(x+1,y+2);
            checkActive(x+1,y-2);
            checkActive(x-1,y+2);
            checkActive(x-1,y-2);
            break;
        case 'Q':
            //Królowa
            i=1;
            while (checkActive(x+i,y)) i++;
            i=1;
            while (checkActive(x+i,y+i)) i++;
            i=1;
            while (checkActive(x+i,y-i)) i++;
            i=1;
            while (checkActive(x,y+i)) i++;
            i=1;
            while (checkActive(x,y-i)) i++;
            i=1;
            while (checkActive(x-i,y)) i++;
            i=1;
            while (checkActive(x-i,y+i)) i++;
            i=1;
            while (checkActive(x-i,y-i)) i++;
            break;
        case 'R':
            //Wieża
            //Zaznaczenie w górę
            i = 1;
            while (checkActive(x+i,y)==true) i++;
            i = 1;
            while (checkActive(x-i,y)==true) i++;
            i = 1;
            while (checkActive(x,y-i)==true) i++;
            i = 1;
            while (checkActive(x,y+i)==true) i++;
            break;
        default:
            //Pionek
            //Funkcja checkActive() nie może zostać użyta, ponieważ pionek posiada inne reguły dla przemieszczania i inne dla atakowania.
            if (currentPlayer==0) {
                if (x+1<8) {
                    if (chesssquares[x+1][y]->isPiece()==false) chesssquares[x+1][y]->setActive(selectColor);
                    if (y+1<8 && chesssquares[x+1][y+1]->isPiece()==true && chesssquares[x+1][y+1]->getPiece()->getPlayer()!=currentPlayer) {
                        chesssquares[x+1][y+1]->setActive(attackColor);
                    }
                    if (y-1>=0 && chesssquares[x+1][y-1]->isPiece()==true && chesssquares[x+1][y-1]->getPiece()->getPlayer()!=currentPlayer) {
                        chesssquares[x+1][y-1]->setActive(attackColor);
                    }
                }
                if (chesssquares[x][y]->getPiece()->getMoveNumber()==0) {
                    if (chesssquares[x+2][y]->isPiece()==false) chesssquares[x+2][y]->setActive(selectColor);
                }
            } else {
                if (x-1>=0) {
                    if (chesssquares[x-1][y]->isPiece()==false) chesssquares[x-1][y]->setActive(selectColor);
                    if (y+1<8 && chesssquares[x-1][y+1]->isPiece()==true && chesssquares[x-1][y+1]->getPiece()->getPlayer()!=currentPlayer) {
                        chesssquares[x-1][y+1]->setActive(attackColor);
                    }
                    if (y-1>=0 && chesssquares[x-1][y-1]->isPiece()==true && chesssquares[x-1][y-1]->getPiece()->getPlayer()!=currentPlayer) {
                        chesssquares[x-1][y-1]->setActive(attackColor);
                    }
                }
                if (chesssquares[x][y]->getPiece()->getMoveNumber()==0) {
                    if (chesssquares[x-2][y]->isPiece()==false) chesssquares[x-2][y]->setActive(selectColor);
                }
            }
            break;
        }
    } else {
        dx = x;
        dy = y;
        move();
    }
}

void chessBoard::updateSquares()
{
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            if (chesssquares[i][j]->isPiece()==true && chesssquares[i][j]->getPiece()->getPlayer()==currentPlayer) {
                chesssquares[i][j]->setActive(true);
            } else {
                chesssquares[i][j]->setActive(false);
            }
        }
    }
}


void chessBoard::setCurrentPlayer(int player)
{
    currentPlayer = player;
}

/*!
    \fn void chessBoard::move()

    Przenosi figury na szachownicy. Jeżeli na docelowo wybranym polu znajduje się jakaś figura, to zostaje ona usunięta.
    Funkcja zawiera obsługę wyjątków. Informuje ona o błędzie dotyczącym niepoprawnych współrzędnych.
*/

void chessBoard::move()
{
    if (chesssquares[dx][dy]->isActive()==false) {
        throw QString("Błąd! Podane współrzędne są niepoprawne.");
    }
    chessPiece *sourcePiece = chesssquares[sx][sy]->getPiece();
    chesssquares[sx][sy]->removePiece();
    if (chesssquares[dx][dy]->isPiece()==true) {
        if (chesssquares[dx][dy]->getPiece()->getSign()=='K') {
            //Szach mat
            emit checkMate(currentPlayer);
            return;
        }
        lost << chesssquares[dx][dy]->getPiece();
        chesssquares[dx][dy]->removePiece();
        emit newLost();
    }
    chesssquares[dx][dy]->setPiece(sourcePiece);

    //Zmiana gracza
    if (currentPlayer==0) currentPlayer=1;
    else currentPlayer=0;

    sourcePiece->nextMove();
    history << chesssquares[sx][sy]->toChessNotation();
    history << chesssquares[dx][dy]->toChessNotation();

    emit nextMove();
    updateSquares();
}

bool chessBoard::checkActive(int x, int y)
{
    if (x>=0 && x<8 && y>=0 && y<8) {
        if (chesssquares[x][y]->isPiece()==true) {
            if (chesssquares[x][y]->getPiece()->getPlayer()!=currentPlayer) {
                chesssquares[x][y]->setActive(attackColor);
            }
            return false;
        } else {
            chesssquares[x][y]->setActive(selectColor);
            return true;
        }
    } else {
        return false;
    }
}

/*!
    \fn void chessBoard::generateChessPieces()

    Generuje nowy zestaw figur i przypisuje je do szachownicy na domyślnych pozycjach.
*/

void chessBoard::generateChessPieces()
{
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            chesssquares[i][j]->removePiece();
        }
    }

    chessPiece *wrook1 = new chessPiece(0,'R',"Wieża 1");
    chessPiece *wknight1 = new chessPiece(0,'N',"Koń 1");
    chessPiece *wbishop1 = new chessPiece(0,'B',"Goniec 1");
    chessPiece *wqueen = new chessPiece(0,'Q',"Królowa");
    chessPiece *wking = new chessPiece(0,'K',"Król");
    chessPiece *wrook2 = new chessPiece(0,'R',"Wieża 2");
    chessPiece *wknight2 = new chessPiece(0,'N',"Koń 2");
    chessPiece *wbishop2 = new chessPiece(0,'B',"Goniec 2");
    for (int i=0; i<8;i++) {
        chesssquares[1][i]->setPiece(new chessPiece(0,'P',"Pionek"));
    }

    /*
    //TEST
    chessPiece *test = new chessPiece(0,'Q',"Test");
    chesssquares[3][3]->setPiece(test);;
    */

    chesssquares[0][0]->setPiece(wrook1);
    chesssquares[0][1]->setPiece(wknight1);
    chesssquares[0][2]->setPiece(wbishop1);
    chesssquares[0][3]->setPiece(wqueen);
    chesssquares[0][4]->setPiece(wking);
    chesssquares[0][5]->setPiece(wbishop2);
    chesssquares[0][6]->setPiece(wknight2);
    chesssquares[0][7]->setPiece(wrook2);
    //chessPiece **bpawns = new chessPiece*[8];
    for (int i=0; i<8;i++) {
        chesssquares[6][i]->setPiece(new chessPiece(1,'P',"Pionek"));
    }

    chessPiece *brook1 = new chessPiece(1,'R',"Wieża 1");
    chessPiece *bknight1 = new chessPiece(1,'N',"Koń 1");
    chessPiece *bbishop1 = new chessPiece(1,'B',"Goniec 1");
    chessPiece *bqueen = new chessPiece(1,'Q',"Królowa 1");
    chessPiece *bking = new chessPiece(1,'K',"Król 1");
    chessPiece *bbishop2 = new chessPiece(1,'B',"Goniec 2");
    chessPiece *bknight2 = new chessPiece(1,'N',"Koń 2");
    chessPiece *brook2 = new chessPiece(1,'R',"Wieża 2");

    chesssquares[7][0]->setPiece(brook1);
    chesssquares[7][1]->setPiece(bknight1);
    chesssquares[7][2]->setPiece(bbishop1);
    chesssquares[7][3]->setPiece(bqueen);
    chesssquares[7][4]->setPiece(bking);
    chesssquares[7][5]->setPiece(bbishop2);
    chesssquares[7][6]->setPiece(bknight2);
    chesssquares[7][7]->setPiece(brook2);
    updateSquares();
}

/*!
    \fn chessBoard::updateColors(QString newblack, QString newwhite, QString newselect, QString newattack)

    \param newblack Nowy kolor pola czarnego
    \param newwhite Nowy kolor pola białego
    \param newselect Nowy kolor pola zaznaczonego
    \param newattack Nowy kolor pola atakującego

    Ustawia nowy zestaw kolorów na szachownicy. W jej argumentach podawane są  ustawiane kolory w postaci tekstowej (QString).
*/


void chessBoard::updateColors(QString newblack, QString newwhite, QString newselect, QString newattack)
{
    blackColor = newblack;
    whiteColor = newwhite;
    attackColor = newattack;
    selectColor = newselect;
    setDefaultColors();
}

/*!
    \fn void chessBoard::resetChessboard()

     Usuwa ona wszystkie pionki (również z listy straconych figur).
     Ponadto resetuje ona numer gracza.
*/


void chessBoard::resetChessboard()
{
    setCurrentPlayer(0);

    for (int i=0; i<8; i++) {
        for(int j=0; j<8;j++) {
            if (chesssquares[i][j]->isPiece()) {
                delete chesssquares[i][j]->getPiece();
            }
        }
    }

    while (lost.isEmpty()==false) {
        delete lost.takeFirst();
    }
}

/*!
    \fn void chessBoard::setDefaultColors()

    Odświeża kolory na szachownicy, przypisując im domyślne wartości (na podstawie zmiennych whiteColor, blackColor itp.).
*/


void chessBoard::setDefaultColors()
{
    for (int i=0; i<8; i++) {
        for(int j=0; j<8;j++) {
            if (i%2==0) {
                if (j%2==0) {
                   chesssquares[i][j]->setBackgroundColor(blackColor);
                } else {
                   chesssquares[i][j]->setBackgroundColor(whiteColor);
                }
            } else {
                if (j%2==0) {
                   chesssquares[i][j]->setBackgroundColor(whiteColor);
                } else {
                   chesssquares[i][j]->setBackgroundColor(blackColor);
                }
            }
        }
    }
}

/*!
    \fn void chessBoard::blockAllSquares()

    Ustawia wszystkie pola jako nieaktywne. Funkcja wywoływana po zakończeniu gry.
*/


void chessBoard::blockAllSquares()
{
    for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            chesssquares[i][j]->setActive(false);
        }
    }
}

/*!
    \fn void chessBoard::readFromText(QString line)

    \param line Pojedyncza linia z pliku tekstowego. Np: "A2 B2"

    Funkcja analizuje współrzędne pola na szachownicy (x, y). Wywoływana jest przez sygnał clicked() z klasy chessSquare.
    W przypadku gdy na danym polu znajduje się figura, funkcja rozpoznaje typ figury, a następnie aktywuje te pola
    na które dana figura może się przemieścić. W przypadku gdy na danym polu nie ma żadnej figury, wywoływana jest funkcja
    move().
*/

void chessBoard::readFromText(QString line)
{
    if (line.contains(QRegExp("^([1-9])([A-Ha-h]).([1-9])([A-Ha-h])"))) {
        int new_sx = line[0].digitValue()-1;
        int new_sy;
        if (line[1].isUpper()) {
            new_sy = line[1].unicode()-65;
        } else {
            new_sy = line[1].unicode()-97;
        }
        validateClick(new_sx,new_sy);
        int new_dx = line[3].digitValue()-1;
        int new_dy;
        if (line[4].isUpper()) {
            new_dy = line[4].unicode()-65;
        } else {
            new_dy = line[4].unicode()-97;
        }
        validateClick(new_dx,new_dy);
    } else {
        throw QString("Błąd! Niepoprawna zawartość pliku!");
    }
}

/*!
    \fn chessBoard::~chessBoard()

    \param x Współrzędna x kliknietego pola
    \param y Współrzędna x kliknietego pola

    Destruktor usuwa wszystkie pola z szachownicy.
*/


chessBoard::~chessBoard()
{
    resetChessboard();
    for (int i=0; i<8; i++) {
        delete [] chesssquares[i];
    }
    delete[] chesssquares;
}
