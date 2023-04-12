#include <bits/stdc++.h>
#include <chrono>
#include "bitboards.h"


using namespace std;

static const string startingFEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";



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

//precompute possible moves for each piece from each square
static void precomputeMoves(){
    for (char file = 0;file<8;file++){
        for(char rank = 0;rank<8;rank++){
            char squareIndex = rank*8+file;
            Bitboard squareKingMoves;
            Bitboard squareWhitePawnMoves;
            Bitboard squareBlackPawnMoves;
            Bitboard squareWhitePawnCaptures;
            Bitboard squareBlackPawnCaptures;
            Bitboard squareKnightMoves;
            Bitboard squareBishopMoves;
            Bitboard squareRookMoves;
            Bitboard squareQueenMoves;


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


struct chessBoard {
    Bitboard whiteKing;
    Bitboard whitePawns;
    Bitboard whiteKnights;
    Bitboard whiteBishops;
    Bitboard whiteRooks;
    Bitboard whiteQueens;
    Bitboard blackKing;
    Bitboard blackPawns;
    Bitboard blackKnights;
    Bitboard blackBishops;
    Bitboard blackRooks;
    Bitboard blackQueens;
    Bitboard whitePieces;
    Bitboard blackPieces;


};

struct chessMove{
    Bitboard from;
    Bitboard to;
    int specialty;


};

class chessPosition{
    chessBoard board;
    bool whiteToMove;

    vector<bool> castlingRights;
    Bitboard enPassant;
    int halfMoves;
    int fullMoves;
    vector<chessMove*> pseudoLegalMoves;
    vector<chessMove*> legalMoves;

    int material;

    void generatePseudoLegalMoves(){
        Bitboard ownPieces;
        Bitboard enemyPieces;
        Bitboard ownKing;
        Bitboard ownPawns;
        Bitboard ownKnights;
        Bitboard ownBishops;
        Bitboard ownRooks;
        Bitboard ownQueens;
        if(whiteToMove){
            ownPieces = board.whitePieces;
            enemyPieces = board.blackPieces;
            ownKing = board.whiteKing;
            ownPawns = board.whitePawns;
            ownKnights = board.whiteKnights;
            ownBishops = board.whiteBishops;
            ownRooks = board.whiteRooks;
            ownQueens = board.whiteQueens;

        }else{
            ownPieces = board.blackPieces;
            enemyPieces = board.whitePieces;
            ownKing = board.blackKing;
            ownPawns = board.blackPawns;
            ownKnights = board.blackKnights;
            ownBishops = board.blackBishops;
            ownRooks = board.blackRooks;
            ownQueens = board.blackQueens;

        }

        //Kingmoves
        bitset<64> thisKingMoves;
        for(int i = 0;i<64;i++){
            if(ownKing[i]){
                thisKingMoves = kingMoves[i];
                thisKingMoves &= ~ownPieces;
                while(thisKingMoves.any()){
                    int targetSquare = thisKingMoves._Find_first();
                    thisKingMoves.reset(targetSquare);
                    chessMove* newMove = new chessMove;
                    newMove->from.set(i);
                    newMove->to.set(targetSquare);
                    newMove->specialty = 0;
                    pseudoLegalMoves.push_back(newMove);
                }

            }
        }
        //Pawnmoves
        ;





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
