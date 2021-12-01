#include "styleswitchdialog.h"



StyleSwitchDialog::StyleSwitchDialog(QWidget *parent, QVector<QString> pathAndNames):QDialog{parent}
{

    colorDialog = new QColorDialog(parent);

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
    connect(pickButton[0], &QPushButton::clicked, this, &StyleSwitchDialog::pickSyntaxColor);

    connect(pickButton[1], &QPushButton::clicked, this, &StyleSwitchDialog::pickCommentColor);

    connect(pickButton[2], &QPushButton::clicked, this, &StyleSwitchDialog::pickLiteralColor);

    connect(pickButton[3], &QPushButton::clicked, this, &StyleSwitchDialog::pickFunctionColor);

    connect(pickButton[4], &QPushButton::clicked, this, &StyleSwitchDialog::pickBackgroundColor);

    connect(pickButton[5],  &QPushButton::clicked, this, &StyleSwitchDialog::pickTextColor);
    connect(acceptChanges, &QPushButton::clicked, this, &StyleSwitchDialog::reformTheme);
    //960*430 set size
    resize(760, 320);
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
        QString deleteString = QString{"Delete"};
        deleteButtons[i]->setText(deleteString);

        layout->addWidget(radioButtons[i], i, 0);
        layout->addWidget(deleteButtons[i], i,1);
    }

    QString buttonName{"Pick color"};
    QVector<QString> labelsNames{"Keyword's color", "Comment's color", "Literals color", "Function color", "Background color", "Text color", "Style name"};
    for (int i = 0; i < 6; ++i)
    {
        labels.push_back(new QLabel(labelsNames[i]));
        pickButton.push_back(new QPushButton{this});
        pickButton[i]->setText(buttonName);

        layout->addWidget(labels[i], i, 2);
        layout->addWidget(pickButton[i], i, 3);
    }
    acceptChanges = new QPushButton("Accept changes");
    smallEditor = new QPlainTextEdit(this);
    smallEditor->setPlainText(tr("do {i--} while(i>0) foo() 'literal' and simple text //Comment"));
    highlighter = new Highlighter(smallEditor->document());
    layout->addWidget(smallEditor, 0, 4, 4, 1);
    layout->addWidget(acceptChanges, 0, 5);
    layout->setColumnStretch(0, 4);
    layout->setColumnStretch(1, 3);
    layout->setColumnStretch(2, 5);
    layout->setColumnStretch(3, 5);
    layout->setColumnStretch(4, 30);
    layout->setColumnStretch(5, 5);
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

    theme = new QSettings(themePath, QSettings::IniFormat);
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

void StyleSwitchDialog::showPossibleChanges()
{
    updateTheme();
    updateBackgroundColor();
    updateTextColor();
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

void StyleSwitchDialog::pickSyntaxColor()
{
    syntaxColor = colorDialog->getColor();
    showPossibleChanges();
}


void StyleSwitchDialog::pickCommentColor()
{
    commentColor = colorDialog->getColor();
    showPossibleChanges();
}


void StyleSwitchDialog::pickLiteralColor()
{
    literalColor = colorDialog->getColor();
    showPossibleChanges();
}


void StyleSwitchDialog::pickFunctionColor()
{
    functionColor = colorDialog->getColor();
    showPossibleChanges();
}


void StyleSwitchDialog::pickBackgroundColor()
{
    backgroundColor = colorDialog->getColor();
    showPossibleChanges();
}

void StyleSwitchDialog::pickTextColor()
{
    textColor = colorDialog->getColor();
    showPossibleChanges();
}

void StyleSwitchDialog::reformTheme()
{
    theme->setValue("syntaxColor", QVariant(syntaxColor));
    theme->setValue("commentColor", QVariant(commentColor));
    theme->setValue("literalColor", QVariant(literalColor));
    theme->setValue("functionColor", QVariant(functionColor));
    theme->setValue("backgroundColor", QVariant(backgroundColor));
    theme->setValue("textColor", QVariant(textColor));
}
