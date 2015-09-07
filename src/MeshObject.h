#pragma once

#include <string>
#include <vector>
#include "trimesh2/TriMesh.h"

using namespace std;

class MeshObject {
public:
    string name;
    int n;
    int displayListIndex;
    trimesh::TriMesh *mesh;

    MeshObject(): mesh(0), displayListIndex(-1){}
    ~MeshObject();

    bool load(string filename);
    void makeDisplayList();
    void deleteDisplayList();
    void draw();
};
