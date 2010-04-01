#include <QApplication>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QTextCodec *unicode = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForCStrings(unicode);

	MainWindow w;
	w.show();

	return a.exec();
}
