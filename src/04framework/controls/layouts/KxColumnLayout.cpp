#include "KxColumnLayout.h"
#include "../base/KxQtHelper.h"

using namespace std;

#define kDefaultColumnWidth 100

KxColumnLayout::KxColumnLayout( QWidget * parent )
: QVBoxLayout(parent)
, fColumnAdjustable(false)
, fColumnWidth(kDefaultColumnWidth)
, fColumnAlignment(Qt::AlignHCenter | Qt::AlignVCenter)
, fLeftOffset(0)
, fRightOffset(0)
, fLeftAttach(false)
, fRightAttach(false)
{
    setContentsMargins(1,1,1,1);
    setSpacing(0);
    setProperty("hasMarginsSet", qVariantFromValue(true));
}


// Description:  Called to add a layout item to the layout.
//
void KxColumnLayout::addItem ( QLayoutItem * item )
{
    QVBoxLayout::addItem(item);
    applyColumnAlignment(item);
}

// Description:  Called when we add a widget to the layout.
//
void KxColumnLayout::addWidget ( QWidget * w )
{
    QLayout::addWidget(w);
}

// Description:  Sets the width of the column. Won't be enforced unless the 
//               children are attached to both sides and the column is not adjustable.
//
void KxColumnLayout::setColumnWidth(int width)
{
    fColumnWidth = width;
    enforceColumnWidth();
    invalidate();
}

int KxColumnLayout::columnWidth()
{
    QRect rect = geometry();
    return rect.width();
}

// Description:  If set to true, children will always take on the width of the layout.
//
void KxColumnLayout::setColumnAdjustable(bool adjustable)
{
    fColumnAdjustable = adjustable;
    invalidate();
}

// Description:  Sets the offset for the children of the layout.
//
void KxColumnLayout::setColumnOffset(KxColumnLayout::OffsetSide side, int offset)
{
    if(side == kLeftSide || side == kBothSides) {
        fLeftOffset = offset;
    } 
    if(side == kRightSide || side == kBothSides) {
        fRightOffset = offset;
    }
    enforceColumnWidth();
    invalidate();
}

// Description:  Sets the attachment and offsets for the children of the layout.
//
void KxColumnLayout::setColumnAttachment(KxColumnLayout::OffsetSide side, int offset)
{
    if(side == kLeftSide) {
        fLeftAttach = true;
        fRightAttach = false;
        fLeftOffset = offset;
        fRightOffset = 0;
    } else if(side == kRightSide) {
        fRightAttach = true;
        fLeftAttach = false;
        fRightOffset = offset;
        fLeftOffset = 0;
    } else if(side == kBothSides) {
        fLeftAttach = true;
        fRightAttach = true;
        fLeftOffset = offset;
        fRightOffset = offset;
    }
    enforceColumnWidth();
    invalidate();
}

// Description:  Sets the alignment of the children of the layout. Usually only
//               enforced on text children.
//
void KxColumnLayout::setColumnAlignment(Qt::Alignment alignment)
{
    fColumnAlignment = alignment;
    for(int i = 0; i < count(); i++) {
        applyColumnAlignment(itemAt(i));
    }
}

// Description:  Sets the alignment of the given layout item.
//
void KxColumnLayout::applyColumnAlignment(QLayoutItem* item)
{
    QWidget *widget = NULL;
    if (item && (widget = item->widget())) {
        QLabel *label = dynamic_cast<QLabel *>(widget);
        if(label != NULL) {
            label->setAlignment(fColumnAlignment);
        } else {
            setAlignment(widget, fColumnAlignment);
        }
    }
}

// Description:  Returns the minimum size of the layout, which we want to
//               be the size hint.
//
QSize KxColumnLayout::minimumSize() const
{
    return sizeHint();
}

// Description:  Calculates and returns the preferred size of the layout.
//
QSize KxColumnLayout::sizeHint() const
{
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    getContentsMargins(&left, &top, &right, &bottom);

    int totalHeight = top + (count() - 1) * spacing() + bottom;
    int maxWidth = 0;
    int	itemOffset = fLeftOffset + fRightOffset;
    for(int i = 0 ; i < count(); i++) {
        QLayoutItem *item = itemAt(i);
        QWidget *w = item->widget();
        if(w != NULL && !w->isHidden()) {
            int itemWidth = KxQtHelper::getBaseControlWidth(w);
            int itemHeight = KxQtHelper::getBaseControlHeight(w);

            // If the column is attached on both sides and not adjustable, the item should take up
            // the set column width, minus the offsets
            if(fLeftAttach && fRightAttach && !fColumnAdjustable) {
                itemWidth = fColumnWidth - itemOffset;
                if(itemWidth < 0) {
                    itemWidth = 0;
                }
            }

            // Look for the biggest widget width, since that will determine the least the column width should be.
            if(maxWidth < itemWidth) {
                maxWidth = itemWidth;
            }

            // Add the widget height to the total.
            totalHeight += itemHeight;
        }
    }

    return QSize(maxWidth + itemOffset + left + right, totalHeight);
}

// Description:  Sets the layout's geometry according to the specified rectangle.
//
void KxColumnLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    applyRowHeights();
}

// Description:  Calculates and sets the geometry of each item in the layout,
//               based on its set position and minimum height.
//
void KxColumnLayout::applyRowHeights()
{
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    getContentsMargins(&left, &top, &right, &bottom);

    QRect rect = geometry();

    int currY = rect.y() + top;	

    for(int i = 0; i < count(); i++) {	
        int currX = rect.x() + left + fLeftOffset;
        int itemWidth = 0;
        int itemHeight = 0;
        QLayoutItem *item = itemAt(i);
        QWidget *w = NULL;
        if(item && (w = item->widget()) && !w->isHidden()) {
            itemHeight = KxQtHelper::getBaseControlHeight(w);

            if(fColumnAdjustable) {
                // If the column is adjustable, the item should take up entire rect, minus the margins and offsets
                itemWidth = rect.width() - left - right - fLeftOffset - fRightOffset;
            } else if(fLeftAttach && fRightAttach) {
                // If the column is attached on both sides, the item should take up the set column width, minus the offsets
                itemWidth = fColumnWidth - fLeftOffset - fRightOffset;
                if(itemWidth < 0) {
                    itemWidth = 0;
                }
            } else {
                // If the column is not adjustable, we'll use the widget's normal width.
                itemWidth = KxQtHelper::getBaseControlWidth(w);

                // If the column is right-attached only, move the item to the right side of the column
                if(fRightAttach && !fLeftAttach) {
                    currX += (fColumnWidth - itemWidth);
                }
            }

            QRect itemRect(currX, currY, itemWidth, itemHeight); 
            w->setGeometry(itemRect);

            currY += (itemHeight + spacing());
        }
    }
}

// Description:  If the children are attached on both sides and the column is not
//               adjustable, we want to make sure the children take on the set column width.
//
void KxColumnLayout::enforceColumnWidth()
{
    if(fLeftAttach && fRightAttach && !fColumnAdjustable) {
        int availableItemWidth = fColumnWidth - fLeftOffset - fRightOffset;
        if(availableItemWidth >= 0) {
            for(int i = 0; i < count(); i++) {
                QLayoutItem* item = itemAt(i);
                QWidget *widget = NULL;
                // Make sure the widget can actually be resized to the width we want it to be.
                if (item && (widget = item->widget()) && widget->minimumWidth() > availableItemWidth) {
                    widget->setMinimumWidth(availableItemWidth);
                }
            }
        }
    }
}
