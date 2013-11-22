#include "KxTabWidget.h"
#include "KxTabBar.h"

// Description:  Constructor
//
KxTabWidget::KxTabWidget(QWidget *parent)
: QTabWidget(parent)
, fCornerWidget(NULL)
, fPopupMenu(NULL)
{
    KxTabBar *myTabBar = new KxTabBar(this);
    setTabBar(myTabBar);
    fPopupMenu = new QMenu();
    QList<QToolButton *>scrollButtons = tabBar()->findChildren<QToolButton*>();
    foreach(QToolButton *b, scrollButtons) {
        b->setContextMenuPolicy(Qt::CustomContextMenu);
        QObject::connect(b, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(postPopupMenu(QPoint)));
    }
    QObject::connect(fPopupMenu, SIGNAL(aboutToShow()), this, SLOT(populatePopupMenu()));
}

KxTabWidget::~KxTabWidget()
{
    delete fPopupMenu;
    delete fCornerWidget;
}

// Description:  This procedure sets the command script
//               for the given command type.
//
void KxTabWidget::setCommandScript(
                                      QObject *script,
                                      const char * commandType)
{
    if (tabBar() != NULL)
    {
       // KxQtHelper::setCommandScript(script, commandType, tabBar());
    }
}

// Description:  This procedure returns the command script for the
//               given command type.
//
QObject * KxTabWidget::commandScript( const char *commandType)
{
    if (tabBar() != NULL)
    {
       // return KxQtHelper::commandScript(commandType, tabBar());
    } 
    return NULL;
}

// Description:  This procedure sets the visibility of the tabs.
//
void KxTabWidget::setTabsVisible(bool visible)
{
    if (tabBar() != NULL)
    {
        if(fCornerWidget != NULL) {
            if(visible) {
                QTabWidget::setCornerWidget(fCornerWidget);
                fCornerWidget->show();
            } else {
                fCornerWidget->hide();
                QTabWidget::setCornerWidget(NULL);
            }
        }
        tabBar()->setVisible(visible);
    }
}

// Description:  This procedure returns true if the tabs are
//               visible, and false otherwise.
//
bool KxTabWidget::tabsAreVisible()
{ 
    if (tabBar() != NULL)
    {
        return tabBar()->isVisible();
    }
    return false;
}

void KxTabWidget::setCornerWidget(QWidget *w)
{
    fCornerWidget = w;
    QTabWidget::setCornerWidget(w);
}

void KxTabWidget::hideCornerWidget()
{
    if(NULL == fCornerWidget) return;

    fCornerWidget->hide();
    QTabWidget::setCornerWidget(NULL);
}

void KxTabWidget::showCornerWidget()
{
    if(NULL == fCornerWidget) return;

    QTabWidget::setCornerWidget(fCornerWidget);
    fCornerWidget->show();
}

void KxTabWidget::postPopupMenu(const QPoint pt)
{
    fPopupMenu->exec(QCursor::pos());
}

void KxTabWidget::populatePopupMenu()
{
    if(fPopupMenu == NULL) return;

    // re-populate menu
    fPopupMenu->clear();
    for(int index = 0; index < tabBar()->count(); index++) {
        QAction *a = fPopupMenu->addAction(tabBar()->tabText(index));
        // mark current tab
        if(index == tabBar()->currentIndex()) {
            a->setCheckable(true);
            a->setChecked(true);
        }
        // selecting item should select corresponding tab
        QObject::connect(fPopupMenu, SIGNAL(triggered(QAction *)), this, SLOT(handlePopupMenuAction(QAction*)));
    }
}

void KxTabWidget::handlePopupMenuAction(QAction *a)
{
    if(a == NULL) return;

    int index = fPopupMenu->actions().indexOf(a);
    if(index >= 0 && index != tabBar()->currentIndex()) {
        KxTabBar *tb = qobject_cast<KxTabBar *>(tabBar());
        tb->handleSelectCommands(NULL, index);
    }
}

QSize KxTabWidget::sizeHint() const
{
    return QTabWidget::minimumSizeHint();
}

QTabBar* KxTabWidget::tabBar() const
{
    return QTabWidget::tabBar();
}
