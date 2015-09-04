#include "CodeEdit.h"
#include <QTextStream>
#include <QFile>

CodeEdit::CodeEdit(QWidget *parent): QTextEdit(parent) {
    modified = false;
    unsavedNewFile = true;
    highlighter = new CubeDescriptionHighlighter(document());
    setFontFamily("monospace");
}

CodeEdit::~CodeEdit() {
    delete highlighter;
}

bool CodeEdit::load(QString _filename) {
    QFile file(_filename);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        setText(stream.readAll());
        filename = _filename;
        return true;
    }
    else {
        return false;
    }
}

bool CodeEdit::save() {
    if (filename.size() == 0) {
        return false;
    }
    QFile file(filename);
    if (file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        stream << toPlainText();
        return true;
    }
    else {
        return false;
    }
}

bool CodeEdit::save(QString _filename) {
    QFile file(_filename);
    if (file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        stream << toPlainText();
        filename = _filename;
        return true;
    }
    else {
        return false;
    }
}

bool CodeEdit::isUnsavedNewFile() {
    return unsavedNewFile;
}
