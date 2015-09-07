#include "MeshObject.h"

#include <vector>
#include <utility>
#include <GL/gl.h>

#include <cstdio>

using namespace std;

bool MeshObject::load(string filename) {
    name = filename;
    mesh = trimesh::TriMesh::read(filename.c_str());
    if (mesh == 0) {
        return false;
    }
    mesh->need_faces();
    n = mesh->faces.size();
    mesh->need_normals();
    return true;
}

void MeshObject::makeDisplayList() {
    printf("%s: making display list\n", name.c_str());
    displayListIndex = glGenLists(1);
    glNewList(displayListIndex, GL_COMPILE);
        glBegin(GL_TRIANGLES);
            for (int i = 0; i < n; i++) {
                trimesh::vec normal = trinorm(mesh->vertices[mesh->faces[i][0]], mesh->vertices[mesh->faces[i][1]], mesh->vertices[mesh->faces[i][2]]);
                trimesh::normalize(normal);
                glNormal3fv(normal);
                for (int j = 0; j < 3; j++) {
                    glVertex3fv(mesh->vertices[mesh->faces[i][j]]);
                }
            }
        glEnd();
    glEndList();
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
    if (mesh != 0) {
        delete mesh;
    }
}
