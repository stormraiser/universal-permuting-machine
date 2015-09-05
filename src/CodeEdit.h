#pragma once

#include <QTextEdit>
#include "CubeDescriptionHighlighter.h"

class CodeEdit: public QTextEdit {

    Q_OBJECT

private:
    bool modified;
    bool unsavedNewFile;
    QString m_filename;
    QString m_title;
    CubeDescriptionHighlighter *highlighter;
    int m_id;

public:
    CodeEdit(int _id, int _newId = 0, QWidget *parent = 0);
    ~CodeEdit();
    bool load(QString _filename);
    bool save();
    bool tryClose();
    int id();
    QString title();
    QString filename();

signals:
    void titleChanged(int _id, QString title);

private slots:
    void checkModified();
};
