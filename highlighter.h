#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <QCompleter>
QT_BEGIN_NAMESPACE
class QTextDocument;

QT_END_NAMESPACE

//! [0]
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    enum keywordsSyntax
    {
        CXX11,
        CXX20,
        C2018
    };
    QColor syntaxColor;
    bool isEnabled;
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QList<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QVector<QString> keywordPatterns11;
    QVector<QString> keywordPatterns20;
    QVector<QString> keywordPatterns_C2018;
    QVector <QString> keywords;

public:
    Highlighter(QTextDocument *parent = nullptr, QColor syntaxColor=Qt::darkBlue, QColor commentColor = Qt::cyan, QColor literalColor = Qt::magenta, QColor functionColor = Qt::yellow, keywordsSyntax keywordsType = keywordsSyntax::CXX20);
    void setSyntaxColor(QColor newColor);
    void highlightBlock(const QString &text) override;


private:
    void setKeywordData();
    inline void setKeywordPatterns11(){keywords = keywordPatterns11;}
    inline void setKeywordPatterns20(){keywords = keywordPatterns20;}
    inline void setKeywordPatterns_C2018(){keywords = keywordPatterns_C2018;}
};
//! [0]

#endif // HIGHLIGHTER_H

