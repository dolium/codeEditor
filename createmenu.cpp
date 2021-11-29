#include "mainwindow.h"
void MainWindow::createMenu()
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    fileToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/QTcreator/Examples/Qt-5.15.2/widgets/widgets/codeeditor/new.png"));
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    fileToolBar->addAction(newAct);


    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/QTcreator/Examples/Qt-5.15.2/widgets/widgets/codeeditor/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);


    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/QTcreator/Examples/Qt-5.15.2/widgets/widgets/codeeditor/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    const QIcon exitIcon = QIcon::fromTheme("C:/Users/ivbon/OneDrive/Documents/Programming/QTcreator/Examples/Qt-5.15.2/widgets/widgets/codeeditor/exit.png");
    QAction *exit = new QAction(exitIcon, tr("&Exit"), this);
    connect(exit, &QAction::triggered, this, &MainWindow::exit);
    exit->setStatusTip(tr("Quit the programm"));
    fileMenu->addAction(exit);
    fileToolBar->addAction(exit);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    const QIcon setColor = QIcon::fromTheme("document-save-as");//change
    viewMenu->addAction(saveAsIcon, tr("&Set background color"), this, &MainWindow::setBackgroungColor);

    const QIcon setBlockColor = QIcon::fromTheme("document-save-as");//change
    viewMenu->addAction(saveAsIcon, tr("&Set block color"), this, &MainWindow::changeBlockColor);



   hideNumBar = new QAction(tr("Number bar"), this);
   hideNumBar->setCheckable(true);
   hideNumBar->setChecked(true);
   connect(hideNumBar, &QAction::triggered, this, &MainWindow::enableNumBar);
   viewMenu->addAction(hideNumBar);

   hideToolBar = new QAction(tr("Tool bar"), this);
   hideToolBar->setCheckable(true);
   hideToolBar->setChecked(true);
   connect(hideToolBar, &QAction::triggered, this, &MainWindow::enableToolBar);
   viewMenu->addAction(hideToolBar);

   hideStatusBar = new QAction(tr("Status bar"), this);
   hideStatusBar->setCheckable(true);
   hideStatusBar->setChecked(true);
   connect(hideStatusBar, &QAction::triggered, this, &MainWindow::enableStatusBar);
   viewMenu->addAction(hideStatusBar);


   QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
   const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
   QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
   connect(cutAct, &QAction::triggered, mr_Editor, &QPlainTextEdit::cut);
   editMenu->addAction(cutAct);

   const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
   QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
   connect(copyAct, &QAction::triggered, mr_Editor, &QPlainTextEdit::copy);
   editMenu->addAction(copyAct);

   const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
   QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
   connect(pasteAct, &QAction::triggered, mr_Editor, &QPlainTextEdit::paste);
   editMenu->addAction(pasteAct);

   const QIcon undoIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
   QAction *undoAct = new QAction(pasteIcon, tr("&Undo"), this);
   connect(undoAct, &QAction::triggered, mr_Editor, &QPlainTextEdit::undo);
   editMenu->addAction(undoAct);

   const QIcon reIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
   QAction *reAct = new QAction(reIcon, tr("&Repeat"), this);
   connect(reAct, &QAction::triggered, mr_Editor, &QPlainTextEdit::redo);
   editMenu->addAction(reAct);

   const QIcon findIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
   QAction *findAct = new QAction(findIcon, tr("&Find"), this);

   mr_Editor->findDialog = new FindDialog(this, FindDialog::mode::FIND);
   connect(findAct, &QAction::triggered, mr_Editor, &Editor::showFindDialog);
   connect(mr_Editor->findDialog->findButton, &QPushButton::clicked, mr_Editor, &Editor::findAndHighlight);

   editMenu->addAction(findAct);

   const QIcon findReplaceIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
   QAction *findReplaceAct = new QAction(findIcon, tr("&Find and replace"), this);

   mr_Editor->replaceDialog = new FindDialog(this, FindDialog::mode::FIND_AND_REPLACE);
   connect(findReplaceAct, &QAction::triggered, mr_Editor, &Editor::showReplaceDialog);
   connect(mr_Editor->replaceDialog->replaceButton, &QPushButton::clicked, mr_Editor, &Editor::replace);
   connect(mr_Editor->replaceDialog->replaceAllButton, &QPushButton::clicked, mr_Editor, &Editor::replaceAll);
   connect(mr_Editor->replaceDialog->findButton, &QPushButton::clicked, mr_Editor, &Editor::findAndHighlightForFindInFindAndReplace);
   editMenu->addAction(findReplaceAct);

   const QIcon styleCreatorIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
   QAction *styleCreateAct = new QAction(findIcon, tr("&Create new style"), this);
   styleDialog = new StyleCreatorDialog(this);
   connect(styleCreateAct, &QAction::triggered, this, &MainWindow::showThemeCreator);
   editMenu->addAction(styleCreateAct);

   styleSwitchDialog = new StyleSwitchDialog(this, this->themesPaths);
   QAction *styleSwitchAct = new QAction("Change style theme", this);
   connect(styleSwitchAct, &QAction::triggered, this, &MainWindow::showStyleSwitch);
   editMenu->addAction(styleSwitchAct);

   QMenu *styleMenu = new QMenu{"Available styles"};
    themesNames = styleSwitchDialog->getStyleNames();
    themesPathsReally = styleSwitchDialog->getStylePaths();
   styleGroup = new QActionGroup(this);
   for (int i = 0; i < themesNames.length(); ++i)
   {
       QAction *themeAct = new QAction("set "+themesNames[i]+" style", this);
       themeAct->setCheckable(true);
       if (i==1) themeAct->setChecked(true);
       stylesButtons.append(themeAct);
       styleGroup->addAction(themeAct);
       connect(themeAct, &QAction::triggered, this, &MainWindow::upplyCheckedTheme);
   }
   for (int i = 0 ; i < themesNames.length(); ++i)
   {
       styleMenu->addAction(stylesButtons[i]);
   }

   editMenu->addMenu(styleMenu);

    //Languages switch menu for viewMenu
    QMenu *languageMenu = new QMenu{"Available languages"};
    languageGroup = new QActionGroup(this);

    QAction *languageAct_CXX2020 = new QAction("set C++ 2020 syntax", this);
    languageAct_CXX2020->setCheckable(true);
    //It is Defaut syntax:
    languageAct_CXX2020->setChecked(true);
    connect(languageAct_CXX2020, &QAction::triggered, this, &MainWindow::setKeywordsCXX2020);
    languageGroup->addAction(languageAct_CXX2020);
    languageMenu->addAction(languageAct_CXX2020);

    QAction *languageAct_CXX2011 = new QAction("set C++ 2011 syntax", this);
    languageAct_CXX2011->setCheckable(true);
    connect(languageAct_CXX2011, &QAction::triggered, this, &MainWindow::setKeywordsCXX2011);
    languageGroup->addAction(languageAct_CXX2011);
    languageMenu->addAction(languageAct_CXX2011);

    QAction *languageAct_C2018 = new QAction("set C 2018 syntax", this);
    languageAct_C2018->setCheckable(true);
    connect(languageAct_C2018, &QAction::triggered, this, &MainWindow::setKeywordsC2018);
    languageGroup->addAction(languageAct_C2018);
    languageMenu->addAction(languageAct_C2018);

    viewMenu->addMenu(languageMenu);

   //connect(mr_Editor, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);
   connect(mr_Editor, &QPlainTextEdit::cursorPositionChanged, status, &mr_statusBar::updateRowColumn);
   connect(mr_Editor, &QPlainTextEdit::cursorPositionChanged, status, &mr_statusBar::updateCountInfo);

}
