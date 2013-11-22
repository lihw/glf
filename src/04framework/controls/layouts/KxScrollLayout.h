#ifndef KxScrollLayout_H
#define KxScrollLayout_H

#include "../base/HQtGui.h"

class KxScrollArea;

class KX_DLLEXPORT KxScrollLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    KxScrollLayout(QWidget *parent , bool isChildResizable = false);
    void			addWidget(QWidget *widget);
    void            addItem(QLayoutItem *item);
    virtual int     count() const;
    int          scrollAreaWidth();
    int          scrollAreaHeight();
    void            getScrollAreaTopLeftCornerValue(
                        int &top, int &left);
    void            scrollByPixel(
                        const QString &direction, int nPixels);
    void            scrollPage(const QString &direction);
    //void            setCommandScript(
    //                    QObject *script,
    //                    const char *commandType);

	//TUIInventory uses QMetaObject::invokeMethod on getChildren, so must be Q_INVOKABLE
	Q_INVOKABLE     QObjectList     getChildren() const; 

private:
    KxScrollArea *   fScrollArea;
    QWidget *       fInternalWidget;
	bool			fUnderConstruction;
};
#endif
