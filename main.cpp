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

	std::list<char> mld;
	std::string s = "caefddaf";
	for(int i = 0; i < (int)s.length(); ++i)
		mld.push_back(s[i]);
	
	document wgt(mld.begin(), mld.end());
	wgt.move(MONITOR_WIDTH/3, MONITOR_HEIGHT/8);
	wgt.resize(MONITOR_WIDTH/3, MONITOR_HEIGHT*0.8);
	wgt.show();
	
	return app.exec();
}
