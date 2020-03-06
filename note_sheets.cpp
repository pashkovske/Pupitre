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
				QPoint(x, y + (i + 4)* style.size),
				QPoint(x + style.width, y + (i + 4)* style.size));

	for(vector<nSymbol>::iterator layout_it = layout->begin();
			layout_it->propereties != -1;
			++layout_it)
		drawNote(style, x + layout_it->position, y + layout_it->propereties*style.size/2);
}

void nStringPainter::drawCursor(QRect cursor)
{
	QPoint top(cursor.x() + cursor.width()/2, cursor.y() + cursor.height()/14);
	QPoint bottom(cursor.x() + cursor.width()/2, cursor.y() + 13*cursor.height()/14);
	drawLine(top, cursor.topRight());
	drawLine(top, cursor.topLeft());
	drawLine(top, bottom);
	drawLine(cursor.bottomLeft(), bottom);
	drawLine(cursor.bottomRight(), bottom);
}

void nStringLayout::setStyle(nStringStyle stl) {style = stl;}
nStringStyle nStringLayout::getStyle() const {return style;}
QPoint nStringLayout::getCursorPosition() const {return cursorPosition;}

it nStringLayout::setLayout(it begin, it end, it cursor_it)
{
	qreal pos = style.space;
	int i;
	it nChar = begin;
	nSymbol sym;
	clear();
	cursorPosition.setY(-2);
	for(i = 0; (pos < style.width - style.space) & (nChar != end); ++i)
	{
		if (nChar == cursor_it)
		{
			cursorPosition.setX(pos - 0.5* style.space);
			cursorPosition.setY(-1);
		}
		sym.position = pos;
		sym.propereties = 17 + 'c' - *nChar;
		push_back(sym);
		pos += style.space;
		++nChar;
	}
	if (cursor_it == nChar)
	{
			cursorPosition.setX(pos - 0.5* style.space);
			cursorPosition.setY(-1);
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
	style.size = 10;
	style.space = space();
	layout.setStyle(style);
	cursor_it = melody.begin();
	layout.setLayout(melody.begin(), melody.begin(), cursor_it);

	setMinimumWidth(format.leftMargin + format.rightMargin + 3*space());

	cursorVisible = true;
	startTimer(1000);
}

qreal document::strWidth() const{
	return width() - format.leftMargin - format.rightMargin;}
qreal document::space() const{return 4*style.size;}

void document::paintEvent(QPaintEvent*)
{
	nStringPainter painter(this);
	painter.setBrush(QBrush(black, SolidPattern));

	style.width = strWidth();
	style.space = space();
	layout.setStyle(style);

	qreal strSize = 16*style.size;

	it strBegin = melody.begin();

	cursor.setSize(QSize(style.size, 7* style.size));
	cursor.moveTo(0, -2);
	for(int i = 0; strBegin != melody.end(); ++i)
	{
		strBegin = layout.setLayout(strBegin, melody.end(), cursor_it);
		if(cursor.y() == -2)
			cursor.moveTo(layout.getCursorPosition() + QPoint(format.leftMargin, 0));
		if(cursor.y() == -1) 
		{
			cursor.moveTo(
					cursor.x(),
					format.topMargin + i* (strSize + format.strSpace) + 2.5* style.size);
			if(cursorVisible)
				painter.drawCursor(cursor);
		}
		painter.drawString(
				&layout,
				format.leftMargin,
				format.topMargin + i* (strSize + format.strSpace));
	}
}

void document::keyPressEvent(QKeyEvent* pressEvent)
{
	cursorVisible = true;
	if((pressEvent->key() == Key_Q) & (pressEvent->modifiers() == ControlModifier))
		QCoreApplication::exit();
	if((pressEvent->key() >= Key_A) & (pressEvent->key() <= Key_H))
		melody.insert(cursor_it, 'a' - Key_A + pressEvent->key());
	if((pressEvent->key() == Key_Right) & (cursor_it != melody.end()))
		++cursor_it;
	if((pressEvent->key() == Key_Left) & (cursor_it != melody.begin()))
		--cursor_it;
	repaint();
}

void document::timerEvent(QTimerEvent*)
{
	cursorVisible = !cursorVisible;
	repaint(cursor);
}
