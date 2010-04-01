#include "square.h"
#include "board.h"

#include <QtGui>

Square::Square(QString text, QWidget *parent)
	: QPushButton(text, parent)
{
	board = qobject_cast<Board*>(parent);

	this->setAcceptDrops(true);
	this->setCheckable(false);
}

QString Square::pieceLetter()
{
	if(this->text().isEmpty())
		return QString();

	QString letter = ""; // pawn
	QByteArray piece = this->text().toUtf8().toBase64();

	if(piece == board->whites().at(1) || piece == board->blacks().at(1))
		letter = tr("N", "knight");
	if(piece == board->whites().at(2) || piece == board->blacks().at(2))
		letter = tr("B", "bishop");
	if(piece == board->whites().at(3) || piece == board->blacks().at(3))
		letter = tr("R", "rook");
	if(piece == board->whites().at(4) || piece == board->blacks().at(4))
		letter = tr("Q", "queen");
	if(piece == board->whites().at(5) || piece == board->blacks().at(5))
		letter = tr("K", "king");

	return letter;
}

QString Square::position()
{
	QGridLayout* layout = qobject_cast<QGridLayout*>(board->layout());
	int idx = layout->indexOf(this);
	int x, y, t1, t2;
	layout->getItemPosition(idx, &x, &y, &t1, &t2);

	QString row = qobject_cast<QLabel*>(layout->itemAtPosition(x, 0)->widget())->text();
	QString col = qobject_cast<QLabel*>(layout->itemAtPosition(0, y)->widget())->text();

	return col+row;
}

void Square::mousePressEvent(QMouseEvent *e)
{
	QPushButton::mousePressEvent(e);

	if(this->text().isEmpty())
		return;

	if(e->button() == Qt::LeftButton)
		dragStartPosition = e->pos();

	e->accept();
}

void Square::mouseMoveEvent(QMouseEvent *e)
{
	if(this->text().isEmpty())
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
