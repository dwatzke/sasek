/* Šášek - a Qt little chess game written just for fun
   Copyright (C) 2010  David Watzke <sasek@watzke.cz>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef SQUARE_H
#define SQUARE_H

#include <QPushButton>
#include <QPoint>

class Board;
class Piece;
class QMouseEvent;
class QDragEnterEvent;
class QDragMoveEvent;
class QDropEvent;

class Square : public QPushButton
{
Q_OBJECT

	friend class Board;
	friend class Piece;

public:
	explicit Square(QString text, Piece *piece = 0, QWidget *parent = 0);

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void dragEnterEvent(QDragEnterEvent *e);
	void dropEvent(QDropEvent *e);

private:
	Board *board;
	Piece *m_piece;
	QPoint dragStartPosition;

	bool isFree();
	QString getPosition(QString id, Square *s = 0);
	Piece *piece() { return m_piece; };
	//QString pieceLetter();
	QString position();
	QByteArray text64()
	{
		return this->text().toUtf8().toBase64();
	}
	void setPiece(Piece *piece);
};

#endif
