#pragma once
#include <QtWidgets>
#include <list>

#define STR_NUM_ON_PAGE 10

struct melodyFormat
{
	int topMargin, rightMargin, leftMargin, bottomMargin,
	    clef, timeSignature, stringSpace;
};

struct noteStyle
{
	int nSize;
};

class cursor : public QWidget
{
	std::list<char>::iterator position;
	void paintEvent(QPaintEvent*);
public:
	std::list<char>::iterator getPos();
	void setPos(std::list<char>::iterator);
};

class noteString : public QWidget
{
	std::list<char>::iterator begin_it;
	std::list<char>::iterator end_it;
	noteStyle style;
	QWidget* _document;

	void paintEvent(QPaintEvent*);
public:
	noteString(QWidget*);
	void setContent(std::list<char>::iterator, std::list<char>::iterator);
};

class document : public QWidget
{
	std::list<char> melody;
	melodyFormat format;
	cursor* _cursor;
	noteString** nString;
	noteStyle style;
	QBoxLayout* layout;

	void paintEvent(QPaintEvent*);
	void keyPressEvent(QKeyEvent*);
public:
	document(std::list<char>::iterator, std::list<char>::iterator);
	~document();
	int getNoteSize();
};
