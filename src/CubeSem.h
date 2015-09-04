#pragma once

#include <string>
#include <vector>
#include "Eigen/Eigen"

using namespace std;

class CubeSem;

#include "cube_yacc.h"

typedef YYLTYPE Location;

class CubeSem {
public:
    static CubeSem *top;

    enum Type {
        semAutoStmt,
        semAxisAngleStmt,
        semBandageItem,
        semBandageStmt,
        semBindingStmt,
        semBlockAliasStmt,
        semBlockEquivalenceStmt,
        semBlockItem,
        semBlockStmt,
        semClickStmt,
        semColorTag,
        semCycleStmt,
        semEmptyStmt,
        semForbidStmt,
        semGeometryStmt,
        semGroupStmt,
        semIdentifier,
        semIncludeStmt,
        semInverseStmt,
        semInvokeStmt,
        semKeyboardStmt,
        semMetadataItem,
        semMetadataStmt,
        semModelStmt,
        semNumber,
        semNumberList,
        semNumberTuple,
        semNull,
        semOperationStmt,
        semPositionAliasStmt,
        semPositionEquivalenceStmt,
        semPositionStmt,
        semRemoveBindingStmt,
        semRemoveBlockStmt,
        semRemoveOperationStmt,
        semRemovePositionStmt,
        semRequireStmt,
        semRotateStmt,
        semShiftStmt,
        semStartItem,
        semStartStmt,
        semStmtBlock,
        semString,
        semStringList,
        semSymmetryStmt,
        semTagStmt,
        semTop,                        // Used by translator only. Stands for top level node.
        semTuple,
        semTupleList,
        semTranslateStmt,
        semTransformStmt               // Used for error reporting only. Stands for any of the three transformations.
    };

    CubeSem(Type _type): type(_type), child(0){cubeSemList.push_back(this);}
    CubeSem(Location _location, Type _type): location(_location), type(_type), child(0){cubeSemList.push_back(this);}

    static string typeString(Type _type) {
        switch (_type) {
        case semAutoStmt:
            return "symmetry expansion statement";
        case semAxisAngleStmt:
            return "axis-angle rotation";
        case semBandageItem:
            return "bandage item";
        case semBandageStmt:
            return "bandage declaration";
        case semBindingStmt:
            return "binding definition";
        case semBlockAliasStmt:
            return "block alias declaration";
        case semBlockEquivalenceStmt:
            return "block equivalence declaration";
        case semBlockItem:
            return "block definition";
        case semBlockStmt:
            return "block definition statement";
        case semColorTag:
            return "tag color definition";
        case semCycleStmt:
            return "cycle statement";
        case semEmptyStmt:
            return "empty statement";
        case semForbidStmt:
            return "forbidden position declaration";
        case semGeometryStmt:
            return "geometry definition";
        case semGroupStmt:
            return "group definition";
        case semIdentifier:
            return "identifier";
        case semIncludeStmt:
            return "include statement";
        case semInverseStmt:
            return "inverse statement";
        case semInvokeStmt:
            return "invoke statement";
        case semKeyboardStmt:
            return "key sequence statement";
        case semClickStmt:
            return "mouse button click statement";
        case semMetadataItem:
            return "metadata item";
        case semMetadataStmt:
            return "metadata statement";
        case semModelStmt:
            return "model definition";
        case semNull:
            return "null semantic value";
        case semNumber:
            return "number";
        case semNumberList:
            return "number list";
        case semNumberTuple:
            return "number tuple";
        case semOperationStmt:
            return "operation definition";
        case semPositionAliasStmt:
            return "position alias declaration";
        case semPositionEquivalenceStmt:
            return "position equivalence declaration";
        case semPositionStmt:
            return "position declaration";
        case semRemoveBindingStmt:
            return "binding removal statement";
        case semRemoveBlockStmt:
            return "block removal statement";
        case semRemoveOperationStmt:
            return "operation removal statement";
        case semRemovePositionStmt:
            return "position removal statement";
        case semRequireStmt:
            return "required position declaration";
        case semRotateStmt:
            return "rotation by Eulerian angle";
        case semShiftStmt:
            return "shift statement";
        case semStartItem:
            return "starting position declaration";
        case semStartStmt:
            return "start statement";
        case semStmtBlock:
            return "statement block";
        case semString:
            return "string";
        case semStringList:
            return "string list";
        case semSymmetryStmt:
            return "symmetry definition";
        case semTagStmt:
            return "tag statement";
        case semTop:
            return "top level";
        case semTransformStmt:
            return "transformation";
        case semTranslateStmt:
            return "translation";
        case semTuple:
            return "tuple";
        case semTupleList:
            return "tuple list";
        default:
            return "";
        }
    }

    Location location;
    Type type;
    double number;
    vector<double> numberList;
    string string1;
    string string2;
    vector<string> stringList;
    CubeSem *child;
    vector<CubeSem*> childList;
    Eigen::Affine3d transformation;

    /*
    CubeSem *copy() {
        CubeSem *ret = new CubeSem(location, type);
        ret->number = number;
        ret->numberList = numberList;
        ret->string1 = string1;
        ret->string2 = string2;
        ret->stringList = stringList;
        ret->child = (child == 0) ? 0 : child->copy();
        for (CubeSem *p : childList) {
            ret->childList.push_back(p->copy());
        }
        ret->transformation = transformation;
        return ret;
    }
    */

    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    static vector<CubeSem*> cubeSemList;
    static void clear() {
        for (CubeSem *p : cubeSemList) {
            delete p;
        }
        cubeSemList.clear();
    }
};
