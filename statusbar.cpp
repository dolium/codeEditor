#include "statusbar.h"

statusBarFooter::statusBarFooter(QMainWindow *parent, QPlainTextEdit* edit) : QWidget(parent)
{
    wordsCount = 0;
    characterCount = 0;
    linesCount = 0;
    this->edit = edit;
    mainWindow = parent;

    layout = new QHBoxLayout(this);



    rowColumnPosition = "Row/column position: "+QString::number(edit->textCursor().blockNumber())+" | " + QString::number(edit->textCursor().positionInBlock());
    rowColumnLabel = new QLabel(rowColumnPosition, this);
    rowColumnLabel->setVisible(true);
    layout->addWidget(rowColumnLabel);


    lastModificationTime = new QLabel("| Time of last modification " + QTime::currentTime().toString(), this);
    lastModificationTime->setVisible(true);
    layout->addWidget(lastModificationTime);

    countInfo = new QLabel("| words count: " + QString::number(wordsCount)+" Lines: "+QString::number(linesCount) + " Characters: " + QString::number(characterCount));
    countInfo->setVisible(true);
    layout->addWidget(countInfo);

    sizeInfo = new QLabel("");
    layout->addWidget(sizeInfo);
    setLayout(layout);
}


void statusBarFooter::updateCountInfo()
{

    auto wordsAndTrash = edit->document()->toPlainText().split(QRegularExpression("\\s+"));
    if (wordsCount!=wordsAndTrash.length()-1)
        emit wordCountChanged(wordsCount);
    wordsCount = wordsAndTrash.length()-1;
    characterCount = edit->document()->characterCount();
    linesCount = edit->document()->blockCount();

    removeAllWidgets();
    delete countInfo;
    countInfo = new QLabel("| words count: " + QString::number(wordsCount)+" Lines: "+QString::number(linesCount) + " Characters: " + QString::number(characterCount));
    addAllWidgets();
}


void statusBarFooter::addAllWidgets()
{
    layout->addWidget(rowColumnLabel);
    layout->addWidget(lastModificationTime);
    layout->addWidget(countInfo);
    layout->addWidget(sizeInfo);
}


void statusBarFooter::removeAllWidgets()
{
    layout->removeWidget(rowColumnLabel);
    layout->removeWidget(lastModificationTime);
    layout->removeWidget(countInfo);
    layout->removeWidget(sizeInfo);
}


void statusBarFooter::updateSizeInfo(QString path)
{
    QFileInfo fileInfo{path};
    fileSize = fileInfo.size();


    removeAllWidgets();
    delete sizeInfo;
    sizeInfo = new QLabel("| file size: "+QString::number(fileSize)+" bytes");
    addAllWidgets();
}


void statusBarFooter::updateRowColumn()
{
    rowColumnPosition = "Row/column position: "+QString::number(edit->textCursor().blockNumber())+" | " + QString::number(edit->textCursor().positionInBlock());
    removeAllWidgets();
    delete rowColumnLabel;
    rowColumnLabel = new QLabel(rowColumnPosition, this);
    addAllWidgets();
}


void statusBarFooter::updateModificationTime()
{
    removeAllWidgets();
    delete lastModificationTime;
    lastModificationTime = new QLabel("| Time of last modification " + QTime::currentTime().toString(), this);
    addAllWidgets();
}
