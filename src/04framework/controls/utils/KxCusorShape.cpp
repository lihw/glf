
#include "KxCursorShape.h"

#include "../base/KxQtHelper.h"

//	Class:   	KxCursorShape
//
//	Author(s):	Brian Kramer
//
//	Description: 
//		The cursorShape class is used to access application defined
//		cursors.  All cursorShape objects are stored within resources.
//		The cursorShape should then be the resource number of the
//		cursor to use. In addition, there are some pre-defined cursors
//		resources that applications may use.


KxCursorShape::KxCursorShape(QString name)
:	fCursorPtr(NULL)
,	fCursorBits(NULL)
,	fMaskBits(NULL)
,	fShape(Qt::BitmapCursor)
#if defined(OSWin_)
,	fWinCursorHdl(NULL)
#endif
{
	fCustomName = name;
}

KxCursorShape::KxCursorShape(Qt::CursorShape shape)
:	fCursorPtr(NULL)
,	fCursorBits(NULL)
,	fCustomName("")
,	fMaskBits(NULL)
#if defined(OSWin_)
,	fWinCursorHdl(NULL)
#endif
{
	fShape = shape;
}

KxCursorShape::KxCursorShape(const KxCursorShape & cursor)
:	fCursorPtr(NULL)
,	fCursorBits(NULL)
,	fCustomName("")
,	fMaskBits(NULL)
,	fShape(Qt::BitmapCursor)
#if defined(OSWin_)
,	fWinCursorHdl(NULL)
#endif
{
    if (!cursor.fCursorPtr.isNull())
	{
		fCursorPtr = cursor.fCursorPtr;
	}
	else if (!cursor.fCustomName.isEmpty())
	{
		fCustomName = cursor.fCustomName;
	}
	else if (cursor.fCursorBits || cursor.fMaskBits)
	{
		fCursorSize = cursor.fCursorSize;
		fHotX = cursor.fHotX;
		fHotY = cursor.fHotY;

		//	Copy the bitmaps.
		int  numBytes = fCursorSize.width() * fCursorSize.height();

		if (cursor.fCursorBits && (fCursorBits = new unsigned char[numBytes])) {
			memcpy(fCursorBits, cursor.fCursorBits, numBytes);
		}

		if (cursor.fMaskBits && (fMaskBits = new unsigned char[numBytes])) {
			memcpy(fMaskBits, cursor.fMaskBits, numBytes);
		}
	}
#if defined(OSWin_)
	else if (cursor.fWinCursorHdl != NULL)
	{
		fWinCursorHdl = cursor.fWinCursorHdl;
	}
#endif
	else
	{
		fShape=cursor.fShape;
	}
}


#if defined(OSWin_)
KxCursorShape::KxCursorShape(HCURSOR hCursor)
:	fCursorPtr(NULL)
,	fCursorBits(NULL)
,	fCustomName("")
,	fMaskBits(NULL)
,	fShape(Qt::BitmapCursor)
,	fWinCursorHdl(hCursor)
{
}
#endif


KxCursorShape::KxCursorShape(
		short width,
		short height,
		short hotX,
		short hotY,
		const unsigned char* cursorBits,
		const unsigned char* maskBits
)
:	fCursorPtr(NULL)
,	fCustomName("")
,	fShape(Qt::BitmapCursor)
#if defined(OSWin_)
,	fWinCursorHdl(NULL)
#endif
{
	//	Make sure that the dimensions are sane.
	if (width < 1) width = 1;
	if (height < 1) height = 1;

	//	A negative hotspot position is fine because Qt interprets that as a
	//	request to put the hotspot in the center. But let's make sure that
	//	positive values are within the dimensions of the cursor.
	if (hotX >= width) hotX = width - 1;
	if (hotY >= height) hotY = height - 1;

	fHotX = hotX;
	fHotY = hotY;
	fCursorSize.setWidth(width);
	fCursorSize.setHeight(height);

	unsigned int numBytes = width * height;

	if (cursorBits && (fCursorBits = new unsigned char[numBytes])) {
		memcpy(fCursorBits, cursorBits, numBytes);
	} else {
		fCursorBits = NULL;
	}

	if (maskBits && (fMaskBits = new unsigned char[numBytes])) {
		memcpy(fMaskBits, maskBits, numBytes);
	} else {
		fMaskBits = NULL;
	}
}


KxCursorShape::~KxCursorShape()
{
	fCursorPtr.clear();
	deleteBitmaps();
}

/*KxCursorShape::KxCursorShape(QCursor cursor)
//CAUTION: attempting to create a QCursor before the QApplication will cause a runtime crash. 
//   only use this after the application has initialized.
{
	fProvidedCursor = cursor;
	fCustomName = "";
	fShape = Qt::BitmapCursor;	//QT::BitmapCursor is one of the shape enums, but it represents a cursor defined in some other way
}*/

QCursor KxCursorShape::getCursor()const
{
	if(fCursorPtr.isNull()) {
		QCursor*  newCursor = NULL;

		if (!fCustomName.isEmpty())
		{
			newCursor = new QCursor(KxQtHelper::createCursor(fCustomName));
		}
		else if (fCursorBits || fMaskBits)
		{
			QBitmap cursorBitmap(fCursorSize);
			QBitmap maskBitmap(fCursorSize);
			
			if (fCursorBits) {
				cursorBitmap = QBitmap::fromData(
									fCursorSize, fCursorBits, QImage::Format_MonoLSB
								);
			}
			
			if (fMaskBits) {
				maskBitmap = QBitmap::fromData(
								fCursorSize, fMaskBits, QImage::Format_MonoLSB
							);
			}

			//	We no longer need the raw data so get rid of it.
			deleteBitmaps();

			newCursor = new QCursor(cursorBitmap, maskBitmap, fHotX, fHotY);
		}
#if defined(OSWin_)
		else if (fWinCursorHdl != NULL)
		{
			newCursor = new QCursor(fWinCursorHdl);
		}
#endif
		else
		{
			newCursor = new QCursor(fShape);
		}

		fCursorPtr = QSharedPointer<QCursor>(newCursor);
	}
    Q_ASSERT(fCursorPtr);
	return *fCursorPtr;
}

bool KxCursorShape::operator==(const KxCursorShape & cursorShape) const
//Behavior note: if the KxCursorShape was defined using the QCursor constructor, it will
//  never be equal to another KxCursorShape, due to QT QCursor implementation
{
	bool result = false;

	if ((&cursorShape == this)
	||	(!fCursorPtr.isNull() && (fCursorPtr == cursorShape.fCursorPtr)))
	{
		result = true;
	}
	else if (!fCustomName.isEmpty())
	{
		result = (fCustomName == cursorShape.fCustomName);
	}
	else if (fCursorBits || fMaskBits)
	{
		result = false;
	}
#if defined(OSWin_)
	else if (fWinCursorHdl != NULL)
	{
		result = (fWinCursorHdl == cursorShape.fWinCursorHdl);
	}
#endif
	else if (fShape == cursorShape.fShape)
	{
		result = true;
	}

	return result;
}

bool KxCursorShape::operator!=(const KxCursorShape & cursorShape) const
{
	return !(*this == cursorShape);
}


//	Although this modifies member variables, they are all mutable, so it is
//	officially a 'const' method.
void KxCursorShape::deleteBitmaps() const
{
	if (fCursorBits) {
		delete [] fCursorBits;
		fCursorBits = NULL;
	}

	if (fMaskBits) {
		delete [] fMaskBits;
		fMaskBits = NULL;
	}
}
