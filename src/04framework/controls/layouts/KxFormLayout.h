#ifndef KxFormLayout_H_
#define KxFormLayout_H_
#include "../base/HQtGui.h"

class KX_DLLEXPORT KxFormLayout : public QLayout
{
	Q_OBJECT
public:
	KxFormLayout(QWidget *parent, int numDivisions = 100, int margin = 0, int spacing = -1);
	KxFormLayout(int numDivisions, int spacing = -1);
	~KxFormLayout();

	void addItem(QLayoutItem *item);
	Qt::Orientations expandingDirections() const;
	bool hasHeightForWidth() const;
	int count() const;
	QLayoutItem *itemAt(int index) const;
	QSize minimumSize() const;
	void setGeometry(const QRect &rect);
	QSize sizeHint() const;
	QLayoutItem *takeAt(int index);

	int					numDivisions() const;
	void				doLayout();
	//	Use to attach control to form or form position or reset to no
	//	attachment.
	//
	void				attach(QWidget *thisControl,
							   KxAttachmentEdge byThisEdge,
							   KxAttachmentType inThisManner,
							   int offset = 0,
							   int position = 0);
							   
	//	Use to attach one control to another control.
	//
	void				attach(QWidget *thisControl, 
							   QWidget *toThisControl,
							   KxAttachmentEdge byThisEdge,
							   KxAttachmentType inThisManner,
							   int offset = 0);

private:
	struct AttachmentInfo
	{
		AttachmentInfo() {
			fType = kAttachNone;
			fOffset = 0;
			fPosition = 0;
			fOtherControl = NULL;
		}
		
		AttachmentInfo(KxAttachmentType type, int offset, int position, QLayoutItem *otherControl) {
			fType = type;
			fOffset = offset;
			fPosition = position;
			fOtherControl = otherControl;
		}

		KxAttachmentType fType;
		int fOffset;
		int fPosition;
		QLayoutItem *fOtherControl;	
	};

	enum SizeType { MinimumSize, SizeHint, LayoutSize };

	QRect calculateItemRect(QLayoutItem *item, QSize &contentSize, QHash<QWidget *, QRect> &positions, QList<QLayoutItem *> &processingItems);
	QRect calculateItemSize(QLayoutItem *item, SizeType sizeType, int &topContentHeight, int &bottomContentHeight, int &leftContentWidth, int &rightContentWidth, QHash<QWidget *, QRect> &positions, QList<QLayoutItem *> &processingItems) const;
	QSize calculateSize(SizeType sizeType) const;

	QList<QLayoutItem *> fLayoutItems;
	QHash<QLayoutItem *, QHash<KxAttachmentEdge, AttachmentInfo> > fHash;
	QHash<QLayoutItem *, QHash<KxAttachmentEdge, AttachmentInfo> > fIntendedAttachments;
	
	int	fNumDivisions;
	bool fNeedLayout;
};

#endif
