#include "aboutdialog.h"

aboutDialog::aboutDialog(QWidget*parent):QDialog(parent)
{

    acceptButton = new QPushButton(tr("&Ok"));

    layout = new QVBoxLayout{this};
    image =  QPixmap{"myPhoto.jpg"};
    developerPhotoLabel = new QLabel(this);
    developerPhotoLabel->setPixmap(image);
    developerInfoLabel = new QLabel("Arseniy");
    buildInfoLabel = new QLabel("Build with Qt 6, run on Qt 6. Builded 03.12.2021");
    layout->addWidget(developerPhotoLabel);
    layout->addWidget(developerInfoLabel);
    layout->addWidget(buildInfoLabel);
    layout->setSpacing(10);
    connect(acceptButton, &QPushButton::clicked, this, &QDialog::close);
    layout->addWidget(acceptButton);
    setLayout(layout);
    setWindowTitle(tr("About"));
}
