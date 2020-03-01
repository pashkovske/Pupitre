#include "note_sheets.h"
#include <iostream>

using namespace Qt;
using std::vector;
typedef std::list<char>::iterator it;

nStringPainter::nStringPainter(document* doc):QPainter(doc) {}

void nStringPainter::drawNote(nStringStyle style, qreal x, qreal y)
{
	drawEllipse(x, y, 1.3* style.size, style.size);
}

void nStringPainter::drawString(nStringLayout* const layout, qreal x, qreal y)
{
	nStringStyle style = layout->getStyle();

	int i;
	for(i = 0; i < 5; ++i)
		drawLine(
				QPointF(x, y + (i + 4)* style.size),
				QPointF(x + style.width, y + (i + 4)* style.size));

	for(vector<nSymbol>::iterator layout_it = layout->begin();
			layout_it->propereties != -1;
			++layout_it)
		drawNote(style, x + layout_it->position, y + layout_it->propereties*style.size/2);
}
/*
cursor::cursor(QWidget* parentWidget):QWidget(parentWidget)
{
	int nSize = parentWidget->getNoteSize();
	setFixedSize(nSize, 7* nSize);
}

void cursor::setPos(list<char>::iterator pos) {position = pos;}

list<char>::iterator cursor::getPos() {return position;}
*/

void nStringLayout::setStyle(nStringStyle stl) {style = stl;}
nStringStyle nStringLayout::getStyle() const {return style;}

it nStringLayout::setLayout(it begin, it end)
{
	qreal pos = 0;
	int i;
	it nChar = begin;
	nSymbol sym;
	clear();
	for(i = 0; (pos < style.width - style.space) & (nChar != end); ++i)
	{
		sym.position = pos;
		sym.propereties = 17 + 'c' - *nChar;
		push_back(sym);
		pos += style.space;
		++nChar;
	}
	sym.propereties = -1;
	push_back(sym);
	return nChar;
}

document::document(it mld_begin, it mld_end):QWidget()
{
	melody = std::list<char>(mld_begin, mld_end);

	format.topMargin = 70;
	format.leftMargin = 70;
	format.rightMargin = 70;
	format.bottomMargin = 70;
	format.strSpace = 0;

	style.width = strWidth();
	style.size = 15;
	style.space = space();
	layout.setStyle(style);
	layout.setLayout(melody.begin(), melody.begin());

//	_cursor->setPos(mld_begin);
	setMinimumWidth(format.leftMargin + format.rightMargin + 2*space());
}

qreal document::strWidth() const{
	return width() - format.leftMargin - format.rightMargin;}
qreal document::space() const{return 4*style.size;}

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
	nStringPainter painter(this);
	painter.setBrush(QBrush(black, SolidPattern));

	style.width = strWidth();
	style.space = space();
	layout.setStyle(style);
	qreal strSize = 16*style.size;
	it strBegin = melody.begin();
	for(int i = 0; strBegin != melody.end(); ++i)
	{
		strBegin = layout.setLayout(strBegin, melody.end());
		painter.drawString(
				&layout,
				format.leftMargin,
				format.topMargin + i* (strSize + format.strSpace));
	}
}
