#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QLineEdit;

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

friend class Board;

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::MainWindow *ui;

	QLabel* statusLabel;
	QLineEdit* notationEdit;

private slots:
	void sendMove();
	void newGame();

public slots:
	void setStatus(const QString& status);
};

#endif // MAINWINDOW_H
