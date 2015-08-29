#pragma once

#include <vector>

using namespace std;

class DisjointSet {
private:
    int size;
    vector<int> parent, rank;

public:
    DisjointSet(int _size) {
        size = _size;
        parent.resize(size);
        rank.resize(size);

        for (int i = 0; i < size; i++)
        {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] == x) {
            return x;
        }
        else {
            parent[x] = find(parent[x]);
            return parent[x];
        }
    }

    int join(int a, int b) {
        a = find(a);
        b = find(b);
        if (a != b) {
            if (rank[a] < rank[b]) {
                parent[a] = b;
                return b;
            }
            else if (rank[a] > rank[b]) {
                parent[b] = a;
                return a;
            }
            else {
                rank[a]++;
                parent[b] = a;
                return a;
            }
        }
        else {
            return a;
        }
    }
};
