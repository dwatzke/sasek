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

	notationEdit = new QLineEdit();
	notationCombo = new QComboBox();
	notationCombo->addItem(tr("Long algebraic notation"));
	QPushButton* notationOk = new QPushButton(tr("Move"));
	connect(notationEdit, SIGNAL(returnPressed()), notationOk, SLOT(click()));
	connect(notationOk, SIGNAL(clicked()), this, SLOT(sendMove()));

	QToolBar* toolBar = this->addToolBar(tr("Main toolbar"));
	toolBar->addWidget(new QLabel(tr("Notation: ")));
	toolBar->addWidget(notationEdit);
	toolBar->addWidget(notationCombo);
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
	ui->board->makeNotationMove(notationEdit->text());
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
