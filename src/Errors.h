#pragma once

#include "CubeSem.h"
#include <string>
#include <sstream>

using namespace std;

class Error {
protected:
    Location location;

public:
    Error(Location _location): location(_location){}
    virtual string message() = 0;
    string toString() {
        stringstream s;
        s << "Line " << location.line << ", column " << location.column << ": " << message();
        return s.str();
    }
};

class WrongNumberOfParametersError: public Error {
private:
    CubeSem::Type type;
    int k;

public:
    WrongNumberOfParametersError(Location _location, CubeSem::Type _type, int _k):
        Error(_location), type(_type), k(_k){}
    string message() {
        stringstream s;
        s << "Wrong number of parameters: a " << CubeSem::typeString(type) << " may not take " << k << " parameters";
        return s.str();
    }
};

class MisplacedStatementError: public Error {
private:
    CubeSem::Type type1, type2;

public:
    MisplacedStatementError(Location _location, CubeSem::Type _type1, CubeSem::Type _type2):
        Error(_location), type1(_type1), type2(_type2){}
    string message() {
        if (type2 == CubeSem::semTop) {
            return string("Misplaced statement: a ").append(CubeSem::typeString(type1)).append(" may not appear at the top level");
        }
        else {
            return string("Misplaced statement: a ").append(CubeSem::typeString(type1)).append(" may not appear inside a ").append(CubeSem::typeString(type2));
        }
    }
};

class GenericError: public Error {
private:
    string s;

public:
    GenericError(Location _location, string _s):
        Error(_location), s(_s){}
    string message() {
        return s;
    }
};
