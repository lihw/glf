#include "KxPaneLayout.h"

#define kPaneLayoutContentsMargin	1
#define kMaxPaneSize 100

KxPaneLayout::KxPaneLayout(QWidget *parent)
: QVBoxLayout(parent)
, fPaneDisplay(k1Pane)
, fActivePane(-1)
, fUnderConstruction(true)
, fSplitterIsMoving(false)
, fInitialShowPerformed(false)
{
	setContentsMargins(kPaneLayoutContentsMargin, kPaneLayoutContentsMargin, 
						kPaneLayoutContentsMargin, kPaneLayoutContentsMargin);
	for(int i = 0; i < 4; i++) {
		fPanes[i] = NULL;
	}

	// Initialize saved stretch factors for panes and secondary splitters
	for(int i = 0; i < 4; i++) {
		fPaneHorizontalStretch[i] = 0;
	}
	fTopSecSplitterHorStretch = 0;
	fBotSecSplitterHorStretch = 0;
	
	for(int i = 0; i < 4; i++) {
		fPaneVerticalStretch[i] = 0;
	}
	fTopSecSplitterVerStretch = 0;
	fBotSecSplitterVerStretch = 0;

	fHiddenParent = new QWidget(parent);
	fHiddenParent->hide();
	
	fSplitter = new QSplitter(parent);
	fTopSecondarySplitter = new QSplitter(fHiddenParent);
	fBottomSecondarySplitter = new QSplitter(fHiddenParent);
	
	fHiddenParent->setObjectName(kKxLayoutInternalWidgetName);
	fSplitter->setObjectName(kKxLayoutInternalWidgetName);
	fTopSecondarySplitter->setObjectName(kKxLayoutInternalWidgetName);
	fBottomSecondarySplitter->setObjectName(kKxLayoutInternalWidgetName);
	
	QObject::connect(fSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(paneSizesChanged(int, int)));
	QObject::connect(fTopSecondarySplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(paneSizesChanged(int, int)));
	QObject::connect(fBottomSecondarySplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(paneSizesChanged(int, int)));
	
	QVBoxLayout::addWidget(fSplitter);
	QVBoxLayout::addWidget(fHiddenParent);
	
	setPaneDisplay(fPaneDisplay);
	fUnderConstruction = false;
}

KxPaneLayout::~KxPaneLayout()
{
	/*const TpanelPtrArray& panelArray = gThePanelManager->panels();
	TuInt32 nPanels = panelArray.length();
	QWidget* panelControl = NULL;
	
	QList<QObject *> children = gUIInventory->getLayoutChildren(this);

	TuInt32 i;
	for (i = 0; i < nPanels; i++) {
		if(NULL != (panelControl = panelArray[i]->panelQControl())) {
			for(int j = 0; j < children.size(); j++) {
				QObject *object = children[j];
				if(panelControl == object) {
					TscriptedPanel *panel = NULL;
					if(NULL != (panel = dynamic_cast<TscriptedPanel *>(panelArray[i]))) {
						QString cmdStr;
						TscriptedPanelType *panelType = panel->scriptPanelType();
						panelType->removeFromLayoutProc(cmdStr);
						if (cmdStr.length() > 0) {
							cmdStr.append(panel->name());
							theCommandEngine->executeCommand(cmdStr,false);
						}
					}
					panelArray[i]->setPanelQControl(NULL);
					panelArray[i]->panelCleanup();
				}
			}
		}
	}*/
}

void KxPaneLayout::addWidget(QWidget *widget)
{
    QLayout::addWidget(widget);
}

void KxPaneLayout::addItem(QLayoutItem *item)
{
	QWidgetItem *wi = dynamic_cast<QWidgetItem *>(item);
	if(wi == NULL) return;
	
	if(!fUnderConstruction) {
		wi->widget()->hide();
		wi->widget()->setParent(fHiddenParent);
		wi->widget()->installEventFilter(this);
		for(int index = 0; index < 4; index++) {
			if(fPanes[index] == wi->widget()) break; // Already in layout
			if(fPanes[index]) continue; // Skip to next vacant index
			fPanes[index] = wi->widget();

			// Check for and set static width/height properties
			// for this widget
			enforceStaticPaneProperties();

			// Apply saved stretch factors to set any necessary
			// static width/height states to the new widget.
			applyHorizontalStretch(false);
			applyVerticalStretch(false);
			break;
		}
		refreshPaneDisplay(fPaneDisplay);
		applyNewPaneSizes();
	}
}

void KxPaneLayout::removeChild(QWidget *w)
{
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] == w) {
			fPanes[index] = NULL;
			break;
		}
	}
}

void KxPaneLayout::setGeometry(const QRect &r)
{
	QVBoxLayout::setGeometry(r);
	if(!fInitialShowPerformed) {
		// We want to apply the saved pane sizes once right
		// after the parent widget is first made visible.
		if(parentWidget() && parentWidget()->isVisible()) {
			applyNewPaneSizes();
			fInitialShowPerformed = true;
		}
	} else if(isStaticPaneVisible()) {
		paneSizesChanged(-1, -1);
	}
}

KxPaneLayout::TpaneDisplay KxPaneLayout::paneDisplay() const
{
	return fPaneDisplay;
}

void KxPaneLayout::setPaneDisplay(TpaneDisplay display)
{
	// notify listeners
	bool doNotification = (display != fPaneDisplay);
	if (doNotification) {
		emit paneLayoutAboutToChange();
	}

	// Changing the pane configuration, so erase all previously saved pane orderings and sizes
	fSplitterPaneOrder.clear();
	fTopSecSplitterPaneOrder.clear();
	fBotSecSplitterPaneOrder.clear();
	fSplitterPaneSizes.clear();
	fTopSecSplitterPaneSizes.clear();
	fBotSecSplitterPaneSizes.clear();

	// Hide all panes and secondary splitters
	while(fBottomSecondarySplitter->count()) {
		fBottomSecondarySplitter->widget(0)->hide();
		fBottomSecondarySplitter->widget(0)->setParent(fHiddenParent);
	}
	while(fTopSecondarySplitter->count()) {
		fTopSecondarySplitter->widget(0)->hide();
		fTopSecondarySplitter->widget(0)->setParent(fHiddenParent);
	}
	while(fSplitter->count()) {
		fSplitter->widget(0)->hide();
		fSplitter->widget(0)->setParent(fHiddenParent);
	}

	// Set static width/height states back to the default, which is none
	setStaticWidthPane(-1, false);
	setStaticHeightPane(-1, false);

	// Set up the main splitter and secondary splitters according to the configuration
	switch(display) {
		case k3PanesSplitTop:
		case k4PanesSplitTop:
			fSplitter->insertWidget(0, fTopSecondarySplitter);
			fTopSecondarySplitter->setOrientation(Qt::Horizontal);
			fTopSecondarySplitter->show();
		case k2PanesSplitHorizontal:
		case k3PanesSplitHorizontal:
		case k4PanesSplitHorizontal:
			fSplitter->setOrientation(Qt::Vertical);
			break;

		case k3PanesSplitLeft:
		case k4PanesSplitLeft:
			fSplitter->insertWidget(0, fTopSecondarySplitter);
			fTopSecondarySplitter->setOrientation(Qt::Vertical);
			fTopSecondarySplitter->show();
		case k2PanesSplitVertical:
		case k3PanesSplitVertical:
		case k4PanesSplitVertical:
			fSplitter->setOrientation(Qt::Horizontal);
			break;

		case k3PanesSplitBottom:
		case k4PanesSplitBottom:
			fSplitter->insertWidget(1, fBottomSecondarySplitter);
			fBottomSecondarySplitter->setOrientation(Qt::Horizontal);
			fSplitter->setOrientation(Qt::Vertical);
			fBottomSecondarySplitter->show();
			break;

		case k3PanesSplitRight:
		case k4PanesSplitRight:
			fSplitter->insertWidget(1, fBottomSecondarySplitter);
			fBottomSecondarySplitter->setOrientation(Qt::Vertical);
			fSplitter->setOrientation(Qt::Horizontal);
			fBottomSecondarySplitter->show();
			break;
		
		case k4PanesQuad:
			fSplitter->insertWidget(0, fTopSecondarySplitter);
			fSplitter->insertWidget(1, fBottomSecondarySplitter);
			fTopSecondarySplitter->setOrientation(Qt::Horizontal);
			fBottomSecondarySplitter->setOrientation(Qt::Horizontal);
			fTopSecondarySplitter->show();
			fBottomSecondarySplitter->show();
			fSplitter->setOrientation(Qt::Vertical);
			break;
			
		default:
			break;
	}

	fPaneDisplay = display;

	// Check for and set static width/height properties
	// for the visible widgets
	enforceStaticPaneProperties();

	// Put the visible widgets into the proper panes
	refreshPaneDisplay(display);
	
	int index;
	int secondarySplitterIndex;
	
	// Save the index of every pane at each position in the splitters
	switch(display) {
		case k4PanesSplitHorizontal:
		case k4PanesSplitVertical:		
			fSplitterPaneOrder.insert(3, 3);
		case k3PanesSplitHorizontal:
		case k3PanesSplitVertical:			
			fSplitterPaneOrder.insert(2, 2);
		case k2PanesSplitHorizontal:
		case k2PanesSplitVertical:			
			fSplitterPaneOrder.insert(1, 1);
		case k1Pane:
			fSplitterPaneOrder.insert(0, 0);
			break;
		
		case k4PanesSplitTop:		
			for(index = 0; index < 3; index++) {
				fTopSecSplitterPaneOrder.insert(index, index);
				fSplitterPaneOrder.insert(0, index);
			}
			fSplitterPaneOrder.insert(1, 3);
			break;
		
		case k3PanesSplitTop:
			for(index = 0; index < 2; index++) {
				fTopSecSplitterPaneOrder.insert(index, index);
				fSplitterPaneOrder.insert(0, index);				
			}
			fSplitterPaneOrder.insert(1, 2);
			break;
	
		case k4PanesSplitBottom:
			fSplitterPaneOrder.insert(0, 0);
			
			secondarySplitterIndex = 0;
			for(index = 3; index > 0; index--) {
				fBotSecSplitterPaneOrder.insert(secondarySplitterIndex, index);
				fSplitterPaneOrder.insert(1, index);
				secondarySplitterIndex++;
			}
			break;
		
		case k3PanesSplitBottom:
			fSplitterPaneOrder.insert(0, 0);
			
			secondarySplitterIndex = 0;
			for(index = 2; index > 0; index--) {
				fBotSecSplitterPaneOrder.insert(secondarySplitterIndex, index);
				fSplitterPaneOrder.insert(1, index);
				secondarySplitterIndex++;
			}
			break;
		
		case k4PanesSplitRight:
			fSplitterPaneOrder.insert(0, 0);
			for(index = 1; index < 4; index++) {
				fBotSecSplitterPaneOrder.insert(index - 1, index);
				fSplitterPaneOrder.insert(1, index);
			}
			break;
		
		case k3PanesSplitRight: 
			fSplitterPaneOrder.insert(0, 0);
			for(index = 1; index < 3; index++) {
				fBotSecSplitterPaneOrder.insert(index - 1, index);
				fSplitterPaneOrder.insert(1, index);
			}
			break;
		
		case k4PanesSplitLeft:
			fTopSecSplitterPaneOrder.insert(0, 0);
			fSplitterPaneOrder.insert(0, 0);
			
			fSplitterPaneOrder.insert(1, 1);
			
			secondarySplitterIndex = 1;
			for(index = 3; index > 1; index--) {
				fTopSecSplitterPaneOrder.insert(secondarySplitterIndex, index);
				fSplitterPaneOrder.insert(0, index);
				
				secondarySplitterIndex++;
			}
			break;
		
		case k3PanesSplitLeft:
			fTopSecSplitterPaneOrder.insert(0, 0);
			fSplitterPaneOrder.insert(0, 0);			
			
			fSplitterPaneOrder.insert(1, 1);
			
			fTopSecSplitterPaneOrder.insert(1, 2);
			fSplitterPaneOrder.insert(0, 2);
			break;
		
		case k4PanesQuad:
			fTopSecSplitterPaneOrder.insert(0, 0);
			fSplitterPaneOrder.insert(0, 0);
			
			fTopSecSplitterPaneOrder.insert(1, 1);
			fSplitterPaneOrder.insert(0, 1);
			
			fBotSecSplitterPaneOrder.insert(1, 2);
			fSplitterPaneOrder.insert(1, 2);
			
			fBotSecSplitterPaneOrder.insert(0, 3);
			fSplitterPaneOrder.insert(1, 3);
			break;
			
		default:
			break;
	}

	// Set the default pane sizes for the configuration:
	QList<int> sizes;
	
	// First, set the sizes for the secondary splitters (or just the main splitter for horizontal/vertical configurations)
	switch(display) {
		case k2PanesSplitHorizontal:
		case k2PanesSplitVertical:
		case k3PanesSplitTop:
		case k3PanesSplitBottom:
		case k3PanesSplitLeft:
		case k3PanesSplitRight:
			sizes << kMaxPaneSize/2 << kMaxPaneSize/2;
			break;
			
		case k3PanesSplitHorizontal:
		case k3PanesSplitVertical:
		case k4PanesSplitTop:
		case k4PanesSplitBottom:
		case k4PanesSplitLeft:
		case k4PanesSplitRight:
			sizes << kMaxPaneSize/3 << kMaxPaneSize/3 << kMaxPaneSize/3;
			break;
			
		case k4PanesSplitHorizontal:
		case k4PanesSplitVertical:
			sizes << kMaxPaneSize/4 << kMaxPaneSize/4 << kMaxPaneSize/4 << kMaxPaneSize/4;
			break;
		
		default:
			break;
	}
	
	// Then, save the sizes
	switch(display) {
		case k2PanesSplitHorizontal:
		case k2PanesSplitVertical:
			for(int i = 0; i < 2; i++) {
				fSplitterPaneSizes[i] = sizes[i];
			}
			break;
		
		case k3PanesSplitHorizontal:
		case k3PanesSplitVertical:
			for(int i = 0; i < 3; i++) {
				fSplitterPaneSizes[i] = sizes[i];
			}
			break;
		
		case k4PanesSplitHorizontal:
		case k4PanesSplitVertical:
			for(int i = 0; i < 4; i++) {
				fSplitterPaneSizes[i] = sizes[i];
			}
			break;
		
		case k3PanesSplitTop:
		case k3PanesSplitLeft:
			for(int i = 0; i < 2; i++) {
				fTopSecSplitterPaneSizes[i] = sizes[i];
			}
			break;
		
		case k4PanesSplitTop:
		case k4PanesSplitLeft:
			for(int i = 0; i < 3; i++) {
				fTopSecSplitterPaneSizes[i] = sizes[i];
			}
			break;
	
		case k3PanesSplitBottom:
		case k3PanesSplitRight:
			for(int i = 0; i < 2; i++) {
				fBotSecSplitterPaneSizes[i] = sizes[i];
			}
			break;
		
		case k4PanesSplitBottom:
		case k4PanesSplitRight:
			for(int i = 0; i < 3; i++) {
				fBotSecSplitterPaneSizes[i] = sizes[i];
			}
			break;
		
		default:
			break;
	}
	
	// Now, for the configurations that had secondary splitters, set and save the sizes for the main splitter (or for the quad configuration, all its splitters
	// since they are all divided in half by default)
	sizes.clear();
	sizes << kMaxPaneSize/2 << kMaxPaneSize/2;
	
	switch(display) {
		case k4PanesQuad:
			for(int i = 0; i < 2; i++) {
				fTopSecSplitterPaneSizes[i] = sizes[i];
			}
			for(int i = 0; i < 2; i++) {
				fBotSecSplitterPaneSizes[i] = sizes[i];
			}
			
		case k3PanesSplitTop:
		case k3PanesSplitLeft:
		case k4PanesSplitTop:
		case k4PanesSplitLeft:
		case k3PanesSplitBottom:
		case k3PanesSplitRight:
		case k4PanesSplitBottom:	
		case k4PanesSplitRight:
			for(int i = 0; i < 2; i++) {
				fSplitterPaneSizes[i] = sizes[i];
			}
			break;
			
		default:
			break;
	}
	
	// If there is just one pane, it gets all the space (i.e. the max size)
	if(display == k1Pane) {
		fSplitterPaneSizes[0] = kMaxPaneSize;
	}

	// Don't try to apply the pane sizes to the layout if the layout is still in the middle of construction
	if(!fUnderConstruction) {
		applyNewPaneSizes();
	}

	// notify listeners
	if (doNotification) {
		emit paneLayoutChanged();
	}
}

void KxPaneLayout::refreshPaneDisplay(TpaneDisplay display)
{
	int index;
	int secondarySplitterIndex;
	
	switch(display) {
		case k2PanesSplitHorizontal:
		case k2PanesSplitVertical:
			if(fPanes[1]) {
				fSplitter->insertWidget(1, fPanes[1]);
				fPanes[1]->show();
			}
			
		case k1Pane:
			if(fPanes[0]) {
				fSplitter->insertWidget(0, fPanes[0]);				
				fPanes[0]->show();
			}
			break;
			
		case k4PanesSplitHorizontal:
		case k4PanesSplitVertical:
			for(index = 0; index < 4; index++) {
				if(fPanes[index]) {
					fSplitter->insertWidget(index, fPanes[index]);
					fPanes[index]->show();
				}
			}
			break;
			
		case k3PanesSplitHorizontal:
		case k3PanesSplitVertical:
			for(index = 0; index < 3; index++) {
				if(fPanes[index]) {
					fSplitter->insertWidget(index, fPanes[index]);
					fPanes[index]->show();
				}
			}
			break;
		
		case k4PanesSplitTop:		
			for(index = 0; index < 3; index++) {
				if(fPanes[index]) {
					fTopSecondarySplitter->insertWidget(index, fPanes[index]);
					fPanes[index]->show();
				}
			}
			if(fPanes[3]) {
				fSplitter->insertWidget(1, fPanes[3]);				
				fPanes[3]->show();
			}
			break;
		
		case k3PanesSplitTop:
			for(index = 0; index < 2; index++) {
				if(fPanes[index]) {
					fTopSecondarySplitter->insertWidget(index, fPanes[index]);
					fPanes[index]->show();
				}			
			}
			if(fPanes[2]) {
				fSplitter->insertWidget(1, fPanes[2]);
				fPanes[2]->show();
			}
			break;
	
		case k4PanesSplitBottom:
			if(fPanes[0]) {
				fSplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			
			secondarySplitterIndex = 0;
			for(index = 3; index > 0; index--) {
				if(fPanes[index]) {
					fBottomSecondarySplitter->insertWidget(secondarySplitterIndex, fPanes[index]);
					fPanes[index]->show();
				}
				secondarySplitterIndex++;
			}
			break;
		
		case k3PanesSplitBottom:
			if(fPanes[0]) {
				fSplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			
			secondarySplitterIndex = 0;
			for(index = 2; index > 0; index--) {
				if(fPanes[index]) {
					fBottomSecondarySplitter->insertWidget(secondarySplitterIndex, fPanes[index]);
					fPanes[index]->show();
				}
				secondarySplitterIndex++;
			}
			break;
		
		case k4PanesSplitRight:
			if(fPanes[0]) {
				fSplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			for(index = 1; index < 4; index++) {
				if(fPanes[index]) {
					fBottomSecondarySplitter->insertWidget(index - 1, fPanes[index]);
					fPanes[index]->show();
				}
			}
			break;
		
		case k3PanesSplitRight: 
			if(fPanes[0]) {
				fSplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			for(index = 1; index < 3; index++) {
				if(fPanes[index]) {
					fBottomSecondarySplitter->insertWidget(index - 1, fPanes[index]);
					fPanes[index]->show();
				}
			}
			break;
		
		case k4PanesSplitLeft:
			if(fPanes[0]) {
				fTopSecondarySplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			
			if(fPanes[1]) {
				fSplitter->insertWidget(1, fPanes[1]);
				fPanes[1]->show();
			}
			
			secondarySplitterIndex = 1;
			for(index = 3; index > 1; index--) {
				if(fPanes[index]) {
					fTopSecondarySplitter->insertWidget(secondarySplitterIndex, fPanes[index]);
					fPanes[index]->show();
				}
				
				secondarySplitterIndex++;
			}
			break;
		
		case k3PanesSplitLeft:
			if(fPanes[0]) {
				fTopSecondarySplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			
			if(fPanes[1]) {
				fSplitter->insertWidget(1, fPanes[1]);
				fPanes[1]->show();
			}
			
			if(fPanes[2]) {
				fTopSecondarySplitter->insertWidget(1, fPanes[2]);
				fPanes[2]->show();
			}
			break;
		
		case k4PanesQuad:
			if(fPanes[0]) {
				fTopSecondarySplitter->insertWidget(0, fPanes[0]);
				fPanes[0]->show();
			}
			
			if(fPanes[1]) {
				fTopSecondarySplitter->insertWidget(1, fPanes[1]);
				fPanes[1]->show();
			}
			
			if(fPanes[2]) {
				fBottomSecondarySplitter->insertWidget(1, fPanes[2]);
				fPanes[2]->show();
			}
			
			if(fPanes[3]) {
				fBottomSecondarySplitter->insertWidget(0, fPanes[3]);
				fPanes[3]->show();
			}
			break;
			
		default:
			break;
	}

	for(int i = 0; i < fSplitter->count(); i++) {
		QSplitterHandle *splitterHandle = fSplitter->handle(i);
		if(splitterHandle != NULL) {
			splitterHandle->removeEventFilter(this);
			splitterHandle->installEventFilter(this);
		}
	}

	for(int i = 0; i < fTopSecondarySplitter->count(); i++) {
		QSplitterHandle *splitterHandle = fTopSecondarySplitter->handle(i);
		if(splitterHandle != NULL) {
			splitterHandle->removeEventFilter(this);
			splitterHandle->installEventFilter(this);
		}
	}

	for(int i = 0; i < fBottomSecondarySplitter->count(); i++) {
		QSplitterHandle *splitterHandle = fBottomSecondarySplitter->handle(i);
		if(splitterHandle != NULL) {
			splitterHandle->removeEventFilter(this);
			splitterHandle->installEventFilter(this);
		}
	}
}

int KxPaneLayout::numVisiblePanes(TpaneDisplay paneDisplay) const
{
	int result = 0;
	switch (paneDisplay) {
		case k1Pane:
			result = 1;
			break;
		case k2PanesSplitHorizontal:
		case k2PanesSplitVertical:
			result = 2;
			break;
						   
		case k3PanesSplitHorizontal:
		case k3PanesSplitVertical:
		case k3PanesSplitTop:
		case k3PanesSplitLeft:
		case k3PanesSplitBottom:
		case k3PanesSplitRight:
			result = 3;
			break;
						   
		case k4PanesSplitHorizontal:
		case k4PanesSplitVertical:
		case k4PanesSplitTop:
		case k4PanesSplitLeft:
		case k4PanesSplitBottom:
		case k4PanesSplitRight:
		case k4PanesQuad:
			result = 4;
			break;
		default:
			break;
	};
	return result;
}

int KxPaneLayout::numVisiblePanes() const
{
	return numVisiblePanes(fPaneDisplay);
}

bool KxPaneLayout::isPaneVisible(int paneIndex) const
{
	bool result = false;
	
	if(0 == paneIndex
	   || (1 == paneIndex && fPaneDisplay != k1Pane)
	   || (2 == paneIndex && !(fPaneDisplay == k1Pane
							   || fPaneDisplay == k2PanesSplitHorizontal
							   || fPaneDisplay == k2PanesSplitVertical))
	   || (3 == paneIndex && (fPaneDisplay == k4PanesSplitHorizontal
							  || fPaneDisplay == k4PanesSplitVertical
							  || fPaneDisplay == k4PanesSplitTop
							  || fPaneDisplay == k4PanesSplitLeft
							  || fPaneDisplay == k4PanesSplitBottom
							  || fPaneDisplay == k4PanesSplitRight
							  || fPaneDisplay == k4PanesQuad))
	   )
	{
		result = true;
	}
	
	return result;
}

void KxPaneLayout::setPane(QWidget *child, int paneIndex)
//
//	Description:
//		Set the given child to the specified pane.  The child must be
//		a child of this control layout.
//
//	Arguments:
//		newPaneIndex - 0, 1, 2, or 3.  No other values are valid.
//
{
	// Check index
	if(paneIndex < 0 || paneIndex > 3) return;
	// Check that specified child is indeed a child of this layout
	QObjectList children = getChildren();
	if(!children.contains(child)) return;  // Child was not found
	// Check if specified child occupies one of panes
	int currentIndex = -1;
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] == child) {
			currentIndex = index;
			break;
		}
	}

	// notify listeners
	emit paneLayoutAboutToChange();

	// If the child at the specified pane has static width/
	// height properties and the child getting switched in
	// doesn't, reset the states to default so the property
	// doesn't get applied to the new child
	if(paneIndex == visiblePaneWithStaticWidthProperty() && !child->property("staticWidthPane").isValid()) {
		setStaticWidthPane(-1, false);
	}

	if(paneIndex == visiblePaneWithStaticHeightProperty() && !child->property("staticHeightPane").isValid()) {
		setStaticHeightPane(-1, false);
	}

	if(currentIndex >= 0) {
		if(paneIndex == currentIndex) return; // Child already in this pane
		// Swap values
		QWidget *currentWidget = fPanes[paneIndex];
		fPanes[paneIndex] = fPanes[currentIndex];
		fPanes[currentIndex] = currentWidget;

		// The visibility states of these panes may have changed, so hide them
		// now and let refreshPaneDisplay make them visible if necessary.
		if(fPanes[paneIndex]) {
			fPanes[paneIndex]->hide();
			fPanes[paneIndex]->setParent(fHiddenParent);
		}
		if(fPanes[currentIndex]) {
			fPanes[currentIndex]->hide();
			fPanes[currentIndex]->setParent(fHiddenParent);
		}
	} else {
		// Move current child to invisible parent
		if(fPanes[paneIndex]) {
			fPanes[paneIndex]->hide();
			fPanes[paneIndex]->setParent(fHiddenParent);
		}
		// Assign new child
		fPanes[paneIndex] = child;
	}

	// Check for and set static width/height properties for
	// the affected children
	enforceStaticPaneProperties();

	// Apply saved stretch factors to set any necessary
	// static width/height states to the affected children.
	applyHorizontalStretch(false);
	applyVerticalStretch(false);

	/*Tpanel *panel = gThePanelManager->panel(fPanes[paneIndex]);
	if(panel) {
		panel->setFocus();
	}*/

	refreshPaneDisplay(fPaneDisplay);
	
	applyNewPaneSizes();

	// notify listeners
	emit paneLayoutChanged();
}

int KxPaneLayout::pane(QWidget *child) const
{
	int result = -1;
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] == child) {
			result = index;
			break;
		}
	}
		
	return result;
}

int KxPaneLayout::paneUnderPointer() const
{
	int result = -1;

	// Get gloabl cursor coordinate
	//
	QPoint mousePos = QCursor::pos();

	//	Loop through the panels.
	//
	for (int index = 0; index < 4; index++) {
		if (fPanes[index] == NULL) {
			continue;
		}
		if (!fPanes[index]->isVisible()) {
			continue;
		}
		// Now check to see if the mouse is inside the panel
		//
		QPoint localPos = fPanes[index]->mapFromGlobal(mousePos);
		if(fPanes[index]->rect().contains(localPos)) {
			result = index;
			break;
		}
	}

	return result;
}

QWidget * KxPaneLayout::paneChild(int index) const
{
	QWidget *result = NULL;
	
	if(index >= 0 && index < 4) {
		result = fPanes[index];
	}
	
	return result;
}

QObjectList KxPaneLayout::getChildren() const
{
	QObjectList result;
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] && !fPanes[index]->isHidden()) {
			result << fPanes[index];
		}
	}
	
	foreach(QObject *obj, fHiddenParent->children()) {
		if(obj->objectName() != kKxLayoutInternalWidgetName) {
			result << obj;
		}
	}

	return result;
}

int KxPaneLayout::getChildCount() const
{
	return getChildren().count();
}


void KxPaneLayout::activatePane(int paneIndex)
{
	if((paneIndex >= 0) && (paneIndex < 4)) {
		
		if(paneIndex != fActivePane) {
			fActivePane = paneIndex;
		}
	}
}

void KxPaneLayout::deactivatePane(int paneIndex)
{
	if((paneIndex >= 0) && (paneIndex < 4)) {
		
		if(paneIndex == fActivePane) {
			fActivePane = -1;
		}
	}
}

int KxPaneLayout::activePane() const
//
//	Description:
//		Returns the current active pane.  -1 is returned if there is no
//		active pane, otherwise 0, 1, 2, or 3 is returned.
//
{
	return fActivePane;
}

void KxPaneLayout::setPaneSize(int paneIndex, int paneWidth, int paneHeight,
							  TpaneDisplay display /* = kPaneDisplayCount */)
{
	if(paneIndex < 0 || paneIndex > 3) {
		return;
	}
	
	TpaneDisplay displayStyle = (display == kPaneDisplayCount ? fPaneDisplay : display);
	if(displayStyle == k1Pane) return; // ignore width and height for a single pane display
	
	switch(displayStyle) {
		case k4PanesQuad:
			if(paneIndex == 0 || paneIndex == 1) {
				allocateNewPaneSizes(paneIndex, paneWidth, fTopSecSplitterPaneSizes.count(), fTopSecSplitterPaneOrder, fTopSecSplitterPaneSizes);
			} else {
				allocateNewPaneSizes(paneIndex, paneWidth, fBotSecSplitterPaneSizes.count(), fBotSecSplitterPaneOrder, fBotSecSplitterPaneSizes);
			}
			allocateNewPaneSizes(paneIndex, paneHeight, fSplitterPaneSizes.count(), fSplitterPaneOrder, fSplitterPaneSizes);
			break;
	
		case k3PanesSplitTop:
		case k4PanesSplitTop:
			if(paneIndex < fTopSecSplitterPaneSizes.count()) {
				allocateNewPaneSizes(paneIndex, paneWidth, fTopSecSplitterPaneSizes.count(), fTopSecSplitterPaneOrder, fTopSecSplitterPaneSizes);
			}
			
		case k2PanesSplitHorizontal:		
		case k3PanesSplitHorizontal:		
		case k4PanesSplitHorizontal:
			allocateNewPaneSizes(paneIndex, paneHeight, fSplitterPaneSizes.count(), fSplitterPaneOrder, fSplitterPaneSizes);
			break;
		
		case k3PanesSplitBottom:
		case k4PanesSplitBottom:
			if(paneIndex != 0) {
				allocateNewPaneSizes(paneIndex, paneWidth, fBotSecSplitterPaneSizes.count(), fBotSecSplitterPaneOrder, fBotSecSplitterPaneSizes);
			}
			allocateNewPaneSizes(paneIndex, paneHeight, fSplitterPaneSizes.count(), fSplitterPaneOrder, fSplitterPaneSizes);
			break;
			
		case k3PanesSplitRight:
		case k4PanesSplitRight:
			if(paneIndex != 0) {
				allocateNewPaneSizes(paneIndex, paneHeight, fBotSecSplitterPaneSizes.count(), fBotSecSplitterPaneOrder, fBotSecSplitterPaneSizes);
			}
			
		case k2PanesSplitVertical:
		case k3PanesSplitVertical:
		case k4PanesSplitVertical:
			allocateNewPaneSizes(paneIndex, paneWidth, fSplitterPaneSizes.count(), fSplitterPaneOrder, fSplitterPaneSizes);
			break;
		
		case k3PanesSplitLeft:
		case k4PanesSplitLeft:
			if(paneIndex != 1) {
				allocateNewPaneSizes(paneIndex, paneHeight, fTopSecSplitterPaneSizes.count(), fTopSecSplitterPaneOrder, fTopSecSplitterPaneSizes);
			}
			allocateNewPaneSizes(paneIndex, paneWidth, fSplitterPaneSizes.count(), fSplitterPaneOrder, fSplitterPaneSizes);
			break;
		
		default:
			break;
	}
	
	applyNewPaneSizes();
}

void KxPaneLayout::paneSize(int paneIndex, int &paneWidth, int &paneHeight) const
{
	bool secSplitterFound = false;
	
	for(int i = 0; i < fTopSecSplitterPaneOrder.count(); i++) {
		if(fTopSecSplitterPaneOrder.contains(i, paneIndex)) {
			secSplitterFound = true;
			
			if(fTopSecondarySplitter->orientation() == Qt::Horizontal) {
				paneWidth = fTopSecSplitterPaneSizes[i];
			} else {
				paneHeight = fTopSecSplitterPaneSizes[i];
			}
			break;
		}
	}
	
	if(!secSplitterFound) {
		for(int i = 0; i < fBotSecSplitterPaneOrder.count(); i++) {
			if(fBotSecSplitterPaneOrder.contains(i, paneIndex)) {
				secSplitterFound = true;
				
				if(fBottomSecondarySplitter->orientation() == Qt::Horizontal) {
					paneWidth = fBotSecSplitterPaneSizes[i];
				} else {
					paneHeight = fBotSecSplitterPaneSizes[i];
				}
				break;
			}	
		}
	}
	
	for(int i = 0; i < fSplitterPaneOrder.count(); i++) {
		if(fSplitterPaneOrder.contains(i, paneIndex)) {
			if(fSplitter->orientation() == Qt::Horizontal) {
				paneWidth = fSplitterPaneSizes[i];
				
				if(!secSplitterFound) {
					paneHeight = kMaxPaneSize;
				}
			} else {
				paneHeight = fSplitterPaneSizes[i];
				
				if(!secSplitterFound) {
					paneWidth = kMaxPaneSize;
				}
			}
			break;
		}
	}
}

void KxPaneLayout::allocateNewPaneSizes(int paneIndex, int panePercent, int paneCount, 
										QMultiHash<int, int>& paneOrder, QHash<int, int>& paneSizes)
{
	int splitterIndex = paneOrder.key(paneIndex);
	int remainder = kMaxPaneSize;
	
	switch(paneCount) {
		case 2:
			if(splitterIndex == 0) {
				paneSizes[0] = panePercent;
				paneSizes[1] = kMaxPaneSize - panePercent;
			} else if(splitterIndex == 1) {
				paneSizes[0] = kMaxPaneSize - panePercent;
				paneSizes[1] = panePercent;
			}
			break;
		
		case 3:
			if(splitterIndex == 0) {
				paneSizes[0] = panePercent;				
				remainder -= paneSizes[0];
				if(remainder > 0) {
					if(remainder <= paneSizes[2]) {
						paneSizes[1] = 0;
						paneSizes[2] = remainder;
					} else {
						paneSizes[1] = remainder - paneSizes[2];
					}
				} else {
					paneSizes[1] = 0;
					paneSizes[2] = 0;
				}
			} else if(splitterIndex == 1) {
				paneSizes[1] = panePercent;
				remainder -= paneSizes[1];
				if(remainder > 0) {
					if(remainder <= paneSizes[0]) {
						paneSizes[2] = 0;
						paneSizes[0] = remainder;
					} else {
						paneSizes[2] = remainder - paneSizes[0];
					}
				} else {
					paneSizes[0] = 0;
					paneSizes[2] = 0;
				}			
			} else if(splitterIndex == 2) {
				paneSizes[2] = panePercent;
				remainder -= paneSizes[2];
				if(remainder > 0) {
					if(remainder <= paneSizes[0]) {
						paneSizes[1] = 0;
						paneSizes[0] = remainder;
					} else {
						paneSizes[1] = remainder - paneSizes[0];
					}
				} else {
					paneSizes[0] = 0;
					paneSizes[1] = 0;
				}		
			}
			break;
			
		case 4:
			if(splitterIndex == 0) {
				paneSizes[0] = panePercent;
				remainder -= paneSizes[0];
				if(remainder > 0) {
					if(remainder <= paneSizes[2] + paneSizes[3]) {
						paneSizes[1] = 0;
						if(remainder <= paneSizes[3]) {
							paneSizes[2] = 0;
							paneSizes[3] = remainder;
						} else {
							paneSizes[2] = remainder - paneSizes[3];
						}
					} else {
						paneSizes[1] = remainder - (paneSizes[2] + paneSizes[3]);
					}
				} else {
					paneSizes[1] = 0;
					paneSizes[2] = 0;
					paneSizes[3] = 0;
				}
			} else if(splitterIndex == 1) {
				paneSizes[1] = panePercent;
				remainder -= paneSizes[1];
				if(remainder > 0) {
					if(remainder <= paneSizes[0] + paneSizes[3]) {
						paneSizes[2] = 0;
						if(remainder <= paneSizes[0]) {
							paneSizes[3] = 0;
							paneSizes[0] = remainder;
						} else {
							paneSizes[3] = remainder - paneSizes[0];
						}
					} else {
						paneSizes[2] = remainder - (paneSizes[0] + paneSizes[3]);
					}
				} else {
					paneSizes[0] = 0;
					paneSizes[2] = 0;
					paneSizes[3] = 0;
				}
			} else if(splitterIndex == 2) {
				paneSizes[2] = panePercent;
				remainder -= paneSizes[2];
				if(remainder > 0) {
					if(remainder <= paneSizes[0] + paneSizes[1]) {
						paneSizes[3] = 0;
						if(remainder <= paneSizes[0]) {
							paneSizes[1] = 0;
							paneSizes[0] = remainder;
						} else {
							paneSizes[1] = remainder - paneSizes[0];
						}
					} else {
						paneSizes[3] = remainder - (paneSizes[0] + paneSizes[1]);
					}
				} else {
					paneSizes[0] = 0;
					paneSizes[1] = 0;
					paneSizes[3] = 0;
				}
			} else if(splitterIndex == 3) {
				paneSizes[3] = panePercent;
				remainder -= paneSizes[3];
				if(remainder > 0) {
					if(remainder <= paneSizes[0] + paneSizes[1]) {
						paneSizes[2] = 0;
						if(remainder <= paneSizes[0]) {
							paneSizes[1] = 0;
							paneSizes[0] = remainder;
						} else {
							paneSizes[1] = remainder - paneSizes[0];
						}
					} else {
						paneSizes[2] = remainder - (paneSizes[0] + paneSizes[1]);
					}
				} else {
					paneSizes[0] = 0;
					paneSizes[1] = 0;
					paneSizes[2] = 0;
				}
			}
			break;
	}
}

void KxPaneLayout::applyNewPaneSizes()
{
	QRect layoutRect = parentWidget()->rect();
	double totalWidth = (double)layoutRect.width();
	double totalHeight = (double)layoutRect.height();
	double maxPaneSize = kMaxPaneSize;
	
	QList<int> sizes;
	int splitterCount;
	double paneSize;
	double panePixelSize;
	
	bool blocked;
	
	if(fPaneDisplay == k4PanesQuad || 
		fPaneDisplay == k4PanesSplitTop || 
		fPaneDisplay == k3PanesSplitTop) {
				
		splitterCount = fTopSecSplitterPaneSizes.count();
		for(int i = 0; i < splitterCount; i++) {
			paneSize = fTopSecSplitterPaneSizes[i];
			panePixelSize = totalWidth / maxPaneSize * paneSize;
			sizes << (int)panePixelSize;
		}
		
		blocked = fTopSecondarySplitter->blockSignals(true);
		fTopSecondarySplitter->setSizes(sizes);
		fTopSecondarySplitter->blockSignals(blocked);		
		sizes.clear();
	}
	
	if(fPaneDisplay == k4PanesSplitLeft ||
		fPaneDisplay == k3PanesSplitLeft) {
		
		splitterCount = fTopSecSplitterPaneSizes.count();
		for(int i = 0; i < splitterCount; i++) {
			paneSize = fTopSecSplitterPaneSizes[i];
			panePixelSize = totalHeight / maxPaneSize * paneSize;
			sizes << (int)panePixelSize;
		}
		
		blocked = fTopSecondarySplitter->blockSignals(true);
		fTopSecondarySplitter->setSizes(sizes);
		fTopSecondarySplitter->blockSignals(blocked);
		sizes.clear();
	}
			
	if(fPaneDisplay == k4PanesQuad || 
		fPaneDisplay == k4PanesSplitBottom || 
		fPaneDisplay == k3PanesSplitBottom) {
		
		splitterCount = fBotSecSplitterPaneSizes.count();
		for(int i = 0; i < splitterCount; i++) {
			paneSize = fBotSecSplitterPaneSizes[i];
			panePixelSize = totalWidth / maxPaneSize * paneSize;
			sizes << (int)panePixelSize;
		}

		blocked = fBottomSecondarySplitter->blockSignals(true);
		fBottomSecondarySplitter->setSizes(sizes);
		fBottomSecondarySplitter->blockSignals(blocked);
		sizes.clear();		
	}
	
	if(fPaneDisplay == k4PanesSplitRight ||
		fPaneDisplay == k3PanesSplitRight) {
		
		splitterCount = fBotSecSplitterPaneSizes.count();
		for(int i = 0; i < splitterCount; i++) {
			paneSize = fBotSecSplitterPaneSizes[i];
			panePixelSize = totalHeight / maxPaneSize * paneSize;
			sizes << (int)panePixelSize;
		}

		blocked = fBottomSecondarySplitter->blockSignals(true);
		fBottomSecondarySplitter->setSizes(sizes);
		fBottomSecondarySplitter->blockSignals(blocked);
		sizes.clear();		
	}
	
	splitterCount = fSplitterPaneSizes.count();
	if(fPaneDisplay == k4PanesSplitLeft ||
		fPaneDisplay == k3PanesSplitLeft ||
		fPaneDisplay == k4PanesSplitRight ||
		fPaneDisplay == k3PanesSplitRight ||
		fPaneDisplay == k2PanesSplitVertical ||
		fPaneDisplay == k3PanesSplitVertical ||
		fPaneDisplay == k4PanesSplitVertical) {
			
		for(int i = 0; i < splitterCount; i++) {
			paneSize = fSplitterPaneSizes[i];
			panePixelSize = totalWidth / maxPaneSize * paneSize;
			sizes << (int)panePixelSize;
		}
	} else {
		for(int i = 0; i < splitterCount; i++) {
			paneSize = fSplitterPaneSizes[i];
			panePixelSize = totalHeight / maxPaneSize * paneSize;
			sizes << (int)panePixelSize;
		}
	}
		
	blocked = fSplitter->blockSignals(true);
	fSplitter->setSizes(sizes);
	fSplitter->blockSignals(blocked);
}

void KxPaneLayout::paneSizesChanged(int position, int index)
{
	QRect layoutRect = parentWidget()->rect();
	double totalWidth = (double)layoutRect.width();
	double totalHeight = (double)layoutRect.height();
	double maxPaneSize = kMaxPaneSize;
	QList<int> newPanePixelSizes;
	QList<int> newPaneSizes;
	double totalPixelSize = 0;
	int totalSize = 0;
	
	if(!fTopSecSplitterPaneSizes.empty()) {
		if(fTopSecondarySplitter->orientation() == Qt::Horizontal) {
			totalPixelSize = totalWidth;
		} else {
			totalPixelSize = totalHeight;
		}
		
		// Calculate each pane size in percentages
		newPanePixelSizes = fTopSecondarySplitter->sizes();
		fTopSecSplitterPaneSizes.clear();
		for(int i = 0; i < fTopSecondarySplitter->count(); i++) {
			double percent = (double)newPanePixelSizes[i] * maxPaneSize / totalPixelSize;
			fTopSecSplitterPaneSizes[i] = (int)percent;
		}
		
		// Add up the percentages to see if it totals to 100%
		for(int i = 0; i < fTopSecSplitterPaneSizes.count(); i++) {
			totalSize += fTopSecSplitterPaneSizes[i];
		}
		
		// If not, then add 1 to each pane percentage until we make up the difference
		int diff = kMaxPaneSize - totalSize;
		if(diff > 0) {
			for(int i = 0; i < fTopSecSplitterPaneSizes.count(); i++) {
				if(diff > 0) {
					fTopSecSplitterPaneSizes[i]++;
					diff--;
				} else {
					break;
				}
			}
		}	
	}
	
	newPanePixelSizes.clear();
	newPaneSizes.clear();
	totalPixelSize = 0;
	totalSize = 0;
	
	if(!fBotSecSplitterPaneSizes.empty()) {
		if(fBottomSecondarySplitter->orientation() == Qt::Horizontal) {
			totalPixelSize = totalWidth;
		} else {
			totalPixelSize = totalHeight;
		}
		
		// Calculate each pane size in percentages
		newPanePixelSizes = fBottomSecondarySplitter->sizes();
		fBotSecSplitterPaneSizes.clear();
		for(int i = 0; i < fBottomSecondarySplitter->count(); i++) {
			double percent = (double)newPanePixelSizes[i] * maxPaneSize / totalPixelSize;
			fBotSecSplitterPaneSizes[i] = (int)percent;
		}
		
		// Add up the percentages to see if it totals to 100%
		for(int i = 0; i < fBotSecSplitterPaneSizes.count(); i++) {
			totalSize += fBotSecSplitterPaneSizes[i];
		}
		
		// If not, then add 1 to each pane percentage until we make up the difference
		int diff = kMaxPaneSize - totalSize;
		if(diff > 0) {
			for(int i = 0; i < fBotSecSplitterPaneSizes.count(); i++) {
				if(diff > 0) {
					fBotSecSplitterPaneSizes[i]++;
					diff--;
				} else {
					break;
				}
			}
		}
	}
	
	newPanePixelSizes.clear();
	newPaneSizes.clear();
	totalPixelSize = 0;
	totalSize = 0;
	
	if(!fSplitterPaneSizes.empty()) {
		if(fSplitter->orientation() == Qt::Horizontal) {
			totalPixelSize = totalWidth;
		} else {
			totalPixelSize = totalHeight;
		}
		
		// Calculate each pane size in percentages
		newPanePixelSizes = fSplitter->sizes();
		fSplitterPaneSizes.clear();
		for(int i = 0; i < fSplitter->count(); i++) {
			double percent = (double)newPanePixelSizes[i] * maxPaneSize / totalPixelSize;
			fSplitterPaneSizes[i] = (int)percent;
		}
		
		// Add up the percentages to see if it totals to 100%
		for(int i = 0; i < fSplitterPaneSizes.count(); i++) {
			totalSize += fSplitterPaneSizes[i];
		}
		
		// If not, then add 1 to each pane percentage until we make up the difference
		int diff = kMaxPaneSize - totalSize;
		if(diff > 0) {
			for(int i = 0; i < fSplitterPaneSizes.count(); i++) {
				if(diff > 0) {
					fSplitterPaneSizes[i]++;
					diff--;
				} else {
					break;
				}
			}
		}
	}
}

void KxPaneLayout::setSeparatorThickness(int thickness)
{
	fSplitter->setHandleWidth(thickness);
	fTopSecondarySplitter->setHandleWidth(thickness);
	fBottomSecondarySplitter->setHandleWidth(thickness);
}

int KxPaneLayout::separatorThickness()
{
	return fSplitter->handleWidth();
}

void KxPaneLayout::setSeperatorMovedCommand(QObject *script)
{
	//KxQtHelper::linkControlSignalToScriptAction(SIGNAL(splitterMoved(int, int)), script, "seperatorMovedCommand", fSplitter);
	//KxQtHelper::linkControlSignalToScriptAction(SIGNAL(splitterMoved(int, int)), script, "seperatorMovedCommand", fTopSecondarySplitter);
	//KxQtHelper::linkControlSignalToScriptAction(SIGNAL(splitterMoved(int, int)), script, "seperatorMovedCommand", fBottomSecondarySplitter);
}

bool KxPaneLayout::eventFilter(QObject *obj, QEvent *event)
{	
	QSplitterHandle *handle = qobject_cast<QSplitterHandle *>(obj);
	if(handle != NULL) {
		if(event->type() == QEvent::MouseButtonPress) {
			fSplitterIsMoving = true;
		} else if(event->type() == QEvent::MouseButtonRelease) {
			fSplitterIsMoving = false;
		}
	} else {
		// If a child widget is hidden when the parent widget is
		// first made visible, the saved pane sizes will not be
		// applied properly. Catch when the child widget is 
		// finally shown and reapply the saved pane sizes again
		// by resetting the variable that we used to trigger a
		// call to applyNewPaneSizes in setGeometry.
		QWidget *widget = qobject_cast<QWidget *>(obj);
		if(widget != NULL && event->type() == QEvent::Show) {
			fInitialShowPerformed = false;

			// Remove the event filter since we only need to
			// account for the initial show.
			widget->removeEventFilter(this);
		}
	}

	// Pass on to the parent class in case it has its own event handlers
	return QVBoxLayout::eventFilter(obj, event);
}

int KxPaneLayout::visiblePaneWithStaticWidthProperty() const
{
	// Check if any of the visible panes explicitly have 
	// a static width property set, since this overrides
	// any static width pane setting the user tries to
	// implement.
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] && index < numVisiblePanes()) {
			QVariant v = fPanes[index]->property("staticWidthPane");
			if(v.isValid()) {
				return index;
			}
		}
	}
	return -1;
}

int KxPaneLayout::visiblePaneWithStaticHeightProperty() const
{
	// Check if any of the visible panes explicitly have 
	// a static height property set, since this overrides
	// any static height pane setting the user tries to
	// implement.
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] && index < numVisiblePanes()) {
			QVariant v = fPanes[index]->property("staticHeightPane");
			if(v.isValid()) {
				return index;
			}
		}
	}
	return -1;
}

void KxPaneLayout::enforceStaticPaneProperties()
{
	// Check if any of the visible panes explicitly have a
	// static width or height property set, and set the static
	// pane state accordingly, overriding any user settings.
	for(int index = 0; index < 4; index++) {
		if(fPanes[index] && index < numVisiblePanes()) {
			QVariant v = fPanes[index]->property("staticWidthPane");
			if(v.isValid()) {
				setStaticWidthPane(index, false, true);
			} else {
				v = fPanes[index]->property("staticHeightPane");
				if(v.isValid()) {
					setStaticHeightPane(index, false, true);
				}
			}
		}
	}
}

bool KxPaneLayout::isStaticPaneVisible() const
{
	// Check if any of the visible panes explicitly have 
	// a static width property set, since this overrides
	// any static width pane setting the user tries to
	// implement.
	for(int index = 0; index < 4; index++) {
		if((fPaneHorizontalStretch[index] == 1 || fPaneVerticalStretch[index] == 1)
			&& index < numVisiblePanes()) {
			return true;
		}
	}

	if(fTopSecSplitterHorStretch == 1 || fTopSecSplitterVerStretch == 1
		|| fBotSecSplitterHorStretch == 1 || fBotSecSplitterVerStretch == 1) {
		return true;
	}

	return false;
}

//	Set the specified pane to keep a static width when the pane
//	layout is dynamically resized.
//
//	Arguments:
//		paneIndex - Index of pane to have a static width (a value of -1
//					passed in here will reset the static width states to
//					the default)
//		restorePaneSizes - If true, reapply the saved pane sizes afterwards (if false,
//							this will be done outside the function)
//		forceSet - If true, the specified pane has a static width property set, so always
//					perform the function
//
void KxPaneLayout::setStaticWidthPane(int paneIndex, bool restorePaneSizes, bool forceSet)
{
	// If this setting is due to a static width property set on the
	// pane widget, we definitely want to perform this function.
	// Otherwise, only do it if there are no other visible panes
	// with the static width property, since that property takes
	// precedence.
	if(forceSet || visiblePaneWithStaticWidthProperty() < 0) {
		for(int i = 0; i < 4; i++) {
			fPaneHorizontalStretch[i] = 0;
		}
		fTopSecSplitterHorStretch = 0;
		fBotSecSplitterHorStretch = 0;

		if(paneIndex >= 0) {
			bool processed = false;
			int startIndex = 0;
			int endIndex = 0;
			bool condition = true;

			switch (fPaneDisplay) {
				case k2PanesSplitVertical:
				case k3PanesSplitVertical:
				case k4PanesSplitVertical:
					processed = true;
					startIndex = 0;
					endIndex = numVisiblePanes();
					break;
				case k3PanesSplitTop:
				case k4PanesSplitTop:
					processed = true;
					startIndex = 0;
					endIndex = numVisiblePanes() - 1;
					condition = (paneIndex < numVisiblePanes() - 1);
					break;
				case k3PanesSplitBottom:			
				case k4PanesSplitBottom:
					processed = true;
					startIndex = 1;
					endIndex = numVisiblePanes();
					condition = (paneIndex > 0);
					break;
				case k4PanesQuad:
					processed = true;
					if(paneIndex < 2) {
						startIndex = 0;
						endIndex = 2;
					} else {
						startIndex = 2;
						endIndex = 4;
					}
					condition = true;
					break;
				default:
					break;
			}

			// Static panes/splitters get a stretch factor of 0.
			// We set the stretch factors of the other panes/splitters
			// that are relevant to enforcing the static state to 1.
			if(processed) {
				if(condition) {
					for(int index = startIndex; index < endIndex; index++) {
						if(index != paneIndex) {
							fPaneHorizontalStretch[index] = 1;
						}
					}
				}
			} else {
				switch (fPaneDisplay) {				
					case k3PanesSplitLeft:
					case k4PanesSplitLeft:
						if(paneIndex == 1) {
							fTopSecSplitterHorStretch = 1;
						} else {
							fPaneHorizontalStretch[1] = 1;
						}
						break;
					case k3PanesSplitRight:			
					case k4PanesSplitRight:
						if(paneIndex == 0) {
							fBotSecSplitterHorStretch = 1;
						} else {
							fPaneHorizontalStretch[0] = 1;
						}
						break;
					default:
						break;
				};
			}
		}

		// Apply the stretch factors we have just set to
		// enforce the new static width state.
		applyHorizontalStretch(restorePaneSizes);
	}
}

//	Change the horizontal stretch factors of the visible panes and/or
//	the secondary splitters in order to make the appropriate pane
//	have a static width.
//
//	Arguments:
//		restorePaneSizes - If true, reapply the saved pane sizes afterwards (if
//							false, this will be done outside the function)
//
void KxPaneLayout::applyHorizontalStretch(bool restorePaneSizes)
{
	bool sizePolicyChanged = false;

	QSizePolicy newTopSplitterPolicy = fTopSecondarySplitter->sizePolicy();
	if(newTopSplitterPolicy.horizontalStretch() != fTopSecSplitterHorStretch) {
		newTopSplitterPolicy.setHorizontalStretch(fTopSecSplitterHorStretch);
		fTopSecondarySplitter->setSizePolicy(newTopSplitterPolicy);
		sizePolicyChanged = true;
	}

	QSizePolicy newBottomSplitterPolicy = fBottomSecondarySplitter->sizePolicy();
	if(newBottomSplitterPolicy.horizontalStretch() != fBotSecSplitterHorStretch) {
		newBottomSplitterPolicy.setHorizontalStretch(fBotSecSplitterHorStretch);
		fBottomSecondarySplitter->setSizePolicy(newBottomSplitterPolicy);
		sizePolicyChanged = true;
	}

	for(int index = 0; index < 4; index++) {
		if(fPanes[index]) {
			QSizePolicy newPolicy = fPanes[index]->sizePolicy();
			if(newPolicy.horizontalStretch() != fPaneHorizontalStretch[index]) {
				newPolicy.setHorizontalStretch(fPaneHorizontalStretch[index]);
				fPanes[index]->setSizePolicy(newPolicy);
				sizePolicyChanged = true;
			}
		}
	}

	// Changing the size policy will trigger a geometry update,
	// which may change the pane proportions from the set values.
	// Restore the saved values if necessary.
	if(sizePolicyChanged && restorePaneSizes) {
		applyNewPaneSizes();
	}
}

//	Set the specified pane to keep a static height when the pane
//	layout is dynamically resized.
//
//	Arguments:
//		paneIndex - Index of pane to have a static height (a value of -1
//					passed in here will reset the static height states to
//					the default)
//		restorePaneSizes - If true, reapply the saved pane sizes afterwards (if false,
//							this will be done outside the function)
//		forceSet - If true, the specified pane has a static height property set, so always
//					perform the function
//
void KxPaneLayout::setStaticHeightPane(int paneIndex, bool restorePaneSizes, bool forceSet)
{
	// If this setting is due to a static height property set on the
	// pane widget, we definitely want to perform this function.
	// Otherwise, only do it if there are no other visible panes
	// with the static height property, since that property takes
	// precedence.
	if(forceSet || visiblePaneWithStaticHeightProperty() < 0) {
		for(int i = 0; i < 4; i++) {
			fPaneVerticalStretch[i] = 0;
		}
		fTopSecSplitterVerStretch = 0;
		fBotSecSplitterVerStretch = 0;

		if(paneIndex >= 0) {
			bool processed = false;
			int startIndex = 0;
			int endIndex = 0;
			bool condition = true;

			switch (fPaneDisplay) {
				case k2PanesSplitHorizontal:
				case k3PanesSplitHorizontal:
				case k4PanesSplitHorizontal:
					processed = true;
					startIndex = 0;
					endIndex = numVisiblePanes();
					break;
				case k3PanesSplitLeft:
				case k4PanesSplitLeft:
					processed = true;
					startIndex = 0;
					endIndex = numVisiblePanes();
					condition = (paneIndex != 1);
					break;
				case k3PanesSplitRight:			
				case k4PanesSplitRight:
					processed = true;
					startIndex = 1;
					endIndex = numVisiblePanes();
					condition = (paneIndex > 0);
					break;
				default:
					break;
			}

			// Static panes/splitters get a stretch factor of 0.
			// We set the stretch factors of the other panes/splitters
			// that are relevant to enforcing the static state to 1.
			if(processed) {
				if(condition) {
					for(int index = startIndex; index < endIndex; index++) {
						if(index != paneIndex) {
							fPaneVerticalStretch[index] = 1;
						}
					}
				}
			} else {
				switch (fPaneDisplay) {				
					case k3PanesSplitTop:
					case k4PanesSplitTop:
						if(paneIndex == numVisiblePanes() - 1) {
							fTopSecSplitterVerStretch = 1;
						} else {
							fPaneVerticalStretch[numVisiblePanes() - 1] = 1;
						}
						break;
					case k3PanesSplitBottom:			
					case k4PanesSplitBottom:
						if(paneIndex == 0) {
							fBotSecSplitterVerStretch = 1;
						} else {
							fPaneVerticalStretch[0] = 1;
						}
						break;
					case k4PanesQuad:
						if(paneIndex >= 2) {
							fTopSecSplitterVerStretch = 1;
						} else {
							fBotSecSplitterVerStretch = 1;
						}
						break;
					default:
						break;
				};
			}
		}

		// Apply the stretch factors we have just set to
		// enforce the new static height state.
		applyVerticalStretch(restorePaneSizes);
	}
}

//	Change the vertical stretch factors of the visible panes and/or
//	the secondary splitters in order to make the appropriate pane
//	have a static height.
//
//	Arguments:
//		restorePaneSizes - If true, reapply the saved pane sizes afterwards (if
//							false, this will be done outside the function)
//
void KxPaneLayout::applyVerticalStretch(bool restorePaneSizes)
{
	bool sizePolicyChanged = false;

	if(fTopSecondarySplitter) {
		QSizePolicy newTopSplitterPolicy = fTopSecondarySplitter->sizePolicy();
		if(newTopSplitterPolicy.verticalStretch() != fTopSecSplitterVerStretch) {
			newTopSplitterPolicy.setVerticalStretch(fTopSecSplitterVerStretch);
			fTopSecondarySplitter->setSizePolicy(newTopSplitterPolicy);
			sizePolicyChanged = true;
		}
	}

	if(fBottomSecondarySplitter) {
		QSizePolicy newBottomSplitterPolicy = fBottomSecondarySplitter->sizePolicy();
		if(newBottomSplitterPolicy.verticalStretch() != fBotSecSplitterVerStretch) {
			newBottomSplitterPolicy.setVerticalStretch(fBotSecSplitterVerStretch);
			fBottomSecondarySplitter->setSizePolicy(newBottomSplitterPolicy);
			sizePolicyChanged = true;
		}
	}

	for(int index = 0; index < 4; index++) {
		if(fPanes[index]) {
			QSizePolicy newPolicy = fPanes[index]->sizePolicy();
			if(newPolicy.verticalStretch() != fPaneVerticalStretch[index]) {
				newPolicy.setVerticalStretch(fPaneVerticalStretch[index]);
				fPanes[index]->setSizePolicy(newPolicy);
				sizePolicyChanged = true;
			}
		}
	}

	// Changing the size policy will trigger a geometry update,
	// which may change the pane proportions from the set values.
	// Restore the saved values if necessary.
	if(sizePolicyChanged && restorePaneSizes) {
		applyNewPaneSizes();
	}
}
