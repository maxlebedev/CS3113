//
//  Chunk.cpp
//  NYUCodebase
//
//  Created by Max Lebedev on 12/2/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#include "Chunk.hpp"

Chunk::Chunk(){
    chunk = new int[CHUNK_W*CHUNK_H];
    chunkSize = 0;
}

void Chunk::populate(int ch[CHUNK_H][CHUNK_W]){
    int k = 0;
    for (int i = 0; i < CHUNK_H; i++){
        for (int j = 0; j < CHUNK_W; j++){
            chunk[k++] = ch[j][i];
        }
    chunkSize += k;
    }
}

void Chunk::set(int i, int j, int val){
    chunk[(i*CHUNK_W)+j] = val;
}

int Chunk::size(){
    return chunkSize;
}