#include "chooseconnectionmenu.h"
#include "ui_chooseconnectionmenu.h"
#include "chooseopponentmenu.h"
#include "customizationmenu.h"
#include "GameSettings.h"

#include <QPropertyAnimation>

ChooseConnectionMenu::ChooseConnectionMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ChooseConnectionMenu)
{
    ui->setupUi(this);
}

ChooseConnectionMenu::~ChooseConnectionMenu()
{
    delete ui;
}

void ChooseConnectionMenu::on_pushButton_return_clicked()
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


void ChooseConnectionMenu::on_pushButton_single_compute_clicked()
{
    GameSettings::platform = PlatformType::SinglePC;
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


void ChooseConnectionMenu::on_pushButton_remote_clicked()
{
    return;

    //остальное не отрабатывает
    GameSettings::platform = PlatformType::LocalMultiplayer;
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

