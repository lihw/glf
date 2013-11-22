#ifndef KxRowColumnLayout_H_
#define KxRowColumnLayout_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxRowColumnLayout : public QLayout 
{
    Q_OBJECT
public:
    enum ConstraintMode {
        kFixNumberOfColumns,
        kFixNumberOfRows
    };

    enum OffsetSide { kLeftSide, kRightSide, kTopSide, kBottomSide, kBothHSides, kBothVSides };

    KxRowColumnLayout( 
        QWidget * parent, 
        ConstraintMode mode = kFixNumberOfColumns, 
        int numberOfRowsOrColumns = 1);

    virtual void addItem( QLayoutItem *item );
    virtual QLayoutItem *itemAt(int index) const;
    virtual QLayoutItem *takeAt(int index);
    int count() const;
    QSize minimumSize() const;

    void setColumnWidth(int column, int width, bool adjustable);
    void setRowHeight(int row, int height);

    void setColumnAlignment(int column, Qt::Alignment alignment);
    void setRowAlignment(int row, Qt::Alignment alignment);

    void setColumnAttachment(int column, KxRowColumnLayout::OffsetSide side, int offset);
    void setRowAttachment(int row, KxRowColumnLayout::OffsetSide side, int offset);

    void setGeometry(const QRect &rect);
    QSize sizeHint() const;

    void setColumnSpacing(int column, int spacing);   
    void setRowSpacing(int row, int spacing);

    bool isHorizontal();
    bool isVertical();

    int numRows() const;
    int numColumns() const;

    int					numberOfRowsSpecifiedByUser();
    int					numberOfColumnsSpecifiedByUser();

protected:	
    void calculateDefaultWidths();
    void calculateDefaultHeights();
    void applyWidthsAndHeights();
    void applyColumnAlignment(int column);
    void applyRowAlignment(int row);

private:
    struct LayoutItemProperties {
        QLayoutItem *item;
        int row;
        int column;
    };

    struct ColumnWidthProperties {
        int width;
        bool isAdjustable;
    };

    struct ColumnAttachProperties {
        bool leftAttach;
        int leftOffset;
        bool rightAttach;
        int rightOffset;
    };

    struct RowAttachProperties {		
        bool topAttach;
        int topOffset;
        bool bottomAttach;
        int bottomOffset;
    };

    QList<LayoutItemProperties>			fLayoutItems;

    QHash<int, ColumnWidthProperties>	fColumnWidths;
    QHash<int, int>						fDefaultColumnWidths;
    QHash<int, int>						fSetRowHeights;
    QHash<int, int>						fDefaultRowHeights;

    QHash<int, ColumnAttachProperties>	fColumnOffsets;
    QHash<int, RowAttachProperties>		fRowOffsets;

    QHash<int, Qt::Alignment>			fColumnAlignments;
    QHash<int, Qt::Alignment>			fRowAlignments;

    QHash<int, int>				fColumnSpacing;
    QHash<int, int>				fRowSpacing;

    int							fAllColumnSpacing;
    int							fAllRowSpacing;

    ConstraintMode  			fConstraintMode;
    int          			fFixedNumberOfRowsOrColumns;
};

#endif