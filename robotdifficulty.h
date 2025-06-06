#ifndef ROBOTDIFFICULTY_H
#define ROBOTDIFFICULTY_H

#include <QMainWindow>

namespace Ui {
class RobotDifficulty;
}

class RobotDifficulty : public QMainWindow
{
    Q_OBJECT

public:
    explicit RobotDifficulty(QWidget *parent = nullptr);
    ~RobotDifficulty();

private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_easy_clicked();

    void on_pushButton_normal_clicked();

    void on_pushButton_hard_clicked();

private:
    Ui::RobotDifficulty *ui;
};

#endif // ROBOTDIFFICULTY_H
