#include "CubeDescriptionHighlighter.h"

CubeDescriptionHighlighter::CubeDescriptionHighlighter(QTextDocument *parent): QSyntaxHighlighter(parent) {
    QStringList keywordPatterns;
    keywordPatterns << "\\balias\\b"
                    << "\\bbandage\\b"
                    << "\\bbinding\\b"
                    << "\\bblock\\b"
                    << "\\bclick\\b"
                    << "\\bcycle\\b"
                    << "\\bequivalence\\b"
                    << "\\bforbid\\b"
                    << "\\bgeometry\\b"
                    << "\\bgroup\\b"
                    << "\\binclude\\b"
                    << "\\binverse\\b"
                    << "\\binvoke\\b"
                    << "\\bkeyboard\\b"
                    << "\\bmetadata\\b"
                    << "\\bmodel\\b"
                    << "\\boperation\\b"
                    << "\\bposition\\b"
                    << "\\bremove\\b"
                    << "\\brequire\\b"
                    << "\\bshift\\b"
                    << "\\bstart\\b"
                    << "\\btag\\b";
    QStringList transformPatterns;
    transformPatterns << "\\baxisangle\\b"
                      << "\\brotate\\b"
                      << "\\btranslate\\b";
    QStringList autoPatterns;
    autoPatterns << "\\bsymmetry\\b"
                 << "\\bauto\\b";
    QRegExp numberRegExp("-?[0-9][1-9]*(\\.[0-9]*)?");
    QRegExp identifierRegExp("[_A-Za-z][_0-9A-Za-z+@#$%&/\\-']*");
    QRegExp stringRegExp("\"[^\"]*\"");
    QRegExp commentRegExp("//[^\n]*");

    QTextCharFormat keywordFormat;
    keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setForeground(Qt::darkBlue);
    QTextCharFormat transformFormat;
    transformFormat.setFontWeight(QFont::Bold);
    transformFormat.setForeground(Qt::darkRed);
    QTextCharFormat autoFormat;
    autoFormat.setFontWeight(QFont::Bold);
    autoFormat.setForeground(Qt::darkGreen);
    QTextCharFormat numberFormat;
    numberFormat.setForeground(Qt::darkMagenta);
    QTextCharFormat identifierFormat;
    identifierFormat.setForeground(Qt::black);
    QTextCharFormat stringFormat;
    stringFormat.setForeground(Qt::blue);
    QTextCharFormat commentFormat;
    commentFormat.setFontItalic(true);
    commentFormat.setForeground(Qt::gray);

    rules.emplace_back(numberRegExp, numberFormat);
    rules.emplace_back(identifierRegExp, identifierFormat);
    for (QString pattern : keywordPatterns) {
        rules.emplace_back(QRegExp(pattern), keywordFormat);
    }
    for (QString pattern : transformPatterns) {
        rules.emplace_back(QRegExp(pattern), transformFormat);
    }
    for (QString pattern : autoPatterns) {
        rules.emplace_back(QRegExp(pattern), autoFormat);
    }
    rules.emplace_back(stringRegExp, stringFormat);
    rules.emplace_back(commentRegExp, commentFormat);
}

void CubeDescriptionHighlighter::highlightBlock(const QString &text)
{
    for (auto p : rules) {
        int index = p.first.indexIn(text);
        while (index >= 0) {
            int length = p.first.matchedLength();
            setFormat(index, length, p.second);
            index = p.first.indexIn(text, index + length);
        }
    }
}
