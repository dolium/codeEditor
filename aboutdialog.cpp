#include "aboutdialog.h"

aboutDialog::aboutDialog(QWidget*parent):QDialog(parent)
{


    image =  QPixmap{"C:/Users/ivbon/OneDrive/Documents/Programming/codeeditor_3/imgs/myPhoto.jpg"};
    developerPhotoLabel = new QLabel(this);
    developerPhotoLabel->setPixmap(image);
    developerInfoLabel = new QLabel("Arseniy");
    buildInfoLabel = new QLabel("Build with Qt 5.15.2, run on Qt 5.15.2. Builded 03.12.2021");

    acceptButton = new QPushButton(tr("&Ok"));
    connect(acceptButton, &QPushButton::clicked, this, &QDialog::close);

    layout = new QVBoxLayout{this};
    layout->addWidget(developerPhotoLabel);
    layout->addWidget(developerInfoLabel);
    layout->addWidget(buildInfoLabel);
    layout->setSpacing(10);
    layout->addWidget(acceptButton);
    setLayout(layout);

    setWindowTitle(tr("About"));
}
