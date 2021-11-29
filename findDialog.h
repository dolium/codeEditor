#ifndef FINDDIALOG_H
#define FINDDIALOG_H
//! [FindDialog header]
#include <QDialog>
#include <QLabel>
#include <QPlainTextEdit>
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
    QString getFindText();
    QString getReplaceText();
    QString textToReplace;
    QLabel *findLabel;
    mode currentMode;
    QPushButton *findButton;
    QPushButton *replaceButton;
    QPushButton *replaceAllButton;
    QLineEdit *lineEdit;
    QLineEdit *replaceLineEdit;

    void findAndHighlight(QString text);
public slots:
    void setFindText();
    void setReplaceText();
private:

    QString findText;
    void createFindMenu();
    void createReplaceMenu();
};
#endif
