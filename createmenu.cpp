#include "mainwindow.h"
void MainWindow::createMenu()
{

    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    mainToolBar = addToolBar(tr("File"));
    const QIcon newIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/newIcon.png");
    QAction *newAct = new QAction(newIcon, tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
    fileMenu->addAction(newAct);
    mainToolBar->addAction(newAct);


    const QIcon openIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/openIcon.png");
    QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    mainToolBar->addAction(openAct);


    const QIcon saveIcon =  QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/saveIcon.png");
    QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    mainToolBar->addAction(saveAct);

    const QIcon saveAsIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/saveAsIcon.png");
    QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));

    const QIcon exitIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/exitIcon.png");
    QAction *exit = new QAction(exitIcon, tr("&Exit"), this);
    connect(exit, &QAction::triggered, this, &MainWindow::exit);
    exit->setStatusTip(tr("Quit the programm"));
    fileMenu->addAction(exit);
    mainToolBar->addAction(exit);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(tr("&Set background color"), this, &MainWindow::setBackgroungColor);


    viewMenu->addAction(tr("&Set block color"), this, &MainWindow::changeBlockColor);



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

   hideHighlightingAct = new QAction("Enable highlighting", this);
   hideHighlightingAct->setCheckable(true);
   hideHighlightingAct->setChecked(true);
   connect(hideHighlightingAct, &QAction::triggered, this, &MainWindow::enableHighlighting);
   viewMenu->addAction(hideHighlightingAct);

   //Start of Languages switch menu for viewMenu
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
   //End of creating language Menu

    QAction *loadThemeAct = new QAction("Load new theme", this);
    connect(loadThemeAct, &QAction::triggered, this, &MainWindow::openNewTheme);
    viewMenu->addAction(loadThemeAct);

    const QIcon styleCreatorIcon = QIcon::fromTheme("edit-paste", QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/paste.png"));
    QAction *styleCreateAct = new QAction( tr("&Create new style"), this);
    styleDialog = new StyleCreatorDialog(this);
    connect(styleCreateAct, &QAction::triggered, this, &MainWindow::showThemeCreator);
    viewMenu->addAction(styleCreateAct);

    styleSwitchDialog = new StyleSwitchDialog(this, this->themeNamesPaths);
    QAction *styleSwitchAct = new QAction("Change style theme", this);
    connect(styleSwitchAct, &QAction::triggered, this, &MainWindow::showStyleSwitch);
    viewMenu->addAction(styleSwitchAct);

    QMenu *styleMenu = new QMenu{"Available styles"};
     themesNames = styleSwitchDialog->getStyleNames();
     themesPaths = styleSwitchDialog->getStylePaths();
    styleGroup = new QActionGroup(this);
    for (int i = 0; i < themesNames.length(); ++i)
    {
        QAction *themeAct = new QAction("set "+themesNames[i]+" style", this);
        themeAct->setCheckable(true);

        if (currentSettings->contains("currentThemePath")and currentSettings->value("currentThemePath").toString()!="")
        {

             QFileInfo tempThemeInfo{currentSettings->value("currentThemePath").toString()};
             QString tempThemeName = tempThemeInfo.baseName();
             if (themesNames[i]==tempThemeName) themeAct->setChecked(true);
        }
        else
        {
             if (i==0) themeAct->setChecked(true);
        }
        stylesButtons.append(themeAct);
        styleGroup->addAction(themeAct);
        connect(themeAct, &QAction::triggered, this, &MainWindow::applyCheckedTheme);
    }

    for (int i = 0 ; i < themesNames.length(); ++i)
    {
        styleMenu->addAction(stylesButtons[i]);
    }

    viewMenu->addMenu(styleMenu);
    applyCheckedTheme();


   QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
   const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/cut.png"));
   QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);
   connect(cutAct, &QAction::triggered, mainEditor, &QPlainTextEdit::cut);
   editMenu->addAction(cutAct);

   const QIcon copyIcon =  QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/copyIcon.png");
   QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
   connect(copyAct, &QAction::triggered, mainEditor, &QPlainTextEdit::copy);
   editMenu->addAction(copyAct);

   const QIcon pasteIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/pasteIcon.png");
   QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
   connect(pasteAct, &QAction::triggered, mainEditor, &QPlainTextEdit::paste);
   editMenu->addAction(pasteAct);

   const QIcon undoIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/undoIcon.png");
   QAction *undoAct = new QAction(undoIcon, tr("&Undo"), this);
   connect(undoAct, &QAction::triggered, mainEditor, &QPlainTextEdit::undo);
   editMenu->addAction(undoAct);

   const QIcon reIcon = QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/redoIcon.png");
   QAction *reAct = new QAction(reIcon, tr("&Repeat"), this);
   connect(reAct, &QAction::triggered, mainEditor, &QPlainTextEdit::redo);
   editMenu->addAction(reAct);

   const QIcon findIcon =  QIcon("C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/findIcon.png");
   QAction *findAct = new QAction(findIcon, tr("&Find"), this);
   findAct->setStatusTip(tr("Find"));

   mainEditor->findDialog = new FindDialog(this, FindDialog::mode::FIND);
   connect(findAct, &QAction::triggered, mainEditor, &Editor::showFindDialog);
   connect(mainEditor->findDialog->findButton, &QPushButton::clicked, mainEditor, &Editor::findAndHighlight);

   editMenu->addAction(findAct);
   mainToolBar->addAction(findAct);
   const QIcon findReplaceIcon = QIcon("C:\\Users\\ivbon\\OneDrive\\Documents\\Programming\\codeeditor_3\\imgs\\findReplaceIcon.png");
   QAction *findReplaceAct = new QAction(findReplaceIcon, tr("&Find and replace"), this);
   findReplaceAct->setStatusTip("find and Replace");

   mainEditor->replaceDialog = new FindDialog(this, FindDialog::mode::FIND_AND_REPLACE);
   connect(findReplaceAct, &QAction::triggered, mainEditor, &Editor::showReplaceDialog);
   connect(mainEditor->replaceDialog->replaceButton, &QPushButton::clicked, mainEditor, &Editor::replace);
   connect(mainEditor->replaceDialog->replaceAllButton, &QPushButton::clicked, mainEditor, &Editor::replaceAll);
   connect(mainEditor->replaceDialog->findButton, &QPushButton::clicked, mainEditor, &Editor::findAndHighlightForFindInFindAndReplace);
   editMenu->addAction(findReplaceAct);
   mainToolBar->addAction(findReplaceAct);


   QAction *selectAllAct = new QAction(findIcon, tr("&Select All"), this);
   connect(selectAllAct, &QAction::triggered, mainEditor, &Editor::selectAll);
   editMenu->addAction(selectAllAct);


    QMenu* formatMenu = menuBar()->addMenu(tr("Format"));

    QAction *applyNewFontAct = new QAction ("Set new font", this);
    connect(applyNewFontAct, &QAction::triggered, this, &MainWindow::setTextFont);
    formatMenu->addAction(applyNewFontAct);
    setWrappingAct = new QAction("Enable width-wrapping", this);
    setWrappingAct->setCheckable(true);
    connect(setWrappingAct, &QAction::triggered, this, &MainWindow::setTextWrapping);
    formatMenu->addAction(setWrappingAct);



    QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
    QAction *showAboutAct = new QAction("About programm");
    connect(showAboutAct, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(showAboutAct);


}
