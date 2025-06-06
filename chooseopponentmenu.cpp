#include "chooseopponentmenu.h"
#include "ui_chooseopponentmenu.h"
#include "mainwindow.h"
#include "chooseconnectionmenu.h"
#include "robotdifficulty.h"
#include "GameSettings.h"

#include <QPropertyAnimation>

ChooseOpponentMenu::ChooseOpponentMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChooseOpponentMenu)
{
    ui->setupUi(this);
}

ChooseOpponentMenu::~ChooseOpponentMenu()
{
    delete ui;
}

void ChooseOpponentMenu::on_pushButton_human_clicked()
{
    GameSettings::opponent = OpponentType::Human;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    ChooseConnectionMenu *chooseConnectionMenu = new ChooseConnectionMenu();
    this->close();
    chooseConnectionMenu->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(chooseConnectionMenu, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}


void ChooseOpponentMenu::on_pushButton_return_clicked()
{
    GameSettings::opponent = OpponentType::Robot;

    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    MainWindow *mainWindow = new MainWindow();
    this->close();
    mainWindow->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(mainWindow, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}


void ChooseOpponentMenu::on_pushButton_robot_clicked()
{
    GameSettings::opponent = OpponentType::Robot;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    RobotDifficulty *robotDifficulty = new RobotDifficulty();
    this->close();
    robotDifficulty->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(robotDifficulty, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

