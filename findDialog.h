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
    FindDialog(QWidget *parent, mode currentMode);
    mode currentMode;

    QString getFindText()const;
    QString getReplaceText()const;

    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;

    void findAndHighlight(QString text);
public slots:
    void setFindText();
    void setReplaceText();
private:
    QString findText;
    void createFindMenu();
    void createReplaceMenu();
    QString textToReplace;
    QLabel *findLabel;
    QLineEdit *lineEdit;
    QLineEdit *replaceLineEdit;
};
#endif
