#ifndef FINDDIALOG_H
#define FINDDIALOG_H
//! [FindDialog header]
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPushButton>
QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
QT_END_NAMESPACE


class FindDialog : public QDialog
{
    Q_OBJECT

public:
    enum mode{FIND, FIND_AND_REPLACE};
    mode currentMode;

    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;

private:
    QString findText;
    QString textToReplace;
    QLabel *findLabel;

    QLineEdit *lineEdit;
    QLineEdit *replaceLineEdit;

public:
    FindDialog(QWidget *parent, mode currentMode);
    QString getFindText()const;
    QString getReplaceText()const;
    void findAndHighlight(QString text);

private:
    void createFindMenu();
    void createReplaceMenu();

public slots:
    void setFindText();
    void setReplaceText();
};
#endif
