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

class noteString : public QWidget
{
	std::list<char>::iterator begin_it;
	std::list<char>::iterator end_it;
//	std::list<char>::iterator cursor;
	noteStyle style;
//	int stringNumber;
//	static numberOfStrings;

//	void keyPressEvent(QKeyEvent*);
	void paintEvent(QPaintEvent*);
public:
	noteString(QWidget*);
	void setContent(std::list<char>::iterator, std::list<char>::iterator);
//	noteString(QWidget*, std::list<char>::iterator begin);
};

class document : public QWidget
{
	std::list<char> melody;
	melodyFormat format;
	noteString** nString;
	QBoxLayout* layout;
	void paintEvent(QPaintEvent*);

	noteStyle style;
public:
	document(std::list<char>::iterator, std::list<char>::iterator);
	~document();
};
