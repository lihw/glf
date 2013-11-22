#include "KxIconTextControl.h"
#include "../../base/KxQtHelper.h"
#include "KxShelfButton.h"
#include "../../utils/KxUtils.h"

#define kMaxNumIcons 3

// Description:  Constructor
//
KxIconTextControl::KxIconTextControl(QWidget *parent)
: QWidget(parent)
, fLabelOffset(0)
, fLabelAlignment(Qt::AlignCenter)
, fCellWidth(160)
, fCellHeight(70)
, fLayout(NULL)
, fStyle(NoStyle)
, fLabelEditor(NULL)
, fPixmapWithOverlay(NULL)
, fSelectionIconPixmap(NULL)
, fHighlightIconPixmap(NULL)
, fSelectionHighlightIconPixmap(NULL)
, fDisabledIconPixmap(NULL)
, fIsSelected(false)
, fIsHighlighted(false)
, fCurrentIconPixmapIndex(0)
{
	fPixmap = new QLabel(this);
	fPixmap->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	fLabel = new QLabel(this);
	setFont(gKxUtils->schemeQFont(KxUtils::kPlainLabelFont));
	fLabel->hide(); // hide label initially
	setStyle(iconOnly);

	fOverlayColor[0] = 0.8f;
	fOverlayColor[1] = 0.8f;
	fOverlayColor[2] = 0.8f;

	fOverlayBackColor[0] = 0.0f;
	fOverlayBackColor[1] = 0.0f;
	fOverlayBackColor[2] = 0.0f;
	fOverlayBackColor[3] = 0.25f;
	
	fIconPixmapArray.clear();
	fIconNameArray.clear();
}

KxIconTextControl::~KxIconTextControl()
{
	for(int i = 0; i < kMaxNumIcons; i++) {
		if(fIconPixmapArray.contains(i) && fIconPixmapArray[i] != NULL) {
			delete fIconPixmapArray.take(i);
		}
	}
	delete fPixmapWithOverlay;
	delete fSelectionIconPixmap;
	delete fHighlightIconPixmap;
	delete fSelectionHighlightIconPixmap;
	delete fDisabledIconPixmap;
}

QSize KxIconTextControl::sizeHint() const
{
	if(fStyle == textOnly) {
		int width , height;
		width = fLabel->sizeHint().width() > fCellWidth ? fCellWidth : fLabel->sizeHint().width();
		height = fLabel->sizeHint().height() > fCellHeight ? fCellHeight : fLabel->sizeHint().height();
		return QSize(width+3 , height+3);
	} else if(fStyle == iconOnly) {
		return QSize(fPixmap->sizeHint().width()+3,fPixmap->sizeHint().height()+3);
	} else if(fStyle == iconAndTextHorizontal) {
		int width = fLabel->sizeHint().width() + fPixmap->sizeHint().width();
		if (width > fCellWidth){
			width = fCellWidth;
		}
		int height;
		if(fLabel->sizeHint().height() > fPixmap->sizeHint().height()) {
			height = fLabel->sizeHint().height() > fCellHeight ? fCellHeight : fLabel->sizeHint().height();
		} else {
			height = fPixmap->sizeHint().height();
		}
		return QSize(width+9, height+3);
	} else if(fStyle == iconAndTextVertical) {
		int height = fLabel->sizeHint().height() + fPixmap->sizeHint().height();
		if(height > fCellHeight){
			height = fCellHeight;
		}
		int width;
		if(fLabel->sizeHint().width() > fPixmap->sizeHint().width()) {
			width = fLabel->sizeHint().width() > fCellWidth ? fCellWidth : fLabel->sizeHint().width();
		} else {
			width = fPixmap->sizeHint().width();
		}
		return QSize(width+3, height+9);
	} else {	// fStyle == iconAndTextCentered
		int width;
		int height;
		if(fLabel->sizeHint().width() > fPixmap->sizeHint().width()) {
			width = fLabel->sizeHint().width() > fCellWidth ? fCellWidth : fLabel->sizeHint().width();
		} else {
			width = fPixmap->sizeHint().width();
		}
		if(fLabel->sizeHint().height() > fPixmap->sizeHint().height()) {
			height = fLabel->sizeHint().height() > fCellHeight ? fCellHeight : fLabel->sizeHint().height();
		} else {
			height = fPixmap->sizeHint().height();
		}
		return QSize(width+3, height+3);
	}
}

QSize KxIconTextControl::minimumSize()const
{
	return sizeHint();
}

/*!
  \brief Sets the specified icon.
  \param index which icon to set
  \param force reload the icon if it's already loaded
  \param useDefaultOnFailure if true, On failure print a warning 
  and use a default icon.
  \return always returns true
*/
bool KxIconTextControl::setIcon(const QString &name, 
                                   int index /*= 0*/, 
                                   bool force /*=false*/,
                                   bool useDefaultOnFailure/*=true*/)
{
	if(!fIconNameArray.contains(index) || name != fIconNameArray[index] || force) {
		fIconNameArray[index] = name;
		QPixmap *pixmap = createIconPixmap(name);
		if(pixmap) {
			if(fIconPixmapArray.contains(index) && fIconPixmapArray[index] != NULL) {
				delete fIconPixmapArray.take(index);
			}
			fIconPixmapArray.insert(index, pixmap);			
			updateIcon(true);
		} 

	}
    if (useDefaultOnFailure) {
        if ((!fIconPixmapArray.contains(index)) || fIconPixmapArray[index] == NULL) {
     /*       Tstring warningMessage, tname, fmt(rIconTextPixmapNotFoundWarning);
            Tstring_fromQString(name, tname);*/
           // warningMessage.tFormat(&fmt, &tname);
           // theCommandEngine->displayWarning(warningMessage);
            QString defaultIcon("vacantCell.png");
            // recursive call using default icon
            setIcon(defaultIcon, index, force, false);
        }
    }
    return true;
}


QString	KxIconTextControl::icon(int index) const
{
	if(fIconNameArray.contains(index)) {
		return fIconNameArray[index];
	} else {
		return QString();
	}
}

bool KxIconTextControl::setSelectionIcon(const QString &name)
{
	fSelectionIconName = name;
	QPixmap *pixmap = createIconPixmap(name);
	if(pixmap) {
		delete fSelectionIconPixmap;
		fSelectionIconPixmap = pixmap;
	}
	updateIcon();
	return (fSelectionIconPixmap != NULL);
}

QString	KxIconTextControl::selectionIcon() const
{
	return fSelectionIconName;
}

bool KxIconTextControl::setHighlightIcon(const QString &name)
{
	fHighlightIconName = name;
	QPixmap *pixmap = createIconPixmap(name);
	if(pixmap) {
		delete fHighlightIconPixmap;
		fHighlightIconPixmap = pixmap;
	}
	updateIcon();
	return (fHighlightIconPixmap != NULL);
}

QString	KxIconTextControl::highlightIcon() const
{
	return fHighlightIconName;
}

bool KxIconTextControl::setSelectionHighlightIcon(const QString &name)
{
	fSelectionHighlightIconName = name;
	QPixmap *pixmap = createIconPixmap(name);
	if(pixmap) {
		delete fSelectionHighlightIconPixmap;
		fSelectionHighlightIconPixmap = pixmap;
	}
	updateIcon();
	return (fSelectionHighlightIconPixmap != NULL);
}

QString KxIconTextControl::selectionHighlightIcon() const
{
	return fSelectionHighlightIconName;
}

bool KxIconTextControl::setDisabledIcon(const QString &name)
{
	fDisabledIconName = name;
	QPixmap *pixmap = createIconPixmap(name);
	if(pixmap) {
		delete fDisabledIconPixmap;
		fDisabledIconPixmap = pixmap;
	}
	updateIcon();
	return (fDisabledIconPixmap != NULL);
}

QString KxIconTextControl::disabledIcon() const
{
	return fDisabledIconName;
}

/*!
  Set the value of text overlayed on top of the icon
  \param[in] text Text value.
*/
void KxIconTextControl::setOverlayText(const QString &text)
{
	fOverlayText = text;
	updateIcon();
}
/*!
  Get the value of text overlayed on top of the icon
  \return Text value.
*/
QString KxIconTextControl::overlayText() const
{
	return fOverlayText;
}

/*!
  Set the color of text overlayed on top of the icon
  \param[in] color RGB values of the color (range 0-1)
*/
void KxIconTextControl::setOverlayColor(float color[3])
{
	fOverlayColor[0] = color[0];
	fOverlayColor[1] = color[1];
	fOverlayColor[2] = color[2];
	updateIcon();
}
/*!
  Get the color of text overlayed on top of the icon
  \return color RGB values (range 0-1).
*/
const float * KxIconTextControl::overlayColor() const
{
	return &(fOverlayColor[0]);
}

/*!
  Set the background color of text overlayed on top of the icon
  \param[in] color RGBA values of the background color (range 0-1)
*/
void KxIconTextControl::setOverlayBackColor(float color[4])
{
	fOverlayBackColor[0] = color[0];
	fOverlayBackColor[1] = color[1];
	fOverlayBackColor[2] = color[2];
	fOverlayBackColor[3] = color[3];
	updateIcon();
}
/*!
  Get the background color of text overlayed on top of the icon
  \return color RGB values (range 0-1).
*/
const float * KxIconTextControl::overlayBackColor() const
{
	return &(fOverlayBackColor[0]);
}

void KxIconTextControl::setText(const QString &text)
{
	fLabel->setText(text);
	if(fStyle != iconOnly) {
		if(!text.isEmpty()) {
			fLabel->show();
		} else {
			fLabel->hide();
		}
	}
	if(parentWidget()) {
		if( !text.isEmpty() ){
			// If this is really a shelf button, and it has no tool tip, make
			// the label (aka, the text) the tool tip.
			//
			KxShelfButton* shelfButton =
				qobject_cast<KxShelfButton*>(parentWidget());
			if( shelfButton && shelfButton->toolTip().isEmpty() ){
				shelfButton->setToolTip( text );
				shelfButton->setStatusTip( text );
			}
		}
		parentWidget()->updateGeometry();
	}
}

QString KxIconTextControl::text() const
{
	return fLabel->text();
}

void KxIconTextControl::setStyle(KxIconTextStyle style)
{
	if(fStyle == style && fLayout != NULL) return;

	fStyle = style;

	if(fLayout) {
		delete fLayout;
	}

	Qt::Alignment labelAlignment = Qt::AlignCenter;
	switch(fStyle) {
		case textOnly:
			if(fLabel->text().length() > 15){
				labelAlignment = Qt::AlignLeft|Qt::AlignVCenter;
			}
		case iconOnly:
			fLayout = new QHBoxLayout(this);
			fLayout->setAlignment(Qt::AlignCenter);
			break;
		case iconAndTextHorizontal:
			fLayout = new QHBoxLayout(this);
			fLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			labelAlignment = Qt::AlignLeft|Qt::AlignVCenter;
			break;
		case iconAndTextCentered:
			fLayout = NULL;
			break;
		default:
			fLayout = new QVBoxLayout(this);
			fLayout->setAlignment(Qt::AlignCenter);
			if(fLabel->text().length() > 15){
				labelAlignment = Qt::AlignLeft;
			}else{
				labelAlignment = Qt::AlignHCenter|Qt::AlignTop;
			}
			break;
	}
	
	if(fStyle != iconAndTextCentered){
		setAlignment(labelAlignment);
	}
	
	if(fLayout) {
		fLayout->addWidget(fPixmap);
		fLayout->addWidget(fLabel);
		fLayout->setContentsMargins(1,1,1,1);
	}

	if(fStyle == textOnly) {
		fPixmap->hide();
		fLabel->show();
	} else if(fStyle == iconOnly) {
		fPixmap->setScaledContents(true);
		fPixmap->show();
		fLabel->hide();
	} else {
		fPixmap->setScaledContents(false);
		if(fStyle == iconAndTextCentered) {
			centerIconAndText();
		}
		fPixmap->show();
		if(!fLabel->text().isEmpty()) fLabel->show();
	}

	if(parentWidget()) {
		parentWidget()->updateGeometry();
	}
}

KxIconTextControl::KxIconTextStyle KxIconTextControl::style() const
{
	return fStyle;
}

void KxIconTextControl::setAlignment(const Qt::Alignment align, bool explicitlySet)
{
	fLabel->setAlignment(align);
	if(explicitlySet) {
		fLabelAlignment = align;
	}
}

Qt::Alignment KxIconTextControl::alignment() const
{
	return fLabel->alignment();
}

void KxIconTextControl::setLabelOffset(const int offset)
{
	fLabelOffset = offset;
	if(fStyle == iconAndTextCentered) {
		centerIconAndText();
	}
}

int KxIconTextControl::labelOffset() const
{
	return fLabelOffset;
}

void KxIconTextControl::setCellSize(const int width, const int height)
{
	if(width != -1)
		fCellWidth = width;
	if(height != -1)
		fCellHeight = height;
}

void KxIconTextControl::cellSize(int &width , int &height)const
{
	width = fCellWidth;
	height = fCellHeight;
}

void KxIconTextControl::setSelected(bool selected)
{
	fIsSelected = selected;
	updateIcon();
}

void KxIconTextControl::editLabel()
{
	// Can't edit label if style is icon-only
	if(fStyle == iconOnly) return;

	if(fLabelEditor.isNull()) {
		fLabelEditor = new QLineEdit(this);
	}
    fLabelEditor->setText(fLabel->text());

	QRect labelRect = fLabel->frameGeometry();
	labelRect.setRight(labelRect.right() + 10);
	fLabelEditor->setGeometry(labelRect);
	QObject::connect(fLabelEditor, SIGNAL(editingFinished()), this, SLOT(acceptNewLabel()));
	fLabelEditor->show();
	fLabelEditor->setFocus(Qt::PopupFocusReason);
}

void KxIconTextControl::acceptNewLabel()
{
	if(fLabelEditor) {
		fLabel->setText(fLabelEditor->text());
		fLabelEditor->deleteLater();
		emit labelChanged(fLabel->text());
	}
}

void KxIconTextControl::enterEvent(QEvent * event)
{
	if(!inherits("KxIconTextStaticLabel")) {
		fIsHighlighted = true;
		updateIcon();
	}
}

void KxIconTextControl::leaveEvent(QEvent * event)
{
	if(!inherits("KxIconTextStaticLabel")) {
		fIsHighlighted = false;
		updateIcon();
	}
}

void KxIconTextControl::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::EnabledChange) {
		updateIcon();
	}
}

QPixmap * KxIconTextControl::createIconPixmap(const QString &name)
{
	return KxQtHelper::createPixmap(name);
}

void KxIconTextControl::updateIcon(bool needToChooseIcon)
{
	QPixmap *pixmap = NULL;

	if(!fIconPixmapArray.isEmpty()) {
		if(needToChooseIcon) {
			pixmap = chooseIcon();
		} else {
			pixmap = fIconPixmapArray[fCurrentIconPixmapIndex];
		}
	}

	if(!isEnabled()) {
		if(fDisabledIconPixmap) {
			pixmap = fDisabledIconPixmap;
		}
	} else if(fIsSelected) {
		if(fIsHighlighted) {
			if(fSelectionHighlightIconPixmap) {
				pixmap = fSelectionHighlightIconPixmap;
			} else if(fSelectionIconPixmap) {
				pixmap = fSelectionIconPixmap;
			}
		} else {
			if(fSelectionIconPixmap) {
				pixmap = fSelectionIconPixmap;
			}
		}
	} else {
		if(fIsHighlighted) {
			if(fHighlightIconPixmap) {
				pixmap = fHighlightIconPixmap;
			}
		}
	}

	if (pixmap) {
		if (!fOverlayText.isEmpty() && fStyle != iconAndTextCentered) {
			// Create a copy of the pixmap to overlay the text onto.
			if (fPixmapWithOverlay)
				delete fPixmapWithOverlay;
			fPixmapWithOverlay = new QPixmap(*pixmap);
			KxQtHelper::overlayTextOnPixmap(
				fPixmapWithOverlay,
				fOverlayText,
				QColor::fromRgbF(
					fOverlayBackColor[0],
					fOverlayBackColor[1],
					fOverlayBackColor[2],
					fOverlayBackColor[3]),
				QColor::fromRgbF(
					fOverlayColor[0],
					fOverlayColor[1],
					fOverlayColor[2]));
			pixmap = fPixmapWithOverlay;
		}

		// Adjust icon using style
		QIcon::Mode mode = QIcon::Normal;
		QStyleOption opt(0);
		if(isEnabled()) {
			if(fIsSelected) {
				mode = QIcon::Selected;
			} else if(fIsHighlighted) {
				mode = QIcon::Active;
			}
		} else {
			mode = QIcon::Disabled;
		}
		QPixmap adjustedIcon = qApp->style()->generatedIconPixmap(mode, *pixmap, &opt);

		fPixmap->setPixmap(adjustedIcon);

		if(parentWidget()) {
			parentWidget()->updateGeometry();
		}
	}
}

QPixmap * KxIconTextControl::chooseIcon()
{
	// iconOnly controls have scaled icons so no need to switch.
	// Or if we are setting the icon for the first time, use the
	// -image/image1 icon by default.
	if((fStyle == iconOnly || fPixmap->pixmap() == NULL) && fIconPixmapArray.contains(0)) {
		fCurrentIconPixmapIndex = 0;
		return fIconPixmapArray[0];
	}

	// If there's only one icon stored (most likely the one passed in by
	// the -image/image1 flag), we don't need to do any calculations.
	if(fIconPixmapArray.count() == 1) {
		fCurrentIconPixmapIndex = fIconPixmapArray.keys().first();
		return fIconPixmapArray.values().first();
	}

	QRect rect = geometry();

	// Calculate how much space we actually have for the icon.
	switch(fStyle) {
		case iconAndTextHorizontal:
			rect.setWidth(rect.width() - 3 - (fLabel->sizeHint().width() + 3 + 3));
			rect.setHeight(rect.height() - 3);
			break;

		case iconAndTextVertical:
			rect.setWidth(rect.width() - 3);
			rect.setHeight(rect.height() - 3 - (fLabel->sizeHint().height() + 3 + 3));
			break;

		case iconAndTextCentered:
			rect.setWidth(rect.width() - 3);
			rect.setHeight(rect.height() - 3);
			break;

		default:
			return NULL;
	}

	QPixmap *icon = NULL;
	bool found = false;
	int currMaxWidth = 0;
	int currMaxHeight = 0;

	// Find the icon closest in size to the available space without going over.
	for(int i = 0; i < kMaxNumIcons; i++) {
		if(fIconPixmapArray.contains(i) && fIconPixmapArray[i] &&
			fIconPixmapArray[i]->width() <= rect.width() &&
			fIconPixmapArray[i]->height() <= rect.height()) {
			found = true;
			if(fIconPixmapArray[i]->width() > currMaxWidth ||
				fIconPixmapArray[i]->height() > currMaxHeight) {
				currMaxWidth = fIconPixmapArray[i]->width();
				currMaxHeight = fIconPixmapArray[i]->height();
				icon = fIconPixmapArray[i];
				fCurrentIconPixmapIndex = i;
			}
		}
	}

	// If there are no icons smaller than the available space,
	// find the smallest one (based on area).
	if(!found) {
		int rectArea = rect.width() * rect.height();
		int currMinArea = 0;

		for(int i = 0; i < kMaxNumIcons; i++) {
			if(fIconPixmapArray.contains(i) && fIconPixmapArray[i]) {
				int iconArea = fIconPixmapArray[i]->width() * fIconPixmapArray[i]->height();
				if(qAbs(iconArea - rectArea) < qAbs(currMinArea - rectArea)) {
					currMinArea = iconArea;
					icon = fIconPixmapArray[i];
					fCurrentIconPixmapIndex = i;
				}
			}
		}
	}

	return icon;
}

void KxIconTextControl::resizeEvent(QResizeEvent *event) {
	// May need to change icons on a resize.
	updateIcon(true);
	
	// Make sure the icon and text are still centered after a resize.
	if(fStyle == iconAndTextCentered) {
		centerIconAndText();
	}
}

void KxIconTextControl::centerIconAndText()
{
	// Move and resize the icon to fill the entire control, so that
	// setting center alignment will actually put it in the center.
	fPixmap->move(0,0);
	fPixmap->resize(width(), height());
	fPixmap->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	// Move the label to the proper position by alignment and offset.
	int labelY = (height() - fLabel->height()) / 2;
	if( fLabelAlignment & Qt::AlignLeft ){
		fLabel->move(fLabelOffset, labelY);
	} else if( fLabelAlignment & Qt::AlignHCenter ){
		fLabel->move((width() - fLabel->width()) / 2 + fLabelOffset, labelY);
	} else if( fLabelAlignment & Qt::AlignRight ){
		fLabel->move(width() - fLabel->width() - fLabelOffset, labelY);
	}

	// Raise the text QLabel to the top so that you can see it over
	// the icon.
	fLabel->raise();
}
