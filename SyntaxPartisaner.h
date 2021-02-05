#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextDocument>

class SyntaxPartisaner : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    SyntaxPartisaner(QTextDocument *parent,QColor keywordColor,
    QColor classColor,QColor singleCommentColor,QColor multiCommentColor,
    QColor quotationColor,QColor funcColor);
    SyntaxPartisaner(QTextDocument *parent);
protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineKey;
    QTextCharFormat singleLineValue;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
    QTextCharFormat numbersFormat;
};