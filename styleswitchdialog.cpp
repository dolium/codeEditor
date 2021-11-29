#include "styleswitchdialog.h"



StyleSwitchDialog::StyleSwitchDialog(QWidget *parent, QVector<QString> pathAndNames):QDialog{parent}
{

    createPathAndNames(pathAndNames);
    acceptButton = new QPushButton(tr("&Accept"));

    QVBoxLayout *layout = new QVBoxLayout;
    createGridGroupBox();
    layout->addWidget(gridGroupBox);
    layout->addWidget(acceptButton);
    for (int i = 0; i <names.length(); ++i)
    {
        connect(radioButtons[i], &QRadioButton::toggled, this, &StyleSwitchDialog::setCurrentData);
    }
    connect(acceptButton,  &QPushButton::clicked, this, &StyleSwitchDialog::accept);
    connect(acceptButton, &QPushButton::clicked, this, &StyleSwitchDialog::addThemesToDelete);
    smallEditor->setPlainText("do {i--} while(i>0) foo() 'literal' and simple text //Comment");
    setLayout(layout);
}


void StyleSwitchDialog::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Grid layout"));
    QGridLayout *layout = new QGridLayout;


    for (int i = 0; i < names.length(); ++i)
    {
        QRadioButton *b = new QRadioButton{this};

        radioButtons.push_back(b);
        radioButtons[i]->setText(names[i]);
        QPushButton *deleteButton = new QPushButton{this};
        deleteButtons.append(deleteButton);
        deleteButtons[i]->setCheckable(true);
        QString deleteString = QString{"Delete style"};
        deleteButtons[i]->setText(deleteString);

        layout->addWidget(radioButtons[i], i, 0);
        layout->addWidget(deleteButtons[i], i,1);
    }

    smallEditor = new QPlainTextEdit(this);
    smallEditor->setPlainText(tr("This widget takes up about two thirds of the "
                                 "grid layout."));
    highlighter = new Highlighter(smallEditor->document());
    layout->addWidget(smallEditor, 0, 2, 3, 1);
    layout->setColumnStretch(0, 10);
    layout->setColumnStretch(1, 3);
    layout->setColumnStretch(2, 30);
    gridGroupBox->setLayout(layout);
}


void StyleSwitchDialog::createPathAndNames(QVector<QString>pathAndNames)
{
    int i = 0;
    while(i<pathAndNames.length())
    {
        if (i%2==0) names.append(pathAndNames[i]);

        else paths.append(pathAndNames[i]);
        ++i;
    }

}


void StyleSwitchDialog::setThemeSettings(QString themePath)
{

    QSettings* theme = new QSettings(themePath, QSettings::IniFormat);
    syntaxColor     = theme->value("syntaxColor").value<QColor>();
    commentColor    = theme->value("commentColor").value<QColor>();
    literalColor    = theme->value("literalColor").value<QColor>();
    functionColor   = theme->value("functionColor").value<QColor>();
    backgroundColor = theme->value("backgroundColor").value<QColor>();
    textColor = theme->value("textColor").value<QColor>();
    updateTheme();
    updateBackgroundColor();
    updateTextColor();
}


void StyleSwitchDialog::updateTheme()
{

  delete highlighter;
  highlighter = new Highlighter(smallEditor->document(), syntaxColor, commentColor, literalColor, functionColor);

}


void StyleSwitchDialog::updateBackgroundColor()
{
    QPalette palette;
    palette.setColor(QPalette::Base, backgroundColor);
    smallEditor->setPalette(palette);
}


void StyleSwitchDialog::updateTextColor()
{
    QPalette palette = smallEditor->palette();
    palette.setColor(QPalette::Text, textColor);
    smallEditor->setPalette(palette);
}


void StyleSwitchDialog::setCurrentData()
{
    for (int i = 0; i < names.length(); ++i)
    {
        if(radioButtons[i]->isChecked())
        {
            currentName = names[i];
            currentPath = paths[i];
        }
    }
    setThemeSettings(currentPath);
}


void StyleSwitchDialog::addThemesToDelete()
{
    for (int i = 0; i < paths.length(); ++i)
    {
        if(deleteButtons[i]->isChecked())
        {
             themesToDelete.append(paths[i]);
        }
    }
}
