#include "cell.h"

Cell::Cell(int row, int col, QWidget *parent)
    : QPushButton(parent), m_row(row), m_col(col)
{
    setMinimumSize(10, 10);;
    setMouseTracking(true);
    setStyleSheet(R"(
    QPushButton {
        background-color: lightblue;
        border: 1px solid gray;
        margin: 0px;
        padding: 0px;
    }
    )");
}

int Cell::row() const { return m_row; }
int Cell::col() const { return m_col; }

void Cell::enterEvent(QEnterEvent *event) {
    emit hovered(m_row, m_col);
    QPushButton::enterEvent(event);
}

void Cell::leaveEvent(QEvent *event) {
    emit hoverLeft(m_row, m_col);
    QPushButton::leaveEvent(event);
}

void Cell::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::RightButton) {
        emit rightClicked(m_row, m_col);
    } else {
        QPushButton::mousePressEvent(event); // Для левого клика и др.
    }
}
