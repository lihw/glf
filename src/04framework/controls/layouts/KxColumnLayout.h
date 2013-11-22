#ifndef KxColumnLayout_H_
#define KxColumnLayout_H_
#include "../base/HQtGui.h"

class KX_DLLEXPORT KxColumnLayout : public QVBoxLayout 
{
    Q_OBJECT
public:
    KxColumnLayout(QWidget *parent);

    enum OffsetSide { kLeftSide, kRightSide, kBothSides };

    virtual QSize minimumSize() const;
    virtual QSize sizeHint() const;
    virtual void setGeometry(const QRect &rect);

    virtual void addItem ( QLayoutItem * item );
    void addWidget ( QWidget * w );

    void setColumnWidth(int width);
    int columnWidth();
    void setColumnAdjustable(bool  adjustable);
    void setColumnOffset(OffsetSide side, int offset);
    void setColumnAttachment(OffsetSide side, int offset);
    void setColumnAlignment(Qt::Alignment alignment);

protected:
    void applyRowHeights();
    void applyColumnAlignment(QLayoutItem * item);
    void enforceColumnWidth();

private:
    bool  fColumnAdjustable;
    int fColumnWidth;
    Qt::Alignment fColumnAlignment;
    int fLeftOffset;
    int fRightOffset;
    bool fLeftAttach;
    bool fRightAttach;
};

#endif