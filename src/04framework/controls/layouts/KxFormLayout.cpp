#include "KxFormLayout.h"
#include "../base/KxQtHelper.h"

KxFormLayout::KxFormLayout(QWidget *parent, int numDivisions, int margin, int spacing)
: QLayout(parent),
  fNumDivisions(numDivisions),
  fNeedLayout(false)
{
	setMargin(margin);
	setSpacing(spacing);
}

KxFormLayout::KxFormLayout(int numDivisions, int spacing)
: fNumDivisions(numDivisions),
  fNeedLayout(false)
{
	setSpacing(spacing);
}

KxFormLayout::~KxFormLayout()
{
	QLayoutItem *l;
	while ((l = takeAt(0)))
		delete l;
}

void KxFormLayout::addItem(QLayoutItem *item)
{
	QHash<KxAttachmentEdge, AttachmentInfo> attachments;
	attachments.clear();
	fLayoutItems.append(item);
	fHash.insert(item, attachments);
}

Qt::Orientations KxFormLayout::expandingDirections() const
{
	return Qt::Horizontal | Qt::Vertical;
}

bool KxFormLayout::hasHeightForWidth() const
{
	return false;
}

int KxFormLayout::count() const
{
	return fLayoutItems.count();
}

QLayoutItem *KxFormLayout::itemAt(int index) const
{
	if(index >= 0 && index < fLayoutItems.count()) {
		return fLayoutItems[index];
	} else {
		return NULL;
	}
}

QSize KxFormLayout::minimumSize() const
{
	return calculateSize(MinimumSize);
}

void KxFormLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
	fNeedLayout = true;
	doLayout();
}

QSize KxFormLayout::sizeHint() const
{
	return calculateSize(SizeHint);
}

QLayoutItem *KxFormLayout::takeAt(int index)
{
	QLayoutItem * deletedItem = NULL;
	if(index >= 0 && index < fLayoutItems.count()) {
		deletedItem = fLayoutItems.takeAt(index);
		fHash.remove(deletedItem);
	}

	if(deletedItem) {
		foreach(QLayoutItem *item, fLayoutItems) {
			if(item) {
				QHash<KxAttachmentEdge, AttachmentInfo> attachments = fHash[item];
				AttachmentInfo topInfo = attachments[kAttachTop];
				AttachmentInfo bottomInfo = attachments[kAttachBottom];
				AttachmentInfo leftInfo = attachments[kAttachLeft];
				AttachmentInfo rightInfo = attachments[kAttachRight];
				AttachmentInfo noInfo;
				bool update = false;
				if(topInfo.fOtherControl == deletedItem) {
					attachments.insert(kAttachTop, noInfo);
					update = true;
				}
				if(bottomInfo.fOtherControl == deletedItem) {
					attachments.insert(kAttachBottom, noInfo);
					update = true;
				}
				if(leftInfo.fOtherControl == deletedItem) {
					attachments.insert(kAttachLeft, noInfo);
					update = true;
				}
				if(rightInfo.fOtherControl == deletedItem) {
					attachments.insert(kAttachRight, noInfo);
					update = true;
				}
				if(update) fHash.insert(item, attachments);
			}
		}
	}

	return deletedItem;
}

void KxFormLayout::doLayout()
{
	if(!fNeedLayout) return;
	 	
	QHash<QWidget *, QRect> positions;
	QList<QLayoutItem *> processingItems;
	QSize contentSize = parentWidget()->size();
	
	foreach(QLayoutItem *item, fLayoutItems) {
		if(item) {
			QRect r = calculateItemRect(item, contentSize, positions, processingItems);
			item->setGeometry(r);
		}
    }
	fNeedLayout = false;
}

QRect KxFormLayout::calculateItemRect(QLayoutItem *item, QSize &contentSize, QHash<QWidget *, QRect> &positions, QList<QLayoutItem *> &processingItems)
{
	QRect itemRect(0,0,-1,-1);

	if(processingItems.contains(item)) {
		return itemRect;
	} else {
		processingItems.append(item);
	}

	if(positions.contains(item->widget())) {
		itemRect = positions[item->widget()];
	} else {
		QSize originalSize(0, 0);

		// If we are constructing the layout, we don't count hidden widgets in our calculations.
		//
		if(!item->widget()->isHidden()) {
			originalSize = item->minimumSize();
		}

		itemRect.setSize(originalSize);

		QHash<KxAttachmentEdge, AttachmentInfo> attachments = fHash[item];
		AttachmentInfo topInfo = attachments[kAttachTop];
		AttachmentInfo bottomInfo = attachments[kAttachBottom];
		AttachmentInfo leftInfo = attachments[kAttachLeft];
		AttachmentInfo rightInfo = attachments[kAttachRight];

		switch (topInfo.fType) {
			case kAttachNone:
				break;
			case kAttachForm:
				itemRect.moveTop(topInfo.fOffset);
				break;
			case kAttachOppositeForm:
				itemRect.moveTop(contentSize.height() + topInfo.fOffset);
				break;
			case kAttachPosition:
				Q_ASSERT(fNumDivisions > 0);
				itemRect.moveTop((topInfo.fPosition * contentSize.height()) / fNumDivisions + topInfo.fOffset);
				break;
			case kAttachControl:
			{
				Q_ASSERT(topInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(topInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				itemRect.moveTop(otherItemRect.bottom() + 1 + topInfo.fOffset);
				break;
			}
			case kAttachOppositeControl:
			{
				Q_ASSERT(topInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(topInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				itemRect.moveTop(otherItemRect.top() + topInfo.fOffset);
				break;
			}
		}

		switch (leftInfo.fType) {
			case kAttachNone:
				break;
			case kAttachForm:
				itemRect.moveLeft(leftInfo.fOffset);
				break;
			case kAttachOppositeForm:
				itemRect.moveLeft(contentSize.width() + leftInfo.fOffset);
				break;
			case kAttachPosition:
				Q_ASSERT(fNumDivisions > 0);
				itemRect.moveLeft((leftInfo.fPosition * contentSize.width()) / fNumDivisions + leftInfo.fOffset);
				break;
			case kAttachControl:
			{
				Q_ASSERT(leftInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(leftInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				itemRect.moveLeft(otherItemRect.right() + 1 + leftInfo.fOffset);
				break;
			}
			case kAttachOppositeControl:
			{
				Q_ASSERT(leftInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(leftInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				itemRect.moveLeft(otherItemRect.left() + leftInfo.fOffset);
				break;
			}
		}

		switch (bottomInfo.fType) {
			case kAttachNone:
				break;
			case kAttachForm:
				if(topInfo.fType == kAttachNone) {
					itemRect.moveBottom(contentSize.height() - 1 - bottomInfo.fOffset);
				} else if(itemRect.bottom() < contentSize.height() - 1 - bottomInfo.fOffset) {
					itemRect.setBottom(contentSize.height() - 1 - bottomInfo.fOffset);
				}
				break;
			case kAttachOppositeForm:
				if(topInfo.fType == kAttachNone) {
					itemRect.moveBottom(-bottomInfo.fOffset);
				} else if(itemRect.bottom() < -bottomInfo.fOffset) {
					itemRect.setBottom(-bottomInfo.fOffset);
				}
				break;
			case kAttachPosition:
				Q_ASSERT(fNumDivisions > 0);
				if(topInfo.fType == kAttachNone) {
					itemRect.moveBottom((bottomInfo.fPosition * contentSize.height()) / fNumDivisions - 1 - bottomInfo.fOffset);
				} else if(itemRect.bottom() < (bottomInfo.fPosition * contentSize.height()) / fNumDivisions - 1 - bottomInfo.fOffset) {
					itemRect.setBottom((bottomInfo.fPosition * contentSize.height()) / fNumDivisions - 1 - bottomInfo.fOffset);
				}
				break;
			case kAttachControl:
			{
				Q_ASSERT(bottomInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(bottomInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				if(topInfo.fType == kAttachNone) {
					itemRect.moveBottom(otherItemRect.top() - 1 - bottomInfo.fOffset);
				} else if(itemRect.bottom() < otherItemRect.top() - 1 - bottomInfo.fOffset) {
					itemRect.setBottom(otherItemRect.top() - 1 - bottomInfo.fOffset);
				}
				
				if(itemRect.top() - topInfo.fOffset < 0) {
					itemRect.moveTop(topInfo.fOffset);
					
					otherItemRect.moveTop(itemRect.bottom() + 1 + bottomInfo.fOffset);
					positions.insert(bottomInfo.fOtherControl->widget(), otherItemRect);
				}
				break;
			}
			case kAttachOppositeControl:
			{
				Q_ASSERT(bottomInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(bottomInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				if(topInfo.fType == kAttachNone) {
					itemRect.moveBottom(otherItemRect.bottom() - bottomInfo.fOffset);
				} else if(itemRect.bottom() < otherItemRect.bottom() - bottomInfo.fOffset) {
					itemRect.setBottom(otherItemRect.bottom() - bottomInfo.fOffset);
				}
				break;
			}
		}
		if(itemRect.height() < originalSize.height()) itemRect.setTop(itemRect.bottom() - originalSize.height());
		
		switch (rightInfo.fType) {
			case kAttachNone:
				break;
			case kAttachForm:
				if(leftInfo.fType == kAttachNone) {
					itemRect.moveRight(contentSize.width() - 1 - rightInfo.fOffset);
				} else if(itemRect.right() < contentSize.width() - 1 - rightInfo.fOffset) {
					itemRect.setRight(contentSize.width() - 1 - rightInfo.fOffset);
				}
				break;
			case kAttachOppositeForm:
				if(leftInfo.fType == kAttachNone) {
					itemRect.moveRight(-rightInfo.fOffset);
				} else if(itemRect.right() < -rightInfo.fOffset) {
					itemRect.setRight(-rightInfo.fOffset);
				}
				break;
			case kAttachPosition:
				Q_ASSERT (fNumDivisions > 0);
				if(leftInfo.fType == kAttachNone) {
					itemRect.moveRight((rightInfo.fPosition * contentSize.width()) / fNumDivisions - 1 - rightInfo.fOffset);
				} else if(itemRect.right() < (rightInfo.fPosition * contentSize.width()) / fNumDivisions - 1 - rightInfo.fOffset) {
					itemRect.setRight((rightInfo.fPosition * contentSize.width()) / fNumDivisions - 1 - rightInfo.fOffset);
				}
				break;
			case kAttachControl:
			{
				Q_ASSERT(rightInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(rightInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				if(leftInfo.fType == kAttachNone) {
					itemRect.moveRight(otherItemRect.left() - 1 - rightInfo.fOffset);
				} else if(itemRect.right() < otherItemRect.left() - 1 - rightInfo.fOffset) {
					itemRect.setRight(otherItemRect.left() - 1 - rightInfo.fOffset);
				}
				
				if(itemRect.left() - leftInfo.fOffset < 0) {
					itemRect.moveLeft(leftInfo.fOffset);
					
					otherItemRect.moveLeft(itemRect.right() + 1 + rightInfo.fOffset);
					positions.insert(rightInfo.fOtherControl->widget(), otherItemRect);
				}
				break;
			}
			case kAttachOppositeControl:
			{
				Q_ASSERT(rightInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemRect(rightInfo.fOtherControl, contentSize, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				if(leftInfo.fType == kAttachNone) {
					itemRect.moveRight(otherItemRect.right() - rightInfo.fOffset);
				} else if(itemRect.right() < otherItemRect.right() - rightInfo.fOffset) {
					itemRect.setRight(otherItemRect.right() - rightInfo.fOffset);
				}
				break;
			}
		}
		if(itemRect.width() < originalSize.width()) itemRect.setLeft(itemRect.right() - originalSize.width());

		if(itemRect.left() - leftInfo.fOffset < 0) {
			itemRect.moveLeft(leftInfo.fOffset); 
		}
		if(itemRect.top() - topInfo.fOffset < 0) {
			itemRect.moveTop(topInfo.fOffset);
		}
		
		positions.insert(item->widget(), itemRect);
	}

	if(processingItems.contains(item)) {
		processingItems.removeAll(item);
	}

	return itemRect;
}

QRect KxFormLayout::calculateItemSize(QLayoutItem *item, SizeType sizeType, int &topContentHeight, int &bottomContentHeight, int &leftContentWidth, int &rightContentWidth, QHash<QWidget *, QRect> &positions, QList<QLayoutItem *> &processingItems) const
{
	QRect itemRect(0,0,-1,-1);

	if(processingItems.contains(item)) {
		return itemRect;
	} else {
		processingItems.append(item);
	}

	if(positions.contains(item->widget())) {
		itemRect = positions[item->widget()];
	} else {
		QSize originalSize(0, 0);

		if (sizeType == MinimumSize)
			originalSize = item->minimumSize();
		else if (sizeType == SizeHint)
			originalSize = item->sizeHint();
		else
			originalSize = item->minimumSize();

		itemRect.setSize(originalSize);

		QHash<KxAttachmentEdge, AttachmentInfo> attachments = fHash[item];
		AttachmentInfo topInfo = attachments[kAttachTop];
		AttachmentInfo bottomInfo = attachments[kAttachBottom];
		AttachmentInfo leftInfo = attachments[kAttachLeft];
		AttachmentInfo rightInfo = attachments[kAttachRight];
		
		bool verticalDone = false;
		bool horizontalDone = false;

		// Calculate the height from the top if the item is is top-attached.
		switch (topInfo.fType) {
			case kAttachNone:
				break;
			case kAttachForm:
				itemRect.moveTop(topInfo.fOffset);
				if(topContentHeight < itemRect.height() + topInfo.fOffset) {
					topContentHeight = itemRect.height() + topInfo.fOffset;
				}
				verticalDone = true;
				break;
			case kAttachOppositeForm:
				if(topInfo.fOffset >= 0) {
					itemRect.moveTop(topContentHeight + topInfo.fOffset);
					topContentHeight += itemRect.height() + topInfo.fOffset;
				} else {
					// An attachOppositeForm with a negative offset would affect the
					// bottom of the form, so update the height from the bottom.
					itemRect.moveBottom(-topInfo.fOffset);
					if(bottomContentHeight < -topInfo.fOffset) {
						bottomContentHeight = -topInfo.fOffset;
					}
				}
				verticalDone = true;
				break;
			case kAttachPosition:
				Q_ASSERT (fNumDivisions > 0);
				if(topContentHeight < ((float)originalSize.height() / (fNumDivisions - topInfo.fPosition)) * fNumDivisions) {
					topContentHeight = (int)(((float)originalSize.height() / (fNumDivisions - topInfo.fPosition)) * fNumDivisions);
				}
				itemRect.moveTop((topInfo.fPosition * topContentHeight) / fNumDivisions + topInfo.fOffset);
				verticalDone = true;
				break;
			case kAttachControl:
			{
				Q_ASSERT(topInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemSize(topInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				itemRect.moveTop(otherItemRect.bottom() + 1 + topInfo.fOffset);
				if(topContentHeight < itemRect.bottom() + 1) {
					topContentHeight = itemRect.bottom() + 1;
				}
				verticalDone = true;
				break;
			}
			case kAttachOppositeControl:
			{
				Q_ASSERT(topInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemSize(topInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				if(topInfo.fOffset >= 0) {
					itemRect.moveTop(otherItemRect.top() + topInfo.fOffset);
					if(topContentHeight < itemRect.bottom() + 1) {
						topContentHeight = itemRect.bottom() + 1;
					}
				} else {
					// An attachOppositeControl with a negative offset would affect
					// the bottom of the form, so update the height from the bottom.
					itemRect.moveBottom(otherItemRect.bottom() - topInfo.fOffset);
					if(bottomContentHeight < itemRect.bottom() + 1) {
						bottomContentHeight = itemRect.bottom() + 1;
					}
				}
				verticalDone = true;
				break;
			}
		}

		// If the item is not top-attached, calculate the height from the bottom.
		if(!verticalDone) {
			switch (bottomInfo.fType) {
				case kAttachNone:
					break;
				case kAttachForm:
					itemRect.moveTop(bottomInfo.fOffset);
					if(bottomContentHeight < itemRect.height() + bottomInfo.fOffset) {
						bottomContentHeight = itemRect.height() + bottomInfo.fOffset;
					}
					verticalDone = true;
					break;
				case kAttachOppositeForm:
					if(bottomInfo.fOffset >= 0) {
						itemRect.moveTop(bottomContentHeight + bottomInfo.fOffset);
						bottomContentHeight += itemRect.height() + bottomInfo.fOffset;
					} else {
						// An attachOppositeForm with a negative offset would affect the
						// top of the form, so update the height from the top.
						itemRect.moveBottom(-bottomInfo.fOffset);
						if(topContentHeight < itemRect.bottom() + 1) {
							topContentHeight = itemRect.bottom() + 1;
						}
					}
					verticalDone = true;
					break;
				case kAttachPosition:
					Q_ASSERT (fNumDivisions > 0);
					if(bottomContentHeight < ((float)originalSize.height() / bottomInfo.fPosition) * fNumDivisions) {
						bottomContentHeight = (int)(((float)originalSize.height() / bottomInfo.fPosition) * fNumDivisions);
					}
					itemRect.moveTop(((fNumDivisions - bottomInfo.fPosition) * bottomContentHeight) / fNumDivisions + bottomInfo.fOffset);
					verticalDone = true;
					break;
				case kAttachControl:
				{
					Q_ASSERT(bottomInfo.fOtherControl != NULL);
					QRect otherItemRect = calculateItemSize(bottomInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
					if(!otherItemRect.size().isValid()) return otherItemRect;
					itemRect.moveTop(otherItemRect.bottom() + 1 + bottomInfo.fOffset);
					if(bottomContentHeight < itemRect.bottom() + 1) {
						bottomContentHeight = itemRect.bottom() + 1;
					}
					verticalDone = true;
					break;
				}
				case kAttachOppositeControl:
				{
					Q_ASSERT(bottomInfo.fOtherControl != NULL);
					QRect otherItemRect = calculateItemSize(bottomInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
					if(!otherItemRect.size().isValid()) return otherItemRect;
					if(bottomInfo.fOffset >= 0) {
						itemRect.moveTop(otherItemRect.top() + bottomInfo.fOffset);
						if(bottomContentHeight < itemRect.bottom() + 1) {
							bottomContentHeight = itemRect.bottom() + 1;
						}
					} else {
						// An attachOppositeControl with a negative offset would affect
						// the top of the form, so update the height from the top.
						itemRect.moveBottom(otherItemRect.bottom() - bottomInfo.fOffset);
						if(topContentHeight < itemRect.bottom() + 1) {
							topContentHeight = itemRect.bottom() + 1;
						}
					}
					verticalDone = true;
					break;
				}
			}
		} else {
			switch (bottomInfo.fType) {
				case kAttachNone:
					break;
				case kAttachPosition:
					// If the item is bottom-attached by position, we still need to make sure the
					// height can accommodate it, so update the height from the top as necessary.
					if(topContentHeight < ((float)originalSize.height() / bottomInfo.fPosition) * fNumDivisions) {
						topContentHeight = (int)(((float)originalSize.height() / bottomInfo.fPosition) * fNumDivisions);
					}
				case kAttachForm:
				case kAttachControl:
					// We still need to account for the bottom offset, if any, so update the height
					// from the top as necessary.
					if(topContentHeight < itemRect.bottom() + 1 + bottomInfo.fOffset) {
						topContentHeight = itemRect.bottom() + 1 + bottomInfo.fOffset;
					}
					break;
				case kAttachOppositeForm:
					// An attachOppositeForm with a negative offset would affect the height from
					// the top, so we must account for it.
					if(bottomInfo.fOffset < 0) {
						itemRect.moveBottom(-bottomInfo.fOffset);
						if(topContentHeight < itemRect.bottom() + 1) {
							topContentHeight = itemRect.bottom() + 1;
						}
					}
					break;
				case kAttachOppositeControl:
					// An attachOppositeControl with a negative offset would affect the height from
					// the top, so we must account for it.
					if(bottomInfo.fOffset < 0) {
						Q_ASSERT(bottomInfo.fOtherControl != NULL);
						QRect otherItemRect = calculateItemSize(bottomInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
						if(!otherItemRect.size().isValid()) return otherItemRect;
						itemRect.moveBottom(otherItemRect.bottom() - bottomInfo.fOffset);
						if(topContentHeight < itemRect.bottom() + 1) {
							topContentHeight = itemRect.bottom() + 1;
						}
					}
					break;
			}
		}

		// If the item is neither top- nor bottom-attached, make sure
		// the height can at least accommodate the item height.
		if(!verticalDone) {
			if(topContentHeight < itemRect.height()) {
				topContentHeight = itemRect.height();
			}
		}

		// Calculate the width from the left if the item is left-attached.
		switch (leftInfo.fType) {
			case kAttachNone:
				break;
			case kAttachForm:
				itemRect.moveLeft(leftInfo.fOffset);
				if(leftContentWidth < itemRect.width() + leftInfo.fOffset) {
					leftContentWidth = itemRect.width() + leftInfo.fOffset;
				}
				horizontalDone = true;
				break;
			case kAttachOppositeForm:
				if(leftInfo.fOffset >= 0) {
					itemRect.moveLeft(leftContentWidth + leftInfo.fOffset);
					leftContentWidth += itemRect.width() + leftInfo.fOffset;
				} else {
					// An attachOppositeForm with a negative offset would affect the
					// right side of the form, so update the width from the right.
					itemRect.moveRight(-leftInfo.fOffset);
					if(rightContentWidth < -leftInfo.fOffset) {
						rightContentWidth = -leftInfo.fOffset;
					}
				}
				horizontalDone = true;
				break;
			case kAttachPosition:
				Q_ASSERT (fNumDivisions > 0);
				if(leftContentWidth < ((float)originalSize.width() / (fNumDivisions - leftInfo.fPosition)) * fNumDivisions) {
					leftContentWidth = (int)(((float)originalSize.width() / (fNumDivisions - leftInfo.fPosition)) * fNumDivisions);
				}
				itemRect.moveLeft((leftInfo.fPosition * leftContentWidth) / fNumDivisions + leftInfo.fOffset);
				horizontalDone = true;
				break;
			case kAttachControl:
			{
				Q_ASSERT(leftInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemSize(leftInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				itemRect.moveLeft(otherItemRect.right() + 1 + leftInfo.fOffset);
				if(leftContentWidth < itemRect.right() + 1) {
					leftContentWidth = itemRect.right() + 1;
				}
				horizontalDone = true;
				break;
			}
			case kAttachOppositeControl:
			{
				Q_ASSERT(leftInfo.fOtherControl != NULL);
				QRect otherItemRect = calculateItemSize(leftInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
				if(!otherItemRect.size().isValid()) return otherItemRect;
				if(leftInfo.fOffset >= 0) {
					itemRect.moveLeft(otherItemRect.left() + leftInfo.fOffset);
					if(leftContentWidth < itemRect.right() + 1) {
						leftContentWidth = itemRect.right() + 1;
					}
				} else {
					// An attachOppositeControl with a negative offset would affect
					// the right side of the form, so update the width from the right.
					itemRect.moveRight(otherItemRect.right() - leftInfo.fOffset);
					if(rightContentWidth < itemRect.right() + 1) {
						rightContentWidth = itemRect.right() + 1;
					}
				}
				horizontalDone = true;
				break;
			}
		}

		// If the item is not left-attached, calculate the width from the right.
		if(!horizontalDone) {
			switch (rightInfo.fType) {
				case kAttachNone:
					break;
				case kAttachForm:
					itemRect.moveLeft(rightInfo.fOffset);
					if(rightContentWidth < itemRect.width() + rightInfo.fOffset) {
						rightContentWidth = itemRect.width() + rightInfo.fOffset;
					}
					horizontalDone = true;
					break;
				case kAttachOppositeForm:
					if(rightInfo.fOffset >= 0) {
						itemRect.moveLeft(rightContentWidth + rightInfo.fOffset);
						rightContentWidth += itemRect.width() + rightInfo.fOffset;
					} else {
						// An attachOppositeForm with a negative offset would affect the
						// left side of the form, so update the width from the left.
						itemRect.moveRight(-rightInfo.fOffset);
						if(leftContentWidth < itemRect.right() + 1) {
							leftContentWidth = itemRect.right() + 1;
						}
					}
					
					horizontalDone = true;
					break;
				case kAttachPosition:
					Q_ASSERT (fNumDivisions > 0);
					if(rightContentWidth < ((float)originalSize.width() / rightInfo.fPosition) * fNumDivisions) {
						rightContentWidth = (int)(((float)originalSize.width() / rightInfo.fPosition) * fNumDivisions);
					}
					itemRect.moveLeft(((fNumDivisions - rightInfo.fPosition) * rightContentWidth) / fNumDivisions + rightInfo.fOffset);
					horizontalDone = true;
					break;
				case kAttachControl:
				{
					Q_ASSERT(rightInfo.fOtherControl != NULL);
					QRect otherItemRect = calculateItemSize(rightInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
					if(!otherItemRect.size().isValid()) return otherItemRect;
					itemRect.moveLeft(otherItemRect.right() + 1 + rightInfo.fOffset);
					if(rightContentWidth < itemRect.right() + 1) {
						rightContentWidth = itemRect.right() + 1;
					}
					horizontalDone = true;
					break;
				}
				case kAttachOppositeControl:
				{
					Q_ASSERT(rightInfo.fOtherControl != NULL);
					QRect otherItemRect = calculateItemSize(rightInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
					if(!otherItemRect.size().isValid()) return otherItemRect;
					if(rightInfo.fOffset >= 0) {
						itemRect.moveLeft(otherItemRect.left() + rightInfo.fOffset);
						if(rightContentWidth < itemRect.right() + 1) {
							rightContentWidth = itemRect.right() + 1;
						}
					} else {
						// An attachOppositeControl with a negative offset would affect
						// the left side of the form, so update the width from the left.
						itemRect.moveRight(otherItemRect.right() - rightInfo.fOffset);
						if(leftContentWidth < itemRect.right() + 1) {
							leftContentWidth = itemRect.right() + 1;
						}
					}
					horizontalDone = true;
					break;
				}
			}
		} else {
			switch (rightInfo.fType) {
				case kAttachNone:
					break;
				case kAttachPosition:
					// If the item is right-attached by position, we still need to make sure the
					// width can accommodate it, so update the width from the left as necessary.
					if(leftContentWidth < ((float)originalSize.width() / rightInfo.fPosition) * fNumDivisions) {
						leftContentWidth = (int)(((float)originalSize.width() / rightInfo.fPosition) * fNumDivisions);
					}
				case kAttachForm:
				case kAttachControl:				
					// We still need to account for the right offset, if any, so update the width
					// from the left as necessary.
					if(leftContentWidth < itemRect.right() + 1 + rightInfo.fOffset) {
						leftContentWidth = itemRect.right() + 1 + rightInfo.fOffset;
					}
					break;
				case kAttachOppositeForm:
					// An attachOppositeForm with a negative offset would affect the width from
					// the left, so we must account for it.
					if(rightInfo.fOffset < 0) {
						itemRect.moveRight(-rightInfo.fOffset);
						if(leftContentWidth < itemRect.right() + 1) {
							leftContentWidth = itemRect.right() + 1;
						}
					}
					break;
				case kAttachOppositeControl:
					// An attachOppositeControl with a negative offset would affect the width from
					// the left, so we must account for it.
					if(rightInfo.fOffset < 0) {
						Q_ASSERT(rightInfo.fOtherControl != NULL);
						QRect otherItemRect = calculateItemSize(rightInfo.fOtherControl, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
						if(!otherItemRect.size().isValid()) return otherItemRect;
						itemRect.moveRight(otherItemRect.right() - rightInfo.fOffset);
						if(leftContentWidth < itemRect.right() + 1) {
							leftContentWidth = itemRect.right() + 1;
						}
					}
					break;
			}
		}

		// If the item is neither left- nor right-attached, make sure
		// the width can at least accommodate the item width.
		if(!horizontalDone) {
			if(leftContentWidth < itemRect.width()) {
				leftContentWidth = itemRect.width();
			}
		}

		positions.insert(item->widget(), itemRect);
	}

	if(processingItems.contains(item)) {
		processingItems.removeAll(item);
	}

	return itemRect;
}

QSize KxFormLayout::calculateSize(SizeType sizeType) const
{
	// If the layout is the top-level layout of a fixed-size window,
	// use any size settings the window has for the size hint values.
	//
	QSize fixedSize = KxQtHelper::getFixedSizeWindowLayoutSizeHint(this);
	if(fixedSize.isValid()) {
		return fixedSize;
	}

	// Default size calculation behaviour
	//
	QHash<QWidget *, QRect> positions;
	QList<QLayoutItem *> processingItems;
	int topContentHeight = 0;
	int bottomContentHeight = 0;
	int leftContentWidth = 0;
	int rightContentWidth = 0;

	for (int i = 0; i < fLayoutItems.count(); ++i) {
		QLayoutItem *item = fLayoutItems.at(i);
		if(item) {
			QRect itemRect = calculateItemSize(item, sizeType, topContentHeight, bottomContentHeight, leftContentWidth, rightContentWidth, positions, processingItems);
		}
	}

	// Use whichever fixed size dimension value is available, and
	// whichever isn't available, use the calculated size hint
	// value.
	//
	QSize result(leftContentWidth + rightContentWidth, topContentHeight + bottomContentHeight);
	if(fixedSize.width() >= 0) {
		return QSize(fixedSize.width(), result.height());
	} else if(fixedSize.height() >= 0) {
		return QSize(result.width(), fixedSize.height());
	} else {
		return result;
	}
}

int	KxFormLayout::numDivisions() const
{
	return fNumDivisions;
}

void KxFormLayout::attach(QWidget *thisControl,
						 KxAttachmentEdge byThisEdge,
						 KxAttachmentType inThisManner,
						 int offset /* = 0 */,
						 int position /* = 0 */)
//
//	Description:
//		Attach the given control.  Note the position argument only has
//		effect when the attachment type is position.
//
{
	//	Note that this method does not support attaching controls to
	//	other controls.
	//
	Q_ASSERT(inThisManner == kAttachNone ||
				 inThisManner == kAttachForm ||
				 inThisManner == kAttachOppositeForm ||
				 inThisManner == kAttachPosition);
		  
	Q_ASSERT(position >= 0);

	// get the attachments;
	QLayoutItem *item = NULL;
	foreach(QLayoutItem *i, fLayoutItems) {
		if(i && i->widget() == thisControl) {
			item = i;
			break;
		}
	}
	Q_ASSERT(item != NULL);
	
	QHash<KxAttachmentEdge, AttachmentInfo> attachments = fHash[item];

	Q_ASSERT(byThisEdge >= kAttachTop && byThisEdge <= kAttachRight);
	if (!(byThisEdge >= kAttachTop && byThisEdge <= kAttachRight))
		return;

	if (!(inThisManner >= kAttachNone && inThisManner <= kAttachOppositeControl))
		inThisManner = kAttachNone;

	//
	// Don't trigger a new layout if attachment has not changed.
	//
	AttachmentInfo info = attachments[byThisEdge];
	if (info.fType != inThisManner || info.fOffset != offset || info.fPosition != position) {
		info.fType = inThisManner;
		info.fOffset = offset;
		info.fPosition = position;
		info.fOtherControl = NULL;
		attachments.insert(byThisEdge, info);
		fHash.insert(item, attachments);
		fNeedLayout = true;
		invalidate();
	}
}

void KxFormLayout::attach(QWidget *thisControl, 
						 QWidget *toThisControl,
						 KxAttachmentEdge byThisEdge,
						 KxAttachmentType inThisManner,
						 int offset /* = 0 */)
//
//	Description:
//		Attach the given controls.
//
{
	//	Note that this method only supports attaching controls to
	//	other controls.
	//
	Q_ASSERT(thisControl != toThisControl);

	// get the attachments;
	QLayoutItem *item = NULL;
	QLayoutItem *otherItem = NULL;
	foreach(QLayoutItem *i, fLayoutItems) {
		if(i) {
			if(i->widget() == thisControl) {
				item = i;
			} else if(i->widget() == toThisControl) {
				otherItem = i;
			}
			if(item && otherItem) break;
		}
	}
	Q_ASSERT(item != NULL && otherItem != NULL);
	
	QHash<KxAttachmentEdge, AttachmentInfo> attachments = fHash[item];

	Q_ASSERT(byThisEdge >= kAttachTop && byThisEdge <= kAttachRight);
	if (!(byThisEdge >= kAttachTop && byThisEdge <= kAttachRight))
		return;

	if (!(inThisManner >= kAttachNone && inThisManner <= kAttachOppositeControl))
		inThisManner = kAttachNone;

	//
	// Don't trigger a new layout if attachment has not changed.
	//
	AttachmentInfo info = attachments[byThisEdge];
	if (info.fType != inThisManner || info.fOffset != offset || info.fOtherControl != otherItem) {
		info.fType = inThisManner;
		info.fOffset = offset;
		info.fPosition = 0;
		info.fOtherControl = otherItem;
		attachments.insert(byThisEdge, info);
		fHash.insert(item, attachments);
		fNeedLayout = true;
		invalidate();
	}
}
