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

#ifndef BOARD_H
#define BOARD_H

#include <QWidget>

class QGridLayout;
class QListWidget;
class Square;

class Board : public QWidget
{
Q_OBJECT

	friend class MainWindow;
	friend class Square;
	//friend class Piece;

public:
	Board(QWidget *parent = 0);

private:
	QGridLayout* layout;
	bool new_game;
	bool want_rotation;
	bool white_view;
	bool white_turn;
	QList<QByteArray> whites;
	QList<QByteArray> blacks;
	QListWidget *notationList;
	Square *squareAt(const QString &position);

	void init();
	void checkGameStatus(Square* source, Square* dest, const QString &sign);
	bool isValidMove(Square *source, Square *dest);
	void makeMove(Square *source, Square *dest);
	void makeNotationMove(const QString &notation);

	void changeTurn();

private slots:
	void rotateBoard();
	void setRotationEnabled(bool);

signals:
	void setStatus(QString);
};

#endif // BOARD_H
