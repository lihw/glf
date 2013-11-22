#ifndef KxPaneLayout_H
#define KxPaneLayout_H

#include "../base/HQtGui.h"

class KX_DLLEXPORT  KxPaneLayout : public QVBoxLayout
{
	Q_OBJECT
	
public:
	KxPaneLayout(QWidget *parent);
	~KxPaneLayout();

	void addWidget(QWidget *widget);
	void addItem(QLayoutItem *item);
	void removeChild(QWidget *w);
	void setGeometry(const QRect &r);

	virtual bool eventFilter(QObject *obj, QEvent *event);

	QWidget* splitterWidget() const { return fSplitter; };

	enum   TpaneDisplay { k1Pane = 0,
						   
						  k2PanesSplitHorizontal,
						  k2PanesSplitVertical,
						   
						  k3PanesSplitHorizontal,
						  k3PanesSplitVertical,
						  k3PanesSplitTop,
						  k3PanesSplitLeft,
						  k3PanesSplitBottom,
						  k3PanesSplitRight,
						   
						  k4PanesSplitHorizontal,
						  k4PanesSplitVertical,
						  k4PanesSplitTop,
						  k4PanesSplitLeft,
						  k4PanesSplitBottom,
						  k4PanesSplitRight,
						  k4PanesQuad,

						  //	Make sure this reflects the number of
						  //	possible display configurations.
						  //
						  kPaneDisplayCount
						};

	void				setPaneDisplay			(TpaneDisplay);
	TpaneDisplay		paneDisplay				() const;
	int					numVisiblePanes			(TpaneDisplay) const;
	int					numVisiblePanes			() const;
	bool				isPaneVisible			(int index) const;

	void				setPane					(QWidget *child, int paneIndex);
	int					pane					(QWidget *child) const;
	int					paneUnderPointer		() const;

	QWidget *			paneChild				(int index) const;
	int					getChildCount			() const;
    //TUIInventory uses QMetaObject::invokeMethod on getChildren, so must be Q_INVOKABLE
	Q_INVOKABLE     QObjectList     getChildren() const; 

	void				activatePane			(int paneIndex);
	void				deactivatePane			(int paneIndex);
	int					activePane				() const;

	void				setPaneSize				(int paneIndex, int paneWidth, int paneHeight, TpaneDisplay display = kPaneDisplayCount);
	void				applyPaneSizes			();
	void				paneSize				(int paneIndex, int &paneWidth, int &paneHeight) const;
	
	void				setSeparatorThickness	(int thickness);
	int					separatorThickness		();
	
	void				setSeperatorMovedCommand(QObject *script);

	int visiblePaneWithStaticWidthProperty		() const;
	int visiblePaneWithStaticHeightProperty		() const;
	void enforceStaticPaneProperties			();

	bool				isStaticPaneVisible		() const;
	void				setStaticWidthPane		(int paneIndex, bool restorePaneSizes = true, bool forceSet = false);
	void				setStaticHeightPane		(int paneIndex, bool restorePaneSizes = true, bool forceSet = false);
	void				applyHorizontalStretch	(bool restorePaneSizes);
	void				applyVerticalStretch	(bool restorePaneSizes);

public slots:
	void				paneSizesChanged		(int position, int index);

signals:
	void				paneLayoutAboutToChange	();
	void				paneLayoutChanged		();

protected:
	void				refreshPaneDisplay		(TpaneDisplay);
	void 				allocateNewPaneSizes	(int paneIndex, int panePercent, int paneCount, QMultiHash<int, int>& paneOrder, QHash<int, int>& paneSizes);
	void 				applyNewPaneSizes		();
	
private:
	int							fActivePane;
	QSplitter *					fSplitter;
	QSplitter *					fTopSecondarySplitter;
	QSplitter *					fBottomSecondarySplitter;
    TpaneDisplay    			fPaneDisplay;
	QPointer<QWidget>			fPanes[4];
	QMultiHash<int, int>		fSplitterPaneOrder;
	QMultiHash<int, int>		fTopSecSplitterPaneOrder;
	QMultiHash<int, int>		fBotSecSplitterPaneOrder;
	QHash<int, int>				fSplitterPaneSizes;
	QHash<int, int>				fTopSecSplitterPaneSizes;
	QHash<int, int>				fBotSecSplitterPaneSizes;
	QWidget *					fHiddenParent;
	bool						fUnderConstruction;
	bool						fSplitterIsMoving;
	bool						fInitialShowPerformed;
	int							fPaneHorizontalStretch[4];
	int							fPaneVerticalStretch[4];
	int							fTopSecSplitterHorStretch;
	int							fTopSecSplitterVerStretch;
	int							fBotSecSplitterHorStretch;
	int							fBotSecSplitterVerStretch;
};

#endif
