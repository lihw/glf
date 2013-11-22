#ifndef KxIconnectableControl_H_
#define KxIconnectableControl_H_

#include "../base/HQtGui.h"
//	This class defines an interface for controls whose values can have a
//	two-way connection to another value source, such as an attribute on a
//	control

class  KX_DLLEXPORT KxIconnectableControl
{
public:
    virtual  ~KxIconnectableControl(){};

public slots:
    // Invoked by the entity at the other end of the connection whenever
    // it has a new value for the control to display 
    virtual void newValueFromConnection(const QVariant &value) = 0;

signals:
    // Emitted by controls which can provide expression strings whenever a 
    // new expression is made avaiable or an existing expression is changed 
    virtual void newExpressionForConnections(const QString &newExpr){}

    // Emitted by the control whenever its value changes, so that connected 
    // entities can immediately be updated 
    //	Set 'isInterim' true if this is just one of a series of values
    //	resulting from a single user action. Set it false for the final
    //	value resulting from a single user action. For example, if the
    //	user is dragging a slider set 'isInterim' true on all values emitted
    //	during the drag and set it false on the final value emitted when
    //	the slider is released.
    //
    //	Not every control which is capable of emitting interim changes
    //	needs to emit them. For example, there's probably no good reason
    //	for a text control to signal every keystroke that a user makes
    //	while editing the text: only the final value is significant. But
    //	if a user is dragging a slider or holding down a spinner button,
    //	they typically expect to see the scene respond to the interim
    //	values.

    virtual void newValueForConnections(const QVariant &newValue, bool isInterim) = 0;
};


#endif