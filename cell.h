#ifndef CELL_H
#define CELL_H


#include <QPushButton>
#include <QMouseEvent>

class Cell : public QPushButton
{
    Q_OBJECT
public:
    explicit Cell(int row, int col, QWidget *parent = nullptr);
    int row() const;
    int col() const;
private:
    int m_row;
    int m_col;
signals:
    void hovered(int row, int col);
    void hoverLeft(int row, int col);
    void rightClicked(int row, int col);
protected:
    void enterEvent(QEnterEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CELL_H
