#ifndef KxFrameLayout_H
#define KxFrameLayout_H

#include "../base/HQtGui.h"
#include "KxBaseFrameLayout.h"

class KX_DLLEXPORT KxFrameLayout : public KxBaseFrameLayout
{
	Q_OBJECT

public:
	KxFrameLayout(QWidget *parent );
	~KxFrameLayout();

private slots:
	void preExpand();
	void postExpand();
	void preCollapse();
	void postCollapse();

};
#endif
