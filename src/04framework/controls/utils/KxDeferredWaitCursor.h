#ifndef KxDeferredWaitCursor_H_
#define KxDeferredWaitCursor_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxDeferredWaitCursor : public QObject {
    Q_OBJECT
public:
    static KxDeferredWaitCursor& instance();
    void start();
    void stop();
    private slots:
        void onTimeout();
private:
    KxDeferredWaitCursor(QObject* parent);
    QTimer *fTimer;
    static KxDeferredWaitCursor* fsInstance;
    int fRequestStack;
    bool fIsOverriden;
};

#endif