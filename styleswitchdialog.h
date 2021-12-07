#ifndef STYLESWITCHDIALOG_H
#define STYLESWITCHDIALOG_H

#include "highlighter.h"
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QRadioButton>
#include <QSettings>
class StyleSwitchDialog : public QDialog
{
public:
    StyleSwitchDialog(QWidget* parent, QVector<QString> pathAndNames);


    QString getPath()const{return currentPath;}
    QString getName()const{return currentName;}
    QVector<QString> getStyleNames()const {return names;}
    QVector<QString> getStylePaths()const{return paths;}
    QVector<QString> getDeleteThemes()const{return themesToDelete;}



private:
    QGroupBox *gridGroupBox;
    QPlainTextEdit *smallEditor;
    QColorDialog *colorDialog;

    QVector<QString> names;
    QVector<QString> paths;

    QVector<QLabel*> labels;
    QVector<QRadioButton *>radioButtons;
    QVector<QString>themesToDelete;
    QVector<QPushButton*> pickButton;
    QVector<QPushButton *>deleteButtons;

    QPushButton *acceptButton;
    QPushButton *acceptChanges;

    QString currentPath;
    QString currentName;
    QSettings* theme;

    Highlighter *highlighter;
    QColor syntaxColor    ;
    QColor commentColor   ;
    QColor literalColor   ;
    QColor functionColor  ;
    QColor backgroundColor;
    QColor textColor      ;


    void createGridGroupBox();
    void createPathAndNames(QVector<QString>);
    void setThemeSettings(QString path);
    void updateTheme();
    void updateBackgroundColor();
    void updateTextColor();
    void showPossibleChanges();


public slots:
    void setCurrentData();
    void addThemesToDelete();
    void pickSyntaxColor();
    void pickCommentColor();
    void pickLiteralColor();
    void pickFunctionColor();
    void pickBackgroundColor();
    void pickTextColor();
    void reformTheme();
};
#endif
