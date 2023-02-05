#include <bits/stdc++.h>
#include <chrono>

using namespace std;

static const string startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

static vector<bitset<64>> kingMoves;
static vector<bitset<64>> whitePawnMoves;
static vector<bitset<64>> blackPawnMoves;
static vector<bitset<64>> whitePawnCaptures;
static vector<bitset<64>> blackPawnCaptures;
static vector<bitset<64>> knightMoves;
static vector<bitset<64>> bishopMoves;
static vector<bitset<64>> rookMoves;
static vector<bitset<64>> queenMoves;

static vector<vector<char>> numSquaresToEdge(64,vector<char>(8));
static const vector<char> dirOffsets = {8,-8,-1,1,7,-7,9,-9};
static const vector<char> knightOffsets = {6,-10,15,-17,17,-15,10,-6};

static const char KCastle = 1;
static const char QCastle = 2;
static const char KnightProm = 3;
static const char BishopProm = 4;
static const char RookProm = 5;
static const char QueenProm = 6;
static const char doublePawnPush = 7;
static const char enPassant = 8;
static random_device randDev;

static const int pawnValue = 100;
static const int knightValue = 300;
static const int bishopValue = 301;
static const int rookValue = 500;
static const int queenValue = 900;
static const vector<int> pieceValues = {0,0,pawnValue,knightValue,bishopValue,rookValue,queenValue,0,
                                        0,0,pawnValue,knightValue,bishopValue,rookValue,queenValue,0,
                                        0,0,-pawnValue,-knightValue,-bishopValue,-rookValue,-queenValue,0};

static void precomputeMoveData(){
    for (char file = 0;file<8;file++){
        for(char rank = 0;rank<8;rank++){
            char numUp = 7-rank;
            char numDown = rank;
            char numLeft = file;
            char numRight = 7-file;

            char squareIndex = rank*8+file;

            numSquaresToEdge[squareIndex] = {numUp,numDown,numLeft,numRight,min(numUp,numLeft),min(numDown,numRight),min(numUp,numRight),min(numDown,numLeft)};
        }
    }
}
static void precomputeMoves(){
    for (char file = 0;file<8;file++){
        for(char rank = 0;rank<8;rank++){
            char squareIndex = rank*8+file;
            bitset<64> squareKingMoves;
            bitset<64> squareWhitePawnMoves;
            bitset<64> squareBlackPawnMoves;
            bitset<64> squareWhitePawnCaptures;
            bitset<64> squareBlackPawnCaptures;
            bitset<64> squareKnightMoves;
            bitset<64> squareBishopMoves;
            bitset<64> squareRookMoves;
            bitset<64> squareQueenMoves;


            char startDirIndex = 0;
            char endDirIndex = 8;
            for(char dirIndex = startDirIndex;dirIndex<endDirIndex;dirIndex++){
                char dirOffset = dirOffsets[dirIndex];
                char numSquares = numSquaresToEdge[squareIndex][dirIndex];
                for(char distance = 1; distance <= numSquares; distance++){
                    char targetSquare = squareIndex + distance * dirOffset;
                    if(distance == 1){
                        squareKingMoves.set(targetSquare);
                    }
                    squareQueenMoves.set(targetSquare);
                    if(dirIndex<4){
                        squareRookMoves.set(targetSquare);
                    }else{
                        squareBishopMoves.set(targetSquare);
                    }
                }
            }
            switch (file) {
                case 0:
                    startDirIndex = 4;
                    endDirIndex = 8;
                    break;
                case 1:
                    startDirIndex = 2;
                    endDirIndex = 8;
                    break;
                case 6:
                    startDirIndex = 0;
                    endDirIndex = 6;
                    break;
                case 7:
                    startDirIndex = 0;
                    endDirIndex = 4;
                    break;
                default:
                    startDirIndex = 0;
                    endDirIndex = 8;
                    break;
            }
            for(char dirIndex = startDirIndex;dirIndex<endDirIndex;dirIndex++){
                char dirOffset = knightOffsets[dirIndex];
                char targetSquare = squareIndex + dirOffset;
                if(targetSquare >= 0 && targetSquare < 64){
                    squareKnightMoves.set(targetSquare);
                }
            }
            if(rank<7){
                squareWhitePawnMoves.set(squareIndex+8);
                if(rank == 1){
                    squareWhitePawnMoves.set(squareIndex+16);
                }
            }
            if(rank>0){
                squareBlackPawnMoves.set(squareIndex-8);
                if(rank == 6){
                    squareBlackPawnMoves.set(squareIndex-16);
                }
            }
            if(file>0){
                if(rank<7){
                    squareWhitePawnCaptures.set(squareIndex+7);
                }
                if(rank>0){
                    squareBlackPawnCaptures.set(squareIndex-9);
                }
            }
            if(file<7){
                if(rank<7){
                    squareWhitePawnCaptures.set(squareIndex+9);
                }
                if(rank>0){
                    squareBlackPawnCaptures.set(squareIndex-7);
                }
            }



            kingMoves.push_back(squareKingMoves);
            whitePawnMoves.push_back(squareWhitePawnMoves);
            blackPawnMoves.push_back(squareBlackPawnMoves);
            whitePawnCaptures.push_back(squareWhitePawnCaptures);
            blackPawnCaptures.push_back(squareBlackPawnCaptures);
            knightMoves.push_back(squareKnightMoves);
            bishopMoves.push_back(squareBishopMoves);
            rookMoves.push_back(squareRookMoves);
            queenMoves.push_back(squareQueenMoves);

        }
    }
}


class chessBoard {
    bitset<64> whiteKing;
    bitset<64> whitePawns;
    bitset<64> whiteKnights;
    bitset<64> whiteBishops;
    bitset<64> whiteRooks;
    bitset<64> whiteQueens;
    bitset<64> blackKing;
    bitset<64> blackPawns;
    bitset<64> blackKnights;
    bitset<64> blackBishops;
    bitset<64> blackRooks;
    bitset<64> blackQueens;
    bitset<64> whitePieces;
    bitset<64> blackPieces;


};

class chessMove{
    bitset<64> from;
    bitset<64> to;
    int specialty;


};

class chessPosition{
    chessBoard board;
    bool whiteToMove;

    vector<bool> castlingRights;
    bitset<64> enPassant;
    int halfMoves;
    int fullMoves;
    vector<chessMove*> pseudoLegalMoves;
    vector<chessMove*> legalMoves;

    int material;

    void generatePseudoLegalMoves(){
        //TODO
    };

    void generateLegalMoves(){
        //TODO
    };

};



int main() {
    precomputeMoveData();
    precomputeMoves();

    return 0;
}
