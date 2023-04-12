//
// Created by heinr on 12.04.2023.
//
#include <bits/stdc++.h>
#include "types.h"
#ifndef BADENGINE2_BITBOARDS_H
#define BADENGINE2_BITBOARDS_H

#endif //BADENGINE2_BITBOARDS_H

using namespace std;

static vector<Bitboard> kingMoves;
static vector<Bitboard> whitePawnMoves;
static vector<Bitboard> blackPawnMoves;
static vector<Bitboard> whitePawnCaptures;
static vector<Bitboard> blackPawnCaptures;
static vector<Bitboard> knightMoves;
static vector<Bitboard> bishopMoves;
static vector<Bitboard> rookMoves;
static vector<Bitboard> queenMoves;

static vector<vector<char>> numSquaresToEdge(64,vector<char>(8));
static const vector<char> dirOffsets = {8,-8,-1,1,7,-7,9,-9};
static const vector<char> knightOffsets = {6,-10,15,-17,17,-15,10,-6};

//precompute squares to edge from each square
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

