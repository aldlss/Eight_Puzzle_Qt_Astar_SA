#include "QtAstar.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QtAstar w;
	w.show();
	return a.exec();
}
