#ifndef KxProgressIndicator_H_
#define KxProgressIndicator_H_
#include "../../base/HQtGui.h"

#define kELFprogressFlagLong		"-progress"
#define kELFprogressFlag			"-pr"

#define kELFbeginProgressFlag		"-bp"
#define kELFbeginProgressFlagLong	"-beginProgress"

#define kELFendProgressFlag			"-ep"
#define kELFendProgressFlagLong		"-endProgress"

#define kELFstatusFlag				"-st"
#define kELFstatusFlagLong			"-status"

#define kELFisInterruptableFlag		"-ii"
#define kELFisInterruptableFlagLong	"-isInterruptable"

#define kELFisCancelledFlag			"-ic"
#define kELFisCancelledFlagLong		"-isCancelled"

class KX_DLLEXPORT KxProgressIndicator 
{
public:
    virtual void setRange(int min, int max)  = 0;
    virtual int minValue() const = 0;
    virtual int maxValue() const = 0;

    virtual void setProgress(int ) = 0;
    virtual int progress() const =0;

    virtual void step(int );
    virtual void setIsInterruptable(bool );
    virtual bool isInterruptable() const;

    virtual void setStatus(const QString &);
    virtual const QString & status() const;

    virtual bool isCancelled() const = 0;
    virtual float percent() const;

    virtual void setShowPercentText(bool );
    bool showPercentText() const;

    virtual void start();
    virtual void finish();
    bool isInUse() const;

protected:
    KxProgressIndicator();
    virtual ~KxProgressIndicator();

private:
    bool fShowPercentText;
    bool fIsInterruptable;
    QString fStatus;
    bool fIsInUse;

};

inline bool KxProgressIndicator::isInUse() const
{
    return fIsInUse;
}

inline void KxProgressIndicator::step(int amount)
{
    setProgress(progress() + amount);
}

inline bool KxProgressIndicator::showPercentText() const
{
    return fShowPercentText;
}

#endif