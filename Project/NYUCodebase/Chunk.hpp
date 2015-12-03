//
//  Chunk.hpp
//  NYUCodebase
//
//  Created by Max Lebedev on 12/2/15.
//  Copyright © 2015 Ivan Safrin. All rights reserved.
//

#ifndef Chunk_hpp
#define Chunk_hpp

#include <stdio.h>

#define CHUNK_W 20
#define CHUNK_H 10

class Chunk{
public:
    Chunk();//int ch[CHUNK_W][CHUNK_H]
    void populate(int ch[CHUNK_H][CHUNK_W]);
    int size();
    int* chunk;
    int chunkSize;
    void set(int i, int j, int val);
};

#endif /* Chunk_hpp */
