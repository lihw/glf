#ifndef KxScrollField_H_
#define KxScrollField_H_

#include "../../base/HQtGui.h"
#include "../../drag/KxDragHandler.h"
#include "../../drag/dragitem/KxPrimitiveDragItem.h"

class KxDragData;

class KX_DLLEXPORT KxScrollField : public QTextEdit , public KxDragHandler
{
    Q_OBJECT
public:
    typedef         QTextEdit Parent;

    KxScrollField(QWidget *parent=0);
    int          lineCount();
    void            setSearchOption(QTextDocument::FindFlags option, bool state);
    void            setSearchString(QString  &);
    QTextDocument::FindFlags searchOption() const;
    bool            searchWraps() const;
    void            setSearchWraps(bool); 
    QString         searchString() const;
    bool		searchAndSelect();
    void            indentSelection(int indentationDelta);
    static int   getLineNumber(QTextCursor textCursor); 
    static void     select(QTextCursor &textCursor, int start, int end);
    void            stepFontSize(int delta);
    void            setUseTabs(bool);
    bool            useTabs() const;
    int             spacesPerTab() const;
    void            setSpacesPerTab(int);

    virtual void	dragDone( QWidget*, QMouseEvent*, Qt::DropAction );
    virtual void	getDragData( QWidget*, QMouseEvent*, KxDragData* );

signals:
    void            fontChanged();

protected:
    virtual void    keyPressEvent(QKeyEvent *e);
    virtual void    keyReleaseEvent(QKeyEvent *e);
    virtual void    focusOutEvent(QFocusEvent *e);
    virtual void    focusInEvent(QFocusEvent *e);
    virtual void	mousePressEvent( QMouseEvent *e);
    virtual void    wheelEvent(QWheelEvent* e);    
    virtual void    changeEvent(QEvent* e);

    //	QTextEdit's built-in drop handling provides us with some nice
    //	features:
    //	
    //	- a cursor which shows where the dragged text will be inserted as
    //	  it is being dragged over the field, even when the field does not 
    //	  have the focus
    //	- automatic handling of the tricky business of moving text within a
    //	  field.
    //	- hiliting of the dropped text
    //
    //	For these reasons, we use QTextEdit's drop handling rather than a
    //	KxDropHandler. If for some reason you find that you need to add
    //	a KxDropHandler then you will need to ensure that your code
    //	provides at least the first two of the features listed above.
    //
    virtual void	dropEvent(QDropEvent* e);

    bool			fDoingDrag;
    bool			fDragAndDropInSameField;
    KxPrimitiveDragItem	fDragItem;
    QString         fIndentString;   // String which is inserted by indentSelection
    int             fSpacesPerTab;   // How many font space characters one tab is equivalent to width-wise (for tab-stops)
private:
    QTextDocument::FindFlags    fSearchOption;
    bool            fSearchWraps;
    QString         fSearchString;
};

inline int KxScrollField::spacesPerTab() const
{ return fSpacesPerTab; }

inline void KxScrollField::setSearchString(QString &searchString)
{
    fSearchString = searchString;
}

inline QTextDocument::FindFlags KxScrollField::searchOption() const
{
    return fSearchOption;
}

inline QString KxScrollField::searchString() const
{
    return fSearchString;
}

inline bool KxScrollField::searchWraps() const {
    return fSearchWraps;
}

inline void KxScrollField::setSearchWraps(bool st) {
    fSearchWraps = st;
}

#endif