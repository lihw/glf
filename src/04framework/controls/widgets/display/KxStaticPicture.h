#ifndef KxStaticPicture_H_
#define KxStaticPicture_H_
#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxStaticPicture : public QWidget 
{
    Q_OBJECT
public:
    KxStaticPicture(QWidget *parent=0);

    bool				setPicture(const QString&);

    void				setTile(bool);
    bool				tile() const;

    const QString &		picture() const;
    void                setPixmap(const QPixmap&);
    const QPixmap&      getPixmap() const;

    virtual QSize 		sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    // methods
    void            initializeNewPixmap();

    // data
    QPixmap			fPixmap;

    bool			fTiled;
    QString			fPictureFilename;
};

inline
const QPixmap& KxStaticPicture::getPixmap() const
{
    return fPixmap;

}
#endif