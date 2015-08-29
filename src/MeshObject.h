#pragma once

#include <string>
#include <vector>
#include "trimesh2/TriMesh.h"

using namespace std;

class MeshObject {
public:
    string name;
    float *vertexData;
    float *normalData;
    int n;
    int displayListIndex;

    MeshObject(): vertexData(0), normalData(0), displayListIndex(-1){}
    ~MeshObject();

    bool load(string filename);
    void makeDisplayList();
    void deleteDisplayList();
    void draw();

    float *vertex(int i, int j) const {
        return vertexData + (i * 9 + j * 3);
    }

    float *normal(int i, int j) const {
        return normalData + (i * 9 + j * 3);
    }
};
