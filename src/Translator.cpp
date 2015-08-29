#include "Translator.h"
#include "DisjointSet.h"
#include "TransformationPrimitive.h"
#include <algorithm>
#include <cstdio>
#include <iostream>

using namespace std;

extern FILE *cubein;
extern CubeSem *cubelval;

int cubeparse();

CubeSem *treeTop = 0;

Cube *Translator::translate(string path, string filename) {
    cout << path << endl << filename << endl;
    tree = parseFile(string(path).append(filename));
    fileStack.push_back(filename);
    m_path = path;
    if (tree == 0) {
        return 0;
    }
    autoLevel = 0;
    transformLevel = 0;
    identLevel = 0;
    envStack.push_back(CubeSem::semTop);
    pass0(tree);
    if (!errorList.empty()) {
        generateMessage();
        cout << getMessage();
        cleanUp();
        return 0;
    }
    printf("pass 0 done.\n");
    processMetadataStmt();
    printf("metadata done.\n");
    processTagStmt();
    printf("tag done.\n");
    processSymmetryStmt();
    printf("symmetry done.\n");
    processGeometryStmt();
    printf("geometry done.\n");
    if (!errorList.empty()) {
        generateMessage();
        cout << getMessage();
        cleanUp();
        return 0;
    }
    pass1(tree);
    printf("pass 1 done.\n");
    processBlockStmt();
    printf("block done.\n");
    processBlockRelationshipStmt();
    printf("block relationship done.\n");
    processPositionStmt();
    printf("position done.\n");
    processPositionRelationshipStmt();
    printf("position relationship done.\n");
    processOperationStmt();
    printf("operation done.\n");
    processBindingStmt();
    printf("binding done.\n");
    processStartStmt();
    printf("start done.\n");

    if (!errorList.empty()) {
        generateMessage();
        cout << getMessage();
        cleanUp();
        return 0;
    }

    generateMessage();
    cout << getMessage();
    Cube *ret = getCube();
    cleanUp();
    printf("all done.\n");
    return ret;
}

CubeSem *Translator::parseFile(string filename) {
    CubeSem *ret = 0;
    if (files.find(filename) == files.end()) {
        files.insert(filename);
        cubein = fopen(string(m_path).append(filename).c_str(), "r");
        if (cubein == 0) {
            return 0;
        }
        printf("parsing file %s...\n", filename.c_str());
        if (cubeparse() == 0) {
            ret = treeTop;
            printf("success.\n");
        }
        else {
            //delete cubelval;
            printf("fail.\n");
        }
        treeTop = 0;
        fclose(cubein);
    }
    return ret;
}

void Translator::pass0(CubeSem *node) {
    if (node == 0) {
        return;
    }

    //
    //cout << string(identLevel * 4, ' ') << node->location.line << ' ' << node->location.column << ' ' << CubeSem::typeString(node->type) << endl;
    //identLevel++;

    switch (node->type) {
    case CubeSem::semBandageStmt:
    case CubeSem::semBlockAliasStmt:
    case CubeSem::semBlockEquivalenceStmt:
    case CubeSem::semBlockStmt:
    case CubeSem::semInverseStmt:
    case CubeSem::semPositionAliasStmt:
    case CubeSem::semPositionEquivalenceStmt:
    case CubeSem::semPositionStmt:
    case CubeSem::semRemoveBindingStmt:
    case CubeSem::semRemoveBlockStmt:
    case CubeSem::semRemoveOperationStmt:
    case CubeSem::semRemovePositionStmt:
    case CubeSem::semStartStmt:
        if (envStack.back() != CubeSem::semTop) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        break;
    case CubeSem::semBindingStmt:
    case CubeSem::semOperationStmt:
        if (envStack.back() != CubeSem::semTop) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        envStack.push_back(node->type);
        pass0(node->child);
        envStack.pop_back();
        break;
    case CubeSem::semGroupStmt:
        if (envStack.back() != CubeSem::semTop) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        pass0(node->child);
        break;
    case CubeSem::semForbidStmt:
    case CubeSem::semRequireStmt:
    case CubeSem::semShiftStmt:
        if (envStack.back() != CubeSem::semOperationStmt) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        break;
    case CubeSem::semIncludeStmt:
        if (envStack.back() != CubeSem::semTop) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        else {
            node->child = parseFile(node->string1);
            fileStack.push_back(node->string1);
            pass0(node->child);
            fileStack.pop_back();
        }
        break;
    case CubeSem::semInvokeStmt:
    case CubeSem::semKeyboardStmt:
    case CubeSem::semClickStmt:
        if (envStack.back() != CubeSem::semBindingStmt) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        break;
    case CubeSem::semModelStmt:
        if (envStack.back() != CubeSem::semGeometryStmt) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        break;
    case CubeSem::semCycleStmt:
        if ((envStack.back() != CubeSem::semOperationStmt) && (envStack.back() != CubeSem::semSymmetryStmt)) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        break;
    case CubeSem::semGeometryStmt:
    case CubeSem::semMetadataStmt:
    case CubeSem::semSymmetryStmt:
    case CubeSem::semTagStmt:
        if (envStack.back() != CubeSem::semTop) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        if (autoLevel != 0) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, CubeSem::semAutoStmt));
        }
        if (transformLevel != 0) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, CubeSem::semTransformStmt));
        }
        switch (node->type) {
        case CubeSem::semGeometryStmt:
            envStack.push_back(node->type);
            pass0(node->child);
            envStack.pop_back();
            geometryStmtList.push_back(node);
            break;
        case CubeSem::semMetadataStmt:
            metadataStmtList.push_back(node);
            break;
        case CubeSem::semSymmetryStmt:
            envStack.push_back(node->type);
            pass0(node->child);
            envStack.pop_back();
            symmetryStmtList.push_back(node);
            break;
        case CubeSem::semTagStmt:
            tagStmtList.push_back(node);
            break;
        default:
            break;
        }
        break;
    case CubeSem::semAutoStmt:
        if (envStack.back() != CubeSem::semTop) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        autoLevel++;
        pass0(node->child);
        autoLevel--;
        break;
    case CubeSem::semRotateStmt:
        if ((envStack.back() != CubeSem::semTop) && (envStack.back() != CubeSem::semGeometryStmt)) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        if ((node->numberList.size() != 3)) {
            errorList.push_back(new WrongNumberOfParametersError(node->location, node->type, node->numberList.size()));
        }
        transformLevel++;
        pass0(node->child);
        transformLevel--;
        break;
    case CubeSem::semAxisAngleStmt:
        if ((node->numberList.size() != 4) && (node->numberList.size() != 7)) {
            errorList.push_back(new WrongNumberOfParametersError(node->location, node->type, node->numberList.size()));
        }
        if (envStack.back() == CubeSem::semSymmetryStmt) {
            if (node->numberList.size() == 7) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
            }
        }
        else if ((envStack.back() != CubeSem::semTop) && (envStack.back() != CubeSem::semGeometryStmt) && (envStack.back() != CubeSem::semOperationStmt)) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        if (node->numberList.size() == 4) {
            node->numberList.insert(node->numberList.begin(), 3, 0);
        }
        if (Eigen::Vector3d(node->numberList[3] - node->numberList[0], node->numberList[4] - node->numberList[1], node->numberList[5] - node->numberList[2]).norm() < 1e-6) {
            errorList.push_back(new GenericError(node->location, "Rotation axis must have a non-zero length"));
        }
        transformLevel++;
        pass0(node->child);
        transformLevel--;
        break;
    case CubeSem::semTranslateStmt:
        if ((envStack.back() != CubeSem::semTop) && (envStack.back() != CubeSem::semGeometryStmt) && (envStack.back() != CubeSem::semOperationStmt)) {
            errorList.push_back(new MisplacedStatementError(node->location, node->type, envStack.back()));
        }
        if ((node->numberList.size() != 3)) {
            errorList.push_back(new WrongNumberOfParametersError(node->location, node->type, node->numberList.size()));
        }
        transformLevel++;
        pass0(node->child);
        transformLevel--;
        break;
    case CubeSem::semStmtBlock:
        for (CubeSem *p : node->childList) {
            pass0(p);
        }
        break;
    default:
        break;
    }

    //
    //identLevel--;
}

void Translator::processMetadataStmt() {
    string key, value;
    for (CubeSem *p : metadataStmtList) {
        for (CubeSem *q : p->childList) {
            auto r = metadata.find(q->string1);
            if (r != metadata.end()) {
                errorList.push_back(new GenericError(q->location, "Duplicating metadata item"));
            }
            else {
                metadata.insert(pair<string, string>(q->string1, q->string2));
            }
        }
    }
    metadataStmtList.clear();
}

void Translator::processTagStmt() {
    for (CubeSem *p : tagStmtList) {
        for (CubeSem *q : p->childList) {
            string tagName = q->string1;
            bool f = ((tagName[0] >= 'A') && (tagName[0] <= 'Z')) || ((tagName[0] >= 'a') && (tagName[0] <= 'z'));
            if (f) {
                for (int i = 1; i < tagName.size(); i++) {
                    if ((tagName[i] < 'a') || (tagName[i] > 'z')) {
                        f = false;
                        break;
                    }
                }
            }
            if (f) {
                bool isPrimary = (tagName[0] >= 'A') && (tagName[0] <= 'Z');
                auto r = tags.find(tagName);
                if (r == tags.end()) {
                    TranslatorTag *tag = new TranslatorTag;
                    tag->firstLocation = q->location;
                    tag->name = tagName;
                    tag->isPrimary = isPrimary;
                    if (q->numberList.size() == 0) {
                        tag->isColored = false;
                    }
                    else if (q->numberList.size() == 3) {
                        tag->isColored = true;
                        tag->color.resize(3);
                        tag->color[0] = q->numberList[0];
                        tag->color[1] = q->numberList[1];
                        tag->color[2] = q->numberList[2];
                    }
                    else {
                        errorList.push_back(new WrongNumberOfParametersError(q->location, q->type, q->numberList.size()));
                    }
                    tags.insert(pair<string, TranslatorTag*>(tagName, tag));
                }
                else {
                    TranslatorTag *tag = r->second;
                    if (tag->isColored) {
                        if (q->numberList.size() != 0) {
                            errorList.push_back(new GenericError(q->location, "Re-definition of a color tag"));
                        }
                    }
                    else {
                        if (q->numberList.size() == 3) {
                            tag->isColored = true;
                            tag->color.resize(3);
                            tag->color[0] = q->numberList[0];
                            tag->color[1] = q->numberList[1];
                            tag->color[2] = q->numberList[2];
                        }
                        else if (q->numberList.size() != 0) {
                            errorList.push_back(new WrongNumberOfParametersError(q->location, q->type, q->numberList.size()));
                        }
                    }
                }
            }
            else {
                errorList.push_back(new GenericError(q->location, "Invalid tag name"));
            }
        }
    }
    int k = 0;
    for (auto p : tags) {
        p.second->id = k;
        k++;
        tagList.push_back(p.second);
    }
    for (int i = 0; i < tags.size(); i++) {
        for (int j = 0; j < tags.size(); j++) {
            if ((i != j) && (tagList[i]->isPrimary) && (tagList[j]->isPrimary) && (tagList[i]->name.find(tagList[j]->name) == 0)) {
                errorList.push_back(new GenericError(tagList[j]->firstLocation, "One tag is a prefix of another tag"));
            }
        }
    }
    tagStmtList.clear();
    /*
    for (TranslatorTag *tag : tagList) {
        cout << tag->toString();
    }
    */
}

void Translator::symmetryPass(CubeSem *node, TranslatorSymmetry *symmetry) {
    if (node == 0) {
        return;
    }

    switch (node->type) {
    case CubeSem::semAxisAngleStmt:
        if (!symmetry->hasTransformation) {
            symmetry->hasTransformation = true;
            symmetry->transformation = Eigen::Affine3d::Identity();
        }
        symmetry->transformation = symmetry->transformation * TransformationPrimitive(node).toTransformation(1);
        symmetryPass(node->child, symmetry);
        break;
    case CubeSem::semStmtBlock:
        for (CubeSem *p : node->childList) {
            symmetryPass(p, symmetry);
        }
        break;
    case CubeSem::semCycleStmt:
        {
            vector<int> tmp;
            for (CubeSem *p : node->childList) {
                int n = p->stringList.size();
                tmp.assign(n, 0);
                bool f = true;
                for (int i = 0; i < n; i++) {
                    auto q = tags.find(p->stringList[i]);
                    if (q == tags.end()) {
                        errorList.push_back(new GenericError(node->location, "Unknown tag name"));
                        f = false;
                        break;
                    }
                    else if (!q->second->isPrimary) {
                        errorList.push_back(new GenericError(node->location, "Tag is not primary"));
                    }
                    else {
                        tmp[i] = q->second->id;
                    }
                }
                if (f) {
                    for (int i = 0; i < n; i++) {
                        if (symmetry->tagPermutation[tmp[i]] != -1) {
                            errorList.push_back(new GenericError(node->location, "Intersecting cycles"));
                        }
                        symmetry->tagPermutation[tmp[i]] = tmp[(i + 1) % n];
                    }
                }
            }
        }
        break;
    default:
        break;
    }
}

void Translator::processSymmetryStmt() {
    vector<int> tmp;
    for (CubeSem *p : symmetryStmtList) {
        auto q = symmetries.find(p->string1);
        TranslatorSymmetry *symmetry;
        if (q == symmetries.end()) {
            for (int i = 0; i < p->string1.size(); i++) {
                if ((p->string1[i] < 'a') || (p->string1[i] > 'z')) {
                    errorList.push_back(new GenericError(p->location, "Invalid symmetry name"));
                    break;
                }
            }
            symmetry = new TranslatorSymmetry;
            symmetry->name = p->string1;
            symmetry->tagPermutation.assign(tags.size(), -1);
            symmetry->hasTransformation = false;
            symmetries.insert(pair<string, TranslatorSymmetry*>(p->string1, symmetry));
        }
        else {
            symmetry = q->second;
        }
        symmetryPass(p->child, symmetry);
    }
    symmetryStmtList.clear();
    for (auto p : symmetries) {
        for (int i = 0; i < tags.size(); i++) {
            if (p.second->tagPermutation[i] == -1) {
                p.second->tagPermutation[i] = i;
            }
        }
    }
    /*
    for (auto p : symmetries) {
        cout << p.second->toString();
    }
    */
}

MeshObject *Translator::loadMeshObject(string filename) {
    auto p = meshObjects.find(filename);
    if (p == meshObjects.end()) {
        MeshObject *meshObject = new MeshObject;
        meshObject->load(string(m_path).append(filename));
        meshObjects.insert(pair<string, MeshObject*>(filename, meshObject));
        return meshObject;
    }
    else {
        return p->second;
    }
}

void Translator::geometryPass(CubeSem *node, TranslatorGeometry *geometry) {
    if (node == 0) {
        return;
    }

    switch (node->type) {
    case CubeSem::semRotateStmt:
    case CubeSem::semAxisAngleStmt:
    case CubeSem::semTranslateStmt:
        transformationStack.push_back(transformationStack.back() * TransformationPrimitive(node).toTransformation(1));
        geometryPass(node->child, geometry);
        transformationStack.pop_back();
        break;
    case CubeSem::semStmtBlock:
        for (CubeSem *p : node->childList) {
            geometryPass(p, geometry);
        }
        break;
    case CubeSem::semModelStmt:
        {
            MeshObject *mesh = loadMeshObject(node->string1);
            if (mesh == 0) {
                errorList.push_back(new GenericError(node->location, "Failed to load mesh file"));
            }
            else {
                geometry->meshList.push_back(mesh);
                geometry->transformationList.push_back(transformationStack.back());
            }
        }
        break;
    default:
        break;
    }
}

void Translator::processGeometryStmt() {
    transformationStack.clear();
    transformationStack.push_back(Eigen::Affine3d::Identity());
    for (CubeSem *p : geometryStmtList) {
        auto q = geometries.find(p->string1);
        if (q != geometries.end()) {
            errorList.push_back(new GenericError(p->location, "Duplicating geometry definitions"));
        }
        else {
            TranslatorGeometry *geometry = new TranslatorGeometry;
            geometry->name = p->string1;
            geometryPass(p->child, geometry);
            geometries.insert(pair<string, TranslatorGeometry*>(p->string1, geometry));
        }
    }
    geometryStmtList.clear();
    /*
    for (auto p : geometries) {
        cout << p.second->toString();
    }
    */
}

Translator::AutoVectorType Translator::makeGroup(const vector<TranslatorSymmetry*> &generators) {
    AutoVectorType ret;
    vector<int> identity;
    AutoMapType tmp;

    //
    //printf("making autogroup, number of generators = %d\n", generators.size());
    identity.resize(tags.size());
    for (int i = 0; i < tags.size(); i++) {
        identity[i] = i;
    }
    ret.push_back(pair< vector<int>, Eigen::Affine3d >(identity, Eigen::Affine3d::Identity()));
    tmp.insert(pair< vector<int>, Eigen::Affine3d >(identity, Eigen::Affine3d::Identity()));
    for (int i = 0; i < ret.size(); i++) {
        vector<int> seed = ret[i].first;
        Eigen::Affine3d seedTransformation = ret[i].second;
        for (TranslatorSymmetry* q : generators) {
            vector<int> prod;
            prod.resize(tags.size());
            for (int j = 0; j < tags.size(); j++) {
                prod[j] = q->tagPermutation[seed[j]];
            }
            if (tmp.find(prod) == tmp.end()) {
                tmp.insert(pair< vector<int>, Eigen::Affine3d >(prod, q->transformation * seedTransformation));
                ret.emplace_back(prod, q->transformation * seedTransformation);
            }
        }
    }
    return ret;
}

Translator::AutoVectorType Translator::makeAutoSet(CubeSem *autoStmt) {
    vector<TranslatorSymmetry*> generators;
    AutoMapType tmp;
    AutoVectorType ret;
    for (CubeSem *p : autoStmt->childList) {
        generators.clear();
        for (string q : p->stringList) {
            auto r = symmetries.find(q);
            //cout << q << endl;
            if (r == symmetries.end()) {
                errorList.push_back(new GenericError(p->location, "Unknown symmetry"));
            }
            else {
                generators.push_back(r->second);
            }
        }
        AutoVectorType group = makeGroup(generators);
        while (!group.empty()) {
            tmp.insert(group.back());
            group.pop_back();
        }
    }
    for (auto p : tmp) {
        ret.push_back(p);
    }
    return ret;
}

const vector<int> &Translator::partitionIdentifier(string str) {
    auto p = identifierCache.find(str);
    if (p == identifierCache.end()) {
        vector<int> parts;
        bool f;
        string str0 = str;
        //
        //cout << str << " ->";
        while (!str.empty()) {
            f = false;
            for (int i = 0; i < tags.size(); i++) {
                if ((tagList[i]->isPrimary) && (str.find(tagList[i]->name) == 0)) {
                    parts.push_back(TAG_CODE_BASE + i);
                    //cout << ' ' << tagList[i]->name;
                    str.erase(0, tagList[i]->name.size());
                    f = true;
                    break;
                }
            }
            if (!f) {
                parts.push_back(str[0]);
                //cout << ' ' << str[0];
                str.erase(0, 1);
            }
        }
        //cout << endl;
        p = identifierCache.insert(pair<string, vector<int> >(str0, parts)).first;
    }
    return p->second;
}

string Translator::makeIdentifier(string str, const vector<int> &permutation) {
    if (str.empty()) {
        return string();
    }
    string ret;
    const vector<int> &parts = partitionIdentifier(str);
    for (int i = 0; i < parts.size(); i++) {
        if (parts[i] < TAG_CODE_BASE) {
            ret.push_back(parts[i]);
            //cout << parts[i] << ' ';
        }
        else {
            ret.append(tagList[permutation[parts[i] - TAG_CODE_BASE]]->name);
            //cout << tagList[permutation[parts[i] - TAG_CODE_BASE]]->name << ' ';
        }
    }
    //cout << "-> " << ret << endl;
    //cout << "@ " << str << ' ' << ret << endl;
    return ret;
}

CubeSem *Translator::autoGenerate(CubeSem *node, const vector<int> &permutation) {
    if (node == 0) {
        return 0;
    }
    CubeSem *ret = new CubeSem(node->location, node->type);
    switch (node->type) {
    case CubeSem::semIncludeStmt:
    case CubeSem::semKeyboardStmt:
    case CubeSem::semModelStmt:
    case CubeSem::semClickStmt:
        ret->string1 = node->string1;
        ret->string2 = node->string2;
    default:
        ret->string1 = makeIdentifier(node->string1, permutation);
        ret->string2 = makeIdentifier(node->string2, permutation);
    }
    ret->number = node->number;
    ret->numberList = node->numberList;
    for (string s : node->stringList) {
        ret->stringList.push_back(makeIdentifier(s, permutation));
    }
    ret->child = autoGenerate(node->child, permutation);
    for (CubeSem *p : node->childList) {
        ret->childList.push_back(autoGenerate(p, permutation));
    }
    return ret;
}

void Translator::pass1(CubeSem *node) {
    if (node == 0) {
        return;
    }
    node->transformation = transformationStack.back();
    switch (node->type) {
    case CubeSem::semAutoStmt:
        {
            CubeSem *generated = new CubeSem(node->location, CubeSem::semStmtBlock);
            AutoVectorType autoSet = makeAutoSet(node);
            //
            //printf("auto expansion at (%d, %d), size of sutoset = %d\n", node->location.line, node->location.column, autoSet.size());
            for (auto p : autoSet) {
                CubeSem *tmp = autoGenerate(node->child, p.first);
                generated->childList.push_back(tmp);
                transformationStack.push_back(transformationStack.back() * p.second);
                pass1(tmp);
                transformationStack.pop_back();
            }
            generated->transformation = transformationStack.back();
            node->child = generated;
        }
        break;
    case CubeSem::semRotateStmt:
    case CubeSem::semAxisAngleStmt:
    case CubeSem::semTranslateStmt:
        transformationStack.push_back(transformationStack.back() * TransformationPrimitive(node).toTransformation(1));
        pass1(node->child);
        transformationStack.pop_back();
        break;
    case CubeSem::semBandageStmt:
        bandageStmtList.push_back(node);
        break;
    case CubeSem::semBindingStmt:
        bindingStmtList.push_back(node);
        pass1(node->child);
        break;
    case CubeSem::semRemoveBindingStmt:
        bindingStmtList.push_back(node);
        break;
    case CubeSem::semBlockAliasStmt:
        blockAliasStmtList.push_back(node);
        break;
    case CubeSem::semBlockEquivalenceStmt:
        blockEquivalenceStmtList.push_back(node);
        break;
    case CubeSem::semBlockStmt:
        blockStmtList.push_back(node);
        for (CubeSem *p : node->childList) {
            pass1(p);
        }
        break;
    case CubeSem::semRemoveBlockStmt:
        blockStmtList.push_back(node);
        break;
    case CubeSem::semGroupStmt:
        pass1(node->child);
        break;
    case CubeSem::semInverseStmt:
        inverseStmtList.push_back(node);
        break;
    case CubeSem::semOperationStmt:
    case CubeSem::semRemoveOperationStmt:
        operationStmtList.push_back(node);
        break;
    case CubeSem::semPositionAliasStmt:
        positionAliasStmtList.push_back(node);
        break;
    case CubeSem::semPositionEquivalenceStmt:
        positionEquivalenceStmtList.push_back(node);
        break;
    case CubeSem::semPositionStmt:
    case CubeSem::semRemovePositionStmt:
        positionStmtList.push_back(node);
        break;
    case CubeSem::semStartStmt:
        startStmtList.push_back(node);
        break;
    case CubeSem::semStmtBlock:
        for (CubeSem *p : node->childList) {
            pass1(p);
        }
        break;
    default:
        break;
    }
}

void Translator::processBlockStmt() {
    for (CubeSem *p : blockStmtList) {
        if (p->type == CubeSem::semBlockStmt) {
            for (CubeSem *q : p->childList) {
                auto r = blocks.find(q->string1);
                if (q->type == CubeSem::semIdentifier) {
                    if (r == blocks.end()) {
                        TranslatorBlock *block = new TranslatorBlock;
                        block->name = q->string1;
                        block->geometry = 0;
                        blocks.insert(pair<string, TranslatorBlock*>(q->string1, block));
                    }
                }
                else {
                    auto s = geometries.find(q->string2);
                    TranslatorBlock *block = 0;
                    if (s == geometries.end()) {
                        errorList.push_back(new GenericError(q->location, "Unknown geometry"));
                    }
                    else {
                        TranslatorGeometry *geometry = s->second;
                        CubeSem *tuple = q->child;
                        if (geometry->meshList.size() != tuple->stringList.size()) {
                            errorList.push_back(new GenericError(q->location, "Number of tags specified when calling a geometry does not match that in the geometry definition"));
                        }
                        else {
                            block = new TranslatorBlock;
                            block->name = q->string1;
                            block->geometry = geometry;
                            bool f = true;
                            for (int i = 0; i < tuple->stringList.size(); i++) {
                                auto u = tags.find(tuple->stringList[i]);
                                if (u == tags.end()) {
                                    errorList.push_back(new GenericError(q->location, "Unknown tag"));
                                    f = false;
                                    delete block;
                                    block = 0;
                                    break;
                                }
                                else if (!u->second->isColored) {
                                    errorList.push_back(new GenericError(q->location, "Tag does not have a color"));
                                }
                                else {
                                    block->tagList.push_back(u->second->id);
                                }
                            }
                        }
                    }
                    if (block != 0) {
                        block->transformation = p->transformation;
                        if (r == blocks.end()) {
                            blocks.insert(pair<string, TranslatorBlock*>(q->string1, block));
                        }
                        else if (r->second->geometry == 0) {
                            delete r->second;
                            blocks.erase(r);
                            blocks.insert(pair<string, TranslatorBlock*>(q->string1, block));
                        }
                        else {
                            if (r->second->geometry != block->geometry) {
                                errorList.push_back(new GenericError(q->location, "Conflicting block definitions"));
                            }
                            else if (r->second->tagList != block->tagList) {
                                errorList.push_back(new GenericError(q->location, "Conflicting block definitions"));
                            }
                            delete block;
                        }
                    }
                }
            }
        }
        else {
            for (CubeSem *q : p->childList) {
                auto r = blocks.find(q->string1);
                if (r == blocks.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown block"));
                }
                else {
                    TranslatorBlock *block = r->second;
                    blocks.erase(r);
                    delete block;
                }
            }
        }
    }
    blockStmtList.clear();
    int k = 0;
    for (auto p : blocks) {
        blockList.push_back(p.second);
        p.second->id = k;
        k++;
    }
}

void Translator::processBlockRelationshipStmt() {
    DisjointSet blockAliasSet(blockList.size());
    vector<int> tmpBlockId;
    for (CubeSem *p : blockAliasStmtList) {
        for (CubeSem *q : p->childList) {
            tmpBlockId.clear();
            for (string r : q->stringList) {
                auto s = blocks.find(r);
                if (s == blocks.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown block"));
                }
                else {
                    tmpBlockId.push_back(s->second->id);
                }
            }
            for (int i = 1; i < tmpBlockId.size(); i++) {
                blockAliasSet.join(tmpBlockId[0], tmpBlockId[i]);
            }
        }
    }
    int n = 0;
    bool f;
    for (int i = 0; i < blockList.size(); i++) {
        if (blockAliasSet.find(i) == i) {
            int k = -1;
            for (int j = 0; j < blockList.size(); j++) {
                if ((blockAliasSet.find(j) == i) && (blockList[j]->geometry != 0)) {
                    k = j;
                    break;
                }
            }
            if (k == -1) {
                k = i;
            }
            for (int j = 0; j < blockList.size(); j++) {
                if (blockAliasSet.find(j) == i) {
                    blockList[j]->alias = k;
                    blockList[j]->aliasedId = n;
                }
            }
            aliasedBlockList.push_back(blockList[k]);
            n++;
        }
    }
    blockAliasStmtList.clear();

    DisjointSet blockEquivalenceSet(n);
    for (CubeSem *p : blockEquivalenceStmtList) {
        for (CubeSem *q : p->childList) {
            tmpBlockId.clear();
            for (string r : q->stringList) {
                auto s = blocks.find(r);
                if (s == blocks.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown block"));
                }
                else {
                    tmpBlockId.push_back(blockList[s->second->alias]->aliasedId);
                }
            }
            for (int i = 1; i < tmpBlockId.size(); i++) {
                blockEquivalenceSet.join(tmpBlockId[0], tmpBlockId[i]);
            }
        }
    }
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (blockEquivalenceSet.find(i) == i) {
            vector<int> tmpEquivalence;
            for (int j = 0; j < n; j++) {
                if (blockEquivalenceSet.find(j) == i) {
                    aliasedBlockList[j]->equivalenceGroup = m;
                    tmpEquivalence.push_back(j);
                }
            }
            blockEquivalenceList.push_back(tmpEquivalence);
            m++;
        }
    }
    blockEquivalenceStmtList.clear();

    DisjointSet bandageSet(n);
    for (CubeSem *p : bandageStmtList) {
        for (CubeSem *q : p->childList) {
            tmpBlockId.clear();
            for (string r : q->stringList) {
                auto s = blocks.find(r);
                if (s == blocks.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown block"));
                }
                else {
                    tmpBlockId.push_back(blockList[s->second->alias]->aliasedId);
                }
            }
            for (int i = 1; i < tmpBlockId.size(); i++) {
                bandageSet.join(tmpBlockId[0], tmpBlockId[i]);
            }
        }
    }
    m = 0;
    for (int i = 0; i < n; i++) {
        if (bandageSet.find(i) == i) {
            vector<int> tmpBandage;
            for (int j = 0; j < n; j++) {
                if (bandageSet.find(j) == i) {
                    aliasedBlockList[j]->bandageGroup = m;
                    tmpBandage.push_back(j);
                }
            }
            bandageList.push_back(tmpBandage);
            m++;
        }
    }
    bandageStmtList.clear();
}

void Translator::processPositionStmt() {
    for (CubeSem *p : positionStmtList) {
        if (p->type == CubeSem::semPositionStmt) {
            for (CubeSem *q : p->childList) {
                for (string r : q->stringList) {
                    auto s = positions.find(r);
                    if (s == positions.end()) {
                        TranslatorPosition *position = new TranslatorPosition;
                        position->name = r;
                        positions.insert(pair<string, TranslatorPosition*>(r, position));
                    }
                }
            }
        }
        else {
            for (CubeSem *q : p->childList) {
                auto r = positions.find(q->string1);
                if (r == positions.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown position"));
                }
                else {
                    TranslatorPosition *position = r->second;
                    positions.erase(r);
                    delete position;
                }
            }
        }
    }
    positionStmtList.clear();
    int k = 0;
    for (auto p : positions) {
        positionList.push_back(p.second);
        p.second->id = k;
        k++;
    }
}

void Translator::processPositionRelationshipStmt() {
    DisjointSet positionAliasSet(positionList.size());
    vector<int> tmpPositionId;
    for (CubeSem *p : positionAliasStmtList) {
        for (CubeSem *q : p->childList) {
            tmpPositionId.clear();
            for (string r : q->stringList) {
                auto s = positions.find(r);
                if (s == positions.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown position"));
                }
                else {
                    tmpPositionId.push_back(s->second->id);
                }
            }
            for (int i = 1; i < tmpPositionId.size(); i++) {
                positionAliasSet.join(tmpPositionId[0], tmpPositionId[i]);
            }
        }
    }
    int n = 0;
    bool f;
    for (int i = 0; i < positionList.size(); i++) {
        if (positionAliasSet.find(i) == i) {
            for (int j = 0; j < positionList.size(); j++) {
                if (positionAliasSet.find(j) == i) {
                    positionList[j]->alias = i;
                    positionList[j]->aliasedId = n;
                }
            }
            aliasedPositionList.push_back(positionList[i]);
            n++;
        }
    }
    positionAliasStmtList.clear();

    DisjointSet positionEquivalenceSet(n);
    for (CubeSem *p : positionEquivalenceStmtList) {
        for (CubeSem *q : p->childList) {
            tmpPositionId.clear();
            for (string r : q->stringList) {
                auto s = positions.find(r);
                if (s == positions.end()) {
                    errorList.push_back(new GenericError(q->location, "Unknown block"));
                }
                else {
                    tmpPositionId.push_back(positionList[s->second->alias]->aliasedId);
                }
            }
            for (int i = 1; i < tmpPositionId.size(); i++) {
                positionEquivalenceSet.join(tmpPositionId[0], tmpPositionId[i]);
            }
        }
    }
    int m = 0;
    for (int i = 0; i < n; i++) {
        if (positionEquivalenceSet.find(i) == i) {
            vector<int> tmpEquivalence;
            for (int j = 0; j < n; j++) {
                if (positionEquivalenceSet.find(j) == i) {
                    aliasedPositionList[j]->equivalenceGroup = m;
                    tmpEquivalence.push_back(j);
                }
            }
            positionEquivalenceList.push_back(tmpEquivalence);
            m++;
        }
    }
    positionEquivalenceStmtList.clear();
}

void Translator::operationPass(CubeSem *node, TranslatorOperation *operation) {
    if (node == 0) {
        return;
    }
    vector<int> tmp;
    int k;

    switch (node->type) {
    case CubeSem::semRotateStmt:
    case CubeSem::semAxisAngleStmt:
    case CubeSem::semTranslateStmt:
        {
            TransformationPrimitive tmp(node);
            tmp.transform(transformationStack.back());
            operationTransformationStack.push_back(tmp);
        }
        operationPass(node->child, operation);
        operationTransformationStack.pop_back();
        break;
    case CubeSem::semStmtBlock:
        for (CubeSem *p : node->childList) {
            operationPass(p, operation);
        }
        break;
    case CubeSem::semCycleStmt:
    case CubeSem::semShiftStmt:
        k = -1;
        for (int i = 0; i < operation->transformationList.size(); i++) {
            if (operation->transformationList[i] == operationTransformationStack) {
                k = i;
                break;
            }
        }
        if (k == -1) {
            k = operation->transformationList.size();
            operation->transformationList.push_back(operationTransformationStack);
        }
        for (CubeSem *p : node->childList) {
            tmp.clear();
            for (string q : p->stringList) {
                auto r = positions.find(q);
                if (r == positions.end()) {
                    errorList.push_back(new GenericError(p->location, "Unknown position"));
                }
                else {
                    tmp.push_back(r->second->aliasedId);
                }
            }
            int n = tmp.size();
            int m = node->type == CubeSem::semCycleStmt ? n : n - 1;
            for (int i = 0; i < m; i++) {
                if ((operation->positionPermutation[tmp[i]] != -1) && (operation->positionPermutation[tmp[i]] != tmp[(i + 1) % n])) {
                    errorList.push_back(new GenericError(p->location, "Conflicting position permutation"));
                }
                else if ((operation->inversePermutation[tmp[(i + 1) % n]] != -1) && (operation->inversePermutation[tmp[(i + 1) % n]] != tmp[i])) {
                    errorList.push_back(new GenericError(p->location, "Conflicting position permutation"));
                }
                else {
                    operation->positionPermutation[tmp[i]] = tmp[(i + 1) % n];
                    operation->inversePermutation[tmp[(i + 1) % n]] = tmp[i];
                    operation->transformationId[tmp[i]] = k;
                }
            }
        }
        break;
    case CubeSem::semForbidStmt:
        for (CubeSem *p : node->childList) {
            for (string q : p->stringList) {
                auto r = positions.find(q);
                if (r == positions.end()) {
                    errorList.push_back(new GenericError(p->location, "Unknown position"));
                }
                else {
                    if (operation->positionPermutation[r->second->aliasedId] >= 0) {
                        errorList.push_back(new GenericError(p->location, "Conflicting position permutation"));
                    }
                    operation->positionPermutation[r->second->aliasedId] = -2;
                    operation->inversePermutation[r->second->aliasedId] = -2;
                }
            }
        }
    default:
        break;
    }
}

void Translator::processOperationStmt() {
    for (CubeSem *p : operationStmtList) {
        if (p->type == CubeSem::semOperationStmt) {
            TranslatorOperation *operation;
            auto q = operations.find(p->string1);
            if (q == operations.end()) {
                operation = new TranslatorOperation;
                operation->name = p->string1;
                operation->positionPermutation.assign(aliasedPositionList.size(), -1);
                operation->inversePermutation.assign(aliasedPositionList.size(), -1);
                operation->transformationId.assign(aliasedPositionList.size(), -1);
                operations.insert(pair<string, TranslatorOperation*>(p->string1, operation));
            }
            else {
                operation = q->second;
            }
            operationTransformationStack.clear();
            transformationStack.clear();
            transformationStack.push_back(p->transformation);
            operationPass(p->child, operation);
        }
        else {
            auto r = operations.find(p->string1);
            if (r == operations.end()) {
                errorList.push_back(new GenericError(p->location, "Unknown operation"));
            }
            else {
                TranslatorOperation *operation = r->second;
                operations.erase(r);
                delete operation;
            }
        }
    }
    operationStmtList.clear();

    for (auto p : operations) {
        for (int i = 0; i < aliasedPositionList.size(); i++) {
            if ((p.second->positionPermutation[i] >= 0) && (p.second->inversePermutation[i] < 0)) {
                p.second->inversePermutation[i] = -2;
            }
            else if ((p.second->inversePermutation[i] >= 0) && (p.second->positionPermutation[i] < 0)) {
                p.second->positionPermutation[i] = -2;
            }
        }
    }
    for (CubeSem *p : inverseStmtList) {
        auto q = operations.find(p->string1);
        auto r = operations.find(p->string2);
        if (r == operations.end()) {
            errorList.push_back(new GenericError(p->location, "Unknown operation"));
        }
        else if (q == operations.end()) {
            TranslatorOperation *operation = new TranslatorOperation;
            operation->name = p->string1;
            operation->inverse = p->string2;
            operation->positionPermutation = r->second->inversePermutation;
            operation->transformationId.assign(aliasedPositionList.size(), -1);
            for (int i = 0; i < aliasedPositionList.size(); i++) {
                if (operation->positionPermutation[i] >= 0) {
                    operation->transformationId[i] = r->second->transformationId[operation->positionPermutation[i]];
                }
            }
            for (vector<TransformationPrimitive> &t : r->second->transformationList) {
                vector<TransformationPrimitive> tl;
                Eigen::Affine3d ts = Eigen::Affine3d::Identity();
                for (TransformationPrimitive &tp : t) {
                    TransformationPrimitive inv = tp.inversed();
                    inv.transform(ts);
                    tl.push_back(inv);
                    ts = ts * tp.toTransformation();
                }
                operation->transformationList.push_back(tl);
            }
            operations.insert(pair<string, TranslatorOperation*>(p->string1, operation));
        }
        else if (q->second->inverse != p->string2) {
            errorList.push_back(new GenericError(p->location, "Conflicting inverse statements"));
        }
    }
    for (auto p : operations) {
        for (int i = 0; i < aliasedPositionList.size(); i++) {
            if (p.second->positionPermutation[i] == -1) {
                p.second->positionPermutation[i] = i;
            }
        }
        p.second->activeTransformationList.resize(p.second->transformationList.size());
    }
    inverseStmtList.clear();
    int k = 0;
    for (auto p : operations) {
        p.second->id = k;
        k++;
        operationList.push_back(p.second);
    }
}

void Translator::bindingPass(CubeSem *node, TranslatorBinding *binding) {
    if (node == 0) {
        return;
    }
    vector< vector<int> > opSequence;
    vector<int> tmp;
    vector<bool> positionCheck;
    vector<bool> positionFlag;
    vector<int> positionPermutation;
    positionPermutation.resize(aliasedPositionList.size());

    switch (node->type) {
    case CubeSem::semRotateStmt:
    case CubeSem::semAxisAngleStmt:
    case CubeSem::semTranslateStmt:
        transformationStack.push_back(transformationStack.back() * TransformationPrimitive(node).toTransformation(1));
        bindingPass(node->child, binding);
        transformationStack.pop_back();
        break;
    case CubeSem::semStmtBlock:
        for (CubeSem *p : node->childList) {
            bindingPass(p, binding);
        }
        break;
    case CubeSem::semClickStmt:
        {
            MeshObject *mesh = loadMeshObject(node->string2);
            if (mesh == 0) {
                errorList.push_back(new GenericError(node->location, "Failed to load mesh file"));
            }
            else if (binding->selector == 0) {
                binding->transformation = transformationStack.back();
                binding->selector = mesh;
            }
            break;
        }
    case CubeSem::semInvokeStmt:
        {
            opSequence.clear();
            for (int i = 0; i < aliasedPositionList.size(); i++) {
                positionPermutation[i] = i;
            }
            positionFlag.assign(aliasedPositionList.size(), false);
            for (CubeSem *p : node->childList) {
                tmp.clear();
                positionCheck.assign(aliasedPositionList.size(), false);
                for (string s : p->stringList) {
                    auto q = operations.find(s);
                    if (q == operations.end()) {
                        errorList.push_back(new GenericError(p->location, "Unknown operation"));
                    }
                    else {
                        for (int i = 0; i < aliasedPositionList.size(); i++) {
                            if (q->second->transformationId[i] != -1) {
                                if (positionCheck[i]) {
                                    errorList.push_back(new GenericError(p->location, "Conflicting operations"));
                                }
                                positionCheck[i] = true;
                                positionFlag[i] = true;
                            }
                            if (positionPermutation[i] != -2) {
                                positionPermutation[i] = q->second->positionPermutation[positionPermutation[i]];
                            }
                        }
                        tmp.push_back(q->second->id);
                    }
                }
                opSequence.push_back(tmp);
            }
            bool f = false;
            for (auto p : binding->invokeList) {
                if (p == opSequence) {
                    f = true;
                    break;
                }
            }
            if (!f) {
                binding->invokeList.push_back(opSequence);
                binding->positionPermutations.push_back(positionPermutation);
                binding->positionFlags.push_back(positionFlag);
            }
        }
        break;
    default:
        break;
    }
}

void Translator::processBindingStmt() {
    for (CubeSem *p : bindingStmtList) {
        TranslatorBinding *binding;
        auto q = bindings.find(p->string1);
        if (q == bindings.end()) {
            binding = new TranslatorBinding;
            binding->name = p->string1;
            binding->selector = 0;
            bindings.insert(pair<string, TranslatorBinding*>(p->string1, binding));
        }
        else {
            binding = q->second;
        }
        transformationStack.clear();
        transformationStack.push_back(p->transformation);
        bindingPass(p->child, binding);
    }
}

void Translator::processStartStmt() {
    startList.assign(blockList.size(), -1);
    for (CubeSem *p : startStmtList) {
        for (CubeSem *q : p->childList) {
            auto r = blocks.find(q->string1);
            auto s = positions.find(q->string2);
            if (r == blocks.end()) {
                errorList.push_back(new GenericError(q->location, "Unknown block"));
            }
            if (s == positions.end()) {
                errorList.push_back(new GenericError(q->location, "Unknown position"));
            }
            if ((r != blocks.end()) && (s != positions.end())) {
                int a = r->second->id;
                int b = s->second->id;
                if ((startList[a] != -1) && (startList[a] != b)) {
                    errorList.push_back(new GenericError(q->location, "Conflicting start positions"));
                }
                startList[a] = b;
            }
        }
    }
    startStmtList.clear();
    for (int i = 0; i < startList.size(); i++) {
        if ((blockList[i]->alias == i) && (startList[i] == -1)) {
            auto s = positions.find(blockList[i]->name);
            if (s == positions.end()) {
                errorList.push_back(new GenericError(Location(0, 0), "Starting position not specified"));
            }
            int k = s->second->id;
            startList[i] = k;
        }
    }
}

void Translator::cleanUp() {
    fileStack.clear();
    envStack.clear();
    transformationStack.clear();
    operationTransformationStack.clear();
    files.clear();
    identifierCache.clear();
    metadata.clear();
    for (auto p : meshObjects) {
        delete p.second;
    }
    for (auto p : tags) {
        delete p.second;
    }
    for (auto p : symmetries) {
        delete p.second;
    }
    for (auto p : geometries) {
        delete p.second;
    }
    for (auto p : blocks) {
        delete p.second;
    }
    for (auto p : positions) {
        delete p.second;
    }
    for (auto p : operations) {
        delete p.second;
    }
    meshObjects.clear();
    tags.clear();
    symmetries.clear();
    geometries.clear();
    blocks.clear();
    positions.clear();
    operations.clear();
    tagList.clear();
    blockList.clear();
    aliasedBlockList.clear();
    blockEquivalenceList.clear();
    bandageList.clear();
    positionList.clear();
    aliasedPositionList.clear();
    positionEquivalenceList.clear();
    startList.clear();
    for (Error *error : errorList) {
        delete error;
    }
    errorList.clear();
    CubeSem::clear();
}

void Translator::generateMessage() {
    stringstream s;
    if (!errorList.empty()) {
        for (Error *error : errorList) {
            s << error->toString() << endl;
        }
    }
    else {
        s << tagList.size() << " tags" << endl;
        /*
        for (TranslatorTag *tag : tagList) {
            s << tag->toString();
        }
        */
        s << meshObjects.size() << " mesh objects" << endl;
        s << symmetries.size() << " symmetries" << endl;
        /*
        for (auto p : symmetries) {
            s << p.second->toString();
        }
        */
        s << geometries.size() << " geometries" << endl;
        /*
        for (auto p : geometries) {
            s << p.second->toString();
        }
        */
        s << aliasedBlockList.size() << " blocks (" << blockList.size() << " aliases)" << endl;
        /*
        for (TranslatorBlock *block : blockList) {
            s << block->toString();
        }
        */
        s << aliasedPositionList.size() << " positions (" <<  positionList.size() << " aliases)" << endl;
        /*
        for (TranslatorPosition *position : positionList) {
            s << position->toString();
        }
        */
        s << operations.size() << " operations" << endl;
        /*
        for (auto p : operations) {
            s << p.second->toString();
        }
        */
        s << bindings.size() << " bindings" << endl;
        /*
        for (auto p : bindings) {
            s << p.second->toString();
        }
        */
    }
    message = s.str();
}

string Translator::getMessage() {
    return message;
}

Cube *Translator::getCube() {
    Cube *cube = new Cube;
    cube->activeBinding = -1;
    for (TranslatorBlock *block : aliasedBlockList) {
        CubeBlock *cubeBlock = new CubeBlock;
        cubeBlock->geometry = block->geometry;
        cubeBlock->colorList.resize(block->tagList.size());
        for (int i = 0; i < block->tagList.size(); i++) {
            cubeBlock->colorList[i].resize(3);
            for (int j = 0; j < 3; j++) {
                cubeBlock->colorList[i][j] = tagList[block->tagList[i]]->color[j];
            }
        }
        cubeBlock->startTransformation = block->transformation;
        cubeBlock->startPosition = positionList[startList[block->id]]->aliasedId;
        printf("%d %s %d %s\n", block->id, block->name.c_str(), positionList[startList[block->id]]->aliasedId, positionList[positionList[startList[block->id]]->alias]->name.c_str());
        cubeBlock->isSelected = false;
        cube->blocks.push_back(cubeBlock);
    }
    for (auto p : meshObjects) {
        cube->meshObjects.push_back(p.second);
    }
    meshObjects.clear();
    for (auto p : geometries) {
        cube->geometries.push_back(p.second);
    }
    geometries.clear();
    cube->operations.swap(operationList);
    operations.clear();
    int k = 0;
    for (auto p : bindings) {
        p.second->id = k;
        cube->bindings.push_back(p.second);
        cube->bindingMap.insert(pair<string, TranslatorBinding*>(p.second->name, p.second));
        k++;
    }
    bindings.clear();
    return cube;
}
