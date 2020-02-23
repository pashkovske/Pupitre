#include "note_sheets.h"

using std::list;

noteString::noteString(QWidget* parentWidget):QWidget(parentWidget)
{
	style.nSize = 15;
}

void noteString::setContent(list<char>::iterator beginit, list<char>::iterator endit)
{
	begin_it = beginit;
	end_it = endit;
}
/*
noteString::noteString(QWidget* parentWidget, list<char>::iterator beginit, list<char>::iterator endit):
	QWidget(parentWidget), begin_it(beginit), end_it(endit)
{
	style.nSize = 15;
	resize(16*style.nSize, 300);
	repaint();
}*/

#define N 4

document::document(list<char>::iterator mld_begin, list<char>::iterator mld_end)
{
	melody = list<char>(mld_begin, mld_end);
	format.topMargin = 70;
	format.leftMargin = 70;
	format.rightMargin = 70;
	format.bottomMargin = 70;
	format.stringSpace = 0;
	style.nSize = 15;
	layout = new QVBoxLayout;
	nString = new noteString*[10];
	layout->setContentsMargins(format.leftMargin, format.topMargin, format.rightMargin, format.bottomMargin);
	layout->setSpacing(format.stringSpacing);
	for(int i = 0; i < N; ++i)
	{
		nString[i] = new noteString(this);
		nString[i]->setContent(mld_begin, mld_begin);
		nString[i]->setFixedHeight(16*style.nSize);
		layout->addWidget(nString[i]);
		nString[i]->show();
	}
	this->setLayout(layout);
}

document::~document()
{
	delete layout;
	for(int i = 0; i < N; ++i)
		delete nString[i];
	delete[] nString;
}

void document::paintEvent(QPaintEvent*)
{
	int horisontalPos;
	list<char>::iterator str_begin = melody.begin(), str_end = str_begin;
	int str_width = width() - format.leftMargin - format.rightMargin;
	for(int i = 0; (i < N); ++i)
	{
		horisontalPos = 4*style.nSize;
		while((horisontalPos < str_width) & (str_end != melody.end()))
		{
			++str_end;
			horisontalPos += 4*style.nSize;
		}

//		nString[i]->resize(str_width, nString[i]->height());
		nString[i]->setContent(str_begin, str_end);

		str_begin = str_end;
	}
}

void noteString::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	for(int i = 0; i < 5; ++i)
		painter.drawLine(
				QPointF(0, (i + 4)* style.nSize),
				QPointF(width(), (i + 4)* style.nSize));
	painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
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
