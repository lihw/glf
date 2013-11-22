#ifndef KxWaitCursor_H_
#define KxWaitCursor_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxWaitCursor {

public:
    KxWaitCursor			();
    ~KxWaitCursor		();	// not virtual to save v.table space

    void 				start				();
    void				stop				();

    static void			pause				();
    static void			resume				();

    static void			setInBatchMode		();

private:
    bool	 		fIsStarted;				// true -> start() was called
    static bool		fBatchMode;
    static int stackCount;
};
#endif