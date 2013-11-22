

#include "KxLayoutHelper.h"

QFormLayout* KxLayoutHelper::addFormLayout(QLayout* parentLayout) 
{
    QWidget* widget = new QWidget;
    parentLayout->addWidget(widget);

    // FIXME: use style to control formlayout's spacing.
    QFormLayout* formLayout = new QFormLayout(widget);
    formLayout->setContentsMargins(10, 0, 2, 0);
    formLayout->setSpacing(0);

    return formLayout;
}

QFormLayout* KxLayoutHelper::addFormLayout(QWidget* parent) 
{
    // FIXME: use style to control formlayout's spacing.
    QFormLayout* formLayout = new QFormLayout(parent);
    formLayout->setContentsMargins(10, 0, 2, 0);
    formLayout->setSpacing(1);

    parent->setLayout(formLayout);

    return formLayout;
}

KxFrameLayout* KxLayoutHelper::addFrameLayout(const QString& name, QWidget* parent) 
{
    KxFrameLayout* frameLayout = new KxFrameLayout(parent);
    frameLayout->setTitle(name);
    frameLayout->setCollapsible(true);

    frameLayout->setMarginWidth(5);
    frameLayout->setMarginHeight(2);

    return frameLayout;
}

KxFrameLayout* KxLayoutHelper::addFrameLayout(const QString& name, QLayout* parent) 
{
    QWidget* widget = new QWidget;
    parent->addWidget(widget);

    KxFrameLayout* frameLayout = new KxFrameLayout(widget);
    frameLayout->setTitle(name);
    frameLayout->setCollapsible(true);

    frameLayout->setMarginWidth(5);
    frameLayout->setMarginHeight(2);

    return frameLayout;
}

