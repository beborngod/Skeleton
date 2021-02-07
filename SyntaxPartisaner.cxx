#include "SyntaxPartisaner.h"

SyntaxPartisaner::SyntaxPartisaner(QTextDocument *parent,QColor keywordColor,
    QColor classColor,QColor singleCommentColor,QColor multiCommentColor,
    QColor quotationColor,QColor funcColor): QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(keywordColor);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" <<"\\bwhile\\b"
                    << "\\bfor\\b" <<"\\bif\\b" <<"\\bor\\b" <<"\\band\\b"
                    << "\\belse\\b" << "\\bbool\\b" << "\\btry\\b" << "\\bcatch\\b"
                    << "\\bthrow\\b" << "\\bassert\\b" << "\\btrue\\b" << "\\bfalse\\b";

    for(const QString &pattern: keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(classColor);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(singleCommentColor);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(multiCommentColor);

    quotationFormat.setForeground(quotationColor);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(funcColor);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

SyntaxPartisaner::SyntaxPartisaner(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QStringList keywordPatterns;

    if(not keywordPatterns.isEmpty()){
        for(const QString &pattern: keywordPatterns) {
            rule.pattern = QRegExp(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void SyntaxPartisaner::highlightBlock(const QString &text)
{
    for(const HighlightingRule &rule: highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while(index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if(previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);


    while(startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if(endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }else{
            commentLength = endIndex - startIndex + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}