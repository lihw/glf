#ifndef KxTableView_H_
#define KxTableView_H_

#include "../../base/HQtGui.h"
#include "KxTable.h"

class KX_DLLEXPORT KxTableView : public QTableView {
    Q_OBJECT;
public:
    KxTableView	(QWidget *parent = NULL);
    virtual			~KxTableView	();

    virtual void	mousePressEvent	(QMouseEvent *);
    virtual void	mouseReleaseEvent(QMouseEvent *);

    // make all columns equally wide, maximizing the table horizontally
    virtual void	expandColumns	();

    // clarify confusing Qt method names
    QHeaderView *	rowHeader		();
    QHeaderView *	columnHeader	();

    // deal with tabs (used by component box)
    void			handleTabKey(int key);

    // convenience methods
    void			showAllColumns(bool show=true);
    void			showAllRows(bool show=true);

signals:
    void			polish			(QEvent *);

    public slots:
//#ifndef NDEBUG
//        void			forgetEditor	(QTableView *);
//#endif /* !NDEBUG */
        virtual void	selectionChanged(const QItemSelection &selected,
            const QItemSelection &deselected);
        protected slots:
            virtual void	dataChanged(const QModelIndex& topLeft,
                const QModelIndex& bottomRight);

protected:
    // needed to get QEvent::Polish
    virtual bool	event			(QEvent *);
    // to handle expanding columns
    virtual void	resizeEvent		(QResizeEvent *);

    QItemSelection preMousePressSelection;
    QItemSelection postMouseReleaseSelection;
};

inline QHeaderView * KxTableView::rowHeader()	{ return verticalHeader(); }
inline QHeaderView * KxTableView::columnHeader() { return horizontalHeader(); }

inline void  KxTableView::handleTabKey(int key)
{
    if (key == Qt::Key_Backtab) {
        setCurrentIndex(moveCursor(MovePrevious, Qt::NoModifier));
    } else if (key == Qt::Key_Tab) {
        setCurrentIndex(moveCursor(MoveNext, Qt::NoModifier));
    }
}
#endif