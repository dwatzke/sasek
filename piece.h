#ifndef PIECE_H
#define PIECE_H

#include <QObject>
#include <QStringList>

class Square;

class Piece : public QObject
{
	friend class Square;
	friend class Board;

public:
	Piece(int piece, bool white);

	int number() const { return m_piece; }
	QString letter() const;

private:
	bool can_00; // kingside castling
	bool can_000;
	bool m_white;
	bool ep_endangered;
	int m_moved;
	int m_piece;
	Square* square;

	bool isWhite()  { return m_white; }
	bool hasMoved() { return m_moved; }

	bool isPawn()   { return m_piece == Pawn;   }
	bool isKnight() { return m_piece == Knight; }
	bool isBishop() { return m_piece == Bishop; }
	bool isRook()   { return m_piece == Rook;   }
	bool isQueen()  { return m_piece == Queen;  }
	bool isKing()   { return m_piece == King;   }

	enum Pieces
	{
		Pawn   = 0,
		Knight = 1,
		Bishop = 2,
		Rook   = 3,
		Queen  = 4,
		King   = 5
	};

	QStringList letters;
};

#endif // PIECE_H
