#include "findDialog.h"

FindDialog::FindDialog(QWidget *parent, mode currentMode)
    : QDialog(parent)
{

    this->currentMode = currentMode;

    if (currentMode==mode::FIND_AND_REPLACE)
       createReplaceMenu();
    else if (currentMode==mode::FIND)
        createFindMenu();


    connect(lineEdit, &QLineEdit::textChanged, this, &FindDialog::setFindText);
    if (currentMode == mode::FIND_AND_REPLACE)
        connect(replaceLineEdit,&QLineEdit::textChanged, this, &FindDialog::setReplaceText);

}


void FindDialog::setFindText()
{
    QString text = lineEdit->text();

    if (text.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"), tr("Please enter text to find\replace"));
        return;
    }
    else
    {
        findText = text;
    }
}


void FindDialog::setReplaceText()
{
    textToReplace = replaceLineEdit->text();
}


void FindDialog::createReplaceMenu()
{
    findLabel = new QLabel(tr("Find and replace:"));

    lineEdit = new QLineEdit;
    replaceLineEdit = new QLineEdit;

    findButton = new QPushButton(tr("&Find next"));
    replaceButton = new QPushButton (tr("Replace"));
    replaceAllButton = new QPushButton(tr("Replace All"));

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(replaceLineEdit);
    layout->addWidget(findButton);
    layout->addWidget(replaceButton);
    layout->addWidget(replaceAllButton);

    setLayout(layout);
    setWindowTitle(tr("Find and replace"));

}


void FindDialog::createFindMenu()
{
    findLabel = new QLabel(tr("Find:"));
    lineEdit = new QLineEdit;
    findButton = new QPushButton(tr("&Find next"));

    QHBoxLayout *layout = new QHBoxLayout;

    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(findButton);

    setWindowTitle(tr("Find"));
    setLayout(layout);
}


QString FindDialog::getFindText() const
{
    return findText;
}


QString FindDialog::getReplaceText() const
{
    return textToReplace;
}
