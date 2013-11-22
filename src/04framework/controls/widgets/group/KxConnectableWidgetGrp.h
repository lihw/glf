#ifndef KxConnectableWidgetGrp_H_
#define KxConnectableWidgetGrp_H_
#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"
#include "../../base/KxIconnectableControl.h"


//	Description:	KxConnectableWidgetGrp represents a group of widgets
//                  which can be connected to other entities (at the
//                  moment, just attributes) as if it was a single control
//                  providing a single value (although that value may be a
//                  vector of 2 or 3 sub-values). This is a pure virtual
//                  class: it is up to sub-classes to implement the
//					KxIconnectableControl interface to communicate with other
//					entities which may be connected to the group.
//
class KX_DLLEXPORT KxConnectableWidgetGrp : public KxWidgetGrp,
    public KxIconnectableControl
{
    Q_OBJECT

public:
    KxConnectableWidgetGrp(QWidget *parent=0) : KxWidgetGrp(parent) {}
    virtual ~KxConnectableWidgetGrp() {}
};
#endif