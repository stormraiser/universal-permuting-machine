#include "Cube.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

void Cube::checkBindingValidity() {
    vector<int> currentPositions;
    currentPositions.resize(blocks.size());
    vector<int> currentBlocks;
    for (TranslatorBinding *binding : bindings) {
        binding->validIndex = -1;
        for (int i = 0; i < blocks.size(); i++) {
            currentPositions[i] = blocks[i]->currentPosition;
        }
        for (int i = 0; i < binding->invokeList.size(); i++) {
            bool f = true;
            for (vector<int> vec : binding->invokeList[i]) {
                for (int j : vec) {
                    currentBlocks.assign(positionCount, -1);
                    for (int k = 0; k < blocks.size(); k++)
                        currentBlocks[currentPositions[k]] = k;
                    // check for forbidden positions
                    for (vector<int> vec2 : operations[j]->forbiddenList) {
                        bool ft = true;
                        for (int k : vec2) {
                            if (currentBlocks[k] == -1) {
                                ft = false;
                                break;
                            }
                        }
                        if (ft) {
                            f = false;
                            break;
                        }
                    }
                    if (!f) {
                        break;
                    }
                    // check for required positions
                    for (vector<int> vec2 : operations[j]->requiredList) {
                        bool ft = false;
                        for (int k : vec2) {
                            if (currentBlocks[k] != -1) {
                                ft = true;
                                break;
                            }
                        }
                        if (!ft) {
                            f = false;
                            break;
                        }
                    }
                    if (!f) {
                        break;
                    }
                    // check for bandage violations
                    for (vector<int> vec2 : bandages) {
                        for (int k = 1; k < vec2.size(); k++) {
                            if (operations[j]->transformationId[currentPositions[vec2[0]]] != operations[j]->transformationId[currentPositions[vec2[k]]]) {
                                f = false;
                                break;
                            }
                        }
                        if (!f) {
                            break;
                        }
                    }
                    if (!f) {
                        break;
                    }
                    for (int k = 0; k < blocks.size(); k++) {
                        currentPositions[k] = operations[j]->positionPermutation[currentPositions[k]];
                    }
                }
                if (!f) {
                    break;
                }
            }
            if (f) {
                binding->validIndex = i;
                break;
            }
        }
    }
}

void Cube::partialOperate(int operationId, double t) {
    TranslatorOperation *operation = operations[operationId];
    operation->generateTransformation(t);
    for (CubeBlock *block : blocks) {
        if (operation->transformationId[block->currentPosition] != -1) {
            block->partialTransform(operation->activeTransformationList[operation->transformationId[block->currentPosition]]);
        }
    }
}

void Cube::operate(int operationId) {
    TranslatorOperation *operation = operations[operationId];
    operation->generateTransformation();
    for (CubeBlock *block : blocks) {
        if (operation->transformationId[block->currentPosition] != -1) {
            block->transform(operation->activeTransformationList[operation->transformationId[block->currentPosition]]);
            block->currentPosition = operation->positionPermutation[block->currentPosition];
        }
    }
    checkBindingValidity();
}

void Cube::operate() {
    if (activeBinding != -1) {
        for (vector<int> vec : bindings[activeBinding]->invokeList[bindings[activeBinding]->validIndex]) {
            for (int k : vec) {
                operate(k);
            }
        }
    }
}

void Cube::prepareDraw() {
    for (MeshObject *meshObject : meshObjects) {
        meshObject->makeDisplayList();
    }
    glGetIntegerv(GL_RED_BITS, &redBits);
    glGetIntegerv(GL_GREEN_BITS, &greenBits);
    glGetIntegerv(GL_BLUE_BITS, &blueBits);
    uintBits = sizeof(GLuint) * 8;
}

void Cube::colorEncode(int k, GLuint &red, GLuint &green, GLuint &blue) {
    k++;
    red = (k % (1 << redBits)) << (uintBits - redBits);
    k >>= redBits;
    green = (k % (1 << greenBits)) << (uintBits - greenBits);
    k >>= greenBits;
    blue = (k % (1 << blueBits)) << (uintBits - blueBits);
}

int Cube::colorDecode(GLuint *color) {
    int ret = 0;
    ret = ret | (color[2] >> (uintBits - blueBits));
    ret = (ret << greenBits) | (color[1] >> (uintBits - greenBits));
    ret = (ret << redBits) | (color[0] >> (uintBits - redBits));
    ret--;
    return ret;
}

void Cube::deleteDisplayList() {
    for (MeshObject *meshObject : meshObjects) {
        meshObject->deleteDisplayList();
    }
}

void Cube::draw() {
    for (CubeBlock *block : blocks) {
        block->draw();
    }
}

void Cube::drawSelection() {
    GLuint red, green, blue;
    for (int i = 0; i < bindings.size(); i++) {
        if ((bindings[i]->selector != 0) && (bindings[i]->validIndex != -1)) {
            colorEncode(i, red, green, blue);
            glColor3ui(red, green, blue);
            bindings[i]->draw();
        }
    }
}

void Cube::setSelection(GLuint *color) {
    int k = colorDecode(color);
    if ((k >= 0) && (k < bindings.size())) {
        setActiveBinding(k);
    }
    else {
        setActiveBinding(-1);
    }
}

void Cube::reset() {
    for (CubeBlock *block : blocks) {
        block->reset();
    }
    checkBindingValidity();
}

bool Cube::setActiveBinding(int k) {
    if (k == activeBinding) {
        return false;
    }
    activeBinding = k;
    if (k == -1) {
        for (CubeBlock *block : blocks) {
            block->isSelected = false;
        }
    }
    else {
        for (CubeBlock *block : blocks) {
            block->isSelected = bindings[k]->positionFlags[bindings[k]->validIndex][block->currentPosition];
        }
    }
    return true;
}

int Cube::setActiveBinding(string str) {
    auto p = bindingMap.find(str);
    if (p == bindingMap.end()) {
        return -1;
    }
    if (!setActiveBinding(p->second->id)) {
        return -2;
    }
    return 0;
}

void Cube::scramble(int step) {
    for (int i = 0; i < step; i++) {
        int k;
        do {
            k = rand() % bindings.size();
        } while (bindings[k]->validIndex == -1);
        setActiveBinding(k);
        operate();
    }
}

int Cube::prepareAnimation() {
    if (activeBinding == -1) {
        return -1;
    }
    else {
        return prepareAnimation(activeBinding);
    }
}

int Cube::prepareAnimation(int index) {
    animationStage = 0;
    animationBinding = index;
    if (bindings[index]->validIndex == -1) {
        return -1;
    }
    return bindings[index]->invokeList[bindings[index]->validIndex].size();
}

void Cube::doAnimation(double t) {
    for (; animationStage < t - 1; animationStage++) {
        for (int k : bindings[animationBinding]->invokeList[bindings[animationBinding]->validIndex][animationStage]) {
            operate(k);
        }
    }
    t -= animationStage;
    for (int k : bindings[animationBinding]->invokeList[bindings[animationBinding]->validIndex][animationStage]) {
        partialOperate(k, t);
    }
}

void Cube::doneAnimation() {
    for (; animationStage < bindings[animationBinding]->invokeList[bindings[animationBinding]->validIndex].size(); animationStage++) {
        for (int k : bindings[animationBinding]->invokeList[bindings[animationBinding]->validIndex][animationStage]) {
            operate(k);
        }
    }
}
