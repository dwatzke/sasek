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

#include "board.h"
#include "square.h"
#include "mainwindow.h"

#include <QtGui>

Board::Board(QWidget *parent)
    : QWidget(parent)
{
	this->window = qobject_cast<MainWindow*>(parent);
	this->want_rotation = true;

	this->m_whites
		<< "4pmZCg==" /*pawn	0*/
		<< "4pmYCg==" /*knight	1*/
		<< "4pmXCg==" /*bishop	2*/
		<< "4pmWCg==" /*rook	3*/
		<< "4pmVCg==" /*queen	4*/
		<< "4pmUCg==";/*king	5*/

	this->m_blacks
		<< "4pmfCg==" /*pawn*/
		<< "4pmeCg==" /*knight*/
		<< "4pmdCg==" /*bishop*/
		<< "4pmcCg==" /*rook*/
		<< "4pmbCg==" /*queen*/
		<< "4pmaCg==";/*king*/

	connect(this, SIGNAL(setStatus(QString)), window, SLOT(setStatus(QString)));

	// load initial setup (of pieces)
	this->init();
	this->changeTurn();

	this->setStyleSheet("Square { font-size:50px; font-weight:bold; text-align: top center; }"
			    "QLabel { font-size:20px; }");

	this->setFixedSize(this->sizeHint());
}

void Board::init()
{
	this->new_game = true;
	this->white_turn = false;
	this->white_view = true;
	this->rochade_possible = true;

	QGridLayout *layout = new QGridLayout(this);
	layout->setSpacing(0);

	QString color = "white";
	QString piece;

	// axis labels
	for(int y=1, x=8; y<=8 && x>=1; y++, x--)
	{
		QLabel* w = new QLabel(QString(y+96));
		w->setStyleSheet("margin-left: 28px;");
		layout->addWidget(w, 0, y);

		w = new QLabel(QString(y+96));
		w->setStyleSheet("margin-left: 28px;");
		layout->addWidget(w, 9, y);

		w = new QLabel(QString::number(x));
		w->setStyleSheet("margin-right: 7px;");
		layout->addWidget(w, y, 0);

		w = new QLabel(QString::number(x));
		w->setStyleSheet("margin-left: 7px;");
		layout->addWidget(w, y, 9);
	}

	for(int x=1; x<=8; x++)
	{
		for(int y=1; y<=8; y++)
		{
			if(x == 2) // black pawn
				piece = m_blacks[0];
			else if(x == 7) // white pawn
				piece = m_whites[0];
			else switch(y)
			{
			// rooks
			case 1:
			case 8: {
				if(x == 1) // black rook
					piece = m_blacks[3];
				else if(x == 8) // white rook
					piece = m_whites[3];

				break;
				}
			// knights
			case 2:
			case 7: {
				if(x == 1) // black
					piece = m_blacks[1];
				else if(x == 8) // white
					piece = m_whites[1];

				break;
				}
			// bishops
			case 3:
			case 6: {
				if(x == 1) // black
					piece = m_blacks[2];
				else if(x == 8) // white
					piece = m_whites[2];

				break;
				}
			// queens
			case 4: {
				if(x == 1) // black
					piece = m_blacks[4];
				else if(x == 8) // white
					piece = m_whites[4];

				break;
				}
			// kings
			case 5: {
				if(x == 1) // black
					piece = m_blacks[5];
				else if(x == 8) // white
					piece = m_whites[5];

				break;
				}
			default:
				piece.clear();
				break;
			}

			QByteArray p = QByteArray::fromBase64(piece.toUtf8());
			Square *b = new Square(p, this);
			b->setFixedSize(80, 80);
			b->setStyleSheet(QString("background-color:%1").arg(color));
			if(color == "white")
				color = "grey";
			else
				color = "white";

			layout->addWidget(b, x, y);
		}

		if(color == "white")
			color = "grey";
		else
			color = "white";

		piece = "";
	}

	notationList = new QListWidget();
	notationList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	notationList->setFixedWidth(150);
	layout->addWidget(notationList, 0, 10, 10, 1);
}

void Board::rotateBoard()
{
	QGridLayout* layout = qobject_cast<QGridLayout*>( this->layout() );
	QList<QWidget*> squares;

	for(int x=0; x<=9; x++)
	{
		for(int y=0; y<=9; y++)
		{
			if((x==0 && y==0) || (x==0 && y==9) || (x==9 && y==0) || (x==9 && y==9))
				continue;
			squares << layout->itemAtPosition(x, y)->widget();
		}
	}

	this->setUpdatesEnabled(false);

	for(int x=9; x>=0; x--)
	{
		for(int y=9; y>=0; y--)
		{
			if((x==0 && y==0) || (x==0 && y==9) || (x==9 && y==0) || (x==9 && y==9))
				continue;
			QWidget* w = squares.takeFirst();
			if(y == 0 || y == 9)
				w->setStyleSheet(w->styleSheet().replace("left","<<").replace("right",">>")
								.replace("<<","right").replace(">>","left"));
			layout->addWidget(w, x, y);
		}
	}

	this->setUpdatesEnabled(true);

	this->white_view = !this->white_view;
}

void Board::makeMove(Square *s /*source*/, Square *d /*destination*/)
{
	// uncheck hack
	s->click();
	s->repaint();

	QByteArray st = s->text().toUtf8().toBase64();
	QByteArray dt = d->text().toUtf8().toBase64();

	// return on a teamkill try
	if((this->whites().contains(st) &&
	    this->whites().contains(dt))
	|| (this->blacks().contains(st) &&
	    this->blacks().contains(dt)))
		{
			return;
		}

	// check if we're moving our piece
	if((this->white_turn && !this->whites().contains(st))
	||(!this->white_turn && !this->blacks().contains(st)))
		{
			return;
		}

	// cannot capture the king ya fool
	// TODO: render this check useless (implement mate detection)
	if(dt == this->whites().at(5) || dt == this->blacks().at(5))
		return;

	// capture?
	QString sign = d->text().isEmpty() ? "-" : "x";

	// move notation
	QString move = s->pieceLetter() + s->position() + sign + d->position();

	// set new piece
	d->setText(s->text());

	// remove old piece
	s->setText("");

	// just to make it shorter and clearer
	QListWidget* l = this->notationList;

	// notation
	if(this->white_turn)
	{
		QString number = QString::number( l->count()+1 );
		l->addItem( number + ". " + move );
	} else
	{
		QListWidgetItem* lastItem = l->item( l->count()-1 );
		QString text = lastItem->text();
		lastItem->setText( text + " " + move );
	}

	this->new_game = false;

	// update status
	this->changeTurn();
}

void Board::makeNotationMove(const QString &notation)
{
	qDebug() << "making move (not yet implemented):" << notation;
}

void Board::changeTurn()
{
	this->white_turn = !this->white_turn;

	if(!new_game && want_rotation)
	{
		if((this->white_turn && !this->white_view)
		||(!this->white_turn &&  this->white_view))
		{
			this->rotateBoard();
		}
	}

	QString status = this->white_turn ?
			 tr("White moves.") :
			 tr("Black moves.") ;
	emit setStatus(status);
}

void Board::setRotationEnabled(bool r)
{
	this->want_rotation = r;
}

Board::~Board()
{

}
