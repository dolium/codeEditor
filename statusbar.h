#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>
#include <QRegularExpression>
#include <QString>
#include <QMainWindow>
#include <QWidget>
#include <QTime>
#include <QPlainTextEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileInfo>
class statusBarFooter : public QWidget
{
    Q_OBJECT
public:
    QLabel *rowColumnLabel;
    QLabel *lastModificationTime;
    int wordsCharacterCount;
    int linesCount;
    QLabel *countInfo;
    QLabel *sizeInfo;
    QString rowColumnPosition;
    QPlainTextEdit* edit;
    QHBoxLayout *layout;
    QMainWindow *mainWindow;
    int fileSize;
    int wordsCount;
    int characterCount;

public:
    statusBarFooter(QMainWindow *parent, QPlainTextEdit* edit);
    void updateSizeInfo(QString path);
public slots:
    void updateRowColumn();
    void updateModificationTime();
    void updateCountInfo();
 signals:
    void wordCountChanged(int wordCount);
private:
    void addAllWidgets();
    void removeAllWidgets();

};

#endif // STATUSBAR_H
