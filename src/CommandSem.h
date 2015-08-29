#pragma once

#include <string>
#include <vector>

using namespace std;

struct CommandSem {
    string s;
    int i;
    vector<string> stringList;

    CommandSem() {commandSemList.push_back(this);}

    static vector<CommandSem*> commandSemList;
    static void clear() {
        for (CommandSem *p : commandSemList) {
            delete p;
        }
        commandSemList.clear();
    }
};
