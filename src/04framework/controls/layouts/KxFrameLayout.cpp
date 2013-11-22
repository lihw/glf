#include "KxFrameLayout.h"

#include "../utils/KxUtils.h"


KxFrameLayout::KxFrameLayout(QWidget *parent)
: KxBaseFrameLayout(parent)
{
   // fHeaderWidget->setFont(gKxUtils->schemeQFont (KxUtils::kBoldLabelFont));

	connect(fHeaderWidget, SIGNAL(preExpand()), this, SLOT(preExpand()));
	connect(fHeaderWidget, SIGNAL(postExpand()), this, SLOT(postExpand()));
	connect(fHeaderWidget, SIGNAL(preCollapse()), this, SLOT(preCollapse()));
	connect(fHeaderWidget, SIGNAL(postCollapse()), this, SLOT(postCollapse()));
}

KxFrameLayout::~KxFrameLayout()
{
}

void KxFrameLayout::preExpand()
{
	//KxQtHelper::executeCommandScript("preExpandCommand", this);
}

void KxFrameLayout::postExpand()
{
	//KxQtHelper::executeCommandScript("expandCommand", this);
}

void KxFrameLayout::preCollapse()
{
	//KxQtHelper::executeCommandScript("preCollapseCommand", this);
}

void KxFrameLayout::postCollapse()
{
	//KxQtHelper::executeCommandScript("collapseCommand", this);
}
