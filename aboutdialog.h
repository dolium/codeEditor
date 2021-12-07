#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGroupBox>
class aboutDialog : public QDialog
{
    Q_OBJECT
public:
    aboutDialog(QWidget *parent);
private:
    QVBoxLayout* layout;
    QLabel* versionLabel;
    QLabel* developerInfoLabel;
    QLabel* developerPhotoLabel;
    QLabel* buildInfoLabel;
    QPushButton *acceptButton;
    QPixmap image;
    void createGridGroupBox();
};

#endif // ABOUTDIALOG_H
