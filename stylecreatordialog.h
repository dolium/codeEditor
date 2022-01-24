#ifndef STYLECREATORDIALOG_H

#define STYLECREATORDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QGroupBox>
#include <QTextEdit>
#include "highlighter.h"
#include "QColorDialog"

class StyleCreatorDialog : public QDialog
{
    Q_OBJECT
public:

    StyleCreatorDialog(QWidget *parent = nullptr);
    QColor getSyntaxColor(){return syntaxColor;}
    QColor getCommentColor(){return commentColor;}
    QColor getLiteralColor(){return literalColor;}
    QColor getFunctionColor(){return functionColor;}
    QColor getBackgroundColor(){return backgroundColor;}
    QColor getTextColor(){return textColor;}
    QString getStyleName(){return name;}
private:
    QGroupBox *gridGroupBox;
    QColorDialog *colorDialog;
    QTextEdit *smallEditor;
    QVector<QLabel*> labels;
    QVector<QPushButton *>pickButton;
    QPushButton *acceptButton;
    QLabel* justLabel;
    QLineEdit *styleNameEdit;
    Highlighter* highlighter;

    QColor syntaxColor = Qt::magenta;
    QColor commentColor = Qt::darkCyan;
    QColor literalColor = Qt::blue;
    QColor functionColor = Qt::darkRed;
    QColor backgroundColor = Qt::gray;
    QColor textColor = Qt::black;

    QString name = "unnamed";


private:
    void createGridGroupBox();
    void showPossibleChanges();
    void updateTheme();
    void updateTextColor();
    void updateBackgroundColor();

public slots:

    void pickSyntaxColor();
    void pickCommentColor();
    void pickLiteralColor();
    void pickFunctionColor();
    void pickBackgroundColor();
    void pickTextColor();
    void setStyleName();
};

#endif // STYLECREATORDIALOG_H




