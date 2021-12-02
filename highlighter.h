#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

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
    Highlighter(QTextDocument *parent = nullptr, QColor syntaxColor=Qt::darkBlue, QColor commentColor = Qt::cyan, QColor literalColor = Qt::magenta, QColor functionColor = Qt::yellow, keywordsSyntax keywordsType = keywordsSyntax::CXX20);
    QColor syntaxColor;
    void setSyntaxColor(QColor newColor);
    bool enabled;

protected:
    void highlightBlock(const QString &text) override;
    void setKeywords();

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
    void setKeywordPatterns11(){keywords = keywordPatterns11;}
    void setKeywordPatterns20(){keywords = keywordPatterns20;}
    void setKeywordPatterns_C2018(){keywords = keywordPatterns_C2018;}
};
//! [0]

#endif // HIGHLIGHTER_H
