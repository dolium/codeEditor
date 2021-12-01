#include "stylecreatordialog.h"
#include "QColorDialog"

StyleCreatorDialog::StyleCreatorDialog(QWidget *parent):QDialog(parent)
{

    acceptButton = new QPushButton(tr("&Accept"));
    colorDialog = new QColorDialog(parent);
    QVBoxLayout *layout = new QVBoxLayout;
    createGridGroupBox();
    layout->addWidget(gridGroupBox);
    layout->addWidget(acceptButton);

    setLayout(layout);



    connect(pickButton[0], &QPushButton::clicked, this, &StyleCreatorDialog::pickSyntaxColor);

    connect(pickButton[1], &QPushButton::clicked, this, &StyleCreatorDialog::pickCommentColor);

    connect(pickButton[2], &QPushButton::clicked, this, &StyleCreatorDialog::pickLiteralColor);

    connect(pickButton[3], &QPushButton::clicked, this, &StyleCreatorDialog::pickFunctionColor);

    connect(pickButton[4], &QPushButton::clicked, this, &StyleCreatorDialog::pickBackgroundColor);

    connect(pickButton[5],  &QPushButton::clicked, this, &StyleCreatorDialog::pickTextColor);

    connect(acceptButton, &QPushButton::clicked, this, &StyleCreatorDialog::setStyleName);

    connect(acceptButton,  &QPushButton::clicked, this, &StyleCreatorDialog::accept);

}




void StyleCreatorDialog::pickSyntaxColor()
{
    syntaxColor = colorDialog->getColor();
}


void StyleCreatorDialog::pickCommentColor()
{
    commentColor = colorDialog->getColor();
}


void StyleCreatorDialog::pickLiteralColor()
{
    literalColor = colorDialog->getColor();
}


void StyleCreatorDialog::pickFunctionColor()
{
    functionColor = colorDialog->getColor();
}


void StyleCreatorDialog::pickBackgroundColor()
{
    backgroundColor = colorDialog->getColor();
}

void StyleCreatorDialog::pickTextColor()
{
    textColor = colorDialog->getColor();
}

void StyleCreatorDialog::setStyleName()
{
    name = styleNameEdit->text();
}


void StyleCreatorDialog::createGridGroupBox()
{
    gridGroupBox = new QGroupBox(tr("Grid layout"));
    QGridLayout *layout = new QGridLayout;


    QString buttonName{"Pick color"};
    QVector<QString> labelsNames{"Keyword's color", "Comment's color", "Literals color", "Function color", "Background color", "Text color", "Style name"};
    for (int i = 0; i < 6; ++i)
    {
        labels.push_back(new QLabel(labelsNames[i]));
        pickButton.push_back(new QPushButton);
        pickButton[i]->setText(buttonName);

        layout->addWidget(labels[i], i, 0);
        layout->addWidget(pickButton[i], i, 1);
    }


    smallEditor = new QTextEdit;
    smallEditor->setPlainText("do {i--} while(i>0) foo() 'literal' and simple text //Comment");

    layout->addWidget(smallEditor, 0, 2, 3, 1);
    styleNameEdit = new QLineEdit("unnamed");

    QLabel* nameLabel = new QLabel(labelsNames.last());
    layout->addWidget(nameLabel, 6, 0);
    layout->addWidget(styleNameEdit, 6, 1);

    layout->setColumnStretch(1, 10);
    layout->setColumnStretch(2, 30);
    gridGroupBox->setLayout(layout);
}
