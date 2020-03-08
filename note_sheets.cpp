#include "note_sheets.h"
#include <iostream>
#include <cmath>

using namespace Qt;
using std::vector;
using std::string;
typedef std::list<unsigned int>::iterator it;
typedef std::string::iterator str_it;

nStringPainter::nStringPainter(document* doc):QPainter(doc) {}

void nStringPainter::drawNote(int x, int y, unsigned int size, unsigned int duration)
{
	if(duration < 2)
	{
		QBrush br = brush();
		setBrush(NoBrush);
		drawEllipse(x, y, 1.3* size, size);
		setBrush(br);
	}
	else
		drawEllipse(x, y, 1.3* size, size);
	x += 1.3* size;
	y += 0.5* size;
	if(duration != 0)
		drawLine(x, y, x, y - 4*size);
}

void nStringPainter::drawString(const nStringLayout& layout, int x, int y)
{
	nStringStyle style = layout.getStyle();

	unsigned int props;
	int i;
	y += 4 * style.size;
	for(i = 0; i < 5; ++i)
		drawLine(
				x, y + i * style.size,
				x + style.width, y + i * style.size);
	int x1, y1;

	for(i = 0; layout[i].props != (unsigned int)-1; ++i)
	{
		x1 = x + layout[i].position;
		props = layout[i].props;
		y1 = y + 8* style.size;
		y1 -= (props & 0x0038)/8 * style.size/2;
		y1 -= (props & 0x01c0)/0x0040 * style.size * 7/2;
		drawNote(x1, y1, style.size, props & 0x0007);
	}
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
	int pos = style.space;
	it nChar = begin;
	nSymbol sym;
	clear();
	cursorPosition.setY(-2);
	for(int i = 0; (pos < style.width - style.space) & (nChar != end); ++i)
	{
		if (nChar == cursor_it)
		{
			cursorPosition.setX(pos - 0.5* style.space);
			cursorPosition.setY(-1);
		}
		sym.position = pos;
		sym.props = *nChar;
		push_back(sym);
		pos += style.space;
		++nChar;
	}
	if (cursor_it == nChar)
	{
			cursorPosition.setX(pos - 0.5* style.space);
			cursorPosition.setY(-1);
	}
	sym.props = (unsigned int)-1;
	push_back(sym);
	return nChar;
}

document::document(const string& mld):QWidget()
{
	melodyInitialization(mld);

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
	currentInput

	cursorVisible = true;
	startTimer(1000);
}

unsigned int document::nSymbolConversion(std::string::const_iterator nChar)
{
	++nChar;
	unsigned int props, tmp = 0;
	switch(*nChar)
	{
		case 'K':
			props = 0x1000;
		break;
		case 'T':
			props = 0x2000;
		break;
		case 'P':
			props = 0x3000;
		break;
		case 'D':
			props = 0x4000;
		break;
		case 'A':
			props = 0x5000;
		break;
		default:
			props = 64* (*(nChar++) - '0');
			props += 8* (*(nChar++) - 'c');
			for(; *nChar != ' '; ++nChar)
				tmp = tmp*10 + *nChar - '0';
			props += log(tmp)/log(2);
	}
	return props;
}

void document::melodyInitialization(const string& mld)
{
	for(auto nChar = mld.begin(); nChar != mld.end(); ++nChar)
		if(*nChar == ' ')
			melody.push_back(nSymbolConversion(nChar));
}

int document::strWidth() const{
	return width() - format.leftMargin - format.rightMargin;}
int document::space() const{return 4*style.size;}

void document::paintEvent(QPaintEvent*)
{
	nStringPainter painter(this);
	painter.setBrush(QBrush(black, SolidPattern));

	style.width = strWidth();
	style.space = space();
	layout.setStyle(style);

	int strSize = 16*style.size;

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
				layout,
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
