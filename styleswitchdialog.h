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
    QString getPath(){return currentPath;}
    QString getName(){return currentName;}
    QVector<QString> getStyleNames(){return names;}
    QVector<QString> getStylePaths(){return paths;}
    QVector<QString> getDeleteThemes(){return themesToDelete;}
    Highlighter *highlighter;
    QColor syntaxColor    ;
    QColor commentColor   ;
    QColor literalColor   ;
    QColor functionColor  ;
    QColor backgroundColor;
    QColor textColor;
    void setThemeSettings(QString path);
    void updateTheme();
    void updateBackgroundColor();
    void updateTextColor();
    void showPossibleChanges();
    QVector<QPushButton *>deleteButtons;
    QVector<QString>themesToDelete;
private:
    QGroupBox *gridGroupBox;
    QPlainTextEdit *smallEditor;
    QVector<QString> names;
    QVector<QString> paths;
    QVector<QRadioButton *>radioButtons;
    QVector<QLabel*> labels;
    QVector<QPushButton*> pickButton;
    QPushButton *acceptButton;
    QPushButton *acceptChanges;
    void createGridGroupBox();
    void createPathAndNames(QVector<QString>);
    QString currentPath;
    QString currentName;
    QColorDialog *colorDialog;
    QSettings* theme;

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
#endif // STYLESWITCHDIALOG_H
