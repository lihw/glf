#ifndef KxCursorShape_H_
#define KxCursorShape_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxCursorShape {
public:
    KxCursorShape(Qt::CursorShape);
    KxCursorShape(QString);
    KxCursorShape(const KxCursorShape &);
#if defined(OSWin_)
    KxCursorShape(HCURSOR);
#endif

    //	Create a cursor from raw bit data. 'cursorBits' and 'maskBits' must
    //	be in xbm format.
    KxCursorShape(
        short width,
        short height,
        short hotX,
        short hotY,
        const unsigned char* cursorBits,
        const unsigned char* maskBits
        );

    bool		operator==(const KxCursorShape &) const;
    bool		operator!=(const KxCursorShape &) const;
    virtual ~KxCursorShape();

    QCursor		getCursor() const;

protected:
    void			deleteBitmaps() const;

private:
    mutable QSharedPointer<QCursor>	fCursorPtr;
    QSize							fCursorSize;
    mutable unsigned char*			fCursorBits;
    QString                         fCustomName;
    short							fHotX;
    short							fHotY;
    mutable unsigned char*			fMaskBits;
    Qt::CursorShape                 fShape;
#if defined(OSWin_)
    HCURSOR							fWinCursorHdl;
#endif
};
#endif