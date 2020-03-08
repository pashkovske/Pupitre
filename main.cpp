#include <QtWidgets>
#include <string>
#include <iostream>
#include "note_sheets.h"

#define MONITOR_HEIGHT 1080
#define MONITOR_WIDTH 1920

using namespace Qt;

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	std::string s = " 1c2 1f4 2e1 0e8";
	
	document wgt(s);
	wgt.move(MONITOR_WIDTH/3, MONITOR_HEIGHT/8);
	wgt.resize(MONITOR_WIDTH/3, MONITOR_HEIGHT*0.8);
	wgt.show();
	
	return app.exec();
}
