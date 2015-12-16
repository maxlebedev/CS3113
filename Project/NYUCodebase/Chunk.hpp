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
#include <iostream>

#define CHUNK_W 20
#define CHUNK_H 10

class Chunk{
public:
    Chunk();//int ch[CHUNK_W][CHUNK_H]
    void populate(int ch[CHUNK_H][CHUNK_W]);
    int size();
    void set(int i, int j, int val);
    int get(int i, int j);
    int sumCardNeighbors(int i, int j);
    
    float tileGlobalX(int i, int j);
    float tileGlobalY(int i, int j);

    void print();
    
    int* chunk;// 0 is nothing, 1 is empty space, 2 is breakable, 3 is not
    int chunkSize;
    int width;
    int height;
    std::pair <int,int> index;

};

#endif /* Chunk_hpp */
