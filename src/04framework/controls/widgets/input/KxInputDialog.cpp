#include "KxInputDialog.h"

// Description:  Constructor
//
KxInputDialog::KxInputDialog(const QString &title, const QString &label, QWidget* parent, Type type, Qt::WindowFlags f)
: QDialog(parent, f)
, fInput(NULL)
, fButtonBox(NULL) 
, fClickedButton(NULL)
, fEscapeButton(NULL)
{
    QDialog::setWindowTitle(title);
    init(label, type);
}

// Description:  This procedure destroys the input dialog.
//
KxInputDialog::~KxInputDialog()
{
}

// Description:  This procedure is used to initialize the dialog's
//               layout.
//
void KxInputDialog::init(const QString &lbl, KxInputDialog::Type type)
{
    QWidget * q = this;
    QVBoxLayout *vbox = new QVBoxLayout(q);

    QLabel *label = new QLabel(lbl, q);
    vbox->addWidget(label);
    vbox->addStretch(1);

    switch (type) {
    case KxInputDialog::LineEdit:
        fInput = new QLineEdit(q);
        break;
    case KxInputDialog::SpinBox:
        fInput = new QSpinBox(q);
        break;
    case KxInputDialog::DoubleSpinBox:
        fInput = new QDoubleSpinBox(q);
        break;
    }

    vbox->addWidget(fInput);
    vbox->addStretch(1);

    fButtonBox = new QDialogButtonBox(Qt::Horizontal, q);
    vbox->addWidget(fButtonBox);

    QObject::connect(fButtonBox, SIGNAL(accepted()), q, SLOT(accept()));
    QObject::connect(fButtonBox, SIGNAL(rejected()), q, SLOT(reject()));

    q->resize(q->sizeHint());
}

// Description:  This is a procedure which gets a text string from
//               the user.
//
QString KxInputDialog::getText(const QString &text)
{
    QLineEdit *le = (QLineEdit *)fInput;
    if (le == NULL)
    {
        return "";
    }
    le->setText(text);
    le->setFocus();
    le->selectAll();

    QString result;
    exec();
    result = le->text();

    return result;
}

// Description:  This procedure is used to get an integer value from
//               the user.
//
int KxInputDialog::getInteger(
                                 int value, int minValue, int maxValue, int step)
{
    QSpinBox *sb = (QSpinBox *)fInput;
    if (sb == NULL)
    {
        return value;
    }
    sb->setRange(minValue, maxValue);
    sb->setValue(value);
    sb->setSingleStep(step);
    sb->selectAll();

    exec();
    return sb->value();
}

// Description:  This procedure is used to get a floating point
//               number from the user.
//
double KxInputDialog::getDouble(double value, double minValue, 
                                   double maxValue)
{
    QDoubleSpinBox *sb = (QDoubleSpinBox *)fInput;
    if (sb == NULL)
    {
        return value;
    }
    sb->setRange(minValue, maxValue);
    sb->setValue(value);

    exec();
    return sb->value();
}

// Description:  This procedure adds buttons to the input dialog.
//
QPushButton * KxInputDialog::addButton(const QString &text)
{
    if (fButtonBox == NULL)
    {
        return NULL;
    }


    QPushButton *pushButton = new QPushButton(text);
    pushButton->setShortcut( QKeySequence::mnemonic(text) );
    pushButton->setFocusPolicy(Qt::StrongFocus);
    fButtonBox->addButton(pushButton, QDialogButtonBox::NoRole);
    QObject::connect(fButtonBox, SIGNAL(clicked(QAbstractButton*)),
        this, SLOT(buttonClicked(QAbstractButton*)));
    return pushButton;
}

// Description:  This procedure sets the default button for this
//               input dialog.
//
void KxInputDialog::setDefaultButton(QPushButton *button)
{
    if (fButtonBox == NULL || !fButtonBox->buttons().contains(button))
    {
        // Can not find the button box or the button.
        //
        return;
    }
    button->setDefault(true);
    button->setFocus();
} 

// Description:  This procedure deals with the key press event.
//
void KxInputDialog::keyPressEvent(QKeyEvent *e)
{
    if (fEscapeButton != NULL && e->key() == Qt::Key_Escape)
    {
        fEscapeButton->click(); 
        return;
    }
    QDialog::keyPressEvent(e);
}
