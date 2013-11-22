#ifndef KxTabWidget_H_
#define KxTabWidget_H_
#include "../../base/HQtGui.h"
class KX_DLLEXPORT KxTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    KxTabWidget(QWidget *parent = 0);
    ~KxTabWidget();
    void setCommandScript(QObject *script, const char * commandType);
    QObject *commandScript(const char *commandType);
    void setTabsVisible(bool visible);
    bool tabsAreVisible();

    void	setCornerWidget(QWidget *w);
    void	hideCornerWidget();
    void	showCornerWidget();
    QSize	sizeHint() const;
    QTabBar* tabBar() const;

    private slots:
        void	postPopupMenu(const QPoint pt);
        void	populatePopupMenu();
        void	handlePopupMenuAction(QAction*);

private:
    QWidget *		fCornerWidget;
    QMenu *			fPopupMenu;
};
#endif