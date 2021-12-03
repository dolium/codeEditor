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
    void createGridGroupBox();
    QGroupBox *gridGroupBox;
    QPushButton *acceptButton;
    QLabel* versionLabel;
    QLabel* developerInfoLabel;
    QLabel* developerPhotoLabel;
    QVBoxLayout* layout;
    QLabel * buildInfoLabel;
    QPixmap image;
};

#endif // ABOUTDIALOG_H
