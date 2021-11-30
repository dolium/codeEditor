#include "mainwindow.h"
#include "highlighter.h"
#include <QMap>
#include <QDirIterator>
#include <QFontMetrics>
//![constructor]
//After cancelling choice of backgroundColor, backColor changes to black. Needs to be fixed


MainWindow::MainWindow()
{


    mr_Editor = new Editor{};
    setCentralWidget(mr_Editor);


    currentBackgroundColor = mr_Editor->backgroundColor;
    backgroundColor = mr_Editor->backgroundColor;
    currentBlockColor = QColor(currentBackgroundColor).lighter(130);
//Replace next code with initializeThemes()
    availableThemes = new QSettings(QDir::currentPath()+QString{"/themes2.ini"}, QSettings::IniFormat);
    themesPaths = availableThemes->value("themesPaths").value<QVector<QString>>();
    findThemes();

    status = new mr_statusBar{this, mr_Editor};
    statusBar()->addWidget(status);

    createMenu();
    syntaxColor = Qt::darkBlue;
    highlighter =  new Highlighter(mr_Editor->document(), syntaxColor);
    colorPicker = new QColorDialog({0,1,1}, this);
    connect(mr_Editor->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);
    setUnifiedTitleAndToolBarOnMac(true);
    setCurrentFileName("untitled.cpp");






    /*current_settings = new QSettings(QDir::currentPath() + "/current_style.ini", QSettings::IniFormat);
    current_settings->setValue("size", QVariant(size()));
    current_settings->setValue("toolBarDisabled", QVariant(true));
    current_settings->sync();
    QSettings* settings2 = new QSettings(QDir::currentPath() + "/my_config_file.ini", QSettings::IniFormat);

    if (settings2->value("toolBarDisabled").toBool()) mr_Editor->lineNumberArea->hide();
    qDebug()<<QDir::currentPath(); //WORKS!!!*/

    //set Default theme and syntax:
       upplyCheckedTheme();
       setKeywordsCXX2020();
}

void MainWindow::showThemeCreator()
{
    styleDialog->show();
    if (styleDialog->exec()==QDialog::Accepted)
    {
       syntaxColor     =styleDialog->getSyntaxColor();
       commentColor    =styleDialog->getCommentColor();
       literalColor    =styleDialog->getLiteralColor();
       functionColor   =styleDialog->getFunctionColor();
       backgroundColor =styleDialog->getBackgroundColor();
       QColor tempTextColor = styleDialog->getTextColor();
       mr_Editor->backgroundColor = backgroundColor;
       styleName = styleDialog->getStyleName();
       createNewTheme(styleName, syntaxColor, commentColor, literalColor, functionColor, backgroundColor, tempTextColor);
    }
}

void MainWindow::showStyleSwitch()
{
    styleSwitchDialog->show();
    if (styleSwitchDialog->exec()==QDialog::Accepted)
    {
        if (currentThemeName!=styleSwitchDialog->getName())
        {
            currentThemeName = styleSwitchDialog->getName();
            currentThemePath = styleSwitchDialog->getPath();
            setThemeSettings(currentThemePath);
        }
        deleteThemes(styleSwitchDialog->getDeleteThemes());
    }
}

void MainWindow::setThemeSettings(QString themePath)
{

    QSettings* theme = new QSettings(themePath, QSettings::IniFormat);
    syntaxColor = theme->value("syntaxColor").value<QColor>();
    commentColor = theme->value("commentColor").value<QColor>();
    literalColor = theme->value("literalColor").value<QColor>();
    functionColor = theme->value("functionColor").value<QColor>();
    backgroundColor = theme->value("backgroundColor").value<QColor>();
    mr_Editor->backgroundColor = backgroundColor;
    mr_Editor->setTextColor(theme->value("textColor").value<QColor>());
    mr_Editor->updateTextColor();
    QFileInfo info{themePath};
    //If we do not have this theme - add it to themes list;
    if (!themesPaths.contains(themePath) and (info.baseName()!=QString("default")))
    {

        themesPaths.append(info.baseName());
        themesPaths.append(info.absoluteFilePath());
        availableThemes->setValue("themesPaths", QVariant(themesPaths));
    }
    updateTheme();
}

void MainWindow::initializeThemes()
{
    availableThemes = new QSettings(QDir::currentPath()+QString{"/themes2.ini"}, QSettings::IniFormat);
    themesPaths = availableThemes->value("themesPaths").value<QVector<QString>>();
    findThemes();
}

void MainWindow::findThemes() //finds themes and appends to themes list
{
    QString directoryPath = "C:/Users/ivbon/OneDrive/Documents/";
    QStringList iniTypeFilter{"*.ini"};
    QDirIterator it(directoryPath, iniTypeFilter, QDir::NoFilter, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        QFileInfo info{it.fileInfo()};
        it.next();
        QString fileName = info.baseName();
        QString filePath = info.filePath();
        QSettings* curTheme = new QSettings(filePath, QSettings::IniFormat);
        QString isTheme = curTheme->value("isTheme").value<QString>();
        if (!themesPaths.contains(fileName) and isTheme==QString{"itIsReallyTheme"})
        {
            themesPaths.append(fileName);
            themesPaths.append(filePath);
        }
    }
}

void MainWindow::deleteThemes(QVector<QString> paths)
{
    for (int i = 0; i < paths.length(); ++i)
    {
        QFileInfo fi(paths[i]);
        QString currentFileName = fi.baseName();
        themesPaths.remove(themesPaths.indexOf(paths[i]));
        themesPaths.remove(themesPaths.indexOf(currentFileName));
        QFile::remove(paths[i]);
        availableThemes->setValue("themesPaths", QVariant(themesPaths));
    }

}

void MainWindow::newFile()
{
    saveAs();
}



void MainWindow::createNewTheme(QString name, QColor _syntaxColor, QColor _commentColor, QColor _literalColor, QColor _functionColor, QColor _backgroundColor, QColor _textColor)
{
    QDir dir{};
    dir.mkdir("themes");

    QString path = QDir::currentPath() + "/themes/" + name + ".ini";
    themesPaths.append(name);
    themesPaths.append(path);
    availableThemes->setValue("themesPaths", QVariant(themesPaths));
    current_settings = new QSettings(path, QSettings::IniFormat);
    current_settings->setValue("name", QVariant(name));
    current_settings->setValue("syntaxColor", QVariant(_syntaxColor));
    current_settings->setValue("commentColor", QVariant(_commentColor));
    current_settings->setValue("literalColor", QVariant(_literalColor));
    current_settings->setValue("functionColor", QVariant(_functionColor));
    current_settings->setValue("backgroundColor", QVariant(_backgroundColor));
    current_settings->setValue("textColor", QVariant(_textColor));
    current_settings->setValue("isTheme", QVariant("itIsReallyTheme"));
    current_settings->sync();


    //Side effect - set this style
    this->syntaxColor    =_syntaxColor    ;
    this->commentColor   =_commentColor   ;
    this->literalColor   =_literalColor   ;
    this->functionColor  =_functionColor  ;
    this->backgroundColor=_backgroundColor;

    mr_Editor->backgroundColor = _backgroundColor;
    mr_Editor->setTextColor(_textColor);
    updateTheme();


}

void MainWindow::updateTheme()
{
    delete highlighter;
    highlighter = new Highlighter(this->mr_Editor->document(), syntaxColor, commentColor, literalColor, functionColor);
    mr_Editor->backgroundColor = backgroundColor;
    currentBackgroundColor = backgroundColor;
    mr_Editor->updateBackgroundColor();
    mr_Editor->updateTextColor();
}

void MainWindow::updateRowColumn()
{

    if (lastModificationTime!=nullptr and rowColumnLabel!=nullptr)
    {
        statusBar()->removeWidget(lastModificationTime);
        statusBar()->removeWidget(rowColumnLabel);
    }

    rowColumnPosition = "Row/column position: "+QString::number(mr_Editor->textCursor().blockNumber())+" | " + QString::number(mr_Editor->textCursor().positionInBlock());
    rowColumnLabel = new QLabel(rowColumnPosition, this);
    statusBar()->clearMessage();


    if (lastModificationTime!=nullptr and rowColumnLabel!=nullptr)
    {
        statusBar()->addWidget(lastModificationTime);
        statusBar()->addWidget(rowColumnLabel);
    }

}

void MainWindow::updateModificationTime()
{
    if (lastModificationTime!=nullptr and rowColumnLabel!=nullptr)
    {
        statusBar()->removeWidget(lastModificationTime);
        statusBar()->removeWidget(rowColumnLabel);
    }


     if (lastModificationTime!=nullptr and rowColumnLabel!=nullptr)
     {

         statusBar()->addWidget(lastModificationTime);
         statusBar()->addWidget(rowColumnLabel);
     }

}
Editor::Editor(QWidget *parent) : QPlainTextEdit(parent)
{
    setWindowTitle(QObject::tr("Editor"));
    setWordWrapMode(QTextOption::NoWrap);
    currentWrapMode = QTextOption::WrapMode::NoWrap;
    lineNumberArea = new LineNumberArea(this);

    //Font
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    setFont(font);
    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(font);
    setTabStopDistance(tabStop * metrics.maxWidth());


    updateBackgroundColor();
    /*
    this->setStyleSheet("QPlainTextEdit {color: white;}");*/
    connect(this, &Editor::blockCountChanged, this, &Editor::updateLineNumberAreaWidth);
    connect(this, &Editor::updateRequest, this, &Editor::updateLineNumberArea);
    connect(this, &Editor::cursorPositionChanged, this, &Editor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);

    highlightCurrentLine();



}

void MainWindow::enableNumBar()
{
    if (!hideNumBar->isChecked())
    {


        mr_Editor->lineNumberArea->setVisible(false);
        mr_Editor->setViewportMargins(0, 0, 0, 0);
        disconnect(mr_Editor, &Editor::blockCountChanged, mr_Editor, &Editor::updateLineNumberAreaWidth);
    }
    else if(hideNumBar->isChecked()==true)
    {
        mr_Editor->lineNumberArea->setVisible(true);
        mr_Editor->setViewportMargins(25, 0, 0, 0);
        connect(mr_Editor, &Editor::blockCountChanged, mr_Editor, &Editor::updateLineNumberAreaWidth);
    }

}


void Editor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(25, 0, 0, 0);
}


void Editor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y()+20, lineNumberArea->width(), rect.height());

}


void Editor::findAndHighlight()
{
    if (textToFind!=findDialog->getFindText()) countFind = 0;
    countFind+=1;
    textToFind = findDialog->getFindText();
    int currentpos = this->textCursor().position();
    moveCursor(QTextCursor::Start);
    for (int i = 0 ; i < countFind; ++i)
    {
        if (!find(textToFind, QTextDocument::FindCaseSensitively)) //If no succes we go to previous cursor position
            {
                QTextCursor cur{this->document()};
                cur.setPosition(currentpos);
                this->doSetTextCursor(cur);
            }
    }
}

void Editor::findAndHighlightR()
{
     countFind = 1;

    textToFind = replaceDialog->getFindText();
    int currentpos = this->textCursor().position();
    moveCursor(QTextCursor::Start);
    for (int i = 0 ; i < countFind; ++i)
    {
        if (!find(textToFind, QTextDocument::FindCaseSensitively)) //If no succes we go to previous cursor position
            {
                QTextCursor cur{this->document()};
                cur.setPosition(currentpos);
                this->doSetTextCursor(cur);
            }
    }
}

void Editor::findAndHighlightForFindInFindAndReplace()
{
    if (textToFind!=replaceDialog->getFindText()) countFind = 0;
    countFind+=1;

    textToFind = replaceDialog->getFindText();
    int currentpos = this->textCursor().position();
    moveCursor(QTextCursor::Start);
    for (int i = 0 ; i < countFind; ++i)
    {
        if (!find(textToFind, QTextDocument::FindCaseSensitively)) //If no succes we go to previous cursor position
            {
                QTextCursor cur{this->document()};
                cur.setPosition(currentpos);
                this->doSetTextCursor(cur);
            }
    }
}

void Editor::showFindDialog()
{
    countFind = 0;
    findDialog->show();

}

void Editor::showReplaceDialog()
{
    countFind = 0;
    replaceDialog->show();
}

void Editor::setTextFont()
{
    this->setFont(QFontDialog::getFont(0, this->font()));
    currentFont = this->font();
}

void Editor::setTextWrapping()
{
    currentWrapMode = QTextOption::NoWrap;
    setWordWrapMode(QTextOption::NoWrap);
    if (setWrappingAct->isChecked())
    {
        setLineWrapMode(QPlainTextEdit::LineWrapMode::WidgetWidth);
        currentWrapMode =  QTextOption::WordWrap;
        setWordWrapMode(QTextOption::WordWrap);
    }

}
void Editor::replace()
{
    if (!this->textCursor().hasSelection())
        findAndHighlightR();
    auto tempcursor = this->textCursor();


        textToReplace = this->replaceDialog->getReplaceText();
        if (tempcursor.hasSelection())
            tempcursor.insertText(textToReplace);
        this->setTextCursor(tempcursor);
    }

void Editor::replaceAll(){

    moveCursor(QTextCursor::Start);
    textToReplace = replaceDialog->getReplaceText();
    textToFind = replaceDialog->getFindText();
    for(int i = 0; i<this->document()->lineCount(); i++)
    {

        if (this->textCursor().hasSelection())
            this->textCursor().insertText(textToReplace);
    }
}
void MainWindow::changeBlockColor()
{
    QColor blockColor = colorPicker->getColor();
    currentBlockColor = blockColor;
    QTextCursor cursor(mr_Editor->textCursor());

    // change block format (will set the yellow background)
    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setBackground(blockColor);
    /*blockFormat.setNonBreakableLines(true);
    blockFormat.setPageBreakPolicy(QTextFormat::PageBreak_AlwaysBefore);*/
    cursor.setBlockFormat(blockFormat);
}


void MainWindow::enableToolBar()
{
    if (hideToolBar->isChecked())
        fileToolBar->setVisible(true);
    else
    {
        fileToolBar->setVisible(false);
    }
}


void MainWindow::enableStatusBar()
{
    if (hideStatusBar->isChecked())
        statusBar()->setVisible(true);
    else
        statusBar()->setVisible(false);
}

void Editor::updateBackgroundColor()
{
    QPalette palette;
    palette.setColor(QPalette::Base, backgroundColor);
    this->setPalette(palette);
    highlightCurrentLine();
}

void Editor::updateTextColor()
{
    QPalette palette = this->palette();
    palette.setColor(QPalette::Text, textColor);
    setPalette(palette);
}

void Editor::setTextColor(QColor color)
{
    textColor = color;

}


void Editor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = this->rect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), 25, cr.height()));
}


void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(backgroundColor).lighter(130);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


//Draw numbers and set grey color color
void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

//![extraAreaPaintEvent_0]

//![extraAreaPaintEvent_1]
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
//![extraAreaPaintEvent_1]

//![extraAreaPaintEvent_2]
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}



void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this,
           tr("Open code"), "",
           tr("C++ code (*.cpp *.h *.hpp *.cxx);;All Files (*)"));
    if (fileName.isEmpty())
           return;
    else
    {

      QFile file(fileName);
      QFileInfo fi(fileName);
      setCurrentFileName(fi.fileName());
      currentFilePath = fileName;
           if (!file.open(QFile::ReadOnly | QFile::Text)) {
               QMessageBox::warning(this, tr("Application"),
                                    tr("Cannot read file %1:\n%2.")
                                    .arg(QDir::toNativeSeparators(fileName), file.errorString()));
               return;
           }

           QTextStream in(&file);
       #ifndef QT_NO_CURSOR
           QGuiApplication::setOverrideCursor(Qt::WaitCursor);
       #endif
           mr_Editor->setPlainText(in.readAll());
       #ifndef QT_NO_CURSOR
           QGuiApplication::restoreOverrideCursor();
       #endif
    }
    //statusBar()->showMessage(tr("FIle loaded successfuly!"));
    //updateModificationTime();

    status->updateModificationTime();
    qDebug()<<currentFilePath;
    status->updateSizeInfo(currentFilePath);
    setWindowModified(false);
}

void MainWindow::updateStatusBar()
{
    statusBar()->removeWidget(status);
    status = new mr_statusBar{this, mr_Editor};
    statusBar()->addWidget(status);

}

void MainWindow::upplyCheckedTheme()
{
    for (int i = 0; i < themesNames.length(); ++i)
    {
        if (stylesButtons[i]->isChecked())
        {
            setThemeSettings(themesPathsReally[i]);
        }

    }
}

void MainWindow::setKeywordsCXX2020()
{
    delete highlighter;
    highlighter = new Highlighter(this->mr_Editor->document(), syntaxColor, commentColor, literalColor, functionColor, Highlighter::CXX20);
}

void MainWindow::setKeywordsCXX2011()
{
    delete highlighter;
    highlighter = new Highlighter(this->mr_Editor->document(), syntaxColor, commentColor, literalColor, functionColor, Highlighter::CXX11);
}

void MainWindow::setKeywordsC2018()
{
    delete highlighter;
    highlighter = new Highlighter(this->mr_Editor->document(), syntaxColor, commentColor, literalColor, functionColor, Highlighter::C2018);
}

void MainWindow::openNewTheme()
{
    QString newThemePath = QFileDialog::getOpenFileName(this,
           tr("Select theme"), "",
           tr("Theme ini-file (*.ini);;All Files (*)"));
    setThemeSettings(newThemePath);

}
bool MainWindow::save()
{
    QString fileName = currentFilePath;
    if (fileName.isEmpty())
        saveAs();
    else
        {
            QString errorMessage;

            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
            QSaveFile file(fileName);
            if (file.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream out(&file);
                out << mr_Editor->toPlainText();
                if (!file.commit())
                {
                    errorMessage = tr("Cannot write file %1:\n%2.")
                                   .arg(QDir::toNativeSeparators(fileName), file.errorString());
                }
            }
            else
            {
                errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
            QGuiApplication::restoreOverrideCursor();

            if (!errorMessage.isEmpty())
            {
                QMessageBox::warning(this, tr("Application"), errorMessage);
                return false;
            }

        }

    //statusBar()->showMessage(tr("File saved successfully!"));
    //updateModificationTime();
    status->updateModificationTime();
    qDebug()<<currentFilePath;
    status->updateSizeInfo(currentFilePath);
    setWindowModified(false);
    return true;
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save code"), "",
        tr("C++ code (*.cpp *.h *.cxx *.hpp);;All Files (*)"));
    QFileInfo fi{fileName};
    currentFilePath = fileName;
    setCurrentFileName(fi.fileName());

    if (fileName.isEmpty())
        return false;
    else
        {
            QString errorMessage;

            QGuiApplication::setOverrideCursor(Qt::WaitCursor);
            QSaveFile file(fileName);
            if (file.open(QFile::WriteOnly | QFile::Text))
            {
                QTextStream out(&file);
                out << mr_Editor->toPlainText();
                if (!file.commit())
                {
                    errorMessage = tr("Cannot write file %1:\n%2.")
                                   .arg(QDir::toNativeSeparators(fileName), file.errorString());
                }
            }
            else
            {
                errorMessage = tr("Cannot open file %1 for writing:\n%2.")
                               .arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
            QGuiApplication::restoreOverrideCursor();

            if (!errorMessage.isEmpty())
            {
                QMessageBox::warning(this, tr("Application"), errorMessage);
                return false;
            }

        }

    //statusBar()->showMessage(tr("File saved successfully!"));
    //updateModificationTime();
    status->updateModificationTime();
    status->updateSizeInfo(currentFilePath);
    setWindowModified(false);
    return true;
}

void MainWindow::setBackgroungColor()
{
    mr_Editor->backgroundColor = colorPicker->getColor();
    currentBackgroundColor = mr_Editor->backgroundColor;
    backgroundColor = mr_Editor->backgroundColor;
    mr_Editor->updateBackgroundColor();
}



void MainWindow::exit()
{
    QWidget::close();
    delete highlighter;
}
//Icons path need to be changed.


void MainWindow::documentWasModified()

{
    setWindowModified(mr_Editor->document()->isModified());
}


void MainWindow::setCurrentFileName(const QString &fileName)

{
    currentFileName = fileName;
    mr_Editor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFileName;
    if (currentFileName.isEmpty())
        shownName = "untitled.cpp";
    setWindowFilePath(shownName);
}
