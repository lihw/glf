

#ifndef KxLayoutHelper_H_
#define KxLayoutHelper_H_

#include "KxFrameLayout.h"

class KX_DLLEXPORT KxLayoutHelper
{
public:
    static QFormLayout* addFormLayout(QLayout* parentLayout);
    static QFormLayout* addFormLayout(QWidget* parent);
    static KxFrameLayout* addFrameLayout(const QString& name, QWidget* parent);
    static KxFrameLayout* addFrameLayout(const QString& name, QLayout* parent);
};

#endif