#ifndef CHOOSEOPPONENTMENU_H
#define CHOOSEOPPONENTMENU_H

#include <QMainWindow>

namespace Ui {
class ChooseOpponentMenu;
}

class ChooseOpponentMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseOpponentMenu(QWidget *parent = nullptr);
    ~ChooseOpponentMenu();

private slots:
    void on_pushButton_human_clicked();

    void on_pushButton_return_clicked();

    void on_pushButton_robot_clicked();

private:
    Ui::ChooseOpponentMenu *ui;
};

#endif // CHOOSEOPPONENTMENU_H
