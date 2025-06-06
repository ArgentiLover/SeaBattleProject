#include "customizationmenu.h"
#include "ui_customizationmenu.h"
#include "chooseopponentmenu.h"
#include "shipplacementmenu.h"
#include "GameSettings.h"

#include <QPropertyAnimation>

CustomizationMenu::CustomizationMenu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CustomizationMenu)
{
    ui->setupUi(this);
    connect(ui->horizontalSlider_time, &QSlider::valueChanged, this, [=](int value){
        ui->label_time->setText(QString::number(value));
    });
}

CustomizationMenu::~CustomizationMenu()
{
    delete ui;
}

void CustomizationMenu::on_pushButton_return_clicked()
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

void CustomizationMenu::on_pushButton_startGame_clicked()
{
    int fieldSize = ui->spinBox_fieldSize->value();
    bool shipsCanBeAdjacent = ui->comboBox_ships_placement->currentText().startsWith("М");
    int ship1Count = ui->spinBox_ships_1->value();
    int ship2Count = ui->spinBox_ships_2->value();
    int ship3Count = ui->spinBox_ships_3->value();
    int ship4Count = ui->spinBox_ships_4->value();

    if (ship1Count + ship2Count + ship3Count + ship4Count == 0) return;

    QString shootingRule = ui->comboBox_shooting->currentText();
    int timeLimit = ui->horizontalSlider_time->value();


    GameSettings::setFieldSize(fieldSize);
    GameSettings::setShipsCanBeAdjacent(shipsCanBeAdjacent);
    GameSettings::setShipCounts(ship1Count, ship2Count, ship3Count, ship4Count);
    GameSettings::setShootingRule(shootingRule);
    GameSettings::setTimeLimit(timeLimit);

    QPropertyAnimation *fadeOut = new QPropertyAnimation(this, "windowOpacity");
    fadeOut->setDuration(200);
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.0);
    fadeOut->start(QAbstractAnimation::DeleteWhenStopped);

    ShipPlacementMenu *shipPlacementMenu = new ShipPlacementMenu();
    this->close();
    shipPlacementMenu->show();
    QPropertyAnimation *fadeIn = new QPropertyAnimation(shipPlacementMenu, "windowOpacity");
    fadeIn->setDuration(200);
    fadeIn->setStartValue(0.0);
    fadeIn->setEndValue(1.0);
    fadeIn->start(QAbstractAnimation::DeleteWhenStopped);
}

