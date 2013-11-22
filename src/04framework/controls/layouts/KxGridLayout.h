#ifndef KxGridLayout_H_
#define KxGridLayout_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxGridLayout : public QLayout
{
	Q_OBJECT
public:
    KxGridLayout(QWidget *parent , bool columnsResize = false, bool allowEmptyCells = true, bool autoGrow = true);
    KxGridLayout(bool columnsResize, bool allowEmptyCells, bool autoGrow = true);
	~KxGridLayout();

    virtual void addItem(QLayoutItem *item);
    virtual Qt::Orientations expandingDirections() const;
	virtual bool hasHeightForWidth() const;
	virtual int heightForWidth(int width) const;
    virtual int count() const;
    virtual QLayoutItem *itemAt(int index) const;
    virtual QSize minimumSize() const;
    virtual void setGeometry(const QRect &rect);
    virtual QSize sizeHint() const;
    virtual QLayoutItem *takeAt(int index);

	void setCellWidth(int width);
	int cellWidth();
	void setCellHeight(int height);
	int cellHeight();

	void setNumberOfRows(int rows);
	int rowCount();
	void setNumberOfColumns(int columns);
	int columnCount();

	bool columnsResizable() const;
	bool allowEmptyCells() const;
	bool autoGrow() const;

	void cellContentInGridOrder(QList<QLayoutItem *> &gridOrderItemList);
	
	void reposition(QObject *object, int newCellPosition);

private:
    void doLayout(const QRect &rect);

    QList<QLayoutItem *> fItemList;
	QHash<int, QLayoutItem *> fPositionToItem;
	QHash<QLayoutItem *, int> fItemToPosition;
	
	int fCellWidth;
	int fCellHeight;
	
	int fNumberOfRows;
	int fNumberOfColumns;
	
	bool fColumnsResizable;
	bool fAllowEmptyCells;
	bool fAutoGrow;
	
	int fNumPositions;
};

#endif
