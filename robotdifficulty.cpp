#include "robotdifficulty.h"
#include "ui_robotdifficulty.h"
#include "chooseopponentmenu.h"
#include "customizationmenu.h"
#include "GameSettings.h"

#include <QPropertyAnimation>

RobotDifficulty::RobotDifficulty(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RobotDifficulty)
{
    ui->setupUi(this);
}

RobotDifficulty::~RobotDifficulty()
{
    delete ui;
}

void RobotDifficulty::on_pushButton_return_clicked()
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


void RobotDifficulty::on_pushButton_easy_clicked()
{
    GameSettings::difficulty = Difficulty::Easy;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    CustomizationMenu *customizationMenu = new CustomizationMenu();
    this->close();
    customizationMenu->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(customizationMenu, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}


void RobotDifficulty::on_pushButton_normal_clicked()
{
    GameSettings::difficulty = Difficulty::Medium;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    CustomizationMenu *customizationMenu = new CustomizationMenu();
    this->close();
    customizationMenu->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(customizationMenu, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}


void RobotDifficulty::on_pushButton_hard_clicked()
{
    GameSettings::difficulty = Difficulty::Hard;
    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    CustomizationMenu *customizationMenu = new CustomizationMenu();
    this->close();
    customizationMenu->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(customizationMenu, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);  // Начальная прозрачность 0%
    fadeIn->setEndValue(1.0);    // Конечная прозрачность 100%
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

