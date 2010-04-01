#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	statusLabel = new QLabel(this);
	this->setStatusBar(new QStatusBar);
	this->statusBar()->addWidget(statusLabel, 10);
	this->statusBar()->setSizeGripEnabled(false);

	notationEdit = new QLineEdit(this);
	QPushButton* notationOk = new QPushButton(tr("OK"));
	connect(notationEdit, SIGNAL(returnPressed()), notationOk, SLOT(click()));
	connect(notationOk, SIGNAL(clicked()), this, SLOT(sendMove()));

	QToolBar* toolBar = this->addToolBar(tr("Main toolbar"));
	toolBar->addWidget(notationEdit);
	toolBar->addWidget(notationOk);

	ui->setupUi(this);
	this->setFixedSize(this->sizeHint());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::newGame()
{
	delete ui->board;
	ui->board = new Board(this);
	this->setCentralWidget(ui->board);
}

void MainWindow::sendMove()
{
	ui->board->makeMove(notationEdit->text());
}

void MainWindow::setStatus(const QString& status)
{
	statusLabel->setText(status);
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}
