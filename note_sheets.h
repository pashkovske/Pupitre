#pragma once
#include <QtWidgets>
#include <list>
#include <vector>
#include <string>

struct melodyFormat
{
	int topMargin, rightMargin, leftMargin, bottomMargin,
	    strSpace;
	int clef, timeSignature;
};

struct nStringStyle
{
	unsigned int size;
	int space, width;
};

struct nSymbol
{
	int position;
	unsigned int size, props;
};

class nStringLayout : public std::vector<nSymbol>
{
	nStringStyle style;
	QPoint cursorPosition;
public:
	void setStyle(nStringStyle);
	nStringStyle getStyle() const;
	QPoint getCursorPosition() const;
	
	std::list<unsigned int>::iterator setLayout(
			std::list<unsigned int>::iterator begin,
			std::list<unsigned int>::iterator end,
			std::list<unsigned int>::iterator cursor);
};

class document : public QWidget
{
	std::list<unsigned int> melody;
	std::list<unsigned int>::iterator cursor_it;
	unsigned int cursorProps;
	std::string currentInput;
	QRect cursor;
	bool cursorVisible;
	nStringLayout layout;
	melodyFormat format;
	nStringStyle style;

	unsigned int nSymbolConversion(std::string::const_iterator);
	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
	void timerEvent(QTimerEvent*);
public:
	document(const std::string&);
	void melodyInitialization(const std::string&);
	int strWidth() const;
	int space() const;
};

class nStringPainter : public QPainter
{
public:
	nStringPainter(document*);
	void drawString(const nStringLayout&, int x, int y);
	void drawNote(int x, int y, unsigned int size, unsigned int duration);
	void drawCursor(QRect);
};
