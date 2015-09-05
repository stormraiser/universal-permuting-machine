#include "CodeEdit.h"
#include <QTextStream>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>
#include <cstdio>
#include <QMessageBox>

CodeEdit::CodeEdit(int _id, int _newId, QWidget *parent): QTextEdit(parent) {
    m_id = _id;
    modified = false;
    unsavedNewFile = true;
    highlighter = new CubeDescriptionHighlighter(document());
    setFontFamily("monospace");
    m_filename = m_title = QString("New Descriptor %1").arg(_newId);
    connect(this, SIGNAL(textChanged()), this, SLOT(checkModified()));
}

CodeEdit::~CodeEdit() {
    delete highlighter;
}

bool CodeEdit::load(QString _filename) {
    QFile file(_filename);
    if (file.open(QFile::ReadOnly)) {
        QTextStream stream(&file);
        setText(stream.readAll());
        m_filename = _filename;
        m_title = QFileInfo(_filename).fileName();
        modified = false;
        unsavedNewFile = false;
        emit titleChanged(m_id, title());
        return true;
    }
    else {
        return false;
    }
}

bool CodeEdit::save() {
    if (unsavedNewFile) {
        m_filename = QFileDialog::getSaveFileName(this, "Save cube descriptor", QDir::current().absolutePath(), "Cube Descriptors (*.cub)");
        m_title = QFileInfo(m_filename).fileName();
    }
    QFile file(m_filename);
    if (file.open(QFile::WriteOnly)) {
        QTextStream stream(&file);
        stream << toPlainText();
        modified = false;
        unsavedNewFile = false;
        emit titleChanged(m_id, title());
        return true;
    }
    else {
        return false;
    }
}

int CodeEdit::id() {
    return m_id;
}

QString CodeEdit::title() {
    if (modified) {
        return QString("*") + m_title;
    }
    else {
        return m_title;
    }
}

void CodeEdit::checkModified() {
    if (!modified) {
        modified = true;
        emit titleChanged(m_id, title());
    }
}

QString CodeEdit::filename() {
    return m_filename;
}

bool CodeEdit::tryClose() {
    if (!modified) {
        return true;
    }
    QMessageBox msgBox;
    msgBox.setWindowTitle("Save file");
    msgBox.setText(QString("\"%1\" has been modified. Save changes?").arg(m_filename));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();
    switch (ret) {
      case QMessageBox::Save:
          save();
          return true;
      case QMessageBox::Discard:
          return true;
      case QMessageBox::Cancel:
          return false;
      default:
          return false;
    }
}
