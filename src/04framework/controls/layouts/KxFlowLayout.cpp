#include "KxFlowLayout.h"
using namespace  std;

KxFlowLayout::KxFlowLayout(QWidget *parent, bool vertical, bool wrap, int margin, int spacing)
: QLayout(parent)
, fVertical(vertical)
, fWrap(wrap)
{
    setMargin(margin);
    setSpacing(spacing);
}


KxFlowLayout::~KxFlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void KxFlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int KxFlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *KxFlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *KxFlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    else
        return 0;
}

Qt::Orientations KxFlowLayout::expandingDirections() const
{
    return 0;
}

void KxFlowLayout::setGeometry(const QRect &rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect);
}

QSize KxFlowLayout::sizeHint() const
{
	QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
        size = size.expandedTo(item->sizeHint());

	return size;
}

QSize KxFlowLayout::minimumSize() const
{
    QSize size;
    QLayoutItem *item;
    foreach (item, itemList)
        size = size.expandedTo(item->minimumSize());

	if(fVertical) {
		size.rheight() = 0;
	} else {
		size.rwidth() = 0;
	}
	return size;
}

bool KxFlowLayout::isVertical()
{
	return fVertical;
}

void KxFlowLayout::setWrap(bool wrap)
{
	fWrap = wrap;
}

bool KxFlowLayout::doesWrap()
{
	return fWrap;
}

int KxFlowLayout::doLayout(const QRect &rect) const
{
    int x = rect.x();
    int y = rect.y();
    if(fVertical) {
		int lineWidth = 0;

	    QLayoutItem *item;
	    foreach (item, itemList) {
	        int nextY = y + item->minimumSize().height() + spacing();

			if(fWrap && nextY - spacing() > rect.bottom() && lineWidth > 0) {
	            y = rect.y();
	            x = x + lineWidth + spacing();
	            nextY = y + item->minimumSize().height() + spacing();
	            lineWidth = 0;
	        }

            item->setGeometry(QRect(QPoint(x, y), item->minimumSize()));

	        y = nextY;
	        lineWidth = qMax(lineWidth, item->minimumSize().width());
	    }
	    return x + lineWidth - rect.x();
	} else {
		int lineHeight = 0;

	    QLayoutItem *item;
	    foreach (item, itemList) {
	        int nextX = x + item->minimumSize().width() + spacing();

			if (fWrap && nextX - spacing() > rect.right() && lineHeight > 0) {
	            x = rect.x();
	            y = y + lineHeight + spacing();
	            nextX = x + item->minimumSize().width() + spacing();
	            lineHeight = 0;
	        }

            item->setGeometry(QRect(QPoint(x, y), item->minimumSize()));

	        x = nextX;
	        lineHeight = qMax(lineHeight, item->minimumSize().height());
	    }
	    return y + lineHeight - rect.y();
	}
}
