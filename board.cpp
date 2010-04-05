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
#include "piece.h"
#include "square.h"
#include "mainwindow.h"

#include <QtGui>

Board::Board(QWidget *parent)
    : QWidget(parent)
{
	want_rotation = true;

	whites
		<< "4pmZCg==" /*pawn	0*/
		<< "4pmYCg==" /*knight	1*/
		<< "4pmXCg==" /*bishop	2*/
		<< "4pmWCg==" /*rook	3*/
		<< "4pmVCg==" /*queen	4*/
		<< "4pmUCg==";/*king	5*/

	blacks
		<< "4pmfCg==" /*pawn*/
		<< "4pmeCg==" /*knight*/
		<< "4pmdCg==" /*bishop*/
		<< "4pmcCg==" /*rook*/
		<< "4pmbCg==" /*queen*/
		<< "4pmaCg==";/*king*/

	MainWindow *window = qobject_cast<MainWindow*>(parent);
	connect(this, SIGNAL(setStatus(QString)), window, SLOT(setStatus(QString)));

	// load initial setup (of pieces)
	init();
	changeTurn();

	setStyleSheet("Square { font-size:50px; font-weight:bold; text-align: top center; }"
			    "QLabel { font-size:20px; }");

	setFixedSize(sizeHint());
}

void Board::init()
{
	new_game = true;
	white_turn = false;
	white_view = true;

	layout = new QGridLayout(this);
	layout->setSpacing(0);

	// axis labels
	for(int y=1, x=8; y<=8 && x>=1; y++, x--)
	{
		QLabel* w = new QLabel(QString(y+96));
		w->setStyleSheet("margin-left: 1.2em;");
		layout->addWidget(w, 0, y);

		w = new QLabel(QString(y+96));
		w->setStyleSheet("margin-left: 1.2em;");
		layout->addWidget(w, 9, y);

		w = new QLabel(QString::number(x));
		w->setStyleSheet("margin-right: 0.3em;");
		layout->addWidget(w, y, 0);

		w = new QLabel(QString::number(x));
		w->setStyleSheet("margin-left: 0.3em;");
		layout->addWidget(w, y, 9);
	}

	QString color = "white";

	for(int x=1; x<=8; x++)
	{
		int piece = -1;
		bool white = true;

		for(int y=1; y<=8; y++)
		{
			if(x == 2) // black pawn
			{
				piece = Piece::Pawn;
				white = false;
			}
			else if(x == 7) // white pawn
			{
				piece = Piece::Pawn;
				white = true;
			}
			else switch(y)
			{
			// rooks
			case 1:
			case 8:
				if(x == 1 || x == 8)
					piece = Piece::Rook;
				break;
			// knights
			case 2:
			case 7:
				if(x == 1 || x == 8)
					piece = Piece::Knight;
				break;
			// bishops
			case 3:
			case 6:
				if(x == 1 || x == 8)
					piece = Piece::Bishop;
				break;
			// queens
			case 4:
				if(x == 1 || x == 8)
					piece = Piece::Queen;
				break;
			// kings
			case 5:
				if(x == 1 || x == 8)
					piece = Piece::King;
				break;

			default:
				piece = -1;
				break;
			}

			if(x == 1)
				white = false;
			else if(x == 8)
				white = true;

			Piece *p = 0;
			QByteArray str = "";

			if(piece != -1)
			{
				p = new Piece(piece, white);

				if(white)
					str = QByteArray::fromBase64(whites[piece]);
				else
					str = QByteArray::fromBase64(blacks[piece]);
			}
			Square *square = new Square(str, p, this);
			square->setFixedSize(80, 80);
			square->setStyleSheet(QString("background-color:%1").arg(color));

			layout->addWidget(square, x, y);

			if(color == "white")
				color = "grey";
			else
				color = "white";
		}

		if(color == "white")
			color = "grey";
		else
			color = "white";
	}

	notationList = new QListWidget();
	notationList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	notationList->setFixedWidth(150);
	layout->addWidget(notationList, 0, 10, 10, 1);
}

void Board::rotateBoard()
{
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

	setUpdatesEnabled(false);

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

	setUpdatesEnabled(true);

	white_view = !white_view;
}

bool Board::isValidMove(Square *s /*source*/, Square *d /*destination*/)
{
	Piece *sp = s->piece();
	Piece *dp = d->piece();

	// there's no piece on the source square
	if(!sp)
		return false;

	// cannot capture your own piece!
	if(dp && dp->isWhite() == sp->isWhite())
		return false;

	// cannot capture the king
	if(dp && dp->letter() == "K")
		return false;

	// not your turn amigo
	if(sp->isWhite() != this->white_turn)
		return false;

	// list of valid destinations for this piece
	QStringList vd;

	Square *sq = 0;
	QString p;

	switch(sp->number())
	{
	case Piece::Pawn:
		// one step forward
		p = s->getPosition("T");
		sq = squareAt(p);
		if(sq && sq->isFree())
			vd << p;

		// two steps forward
		p = s->getPosition("T", squareAt(p));
		sq = squareAt(p);
		if(sq && sq->isFree() && !sp->hasMoved())
			vd << p;

		// left-capture
		p = s->getPosition("TL");
		sq = squareAt(p);
		if(sq && !sq->isFree())
			vd << p;

		// right-capture
		p = s->getPosition("TR");
		sq = squareAt(p);
		if(sq && !sq->isFree())
			vd << p;

		break;

	default:
		break;
	}

	qDebug() << vd;
	if(vd.count() > 0 && !vd.contains(d->position()))
		return false;

	return true;
}

void Board::makeMove(Square *s /*source*/, Square *d /*destination*/)
{
	// uncheck hack
	s->click();
	s->repaint();

	if(!isValidMove(s, d))
		return;

	// capture?
	QString sign = d->piece() ? "x" : "-";

	// move notation
	QString move = s->piece()->letter() + s->position() + sign + d->position();

	// set new piece
	d->setPiece(s->piece());

	// remove old piece
	s->setPiece(0);

	// just to make it shorter and clearer
	QListWidget* l = notationList;

	// notation
	if(white_turn)
	{
		QString number = QString::number( l->count()+1 );
		l->addItem( number + ". " + move );
	} else
	{
		QListWidgetItem* lastItem = l->item( l->count()-1 );
		QString text = lastItem->text();
		lastItem->setText( text + " " + move );
	}

	new_game = false;

	changeTurn();
}

void Board::makeNotationMove(const QString &notation)
{
	qDebug() << "making move (not yet implemented):" << notation;
}

Square *Board::squareAt(QString position)
{
	Square* square;

	for(int x=1; x<=8; x++)
	{
		for(int y=1; y<=8; y++)
		{
			square = qobject_cast<Square*>(layout->itemAtPosition(x, y)->widget());
			if(square && square->position() == position)
			{
				return square;
			}
		}
	}

	return 0;
}

void Board::changeTurn()
{
	white_turn = !white_turn;

	if(!new_game && want_rotation)
	{
		if((white_turn && !white_view)
		||(!white_turn &&  white_view))
		{
			rotateBoard();
		}
	}

	QString status = white_turn ?
			 tr("White moves.") :
			 tr("Black moves.") ;
	emit setStatus(status);
}

void Board::setRotationEnabled(bool r)
{
	want_rotation = r;
}
