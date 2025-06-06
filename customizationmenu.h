#ifndef CUSTOMIZATIONMENU_H
#define CUSTOMIZATIONMENU_H

#include <QMainWindow>

namespace Ui {
class CustomizationMenu;
}

class CustomizationMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit CustomizationMenu(QWidget *parent = nullptr);
    ~CustomizationMenu();

private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_startGame_clicked();

private:
    Ui::CustomizationMenu *ui;
};

#endif // CUSTOMIZATIONMENU_H
