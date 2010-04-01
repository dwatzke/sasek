#ifndef SQUARE_H
#define SQUARE_H

#include <QPushButton>
#include <QPoint>

class Board;
class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;

class Square : public QPushButton
{
Q_OBJECT

friend class Board;

public:
	explicit Square(QString text, QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

private:
	Board *board;
	QPoint dragStartPosition;

	QString pieceLetter();
	QString position();
};

#endif
