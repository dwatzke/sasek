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

class MainWindow;
class QListWidget;
class Square;

class Board : public QWidget
{
Q_OBJECT

	friend class MainWindow;
	friend class Square;

public:
	Board(QWidget *parent = 0);
	~Board();

	const QStringList whites() { return m_whites; }
	const QStringList blacks() { return m_blacks; }
	void changeTurn();

private:
	MainWindow *window;
	bool new_game;
	bool want_rotation;
	bool white_view;
	bool white_turn;
	bool rochade_possible;
	QStringList m_whites;
	QStringList m_blacks;
	QListWidget *notationList;

	void init();
	void makeMove(Square* source, Square* dest);
	void makeNotationMove(const QString &notation);

private slots:
	void rotateBoard();
	void setRotationEnabled(bool);

signals:
	void setStatus(QString);
	//void turnChanged();
};

#endif // BOARD_H
