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
