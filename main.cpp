#include <QtWidgets>
#include <string>
#include <iostream>
#include "note_sheets.h"

#define MONITOR_HEIGHT 1080
#define MONITOR_WIDTH 1920
/*#define TOP_SPACE 70
#define STRING_SPACE 50
#define NOTE_SIZE 15
#define BORDER_SPACE 70
#define HORISONTAL_SPACE 35
*/
using namespace Qt;

/*class MyWidget : public QWidget
{
	std::string notes;

	void keyPressEvent(QKeyEvent* PressEvent)
	{
		if (PressEvent->key() == Key_Escape)
		       QCoreApplication::exit();
		if ((PressEvent->key() >= Key_A) & (PressEvent->key() <= Key_G))
		{
			notes += ('a' + PressEvent->key() - Key_A);
			this -> repaint();
		}
	}
};
*/

int main(int argc, char** argv)
{
	QApplication app(argc, argv);

	std::list<char> mld;
	std::string s = "caefddaf\0\0";
	for(int i = 0; i <= (int)s.length(); ++i)
		mld.push_back(s[i]);
std::cout << mld.size();
	
	document wgt(mld.begin(), mld.end());
	wgt.move(MONITOR_WIDTH/3, MONITOR_HEIGHT/8);
	wgt.resize(MONITOR_WIDTH/3, MONITOR_HEIGHT*0.8);
	wgt.show();
	
	return app.exec();
}
