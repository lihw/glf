#ifndef KxFlowLayout_H_
#define KxFlowLayout_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxFlowLayout : public QLayout
{
	Q_OBJECT
public:
    KxFlowLayout(QWidget *parent, bool vertical = false, bool wrap = false, int margin = 0, int spacing = -1);
    ~KxFlowLayout();

    void addItem(QLayoutItem *item);
    Qt::Orientations expandingDirections() const;
    int count() const;
    QLayoutItem *itemAt(int index) const;
    QSize minimumSize() const;
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;
    QLayoutItem *takeAt(int index);
	bool isVertical();
	void setWrap(bool wrap);
	bool doesWrap();

private:
    int doLayout(const QRect &rect) const;

    QList<QLayoutItem *> itemList;
	bool fVertical;
	bool fWrap;
};

#endif
