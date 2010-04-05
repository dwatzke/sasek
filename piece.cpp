#include "piece.h"

#include <QtGui>

Piece::Piece(int piece, bool white)
{
	can_00 = true;
	can_000 = true;
	m_white = white;
	ep_endangered = false;
	m_moved = false;
	m_piece = piece;

	letters << "" << tr("N", "knight") << tr("B", "bishop") << tr("R", "rook") << tr("Q", "queen") << tr("K", "king");
}

QString Piece::letter() const
{
	QString letter = QString::number(this->number());

	for(int i=Piece::Pawn; i<=Piece::King; i++)
		letter.replace(QString::number(i), letters[i]);

	return letter;
}
