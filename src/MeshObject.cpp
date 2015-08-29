#include "MeshObject.h"

#include <vector>
#include <utility>
#include <GL/gl.h>

#include <cstdio>

using namespace std;

bool MeshObject::load(string filename) {
    name = filename;
    trimesh::TriMesh *mesh = trimesh::TriMesh::read(filename.c_str());
    if (!mesh) {
        return false;
    }
    mesh->need_faces();
    n = mesh->faces.size();

    vertexData = new float[n * 9];
    normalData = new float[n * 9];

    mesh->need_normals();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                vertex(i, j)[k] = mesh->vertices[mesh->faces[i][j]][k];
                normal(i, j)[k] = mesh->normals[mesh->faces[i][j]][k];
            }
        }
    }

    /*
    int m = mesh->vertices.size();
    vector<trimesh::point> faceNormal;
    vector< vector<trimesh::point> > faceVertexNormal;
    vector< vector<trimesh::point> > faceVertexNormal2;
    vector< vector< pair<int, int> > > vertexRef;
    faceNormal.resize(n);
    faceVertexNormal.resize(n);
    faceVertexNormal2.resize(n);
    vertexRef.resize(m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < 3; j++) {
            vertexRef[mesh->faces[i][j]].emplace_back(i, j);
        }
        trimesh::point v0 = mesh->vertices[mesh->faces[i][1]] - mesh->vertices[mesh->faces[i][0]];
        trimesh::point v1 = mesh->vertices[mesh->faces[i][2]] - mesh->vertices[mesh->faces[i][1]];
        trimesh::point v2 = mesh->vertices[mesh->faces[i][0]] - mesh->vertices[mesh->faces[i][2]];
        faceVertexNormal[i].push_back(v2 % v0 / trimesh::len(v2) / trimesh::len(v0));
        faceVertexNormal[i].push_back(v0 % v1 / trimesh::len(v0) / trimesh::len(v1));
        faceVertexNormal[i].push_back(v1 % v2 / trimesh::len(v1) / trimesh::len(v2));
        faceVertexNormal2[i].resize(3);
        faceNormal[i] = faceVertexNormal[i][0];
        trimesh::normalize(faceNormal[i]);
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < vertexRef[i].size(); j++) {
            int a = vertexRef[i][j].first;
            int p = vertexRef[i][j].second;
            for (int k = j + 1; k < vertexRef[i].size(); k++) {
                int b = vertexRef[i][k].first;
                int q = vertexRef[i][k].second;
                float t = faceNormal[a] ^ faceNormal[b]
            }
        }
    }
    */

    delete mesh;
}

void MeshObject::makeDisplayList() {
    printf("%s: making display list\n", name.c_str());
    displayListIndex = glGenLists(1);
    glNewList(displayListIndex, GL_COMPILE);
        glBegin(GL_TRIANGLES);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < 3; j++) {
                    glNormal3fv(normal(i, j));
                    glVertex3fv(vertex(i, j));
                }
            }
        glEnd();
    glEndList();

    delete vertexData;
    delete normalData;
    vertexData = 0;
    normalData = 0;
    printf("done.\n");
}

void MeshObject::draw() {
    if (displayListIndex != -1) {
        glCallList(displayListIndex);
    }
}

void MeshObject::deleteDisplayList() {
    if (displayListIndex != -1) {
        glDeleteLists(displayListIndex, 1);
    }
}

MeshObject::~MeshObject() {
    if (vertexData != 0) {
        delete vertexData;
        delete normalData;
    }
}
