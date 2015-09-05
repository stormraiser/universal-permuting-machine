#pragma once

#include <string>
#include <set>
#include <map>
#include <vector>
#include "Eigen/Eigen"
#include "Eigen/StdVector"
#include "CubeSem.h"
#include "TransformationPrimitive.h"
#include "Objects.h"
#include "Errors.h"
#include "Cube.h"

#define TAG_CODE_BASE 1000

using namespace std;

class Translator {
public:
    static vector<Error*> globalErrorList;

private:
    vector<string> fileStack;
    vector<CubeSem::Type> envStack;
    vector< Eigen::Affine3d, Eigen::aligned_allocator<Eigen::Affine3d> > transformationStack;
    vector<TransformationPrimitive> operationTransformationStack;

    vector<CubeSem*> metadataStmtList;
    vector<CubeSem*> tagStmtList;
    vector<CubeSem*> symmetryStmtList;
    vector<CubeSem*> geometryStmtList;
    vector<CubeSem*> blockStmtList;
    vector<CubeSem*> blockAliasStmtList;
    vector<CubeSem*> blockEquivalenceStmtList;
    vector<CubeSem*> bandageStmtList;
    vector<CubeSem*> positionStmtList;
    vector<CubeSem*> positionAliasStmtList;
    vector<CubeSem*> positionEquivalenceStmtList;
    vector<CubeSem*> operationStmtList;
    vector<CubeSem*> inverseStmtList;
    vector<CubeSem*> bindingStmtList;
    vector<CubeSem*> startStmtList;

    set<string> files;
    map<string, vector<int> > identifierCache;
    map<string, string> metadata;

    map<string, MeshObject*> meshObjects;
    map<string, TranslatorTag*> tags;
    map<string, TranslatorSymmetry*> symmetries;
    map<string, TranslatorGeometry*> geometries;
    map<string, TranslatorBlock*> blocks;
    map<string, TranslatorPosition*> positions;
    map<string, TranslatorOperation*> operations;
    map<string, TranslatorBinding*> bindings;

    vector<TranslatorTag*> tagList;
    vector<TranslatorBlock*> blockList;
    vector<TranslatorBlock*> aliasedBlockList;
    vector< vector<int> > blockEquivalenceList;
    vector< vector<int> > bandageList;
    vector<TranslatorPosition*> positionList;
    vector<TranslatorPosition*> aliasedPositionList;
    vector< vector<int> > positionEquivalenceList;
    vector<TranslatorOperation*> operationList;
    vector<int> startList;

    vector<Error*> errorList;

    Cube *cube;
    CubeSem *tree;
    string message;
    string m_path;

    int autoLevel, transformLevel, identLevel;

    typedef vector< pair< vector<int>, Eigen::Affine3d >, Eigen::aligned_allocator< pair< vector<int>, Eigen::Affine3d > > > AutoVectorType;
    typedef map< vector<int>, Eigen::Affine3d, less< vector<int> >, Eigen::aligned_allocator< pair< vector<int>, Eigen::Affine3d > > > AutoMapType;

    CubeSem *parseFile(string filename);
    MeshObject *loadMeshObject(string filename);
    void pass0(CubeSem *node);
    void processMetadataStmt();
    void processTagStmt();
    void symmetryPass(CubeSem *node, TranslatorSymmetry *symmetry);
    void processSymmetryStmt();
    void geometryPass(CubeSem *node, TranslatorGeometry *geometry);
    void processGeometryStmt();
    void pass1(CubeSem *node);
    void processBlockStmt();
    void processBlockRelationshipStmt();
    void processBandageStmt();
    void processPositionStmt();
    void processPositionRelationshipStmt();
    void operationPass(CubeSem *node, TranslatorOperation *operation);
    void processOperationStmt();
    void bindingPass(CubeSem *node, TranslatorBinding *binding);
    void processBindingStmt();
    void processStartStmt();
    const vector<int> &partitionIdentifier(string str);
    string makeIdentifier(string str, const vector<int> &permutation);
    AutoVectorType makeGroup(const vector<TranslatorSymmetry*> &generators);
    AutoVectorType makeAutoSet(CubeSem *autoStmt);
    CubeSem *autoGenerate(CubeSem *CubeSem, const vector<int> &permutation);
    void generateMessage();
    void cleanUp();
    Cube *getCube();

public:
    Cube *translate(string path, string filename);
    string getMessage();
};
