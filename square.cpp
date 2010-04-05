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

#include "square.h"
#include "piece.h"
#include "board.h"

#include <QtGui>

Square::Square(QString text, Piece* xpiece, QWidget *parent)
	: QPushButton(text, parent)
{
	board = qobject_cast<Board*>(parent);
	m_piece = xpiece;
	if(m_piece)
		m_piece->square = this;

	this->setAcceptDrops(true);
	this->setCheckable(false);
}

bool Square::isFree()
{
	return m_piece == 0;
}

/* id (relative view for each color of piece)
TL    T   TR

 L  piece  R

BL    B   BR

example: getPosition("TL") for white piece at c5 is b6
*/
QString Square::getPosition(QString id, Square *s)
{
	bool wv = board->white_view;

	board->setUpdatesEnabled(false);

	if(!wv)
		board->rotateBoard();

	QString d;
	if(s == 0)
		d = this->position();
	else
		d = s->position();

	qDebug() << "starting position:" << d;

	int col = (int)d[0].toAscii();
	int row = QString(d[1]).toInt();

	int up = board->white_turn ? 1 : -1;
	int down = -up;
	int right = up;
	int left = down;

	if(id.contains("T"))
		row += up;

	if(id.contains("B"))
		row += down;

	if(id.contains("L"))
		col += left;

	if(id.contains("R"))
		col += right;

	QString p = QChar(col) + QString::number(row);

	if(wv != board->white_view)
		board->rotateBoard();

	board->setUpdatesEnabled(true);

	qDebug() << "final position:" << p;

	return p;
}

QString Square::position()
{
	QGridLayout* layout = board->layout;
	int idx = layout->indexOf(this);
	int x, y, t1, t2;
	layout->getItemPosition(idx, &x, &y, &t1, &t2);

	QString row = qobject_cast<QLabel*>(layout->itemAtPosition(x, 0)->widget())->text();
	QString col = qobject_cast<QLabel*>(layout->itemAtPosition(0, y)->widget())->text();

	return col+row;
}

void Square::setPiece(Piece *piece)
{
	m_piece = piece;

	if(!piece)
	{
		setText("");
		return;
	}

	QString text;
	int no = piece->number();

	if(piece->isWhite())
		text = board->whites[no];
	else
		text = board->blacks[no];

	piece->m_moved = true;

	this->setText(QByteArray::fromBase64(text.toUtf8()));
}

void Square::mousePressEvent(QMouseEvent *e)
{
	QPushButton::mousePressEvent(e);

	if(this->isFree())
		return;

	if(e->button() == Qt::LeftButton)
		dragStartPosition = e->pos();

	e->accept();
}

void Square::mouseMoveEvent(QMouseEvent *e)
{
	if(this->isFree())
		return;

	if(!(e->buttons() & Qt::LeftButton))
		return;

	if((e->pos() - dragStartPosition).manhattanLength() < QApplication::startDragDistance())
		return;

	QDrag* drag = new QDrag(this);

	QMimeData* mime = new QMimeData();
	mime->setText(this->text());
	drag->setMimeData(mime);

	drag->exec(Qt::MoveAction);
}

void Square::dragEnterEvent(QDragEnterEvent *e)
{
	e->acceptProposedAction();
}

void Square::dropEvent(QDropEvent *e)
{
	if(!e->source())
		return;

	// get source square
	Square* s = qobject_cast<Square*>(e->source());

	board->makeMove(s, this);
}
