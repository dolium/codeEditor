
#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QMenuBar>
#include <QToolBar>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QColorDialog>
#include <QMainWindow>
#include <QPainter>
#include <QTextBlock>
#include <QDebug>
#include <QPalette>
#include <QFileDialog>
#include <QMessageBox>
#include <QGuiApplication>
#include <QSaveFile>
#include <QStatusBar>
#include <QWidget>
#include <QCheckBox>
#include <QDir>
#include <QSettings>
#include "findDialog.h"

#include "highlighter.h"
#include "stylecreatordialog.h"
#include "styleswitchdialog.h"
#include <QMetaType>
#include <QCoreApplication>
#include <QVariant>
#include <QTime>
#include <QActionGroup>
#include "mr_statusbar.h"
QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;

QT_END_NAMESPACE

class LineNumberArea;

//![codeeditordefinition]



class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);
    FindDialog *findDialog;
    FindDialog *replaceDialog;
    QWidget *lineNumberArea;
    QColor backgroundColor = Qt::gray;
    QColor textColor = Qt::black;
    QString textToFind;
    QString textToReplace;
    int countFind;
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    void updateBackgroundColor();
    void updateTextColor();
    void setTextColor(QColor color);
    void findAndHighlight();
    void findAndHighlightR();
    void findAndHighlightForFindInFindAndReplace();
protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);
    void replace();
    void replaceAll();
    void showFindDialog();
    void showReplaceDialog();


friend class MainWindow;
};

//![codeeditordefinition]
//![extraarea]

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(Editor *editor) : QWidget(editor), codeEditor(editor)
    {}



protected:
    void paintEvent(QPaintEvent *event) override
    {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    Editor *codeEditor;
};


class MainWindow:public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    QActionGroup *styleGroup;
    Highlighter *highlighter;
    mr_statusBar *status;
    StyleCreatorDialog *styleDialog;
    StyleSwitchDialog *styleSwitchDialog;
    Editor* mr_Editor;
    QToolBar *fileToolBar;
    QAction *hideNumBar;
    QAction *hideToolBar;
    QAction *hideStatusBar;
    QColorDialog* colorPicker;
    QString currentFileName;
    QSettings* current_settings;
    QSettings* availableThemes;
    QColor currentBackgroundColor;
    QColor currentBlockColor;
    QString styleName;
    QVector<QAction*> stylesButtons;
    QVector<QString> themesPaths; //Vector of pairs name of style : path to this style
    QString currentThemeName;
    QString currentThemePath;
    QString currentFilePath;
    QString rowColumnPosition;
    QLabel *rowColumnLabel;
    QLabel *lastModificationTime;
    QColor commentColor;
    QColor syntaxColor;
    QColor literalColor;
    QColor functionColor;
    QColor backgroundColor;
    QVector<QString> themesNames;
    QVector<QString> themesPathsReally;
    void createMenu();
    void setCurrentFileName(const QString &fileName);
    void createNewTheme(QString name = "Theme_1", QColor syntaxColor = Qt::magenta, QColor commentColor = Qt::darkCyan, QColor literalColor = Qt::darkRed, QColor functionColor = Qt::red, QColor backgroundColor = Qt::gray, QColor textColor = Qt::black);
    void setThemeSettings(QString themePath);
    void initializeThemes();
    void findThemes();
    void deleteThemes(QVector<QString> paths);
public slots:
    void newFile(){}
    void open();
    bool save();
    bool saveAs();
    void about(){}
    void documentWasModified();
    void setBackgroungColor();
    void exit();
    void changeBlockColor();
    void enableToolBar();
    void enableStatusBar();
    void enableNumBar();
    void showThemeCreator();
    void showStyleSwitch();
    void updateTheme();
    void updateRowColumn();
    void updateModificationTime();
    void updateStatusBar();
    void upplyCheckedTheme();

};
//![extraarea]

#endif