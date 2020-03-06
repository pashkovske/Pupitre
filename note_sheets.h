#pragma once
#include <QtWidgets>
#include <list>
#include <vector>

#define STR_NUM_ON_PAGE 10

struct melodyFormat
{
	qreal topMargin, rightMargin, leftMargin, bottomMargin,
	    strSpace;
	int clef, timeSignature;
};

struct nSymbol
{
	qreal position, size;
	int propereties;
};

struct nStringStyle
{
	qreal size, space, width;
};

class nStringLayout : public std::vector<nSymbol>
{
	nStringStyle style;
	QPoint cursorPosition;
public:
	void setStyle(nStringStyle);
	nStringStyle getStyle() const;
	QPoint getCursorPosition() const;
	
	std::list<char>::iterator setLayout(
			std::list<char>::iterator begin,
			std::list<char>::iterator end,
			std::list<char>::iterator cursor);
};

class document : public QWidget
{
	std::list<char> melody;
	std::list<char>::iterator cursor_it;
	QRect cursor;
	bool cursorVisible;
	nStringLayout layout;
	melodyFormat format;
	nStringStyle style;

	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
	void timerEvent(QTimerEvent*);
public:
	document(std::list<char>::iterator begin, std::list<char>::iterator end);
	qreal strWidth() const;
	qreal space() const;
};

class nStringPainter : public QPainter
{
public:
	nStringPainter(document*);
	void drawString(nStringLayout* const, qreal x, qreal y);
	void drawNote(nStringStyle, qreal x, qreal y);
	void drawCursor(QRect);
};
