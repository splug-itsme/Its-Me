#include "myclass.h"
#include <QtWidgets/QApplication>

//#define SAVE_ROI_VIDEO
//#define SAVE_TEST_FILES
//#define DETECT_EMOTION

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MyClass w;
	w.show();
	return a.exec();
}
