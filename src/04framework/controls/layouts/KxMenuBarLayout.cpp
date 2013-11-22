#include "KxMenuBarLayout.h"

#define kMenuBarLayoutContentsMargin	1

KxMenuBarLayout::KxMenuBarLayout(QWidget *parent)
: QVBoxLayout(parent)
{
	setContentsMargins(kMenuBarLayoutContentsMargin, kMenuBarLayoutContentsMargin, 
						kMenuBarLayoutContentsMargin, kMenuBarLayoutContentsMargin);
	
}

KxMenuBarLayout::~KxMenuBarLayout()
{
}
