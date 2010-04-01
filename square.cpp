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

void Square::dropEvent(QDropEvent *e)
{
	if(!e->source())
		return;

	// get source square
	Square* s = qobject_cast<Square*>(e->source());

	// uncheck
	s->click();
	s->repaint();

	// return on a teamkill try
	if((board->whites().contains(s->text().toUtf8().toBase64()) &&
	    board->whites().contains(this->text().toUtf8().toBase64()))
	|| (board->blacks().contains(s->text().toUtf8().toBase64()) &&
	    board->blacks().contains(this->text().toUtf8().toBase64())))
		{
			return;
		}

	// check if we're moving our piece
	if((board->white_turn && !board->whites().contains(s->text().toUtf8().toBase64()))
	|| (!board->white_turn && !board->blacks().contains(s->text().toUtf8().toBase64())))
		{
			return;
		}

	// remove piece
	s->setText("");

	// set new
	this->setText(e->mimeData()->text());

	QListWidget* l = board->notationList;

	if(board->white_turn)
	{
		QString number = QString::number( l->count()+1 );
		l->addItem( number + ". " + s->position() + "-" + this->position() );
	} else
	{
		QListWidgetItem* lastItem = l->item(l->count()-1);
		QString text = lastItem->text();
		lastItem->setText( text + " " + s->position() + "-" + this->position() );
	}

	board->new_game = false;

	// update status
	board->changeTurn();
}
