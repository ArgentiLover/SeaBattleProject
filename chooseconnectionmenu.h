#ifndef CHOOSECONNECTIONMENU_H
#define CHOOSECONNECTIONMENU_H

#include <QMainWindow>

namespace Ui {
class ChooseConnectionMenu;
}

class ChooseConnectionMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChooseConnectionMenu(QWidget *parent = nullptr);
    ~ChooseConnectionMenu();

private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_single_compute_clicked();

    void on_pushButton_remote_clicked();

private:
    Ui::ChooseConnectionMenu *ui;
};

#endif // CHOOSECONNECTIONMENU_H
