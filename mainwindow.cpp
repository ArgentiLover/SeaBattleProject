#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "chooseopponentmenu.h"

#include <QPropertyAnimation>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_start_clicked()
{
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    ChooseOpponentMenu *chooseOpponentMenu = new ChooseOpponentMenu();
    this->close();
    chooseOpponentMenu->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(chooseOpponentMenu, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

