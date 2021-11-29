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
    QVector<QPushButton *>deleteButtons;
    QVector<QString>themesToDelete;
private:
    QGroupBox *gridGroupBox;
    QPlainTextEdit *smallEditor;
    QVector<QString> names;
    QVector<QString> paths;
    QVector<QRadioButton *>radioButtons;
    QPushButton *acceptButton;
    void createGridGroupBox();
    void createPathAndNames(QVector<QString>);
    QString currentPath;
    QString currentName;
public slots:
    void setCurrentData();
    void addThemesToDelete();

};
#endif // STYLESWITCHDIALOG_H
