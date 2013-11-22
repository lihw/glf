#ifndef KxTable_H_
#define KxTable_H_

#include "../../base/HQtGui.h"

class KxTableView;

class KX_DLLEXPORT KxTable : public QAbstractTableModel
{
    Q_OBJECT
public:

    // ENUMERATIONS
    enum TlabelType {
        kNoLabel,
        kRowLabel,
        kColumnLabel,
        kAllLabels
    };


    KxTable(QWidget *parent = 0, int rows = 0, int columns = 0);
    virtual ~KxTable();

    // QAbstractTableModel interface
    virtual int				rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int				columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant		data(const QModelIndex &index, int role = Qt::DisplayRole) const;	
    virtual QVariant		headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    virtual Qt::ItemFlags	flags(const QModelIndex &index) const;
    virtual bool			setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

    // Maya interface
    virtual bool		getCell(int row, int column, QString &value) const = 0;
    virtual void			getLabel(Qt::Orientation orientation, int number, QString &value) const;
    virtual bool		cellChanged(int row, int column, const QString& );

    // convenience methods
    virtual void			getColumnLabel(int index, QString &value) const;
    virtual void			getRowLabel(int index, QString &value) const;
    static QString &		generateNewCellValue(const QString &currentValue,
        const QString &newValue);

    // rows/columns
    void					addRow();
    void					setNumberOfRows(int count);
    int					numberOfRows() const;
    void					addColumn();
    void					setNumberOfColumns(int count);
    int					numberOfColumns() const;

    void					setColumnWidth(int firstCol, int lastCol, int columnWidth);

    // visibility
    void					hideRowLabels	();
    void					showRowLabels	();
    void					hideColumnLabels();
    void					showColumnLabels();

    void					hideColumns(int firstCol, int lastCol );
    void					showColumns(int firstCol, int lastCol );
    void                    showAllColumns();


    // selection
    bool				isSelected(int row, int col);
   // void                    getSelectedColumns( boolArray& selected );
    bool                selection(  int& firstRow, int& lastRow,
        int& firstCol, int& lastCol, int rangeIndex);
    void					clearSelection (bool notify=true);
    void					setSelection (int row, int col, bool notify=true);
    void					setSelection (int firstRow, int lastRow, int firstCol, int lastCol, bool notify=true);
    void					addToSelection (int row, int col, bool notify=true);
    void					removeFromSelection (int row, int col, bool notify=true);

    // spans
    void					setSpan (int row, int column, int rowSpan, int columnSpan);

    // other
    void					redrawCells();
    void					redrawCells(int firstRow, int lastRow, int firstColumn, int lastColumn);
    void                    redrawLabels(Qt::Orientation);
    virtual bool 		suspendUpdates(bool newValue);

    KxTableView *		tableView() const;
    virtual void			setTableView(KxTableView *);

    protected slots:
        virtual void			selectionChanged();
        virtual void			cellEntered(const QModelIndex& index);

//#ifndef NDEBUG
//        // debugging slots
//        virtual void			selectionModelDestroyed();
//        virtual void			tableViewDestroyed();
//#endif /* !NDEBUG */

protected:
    void					setSelection(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command, bool notify=true);
    bool					tableViewIsNULL() const;

    // ENUMERATIONS
    enum {
        kShowRowLabels,
        kShowColumnLabels,
        kUse,
        kIsChannelBox,
        kTrackingPopupMenu,
        kInDirectDraw,
        kAllowDirectDraw,
        kEditFieldChanged,
        kAnchorValid,
        kInRowSelect,
        kInColumnSelect,
        kSaveAllowDirectDraw,
        kLastFlag
    };


    QList<QRect>				fSelectionBlocks;

private:
    QPointer<KxTableView> fTableView;

    int					fRows;
    int					fColumns;
};

inline void KxTable::addRow()
{ setNumberOfRows(fRows + 1); }

inline void KxTable::addColumn()
{ setNumberOfColumns(fColumns + 1); }

#endif