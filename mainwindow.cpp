#include "mainwindow.h"
#include "highlighter.h"
#include <QAbstractItemView>
#include <QScrollBar>

MainWindow::MainWindow()
{

    if (QFile::exists(QDir::currentPath() + "/settings/"+"currentSettings.ini")) currentSettings = new QSettings(QDir::currentPath() + "/settings/"+"currentSettings.ini", QSettings::IniFormat);
    mainEditor = new Editor{};
    setCentralWidget(mainEditor);


    backgroundColor = mainEditor->backgroundColor;
    currentBlockColor = QColor(mainEditor->backgroundColor).lighter(130);

    initializeThemes();


    syntaxColor = Qt::darkBlue;
    highlighter =  new Highlighter(mainEditor->document(), syntaxColor);
    createMenu(); //Very sensitive to its position

    colorPicker = new QColorDialog({0,1,1}, this);
    connect(mainEditor->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::documentWasModified);
    setUnifiedTitleAndToolBarOnMac(true);
    setCurrentFileName("untitled.cpp");


    setKeywordsCXX2020();

    if (QFile::exists(QDir::currentPath() + "/settings/"+"currentSettings.ini"))
    {
        applySettings();
    }

    else
    {
        initSettings();
    }
    status = new statusBarFooter{this, mainEditor};
    statusBar()->addWidget(status);
    connect(mainEditor, &QPlainTextEdit::cursorPositionChanged, status, &statusBarFooter::updateRowColumn);
    connect(mainEditor, &QPlainTextEdit::cursorPositionChanged, status, &statusBarFooter::updateCountInfo);
    //Completer:
    mainEditor->refreshCompleter();
    connect(status, &statusBarFooter::wordCountChanged, mainEditor, &Editor::refreshCompleter);
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
       mainEditor->backgroundColor = backgroundColor;
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

    QSettings* curTheme = new QSettings(themePath, QSettings::IniFormat);

    QString isTheme = curTheme->value("isTheme").value<QString>();
    if (isTheme==QString{"itIsReallyTheme"})
    {



        QSettings* theme = new QSettings(themePath, QSettings::IniFormat);
        currentThemePath = themePath;
        syntaxColor = theme->value("syntaxColor").value<QColor>();
        commentColor = theme->value("commentColor").value<QColor>();
        literalColor = theme->value("literalColor").value<QColor>();
        functionColor = theme->value("functionColor").value<QColor>();
        backgroundColor = theme->value("backgroundColor").value<QColor>();
        mainEditor->backgroundColor = backgroundColor;
        mainEditor->setTextColor(theme->value("textColor").value<QColor>());
        mainEditor->updateTextColor();
        QFileInfo info{themePath};
        //If we do not have this theme - add it to themes list;


        if (!themeNamesPaths.contains(themePath) and (info.baseName()!=QString("default")))
        {

            themeNamesPaths.append(info.baseName());
            themeNamesPaths.append(info.absoluteFilePath());
            availableThemes->setValue("themesPaths", QVariant(themeNamesPaths));
        }
        updateTheme();
    }
}


void MainWindow::initializeThemes()
{
    availableThemes = new QSettings(QDir::currentPath()+QString{"/themes2.ini"}, QSettings::IniFormat);
    themeNamesPaths = availableThemes->value("themesPaths").value<QVector<QString>>();
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
        if (!themeNamesPaths.contains(fileName) and isTheme==QString{"itIsReallyTheme"})
        {
            themeNamesPaths.append(fileName);
            themeNamesPaths.append(filePath);
        }
    }
}


void MainWindow::deleteThemes(QVector<QString> paths)
{
    for (int i = 0; i < paths.length(); ++i)
    {
        QFileInfo fi(paths[i]);
        QString currentFileName = fi.baseName();
        themeNamesPaths.remove(themeNamesPaths.indexOf(paths[i]));
        themeNamesPaths.remove(themeNamesPaths.indexOf(currentFileName));
        QFile::remove(paths[i]);
        availableThemes->setValue("themesPaths", QVariant(themeNamesPaths));
    }

}


void MainWindow::applySettings()
{
    currentSettings = new QSettings(QDir::currentPath() + "/settings/"+"currentSettings.ini", QSettings::IniFormat);
    if (!currentSettings->value("toolBarEnabled").toBool())
    {
        hideToolBar->setChecked(false);
        enableToolBar();
    }
    if (!currentSettings->value("numberBarEnabled").toBool())
    {
        hideNumBar->setChecked(false);
        enableNumBar();
    }
    if (currentSettings->value("wordWrappingEnabled").toBool())
    {
        setWrappingAct->setChecked(true);
        setTextWrapping();
    }
    if (!currentSettings->value("statusBarEnabled").toBool())
    {
        hideStatusBar->setChecked(false);
        enableStatusBar();
    }
    setThemeSettings(currentSettings->value("currentThemePath").toString());

    //FONT
    QFont ff;
    ff.fromString(currentSettings->value("currentFontFamily").toString());
    ff.setStyleHint(QFont::Monospace);
    ff.setFixedPitch(true);
    mainEditor->setFont(ff);
    const int tabStop = 4;  // 4 characters

    QFontMetrics metrics(ff);
    mainEditor->setTabStopDistance(tabStop * metrics.maxWidth());

    mainEditor->currentFont = ff;

     mainEditor->setFont(ff);

     //End of font
}


void MainWindow::initSettings()
{
    QString path = QDir::currentPath() + "/settings/"+"currentSettings.ini";
    currentSettings = new QSettings(path, QSettings::IniFormat);
    currentSettings->setValue("toolBarEnabled", QVariant(true));
    currentSettings->setValue("numberBarEnabled", QVariant(true));
    currentSettings->setValue("wordWrappingEnabled", QVariant(true));
    currentSettings->setValue("statusBarEnabled", QVariant(true));
    currentSettings->setValue("currentThemePath", QVariant(currentThemePath));
    currentSettings->setValue("currentFontFamily", QVariant(mainEditor->currentFont.toString()));
    currentSettings->sync();
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
    themeNamesPaths.append(name);
    themeNamesPaths.append(path);
    availableThemes->setValue("themesPaths", QVariant(themeNamesPaths));
    currentThemeSettings = new QSettings(path, QSettings::IniFormat);
    currentThemeSettings->setValue("name", QVariant(name));
    currentThemeSettings->setValue("syntaxColor", QVariant(_syntaxColor));
    currentThemeSettings->setValue("commentColor", QVariant(_commentColor));
    currentThemeSettings->setValue("literalColor", QVariant(_literalColor));
    currentThemeSettings->setValue("functionColor", QVariant(_functionColor));
    currentThemeSettings->setValue("backgroundColor", QVariant(_backgroundColor));
    currentThemeSettings->setValue("textColor", QVariant(_textColor));
    currentThemeSettings->setValue("isTheme", QVariant("itIsReallyTheme"));

    currentThemeSettings->sync();
    currentSettings->setValue("currentThemePath", QVariant(path));
    currentSettings->sync();


    //Side effect - set this style
    this->syntaxColor    =_syntaxColor    ;
    this->commentColor   =_commentColor   ;
    this->literalColor   =_literalColor   ;
    this->functionColor  =_functionColor  ;
    this->backgroundColor=_backgroundColor;

    mainEditor->backgroundColor = _backgroundColor;
    mainEditor->setTextColor(_textColor);
    updateTheme();


}


void MainWindow::updateTheme()
{
    delete highlighter;
    highlighter = new Highlighter(this->mainEditor->document(), syntaxColor, commentColor, literalColor, functionColor);
    mainEditor->backgroundColor = backgroundColor;
    mainEditor->updateBackgroundColor();
    mainEditor->updateTextColor();


}


void MainWindow::updateRowColumn()
{

    if (lastModificationTime!=nullptr and rowColumnLabel!=nullptr)
    {
        statusBar()->removeWidget(lastModificationTime);
        statusBar()->removeWidget(rowColumnLabel);
    }

    rowColumnPosition = "Row/column position: "+QString::number(mainEditor->textCursor().blockNumber())+" | " + QString::number(mainEditor->textCursor().positionInBlock());

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




    updateBackgroundColor();

    connect(this, &Editor::blockCountChanged, this, &Editor::updateLineNumberAreaWidth);
    connect(this, &Editor::updateRequest, this, &Editor::updateLineNumberArea);
    connect(this, &Editor::cursorPositionChanged, this, &Editor::highlightCurrentLine);

    updateLineNumberAreaWidth(0);

    highlightCurrentLine();




}
void Editor::keyPressEvent(QKeyEvent *e)
{
    if (completer && completer->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    const bool isShortcut = (e->modifiers().testFlag(Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!completer || !isShortcut) // do not process the shortcut when we have a completer
        QPlainTextEdit::keyPressEvent(e);

    const bool ctrlOrShift = e->modifiers().testFlag(Qt::ControlModifier) ||
                             e->modifiers().testFlag(Qt::ShiftModifier);
    if (!completer || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    const bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    QString completionPrefix = textUnderCursor();

    if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                      || eow.contains(e->text().right(1)))) {
        completer->popup()->hide();
        return;
    }

    if (completionPrefix != completer->completionPrefix()) {
        completer->setCompletionPrefix(completionPrefix);
        completer->popup()->setCurrentIndex(completer->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(completer->popup()->sizeHintForColumn(0)
                + completer->popup()->verticalScrollBar()->sizeHint().width());
    completer->complete(cr); // popup it up!
}
QString Editor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
void Editor::insertCompletion(const QString &completion)
{
    if (completer->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - completer->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
    qDebug()<<"Works wtf...";
}

void MainWindow::enableNumBar()
{
    if (!hideNumBar->isChecked())
    {
        mainEditor->lineNumberArea->setVisible(false);
        mainEditor->setViewportMargins(0, 0, 0, 0);
        disconnect(mainEditor, &Editor::blockCountChanged, mainEditor, &Editor::updateLineNumberAreaWidth);
        currentSettings->setValue("numberBarEnabled", QVariant(false));
        currentSettings->sync();
    }
    else if(hideNumBar->isChecked()==true)
    {
        mainEditor->lineNumberArea->setVisible(true);
        mainEditor->setViewportMargins(25, 0, 0, 0);
        connect(mainEditor, &Editor::blockCountChanged, mainEditor, &Editor::updateLineNumberAreaWidth);
        currentSettings->setValue("numberBarEnabled", QVariant(true));
        currentSettings->sync();

    }

}


void MainWindow::enableHighlighting()
{
    if (!hideHighlightingAct->isChecked())
    {
        highlighter->isEnabled = false;
        mainEditor->setPlainText(mainEditor->document()->toPlainText());
    }
    else
    {
        highlighter->isEnabled = true;
        mainEditor->setPlainText(mainEditor->document()->toPlainText());
    }
}


void Editor::updateLineNumberAreaWidth(int )
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
        if (!find(textToFind, QTextDocument::FindCaseSensitively)) //If no succes go to previous cursor position
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
        if (!find(textToFind, QTextDocument::FindCaseSensitively)) //If no succes go to previous cursor position
            {
                QTextCursor cur{this->document()};
                cur.setPosition(currentpos);
                this->doSetTextCursor(cur);
            }
    }
}

void Editor::refreshCompleter()
{
    QStringList wordList;
    QMap<QString, QStringList> wordCompleteDict;
    wordCompleteDict["countVariablesNames"];
    wordCompleteDict["countVariablesTypes"];
    wordCompleteDict["countVariables"];
    wordCompleteDict["cycles"];
    wordCompleteDict["debugStack"];
    QRegularExpression containerNameExp{"(?:(?:(?:QVector|QList|QQueue|QStack|std::vector|std::list)<[A-Za-z]+>)|QStringList)\\s*\\K[A-Za-z]+[A-Za-z0-9]*\\b(?!\\(\\))"};
    QRegularExpressionMatchIterator containerNames= containerNameExp.globalMatch(this->document()->toPlainText());
    while(containerNames.hasNext())
    {
      wordCompleteDict["containerNames"]<<containerNames.next().captured(0);
    }
    for (int i = wordCompleteDict["containerNames"].size()-1; i >=0; i-- )
    {
        QString& container = wordCompleteDict["containerNames"][i];
        wordCompleteDict["cycles"]<<("for (int i = 0; i < "+container+".size(); ++i) \n {\n \n } ");

    }
    QRegularExpression countVariableExp{"((?:\\bint|short|size_t|long long|(?:long(?! long)))\\b)\\s+\\K[a-zA-Z]+[0-9]*\\b(?!\\(\\))"};
    QRegularExpressionMatchIterator countVariableNames= countVariableExp.globalMatch(this->document()->toPlainText());
    while(countVariableNames.hasNext())
    {
        auto tempCaptured = countVariableNames.next();
        wordCompleteDict["countVariablesNames"]<<tempCaptured.captured(0);
        wordCompleteDict["countVariablesTypes"]<<tempCaptured.captured(1);
    }
    for (int i = wordCompleteDict["countVariablesNames"].size()-1; i >=0; i--)
    {
        wordCompleteDict["cycles"]<<("for ("+wordCompleteDict["countVariablesTypes"][i]+" i = 0; i < "+wordCompleteDict["countVariablesNames"][i]+" ++i) \n {\n \n } ");
    }
    if(wordCompleteDict["countVariablesNames"].size()!=0)
    {
         wordCompleteDict["debugStack"]<<"qDebug()<<\"this is value of "+wordCompleteDict["countVariablesNames"].last()+": \" "+" <<"+wordCompleteDict["countVariablesNames"].last()+";";
         wordCompleteDict["debugStack"]<<"std::cout<<\"this is value of "+wordCompleteDict["countVariablesNames"].last()+": \" "+" <<"+wordCompleteDict["countVariablesNames"].last()+";";
    }
    if(wordCompleteDict["containerNames"].size()!=0)
    {
        wordCompleteDict["debugStack"]<<"qDebug()<<\"this is value of "+wordCompleteDict["containerNames"].last()+": \" "+"<<"+wordCompleteDict["containerNames"].last()+";";
        wordCompleteDict["debugStack"]<<"std::cout<<\"this is value of "+wordCompleteDict["containerNames"].last()+": \" "+"<<"+wordCompleteDict["containerNames"].last()+";";
    }
    for (auto key:wordCompleteDict.keys())
    {
        if (key!="countVariablesTypes")
        {
            wordCompleteDict["allWords"]<<wordCompleteDict[key];
        }
    }
    completer = new QCompleter(wordCompleteDict["allWords"], this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setWidget(this);
    QObject::connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
                         this, &Editor::insertCompletion);

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


void MainWindow::setTextFont()
{
    mainEditor->setFont(QFontDialog::getFont(0, mainEditor->font()));
    mainEditor->currentFont = mainEditor->font();
    currentSettings->setValue("currentFontFamily", QVariant(mainEditor->currentFont.toString()));
    currentSettings->sync();

}


void MainWindow::setTextWrapping()
{
    mainEditor->currentWrapMode = QTextOption::NoWrap;
    mainEditor->setWordWrapMode(QTextOption::NoWrap);
    if (setWrappingAct->isChecked())
    {
        mainEditor->setLineWrapMode(QPlainTextEdit::LineWrapMode::WidgetWidth);
        mainEditor->currentWrapMode =  QTextOption::WordWrap;
        mainEditor->setWordWrapMode(QTextOption::WordWrap);
        currentSettings->setValue("wordWrappingEnabled", QVariant(true));
        currentSettings->sync();


    }
    else
    {
        currentSettings->setValue("wordWrappingEnabled", QVariant(false));
        currentSettings->sync();
    }

}


void MainWindow::about()
{
    aboutDialog *dialog = new aboutDialog{this};
    dialog->show();
}


void Editor::selectLine()
{
    moveCursor(QTextCursor::StartOfLine);
    moveCursor(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
}


void Editor::selectWord()
{
    moveCursor(QTextCursor::StartOfWord);
    moveCursor(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
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
    QTextCursor cursor(mainEditor->textCursor());

    QTextBlockFormat blockFormat = cursor.blockFormat();
    blockFormat.setBackground(blockColor);

    cursor.setBlockFormat(blockFormat);
}


void MainWindow::enableToolBar()
{
    if (hideToolBar->isChecked())
    {
        mainToolBar->setVisible(true);
        currentSettings->setValue("toolBarEnabled", QVariant(true));
        currentSettings->sync();

    }

    else
    {
        mainToolBar->setVisible(false);
        currentSettings->setValue("toolBarEnabled", QVariant(false));
        currentSettings->sync();


    }
}


void MainWindow::enableStatusBar()
{
    if (hideStatusBar->isChecked())
    {
        statusBar()->setVisible(true);
        currentSettings->setValue("statusBarEnabled", true);
        currentSettings->sync();

    }
    else
    {
        statusBar()->setVisible(false);
        currentSettings->setValue("statusBarEnabled", false);
        currentSettings->sync();


    }
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

//Draw numbers and set smart-color
void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), backgroundColor.lighter(75));


    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());
    int newR = 255 - backgroundColor.lighter(75).red();
    int newG = 255- backgroundColor.lighter(75).green();
    int newB = 255 - backgroundColor.lighter(75).blue();
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(newR,newG,newB));
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
      if (!file.open(QFile::ReadOnly | QFile::Text))
      {
          QMessageBox::warning(this, tr("Application"),tr("Cannot read file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString()));
          return;
      }

           QTextStream in(&file);
           QGuiApplication::setOverrideCursor(Qt::WaitCursor);
           mainEditor->setPlainText(in.readAll());
           QGuiApplication::restoreOverrideCursor();
    }

    status->updateModificationTime();
    status->updateSizeInfo(currentFilePath);
    setWindowModified(false);
    mainEditor->refreshCompleter();


}

void MainWindow::updateStatusBar()
{
    statusBar()->removeWidget(status);
    status = new statusBarFooter{this, mainEditor};
    statusBar()->addWidget(status);
}

void MainWindow::applyCheckedTheme()
{
    for (int i = 0; i < themesNames.length(); ++i)
    {
        if (stylesButtons[i]->isChecked())
        {
            currentSettings->setValue("currentThemePath", QVariant(themesPaths[i]));
            setThemeSettings(themesPaths[i]);
        }
    }
}

void MainWindow::setKeywordsCXX2020()
{
    delete highlighter;
    highlighter = new Highlighter(this->mainEditor->document(), syntaxColor, commentColor, literalColor, functionColor, Highlighter::CXX20);
}


void MainWindow::setKeywordsCXX2011()
{
    delete highlighter;
    highlighter = new Highlighter(this->mainEditor->document(), syntaxColor, commentColor, literalColor, functionColor, Highlighter::CXX11);
}


void MainWindow::setKeywordsC2018()
{
    delete highlighter;
    highlighter = new Highlighter(this->mainEditor->document(), syntaxColor, commentColor, literalColor, functionColor, Highlighter::C2018);
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
                out << mainEditor->toPlainText();
                if (!file.commit())
                {
                    errorMessage = tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
                }
            }
            else
            {
                errorMessage = tr("Cannot open file %1 for writing:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
            QGuiApplication::restoreOverrideCursor();

            if (!errorMessage.isEmpty())
            {
                QMessageBox::warning(this, tr("Application"), errorMessage);
                return false;
            }

        }


    status->updateModificationTime();
    status->updateSizeInfo(currentFilePath);
    setWindowModified(false);
    return true;
}


bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save code"), "", tr("C++ code (*.cpp *.h *.cxx *.hpp);;All Files (*)"));
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
                out << mainEditor->toPlainText();
                if (!file.commit())
                {
                    errorMessage = tr("Cannot write file %1:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
                }
            }
            else
            {
                errorMessage = tr("Cannot open file %1 for writing:\n%2.").arg(QDir::toNativeSeparators(fileName), file.errorString());
            }
            QGuiApplication::restoreOverrideCursor();

            if (!errorMessage.isEmpty())
            {
                QMessageBox::warning(this, tr("Application"), errorMessage);
                return false;
            }

        }


    status->updateModificationTime();
    status->updateSizeInfo(currentFilePath);
    setWindowModified(false);
    return true;
}


void MainWindow::setBackgroungColor()
{
    mainEditor->backgroundColor = colorPicker->getColor();
    backgroundColor = mainEditor->backgroundColor;
    mainEditor->updateBackgroundColor();
}


void MainWindow::exit()
{
    QWidget::close();
    delete highlighter;
}


void MainWindow::documentWasModified()

{
    setWindowModified(mainEditor->document()->isModified());
}


void MainWindow::setCurrentFileName(const QString &fileName)

{
    currentFileName = fileName;
    mainEditor->document()->setModified(false);
    setWindowModified(false);

    QString shownName = currentFileName;
    if (currentFileName.isEmpty())
    {
        shownName = "untitled.cpp";
    }
    setWindowFilePath(shownName);
}


void Editor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = createStandardContextMenu();

    QAction *selectLineAct = new QAction(tr("&Select line"), this);
    connect(selectLineAct, &QAction::triggered, this, &Editor::selectLine);
    menu->addAction(selectLineAct);

    QAction *selectWordAct = new QAction(tr("&Select word"), this);
    connect(selectWordAct, &QAction::triggered, this, &Editor::selectWord);
    menu->addAction(selectWordAct);

    menu->exec(event->globalPos());
}

