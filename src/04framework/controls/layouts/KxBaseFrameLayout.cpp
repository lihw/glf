#include "KxBaseFrameLayout.h"
#include "../utils/KxUtils.h"

#define kDefaultFrameMargin 0
#define kHeaderWidgetHeightBuffer 9

//---------------------------------------------------------------------------------------

KxBaseFrameLayoutHeader::KxBaseFrameLayoutHeader(KxBaseFrameLayout *layout, QWidget *parent)
: QWidget(parent)
, fLayout(layout)
{
    Q_ASSERT(fLayout);
}

void KxBaseFrameLayoutHeader::setBackgroundColor(const QColor & color)
{
    fBackgroundColor = color;
    update();
}

void KxBaseFrameLayoutHeader::paintEvent(QPaintEvent *event)
{
    Q_ASSERT(fLayout);

    QStylePainter p(this);
    int headerHeight = fLayout->headerHeight();

    // Draw title panel
    QRect r(rect());
    r.setHeight(headerHeight);
    if(parentWidget()) {
        r.setWidth(parentWidget()->width());
    }
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    if(fBackgroundColor.isValid()) {
        brush.setColor(fBackgroundColor);
    } else {
        brush.setColor(qApp->palette().window().color().lighter(115));
    }
    qDrawShadePanel(&p, r, qApp->palette(), false, 1, &brush);

    // Draw collapse indicator if needed
    if(fLayout->isCollapsible()) {
        QStyle::PrimitiveElement pe = (fLayout->isCollapsed()) ? QStyle::PE_IndicatorArrowRight : QStyle::PE_IndicatorArrowDown;
        QStyleOptionButton option;
        option.initFrom(this);
        option.rect.setHeight(headerHeight-2);
        option.rect.setWidth(headerHeight-2);
        option.rect.setLeft(style()->pixelMetric(QStyle::PM_ButtonMargin));
        p.drawPrimitive(pe, option);
    }

    r.setLeft(r.left() + style()->pixelMetric(QStyle::PM_ButtonMargin));
    if(fLayout->isCollapsible()) {
        r.setLeft(r.left() + headerHeight);
    }
    r.setLeft(r.left() + fLayout->titleIndent());

    p.drawItemText(r, Qt::AlignLeft | Qt::AlignVCenter, qApp->palette(), isEnabled(), fLayout->title());
}

void KxBaseFrameLayoutHeader::mouseReleaseEvent(QMouseEvent* event)
{
    Q_ASSERT(fLayout);
    // Check if click happened in header area
    QRect r(rect());
    if(event->y() <= fLayout->headerHeight()) {
        if (fLayout->isCollapsed()) {
            emit preExpand();
            fLayout->setCollapseState(false);
            emit postExpand();
        } else {
            emit preCollapse();
            fLayout->setCollapseState(true);
            emit postCollapse();
        }
    }
}

//---------------------------------------------------------------------------------------

KxBaseFrameLayout::KxBaseFrameLayout(QWidget *parent)
: QVBoxLayout(parent)
, fCollapsible(false)
, fIsCollapsed(false)
, fTitleIndent(0)
, fBorderVisible(true)
, fBorderStyle(KxBaseFrameLayout::kIn)
, fMarginWidth(kDefaultFrameMargin)
, fMarginHeight(kDefaultFrameMargin)
, fUnderConstruction(true)
{
    fHeaderWidget = new KxBaseFrameLayoutHeader(this, parent);
    fHeaderWidget->setFont(gKxUtils->schemeQFont (KxUtils::kBoldLabelFont));
    fHeaderWidget->setObjectName(kKxLayoutInternalWidgetName);
    fHeaderWidget->move(0,0);
    setHeaderHeight();

    // Create frame widget to hold layout content
    fBorder = new QFrame(parent);
    fBorder->setObjectName(kKxLayoutInternalWidgetName);
    fBorder->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    QVBoxLayout::addWidget(fBorder);
    fBorderLayout = new QVBoxLayout(fBorder);
    fBorderLayout->setObjectName(kKxLayoutInternalWidgetName);
    fBorder->setLayout(fBorderLayout);
    setProperty("hasMarginsSet", qVariantFromValue(true));
    if(parent) {
        parent->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    }
    fUnderConstruction = false;

    // The frame header is visible by default.
    showTitle();
}

KxBaseFrameLayout::~KxBaseFrameLayout()
{
}

void KxBaseFrameLayout::addWidget(QWidget *widget)
{
    QLayout::addWidget(widget);
}

void KxBaseFrameLayout::addItem(QLayoutItem *item)
{
    if(!fUnderConstruction) {
        // Items are put into the frame widget, not the frame layout itself
        if(item->widget()) {
            fBorderLayout->addWidget(item->widget());
        }
        if(fIsCollapsed) {
            if(item->widget()) {
                item->widget()->hide();
            }
        }
    } else {
        QVBoxLayout::addItem(item);
    }
}

int KxBaseFrameLayout::count() const
{
    if(!fUnderConstruction) {
        if(fBorderLayout != NULL) {
            return fBorderLayout->count();
        } else {
            return 0;
        }
    } else {
        return QVBoxLayout::count();
    }
}

QLayoutItem * KxBaseFrameLayout::itemAt(int index) const
{
    if(!fUnderConstruction) {
        if(fBorderLayout != NULL) {
            return fBorderLayout->itemAt(index);
        } else {
            return NULL;
        }
    } else {
        return QVBoxLayout::itemAt(index);
    }
}

// Description:  Returns the minimum size of the layout, which we want to
//               be the size hint.
//
QSize KxBaseFrameLayout::minimumSize() const
{
    return sizeHint();
}

QSize KxBaseFrameLayout::sizeHint() const
{
    QSize size(0,0);
    if(!fUnderConstruction && fBorderLayout != NULL) {
        if(fIsCollapsed) {
            // If the layout is collapsed, take the width that was saved
            // when the layout was expanded, since the current size
            // of fBorder is 0 and we want to retain the old width.
            size = size.expandedTo(QSize(fExpandedSize.width(), fBorderLayout->minimumSize().height()));
        } else {
            size = size.expandedTo(fBorderLayout->minimumSize());
        }
        // Include the size of the border frame, if any
        size.rwidth() += (fBorder->frameWidth() * 2);
        size.rheight() += (fBorder->frameWidth() * 2);
    } else {
        size = size.expandedTo(QVBoxLayout::minimumSize());
    }

    if(NULL != fHeaderWidget && !fHeaderWidget->isHidden()) {
        QSize headerSize;
        int margin = fHeaderWidget->style()->pixelMetric(QStyle::PM_ButtonMargin);
        headerSize.setHeight(isTitleVisible() ? fHeaderHeight : margin * 2);
        headerSize.setWidth(margin * 2);
        if(isCollapsible()) {
            headerSize.rwidth() += (fHeaderHeight-2);
        }
        headerSize.rwidth() += titleIndent();
        QFontMetrics fm(fHeaderWidget->font());
        QRect bounds = fm.boundingRect(QRect(0,0,1,1), Qt::AlignCenter|Qt::TextSingleLine, title());
        headerSize.rwidth() += bounds.width();

        if(headerSize.width() > size.width()) {
            size.setWidth(headerSize.width());
        }

        if(!fUnderConstruction && fBorderLayout != NULL) {
            // If we got the size from the frame widget's layout,
            // then the frame header was not accounted for
            size.rheight() += headerSize.height();
        }

        if(fIsCollapsed) {
            size.setHeight(headerSize.height());
        }
    }

    return size;
}

QObjectList KxBaseFrameLayout::getChildren() const
{
    QObjectList children;

    if (fBorderLayout != NULL) {
        for (int index = 0; index < fBorderLayout->count(); index++) {
            QLayoutItem *item = fBorderLayout->itemAt(index);
            if(item && item->widget() && item->widget()->objectName() != kKxLayoutInternalWidgetName) {
                children.append(item->widget());
            }
        }
    }

    return children;
}

void KxBaseFrameLayout::setGeometry(const QRect& rect)
{
    QLayout::setGeometry(rect);
    if(NULL != fHeaderWidget && !fHeaderWidget->isHidden()) {
        fHeaderWidget->setGeometry(QRect(0, 0, rect.width(), fHeaderHeight));
    }
    if(NULL != fBorder && !fBorder->isHidden()) {
        if(!fHeaderWidget->isHidden()) {
            fBorder->setGeometry(QRect(0, fHeaderHeight, rect.width(), rect.height() - fHeaderHeight));
        } else {
            fBorder->setGeometry(QRect(0, 0, rect.width(), rect.height()));
        }
    }
}

// Description:	 This procedure is called to set the title of the
//				 frame layout.
//
void KxBaseFrameLayout::setTitle(const QString & newTitle)
{
    fTitle = newTitle;
    fHeaderWidget->update();
}

// Description:	 This procedure is called to return the title of
//				 the frame layout.
//
QString KxBaseFrameLayout::title() const
{
    return fTitle;
}

// Description:	 This procedure is called to set the frame layout's
//				 collapsible state. 
//
void KxBaseFrameLayout::setCollapsible(bool isCollapsible)
{
    fCollapsible = isCollapsible;
    fHeaderWidget->update();
}

void KxBaseFrameLayout::setFont(const QFont & font)
{
    fHeaderWidget->setFont(font);
    setHeaderHeight();
    fHeaderWidget->update();
}

const QFont & KxBaseFrameLayout::font() const
{
    return fHeaderWidget->font();
}

void KxBaseFrameLayout::setHeaderHeight()
{
    fHeaderHeight = fHeaderWidget->font().pixelSize() + kHeaderWidgetHeightBuffer;
}

int KxBaseFrameLayout::headerHeight() const
{
    return fHeaderHeight;
}

// Description:	 This procedure returns true, if the frame layout 
//				 is collapsible.
//				 It returns false, otherwise.
//		 
bool KxBaseFrameLayout::isCollapsible() const
{
    return fCollapsible;
}

// Description:	 This procedure is called to tell the frame layout
//				 if it should be in collapsed mode or expand mode.
//
void KxBaseFrameLayout::setCollapseState(bool isCollapsed)
{
    if(fCollapsible) {
        if(fIsCollapsed == isCollapsed) return;

        // Change state
        fIsCollapsed = isCollapsed;

        // Save the current layout size if we're collapsing so we can ensure
        // the layout doesn't shrink in width after it is collapsed.
        if(isCollapsed) fExpandedSize = fBorderLayout->minimumSize();

        // Hide/Show contents
        for(int index = 0; index < count(); index++) {
            QLayoutItem *item = itemAt(index);
            QWidget *w = item->widget();
            if(NULL != w) {
                w->setVisible(!isCollapsed);
            }
        }

        fBorder->setVisible(!isCollapsed);
        if(!isCollapsed) {
            fBorderLayout->invalidate();
        }

        if(parentWidget()) {
            parentWidget()->update();
        }
    }
}

// Description:	 This procedure is called to return the current 
//				 collapsed or expand state of the frame layout.
//
bool KxBaseFrameLayout::isCollapsed() const
{
    return fIsCollapsed;
}

void KxBaseFrameLayout::showTitle()
{
    Q_ASSERT(fHeaderWidget);
    fHeaderWidget->show();
    applyContentsMargins();
}

void KxBaseFrameLayout::hideTitle()
{
    Q_ASSERT(fHeaderWidget);
    fHeaderWidget->hide();
    applyContentsMargins();
}

bool KxBaseFrameLayout::isTitleVisible() const
{
    Q_ASSERT(fHeaderWidget);
    return !fHeaderWidget->isHidden();
}

void KxBaseFrameLayout::setTitleIndent(int indent)
{
    fTitleIndent = indent;
}

int KxBaseFrameLayout::titleIndent() const
{
    return fTitleIndent;
}

void KxBaseFrameLayout::showBorder(bool show)
{
    if(show) {
        setBorderStyle(fBorderStyle);
    } else {
        setBorderStyle(KxBaseFrameLayout::kNoStyle);
    }

    fBorderVisible = show;
}

bool KxBaseFrameLayout::isBorderVisible() const
{
    return fBorderVisible;
}

void KxBaseFrameLayout::setBorderStyle(KxBaseFrameLayout::BorderStyle style)
{
    if(!fUnderConstruction && fBorder != NULL) {
        switch(style) {
            case kIn:
                fBorder->setFrameStyle(QFrame::Panel | QFrame::Sunken);
                break;
            case kOut:
                fBorder->setFrameStyle(QFrame::Panel | QFrame::Raised);
                break;
            case kEtchedIn:
                fBorder->setFrameStyle(QFrame::Box | QFrame::Sunken);
                break;
            case kEtchedOut:
                fBorder->setFrameStyle(QFrame::Box | QFrame::Raised);
                break;
            case kNoStyle:
            default:
                fBorder->setFrameStyle(QFrame::NoFrame);
                break;
        }
    }

    fBorderStyle = style;
}

KxBaseFrameLayout::BorderStyle KxBaseFrameLayout::borderStyle() const
{
    return fBorderStyle;
}

void KxBaseFrameLayout::setMarginWidth(int w)
{
    fMarginWidth = w;
    applyContentsMargins();
}

int KxBaseFrameLayout::marginWidth() const
{
    return fMarginWidth;
}

void KxBaseFrameLayout::setMarginHeight(int h)
{
    fMarginHeight = h;
    applyContentsMargins();
}

int KxBaseFrameLayout::marginHeight() const
{
    return fMarginHeight;
}



void KxBaseFrameLayout::applyContentsMargins()
{
    if(fHeaderWidget->isHidden()) {
        setContentsMargins(0, 0, 0, 0);
    } else {
        setContentsMargins(0, fHeaderHeight, 0, 0);
    }

    // We want the margins to be around the content, not the frame
    if(fBorderLayout != NULL) {
        fBorderLayout->setContentsMargins(fMarginWidth, fMarginHeight, fMarginWidth, fMarginHeight);
    }
}
