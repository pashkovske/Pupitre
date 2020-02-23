#include "note_sheets.h"

using std::list;
using namespace Qt;

noteString::noteString(QWidget* parentWidget):QWidget(parentWidget),_document(parentWidget)
{
	style.nSize = _document->getNoteSize();
}

void noteString::setContent(list<char>::iterator beginit, list<char>::iterator endit)
{
	begin_it = beginit;
	end_it = endit;
}

void noteString::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	for(int i = 0; i < 5; ++i)
		painter.drawLine(
				QPointF(0, (i + 4)* style.nSize),
				QPointF(width(), (i + 4)* style.nSize));
	painter.setBrush(QBrush(black, SolidPattern));
	list<char>::iterator note_it = begin_it;
	int horisontalPos = 3*style.nSize;
	while(note_it != end_it)
	{
		painter.drawEllipse(
				horisontalPos, (17 + 'c' - *note_it)*style.nSize/2,
				1.3*style.nSize, style.nSize);
		horisontalPos += 4*style.nSize;
		note_it++;
	}
}

cursor::cursor(QWidget* parentWidget):QWidget(parentWidget)
{
	int nSize = parentWidget->getNoteSize();
	setFixedSize(nSize, 7* nSize);
}

void cursor::setPos(list<char>::iterator pos) {position = pos;}

list<char>::iterator cursor::getPos() {return position;}

#define N 4

document::document(list<char>::iterator mld_begin, list<char>::iterator mld_end)
{
	melody = list<char>(mld_begin, mld_end);
	nString = new noteString*[10];

	format.topMargin = 70;
	format.leftMargin = 70;
	format.rightMargin = 70;
	format.bottomMargin = 70;
	format.stringSpace = 0;
	style.nSize = 15;

	layout = new QVBoxLayout;
	layout->setContentsMargins(format.leftMargin, format.topMargin, format.rightMargin, format.bottomMargin);
	layout->setSpacing(format.stringSpace);

	for(int i = 0; i < N; ++i)
	{
		nString[i] = new noteString(this);
		nString[i]->setContent(mld_begin, mld_begin);
		nString[i]->setFixedHeight(16*style.nSize);
		layout->addWidget(nString[i]);
	}
	layout->addStretch(0);
	this->setLayout(layout);

	_cursor = new cursor;
	_cursor->setPos(mld_begin);
}

document::~document()
{
	delete layout;
	for(int i = 0; i < N; ++i)
		delete nString[i];
	delete cursor;
	delete[] nString;
}

int document::getNoteSize() {return style.nSize;}

void document::keyPressEvent(QKeyEvent* pressEvent)
{
	if(pressEvent->key() == Key_Escape)
		QCoreApplication::exit();
	if((pressEvent->key() >= Key_A) & (pressEvent->key() <= Key_H))
	{
		melody.push_back('a' - Key_A + pressEvent->key());
		repaint();
	}
}

void document::paintEvent(QPaintEvent*)
{
	int horisontalPos;
	list<char>::iterator str_begin = melody.begin(), str_end = str_begin;
	int str_width = width() - format.leftMargin - format.rightMargin;
	for(int i = 0; (i < N); ++i)
	{
		horisontalPos = 5*style.nSize;
		while((horisontalPos < str_width) & (str_end != melody.end()))
		{

			++str_end;
			horisontalPos += 4*style.nSize;
		}

		nString[i]->setContent(str_begin, str_end);
		if((str_begin == str_end) & !(nString[i]->isHidden()))
			nString[i]->hide();
		if((str_begin != str_end) & (nString[i]->isHidden()))
			nString[i]->show();

		str_begin = str_end;
	}
}
