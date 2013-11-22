#ifndef KxMenuBarLayout_H
#define KxMenuBarLayout_H

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxMenuBarLayout : public QVBoxLayout
{
	Q_OBJECT

public:
	KxMenuBarLayout(QWidget *parent );
	~KxMenuBarLayout();
};

#endif
