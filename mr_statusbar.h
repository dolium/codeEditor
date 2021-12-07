#ifndef MR_STATUSBAR_H
#define MR_STATUSBAR_H

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
class mr_statusBar : public QWidget
{
    Q_OBJECT
public:
    mr_statusBar(QMainWindow *parent, QPlainTextEdit* edit);
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


    void updateSizeInfo(QString path);
public slots:
    void updateRowColumn();
    void updateModificationTime();
    void updateCountInfo();
private:
    void addAllWidgets();
    void removeAllWidgets();

};

#endif // MR_STATUSBAR_H
