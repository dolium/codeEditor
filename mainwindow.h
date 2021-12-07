
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
#include <QFontDialog>
#include <QFont>
#include "statusbar.h"
#include "aboutdialog.h"
#include <QMap>
#include <QDirIterator>
#include <QFontMetrics>


class Editor : public QPlainTextEdit
{
    Q_OBJECT
public:
    Editor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
private:
    void contextMenuEvent(QContextMenuEvent *event) override;
    QTextOption::WrapMode currentWrapMode;

    QWidget *lineNumberArea;

    FindDialog *findDialog;
    FindDialog *replaceDialog;

    QFont currentFont;

    QColor backgroundColor = Qt::gray; //stored here to use in updateBackgroundColor method.
    //Copy stored in MainWindow too for use with QColorPicker dialog
    QColor textColor = Qt::black;

    QString textToFind;
    QString textToReplace;

    int countFind;

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
    void selectLine();
    void selectWord();
    friend class MainWindow;
};



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
private:
    Editor* mainEditor;
    QToolBar *fileToolBar;
    Highlighter *highlighter;
    statusBarFooter *status;

    StyleCreatorDialog *styleDialog;
    StyleSwitchDialog *styleSwitchDialog;
    QColorDialog* colorPicker;


    QActionGroup *styleGroup;
    QActionGroup *languageGroup;
    QAction *hideNumBar;
    QAction *hideToolBar;
    QAction *hideStatusBar;
    QAction *setWrappingAct;
    QAction *hideHighlightingAct;
    QVector<QAction*> stylesButtons;



    QSettings* currentThemeSettings;
    QSettings* availableThemes;
    QSettings* currentSettings;


    QString styleName;
    QString currentFileName;
    QString currentThemeName;
    QString currentThemePath;
    QString currentFilePath;
    QString rowColumnPosition;

    QVector<QString> themesNames;
    QVector<QString> themesPaths;
    QVector<QString> themeNamesPaths;//Vector of pairs name of style : path to this style. Ordered sequentially name1 path1 name2 path2

    QLabel *rowColumnLabel;
    QLabel *lastModificationTime;

    QColor commentColor;
    QColor syntaxColor;
    QColor literalColor;
    QColor functionColor;
    QColor backgroundColor;
    QColor currentBlockColor;

    void createMenu();
    void setCurrentFileName(const QString &fileName);
    void createNewTheme(QString name = "Theme_1", QColor syntaxColor = Qt::magenta, QColor commentColor = Qt::darkCyan, QColor literalColor = Qt::darkRed, QColor functionColor = Qt::red, QColor backgroundColor = Qt::gray, QColor textColor = Qt::white);
    void setThemeSettings(QString themePath);
    void initializeThemes();
    void findThemes();
    void deleteThemes(QVector<QString> paths);
    void applySettings();
    void initSettings();
public slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void setBackgroungColor();
    void exit();
    void changeBlockColor();
    void enableToolBar();
    void enableStatusBar();
    void enableNumBar();
    void enableHighlighting();
    void showThemeCreator();
    void showStyleSwitch();
    void updateTheme();
    void updateRowColumn();
    void updateModificationTime();
    void updateStatusBar();
    void applyCheckedTheme();
    void setKeywordsCXX2020();
    void setKeywordsCXX2011();
    void setKeywordsC2018();
    void openNewTheme();
    void setTextFont();
    void setTextWrapping();

};
//![extraarea]

#endif
