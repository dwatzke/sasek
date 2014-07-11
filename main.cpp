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

#include <QApplication>
#include <QLocale>
#include <QTextCodec>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("sasek");
	a.setOrganizationDomain("watzke.cz");

	//QTextCodec *unicode = QTextCodec::codecForName("UTF-8");
	//QTextCodec::setCodecForCStrings(unicode);
	//QTextCodec::setCodecForTr(unicode);

	QTranslator t;
	t.load(a.applicationName() + "_" + QLocale::system().name());
	a.installTranslator(&t);

	MainWindow w;
	w.show();

	return a.exec();
}
