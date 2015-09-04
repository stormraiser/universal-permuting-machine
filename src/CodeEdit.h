#pragma once

#include <QTextEdit>
#include "CubeDescriptionHighlighter.h"

class CodeEdit: public QTextEdit {

    Q_OBJECT

private:
    bool modified;
    bool unsavedNewFile;
    QString filename;
    CubeDescriptionHighlighter *highlighter;

public:
    CodeEdit(QWidget *parent = 0);
    ~CodeEdit();
    bool load(QString _filename);
    bool save();
    bool save(QString _filename);
    bool isUnsavedNewFile();
};
