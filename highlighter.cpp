#include "highlighter.h"


Highlighter::Highlighter(QTextDocument *parent, QColor syntaxColor, QColor commentColor, QColor literalColor, QColor functionColor, keywordsSyntax syntaxType)
    : QSyntaxHighlighter(parent)
{
    enabled = true;
    this->syntaxColor = syntaxColor;
    HighlightingRule rule;

    keywordFormat.setForeground(syntaxColor);
    setKeywordData();
    if (syntaxType == keywordsSyntax::CXX20) setKeywordPatterns20();
    if (syntaxType == keywordsSyntax::CXX11) setKeywordPatterns11();
    if (syntaxType == keywordsSyntax::C2018) setKeywordPatterns_C2018();
    for (const QString &wordPattern : keywords)
    {
        rule.pattern = QRegularExpression(wordPattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }



    singleLineCommentFormat.setForeground(commentColor);
    rule.pattern = QRegularExpression(QStringLiteral("//[^\n]*"));
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(commentColor);


    quotationFormat.setForeground(literalColor);
    rule.pattern = QRegularExpression(QStringLiteral("\".*\""));
    rule.format = quotationFormat;
    highlightingRules.append(rule);


    functionFormat.setFontItalic(true);
    functionFormat.setForeground(functionColor);
    rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
    rule.format = functionFormat;
    highlightingRules.append(rule);


    commentStartExpression = QRegularExpression(QStringLiteral("/\\*"));
    commentEndExpression = QRegularExpression(QStringLiteral("\\*/"));
}

void Highlighter::setSyntaxColor(QColor newColor)
{
    this->syntaxColor = newColor;

}


void Highlighter::highlightBlock(const QString &text)
{
    if(enabled == false){return;}
    for (const HighlightingRule &rule : highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0)
    {

        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
