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
/*
class cursor : public QWidget
{
	std::list<char>::iterator position;
	void paintEvent(QPaintEvent*);
public:
	std::list<char>::iterator getPos();
	void setPos(std::list<char>::iterator);
};
*/
class nStringLayout : public std::vector<nSymbol>
{
	nStringStyle style;
public:
	void setStyle(nStringStyle);
	nStringStyle getStyle() const;
	std::list<char>::iterator setLayout(
			std::list<char>::iterator begin,
			std::list<char>::iterator end);
};

class document : public QWidget
{
	std::list<char> melody;
	nStringLayout layout;
	melodyFormat format;
//	cursor* _cursor;
	nStringStyle style;

	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
public:
	document(std::list<char>::iterator begin, std::list<char>::iterator end);
	qreal strWidth() const;
	qreal space() const;
//	std::vector<nSymbol>::iterator getStringLayout();
};

class nStringPainter : public QPainter
{
public:
	nStringPainter(document*);
	void drawString(nStringLayout* const, qreal x, qreal y);
	void drawNote(nStringStyle, qreal x, qreal y);
};
