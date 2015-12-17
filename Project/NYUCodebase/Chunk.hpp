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
#include <map>
#include <math.h>
#include <vector>
using namespace std;

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
    std::pair <float,float> toGlobalCoords(int i, int j);
//    std::pair <float,float> fromGlobalCoords(std::map<std::pair<int, int>, Chunk> ChunkMap, int x, int y);
    static vector<int> fromGlobalCoords(float x, float y);

    void print();
    
    int* chunk;// 0 is nothing, 1 is empty space, 2 is breakable, 3 is not
    int chunkSize;
    static const int width = 20;
    static const int height = 10;
    std::pair <int,int> index;

};

#endif /* Chunk_hpp */
