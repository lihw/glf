#ifndef KxBaseFrameLayout_H_
#define KxBaseFrameLayout_H_
#include "../base/HQtGui.h"
class KxBaseFrameLayout;

class KX_DLLEXPORT KxBaseFrameLayoutHeader : public QWidget
{
    Q_OBJECT

public:
    KxBaseFrameLayoutHeader(KxBaseFrameLayout *layout, QWidget *parent = NULL);
    void setBackgroundColor(const QColor & color);

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

signals:
    void preExpand();
    void postExpand();
    void preCollapse();
    void postCollapse();

protected:
    KxBaseFrameLayout *	fLayout;
    QColor fBackgroundColor;
};

class KX_DLLEXPORT KxBaseFrameLayout : public QVBoxLayout
{
    Q_OBJECT

public:
    KxBaseFrameLayout(QWidget *parent);
    ~KxBaseFrameLayout();

    enum BorderStyle { kIn, kOut, kEtchedIn, kEtchedOut, kNoStyle };

    void addWidget(QWidget *widget);
    virtual void addItem(QLayoutItem *item);
    virtual int count() const;
    virtual QLayoutItem * itemAt(int index) const;
    virtual QSize minimumSize() const;
    virtual QSize sizeHint() const;
    virtual void setGeometry(const QRect &rect);

    //TUIInventory uses QMetaObject::invokeMethod on getChildren, so must be Q_INVOKABLE
    Q_INVOKABLE     QObjectList     getChildren() const; 

    void setTitle(const QString & newTitle);
    QString title() const;

    void setCollapsible(bool isCollapsible);
    bool isCollapsible() const;

    void setCollapseState(bool isCollapsed);
    bool isCollapsed() const;

    void showTitle();
    void hideTitle();
    bool isTitleVisible() const;

    void setTitleIndent(int indent);
    int titleIndent() const;

    void showBorder(bool show);
    bool isBorderVisible() const;

    void setBorderStyle(BorderStyle style);
    BorderStyle borderStyle() const;

    void setMarginWidth(int w);
    int marginWidth() const;

    void setMarginHeight(int h);
    int marginHeight() const;

    void setFont(const QFont &);
    const QFont & font() const;

    void setHeaderHeight();
    int headerHeight() const;

protected:
    void			applyContentsMargins();

    KxBaseFrameLayoutHeader *	fHeaderWidget;
    int							fHeaderHeight;
    QFrame *					fBorder;
    QVBoxLayout *				fBorderLayout;
    int							fMarginWidth;
    int							fMarginHeight;
    QString						fTitle;
    int							fTitleIndent;
    bool						fCollapsible;
    bool						fIsCollapsed;
    bool						fBorderVisible;
    BorderStyle					fBorderStyle;
    bool						fUnderConstruction;
    QFont						fFont;
    QSize						fExpandedSize;
};
#endif