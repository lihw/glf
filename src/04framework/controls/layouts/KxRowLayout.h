#ifndef KxRowLayout_H_
#define KxRowLayout_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxRowLayout : public QHBoxLayout 
{
    Q_OBJECT
public:
    KxRowLayout(QWidget *parent);
    enum OffsetSide { kLeftSide, kRightSide, kTopSide, kBottomSide, kBothHSides, kBothVSides };

    void setAutoDistributeWidths(bool autoDistribute);
    void setColumnAdjustable(int column);
    void setColumnWidth(int column, int width, bool adjustable);
    void setColumnAlignment(int column, Qt::Alignment alignment);
    void setColumnAttachment(int column, OffsetSide side, int offset);
    void setRowAttachment(int column, OffsetSide side, int offset);

    QSize minimumSize() const;
    void setGeometry(const QRect &rect);
    QSize sizeHint() const;

    virtual void addItem ( QLayoutItem * item );
    void addWidget ( QWidget * w );
    void insertItem ( int index, QLayoutItem * item );
    void insertWidget ( int index, QWidget * w );

    void setNumColumns( int num );
    int numColumns();

protected:
    void applyColumnWidths();
    void applyColumnAlignment(int column);

private:
    struct ColumnWidthProperties {
        int width;
        bool isAdjustable;
    };

    struct ColumnAttachProperties {
        bool leftAttach;
        int leftOffset;
        bool rightAttach;
        int rightOffset;
        bool topAttach;
        int topOffset;
        bool bottomAttach;
        int bottomOffset;
    };

    QHash<int, ColumnWidthProperties>	fColumnWidths;
    QHash<int, ColumnAttachProperties>	fColumnOffsets;
    QHash<int, Qt::Alignment>			fColumnAlignments;
    int									fNumColumns;
    int									fAdjustableColumn;
    bool 							fAutoDistributeWidths;

};

#endif