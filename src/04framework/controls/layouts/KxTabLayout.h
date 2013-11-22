#ifndef KxTabLayout_H
#define KxTabLayout_H

#include "../base/HQtGui.h"
#include "../drag/KxDragHandler.h"
#include "../drag/KxDropHandler.h"
#include "../widgets/tab/KxTabWidget.h"

class QTabWidget;

class KX_DLLEXPORT KxTabLayout : public QVBoxLayout,
		public KxDropHandler
{
    Q_OBJECT

public:
    KxTabLayout(QWidget *parent, bool scrollable = true, bool isChildResizable = false);
    void			addWidget(QWidget *widget);
    virtual void    addItem(QLayoutItem *item);

    virtual int     count() const;
	virtual QLayoutItem * itemAt(int index) const;
	virtual QLayoutItem * takeAt(int index);
	virtual QSize 	sizeHint() const;
    bool            isScrollable();
    void            setTabLabel(QWidget *childWidget, const QString &label);
    void            setTabLabel(int index, const QString &label);
    QString         tabLabel(int index) const;
    void            setTabsVisible(bool);
    bool        tabsAreVisible();
	
	bool		setImage(const QString &);
	const QString &	image() const;
	void			showImage();
	void			hideImage();
	bool		imageVisible() const;
	
    QWidget *       cornerWidget();
    QWidget *       currentWidget();
    int          currentIndex();
    void            selectWidget(QWidget *);
    void            selectWidget(int );
    void            moveTab(int fromIndex, int toIndex);
    //void            linkControlSignalToScriptAction(
    //                    const char* signal,
    //                    QObject* receiverScript,
    //                    const char* commandType); 
    //void            setCommandScript(
    //                    QObject *script,
    //                    const char * commandType);
    //QObject *       commandScript(const char * commandType);

	Q_INVOKABLE QObjectList		getChildren() const; //TUIInventory uses QMetaObject::invokeMethod, so must be Q_INVOKABLE 
	virtual QRect	getDropRect( QWidget*, QDropEvent*, int, void* );
	virtual void	getDropTypes(	const QWidget* target,
								const QDropEvent* event,
								KxDragItemDataTypeArray& dropTypes) const;
	virtual bool	dropData(
								QWidget*			target,
								const QDropEvent*	event,
								int					dataType,
								void*				data
							);
	inline bool		positionMatters() const { return true; }

private:
    int             indexOf(QWidget *childWidget);

    KxTabWidget * fTabWidget;
    bool             fScrollable;
	bool             fIsChildResizable;
	
	QString			fImageName;
	QList<QLayoutItem *> fItemList;
};

inline bool KxTabLayout::isScrollable()
{
    return fScrollable;
}

#endif
