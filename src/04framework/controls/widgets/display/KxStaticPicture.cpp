#include "KxStaticPicture.h"
#include "../../base/KxQtHelper.h"

KxStaticPicture::KxStaticPicture(QWidget *parent)
: QWidget(parent)
, fTiled(false)
{
}

void KxStaticPicture::initializeNewPixmap()
{
    if(!fPixmap.hasAlpha()) {
        QBitmap mask = fPixmap.createMaskFromColor(QColor(0xC1, 0xC1, 0xC1));
        fPixmap.setMask(mask);
    }
}

/*!
  Load an image file into memory using all possible image loaders.

  \param filename of image file.  First the full file path will be
  tried, if that fails the icon-style loading will be attempted.

  \return true if an image was loaded
 */
bool KxStaticPicture::setPicture(const QString &name)
{
    if (name.isEmpty()) 
        return false;
    QString vname(name);
    QPixmap *p = KxQtHelper::createPixmap(vname);
	if(NULL != p) {
        fPixmap = *p;
        delete p;
		fPictureFilename = name;
        initializeNewPixmap();
        update();
        return true;
	}
	return false;
}

void KxStaticPicture::setTile(bool inTile)
{
	fTiled = inTile;
	update();
}

bool KxStaticPicture::tile() const
{
	return fTiled;
}

const QString& KxStaticPicture::picture() const
{
	return fPictureFilename;
}

void KxStaticPicture::paintEvent(QPaintEvent * event)
{
	QPainter painter(this);
	if(fTiled) {
		painter.drawTiledPixmap(rect(), fPixmap);
	} else {
		painter.drawPixmap(0,0, fPixmap);
	}
}

void KxStaticPicture::setPixmap(const QPixmap& pixmap)
{
    fPixmap = pixmap;
    initializeNewPixmap();
}

QSize KxStaticPicture::sizeHint() const
{
	return QSize(fPixmap.width(), fPixmap.height());
}
