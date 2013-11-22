#include "KxLabelledComboBox.h"

#include "../../base/KxQtHelper.h"
#include "../../base/KxUIInventory.h"
#include "../../utils/KxUtils.h"

// TelfUtils.h includes X.h, which has KeyPress and KeyRelease defines
// that conflict with the QEvent::Type enum values of the same names.
// Undefine to prevent issues.
#if defined( OSLinux_ )

#ifdef KeyPress
#undef KeyPress
#endif

#ifdef KeyRelease
#undef KeyRelease
#endif

#endif

#define kRowSpacing 5

KxLabelledComboBox::KxLabelledComboBox(QWidget *parent)
: QComboBox(parent)
, fOldValue(0)
{
    fLabel = new QLabel(this);
    fLabel->setFont(gKxUtils->schemeQFont(KxUtils::kPlainLabelFont));
    fLabel->hide();

    setSizeAdjustPolicy(QComboBox::AdjustToContents);

    //	Whenever the user changes the value of the comboBox, inform our
    //	connections of the new value.
    connect(this, SIGNAL(activated(int)), this, SLOT(newValueFromUser(int)));
}

KxLabelledComboBox::~KxLabelledComboBox()
{
}

void KxLabelledComboBox::setLabel(QString text)
{
    fLabel->setText(text);
    updateGeometry();
    update();
}

QString KxLabelledComboBox::label() const
{
    return fLabel->text();
}

QSize KxLabelledComboBox::sizeHint() const
{
    int width = fLabel->text().isEmpty() ? (QComboBox::sizeHint().width()) :
        (fLabel->sizeHint().width() + kRowSpacing + QComboBox::sizeHint().width());
    int height;
    if(fLabel->sizeHint().height() > QComboBox::sizeHint().height()) {
        height = fLabel->sizeHint().height();
    } else {
        height = QComboBox::sizeHint().height();
    }
    return QSize(width, height);
}

QSize KxLabelledComboBox::minimumSizeHint() const
{
    return sizeHint();
}

QSize KxLabelledComboBox::minimumSize() const
{
    return sizeHint();
}

void KxLabelledComboBox::paintEvent(QPaintEvent * event)
{
    QStylePainter p(this);
    p.setPen(palette().color(QPalette::Text));

    QStyleOptionComboBox opt;
    initStyleOption(&opt);

    if(!fLabel->text().isEmpty()) {
        QFont f = fLabel->font();
        p.setFont(f);

        QRect textRect(rect());
        textRect.setWidth(fLabel->sizeHint().width());

        // Draw the text
        p.drawText(textRect, Qt::AlignVCenter | Qt::AlignRight,
            fLabel->text());

        opt.rect = QRect(rect().x() + textRect.width() + kRowSpacing, opt.rect.y(), rect().width() - textRect.width() - kRowSpacing, rect().height());
    }

    // draw the combobox frame, focusrect and selected etc.
    p.drawComplexControl(QStyle::CC_ComboBox, opt);

    // draw the icon and text
    p.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

void KxLabelledComboBox::wheelEvent(QWheelEvent * event)
{
    //	Ignore wheel events since they cause scrolling of layouts (like in
    //	the AE) to be interrupted when the scroll switches to the combo box.
    event->ignore();
    return;
}

void KxLabelledComboBox::showPopup()
{
    QComboBox::showPopup();
    QAbstractItemView *v = this->view();
    v->installEventFilter(this);
}

void KxLabelledComboBox::hidePopup()
{
    QAbstractItemView *v = this->view();
    v->removeEventFilter(this);
    QComboBox::hidePopup();
}

bool KxLabelledComboBox::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyPress || e->type() == QEvent::KeyRelease) {
        // accept all key events so they are not processed as hotkeys
        obj->event(e);
        e->accept();
        return true;
    } else {
        return QComboBox::eventFilter(obj, e);
    }
}


void KxLabelledComboBox::newValueFromConnection(const QVariant& newValue)
{
    if (newValue != fOldValue) {
        //	Do we have an item which matches this value?
        int	newValueIndex = findData(newValue);

        if (newValueIndex >= 0) {
            setCurrentIndex(newValueIndex);
        }

        //	Even if we didn't find a match, we still want to record the value
        //	that the connection sent us. That way if the user re-selects the
        //	same item from the comboBox we will recognize it as a new value as
        //	far as connections are concerned and send it back out to them.
        fOldValue = newValue;
    }
}


void KxLabelledComboBox::newValueFromUser(int newItemIndex)
{
    QVariant	newValue = itemData(newItemIndex);

    if ((newValue != QVariant::Invalid) && (newValue != fOldValue)) {
        fOldValue = newValue;

        //	Let any connected attributes know about the new value.
        emit newValueForConnections(newValue, false);
    }
}

