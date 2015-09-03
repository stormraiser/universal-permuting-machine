#pragma once

#include <QSyntaxHighlighter>
#include <vector>
#include <utility>

using namespace std;

class CubeDescriptionHighlighter : public QSyntaxHighlighter {

    Q_OBJECT

private:
    vector< pair<QRegExp, QTextCharFormat> > rules;

public:
    CubeDescriptionHighlighter(QTextDocument *parent);
    void highlightBlock(const QString &text);
};
