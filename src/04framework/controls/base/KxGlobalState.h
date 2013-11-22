#ifndef KxGlobalState_H_
#define KxGlobalState_H_
#include "HQtGui.h"

class KX_DLLEXPORT  KxGlobalState {
public:
    static QWidget*					currentQWindow();
    static QLayout*					currentQLayout();
    static QWidget*					currentQControl();
    static QMenuBar*				currentQMenuBar();
    static QMenu*					currentQMenu();
    static QButtonGroup*			currentQRadioCluster();
    static QButtonGroup*			currentQToolRadioCluster();
    static QButtonGroup*			currentQIconTextRadioCluster();
    static QActionGroup*			currentQRadioMenuItemCluster();
    //static TelfCmdTemplate*  		currentCmdTemplate();
    //static Tpanel*					currentPanel();
    //static Teditor*					currentEditor();

    static void		setCurrentQWindow(QWidget*);
    static void		setCurrentQLayout(QLayout*);
    static void		setCurrentQControl(QWidget*);
    static void		setCurrentQMenuBar(QMenuBar*);
    static void		setCurrentQMenu(QMenu*);
    static void		setCurrentQRadioCluster(QButtonGroup*);
    static void		setCurrentQToolRadioCluster(QButtonGroup*);
    static void		setCurrentQIconTextRadioCluster(QButtonGroup*);
    static void		setCurrentQRadioMenuItemCluster(QActionGroup*);
    //static void		setCurrentCmdTemplate(TelfCmdTemplate*);
    //static void		setCurrentPanel(Tpanel*);
    //static void		setCurrentEditor(Teditor*);

private:
    static QPointer<QWidget>		currQWindow;
    static QPointer<QLayout>		currQLayout;
    static QPointer<QWidget>		currQControl;
    static QPointer<QMenuBar>		currQMenuBar;
    static QPointer<QMenu>			currQMenu;
    static QPointer<QButtonGroup>	currQRadioCluster;
    static QPointer<QButtonGroup>	currQToolRadioCluster;
    static QPointer<QButtonGroup>	currQIconTextRadioCluster;
    static QPointer<QActionGroup>	currQRadioMenuItemCluster;
    //static TelfCmdTemplate *  		currCmdTemplate;
    //static Tpanel *					currPanel;
    //static Teditor *				currEditor;
};
#endif