#include "KxDarkStyle.h"

// checkbox, on
static const char * const sKiwiDarkStyleCheck[] = {
"9 9 4 1",
".      c #525252",
"+      c #000000",
"@      c #A7A7A7",
"       c None",
"        .",
"       ..",
"      ...",
"     ... ",
".   ...  ",
".. ...   ",
".....    ",
" ...     ",
"  .      "};

// checkbox, on and sunken
static const char * const sKiwiDarkStyleCheckSunken[] = {
	"9 9 4 1",
	".      c #525252",
	"+      c #000000",
	"@      c #A7A7A7",
	"       c None",
	"        .",
	"       ..",
	"      ...",
	"     ... ",
	".   ...  ",
	".. ...   ",
	".....    ",
	" ...     ",
	"  .      "};


QColor KxDarkStyle::fSelectedToolButtonColor(175,175,175);
QColor KxDarkStyle::fPanelFocusRectColor(130,130,130);

static QString uniqueName(const QString &key, const QStyleOption *option, const QSize &size)
{
	QString tmp;
	const QStyleOptionComplex *complexOption = qstyleoption_cast<const QStyleOptionComplex *>(option);
	tmp.sprintf("%s-%d-%d-%d-%dx%d", key.toLatin1().constData(), uint(option->state),
		complexOption ? uint(complexOption->activeSubControls) : uint(0),
		option->palette.serialNumber(), size.width(), size.height());
	return tmp;
}

//! From QPlastiqueStyle implementation
static QColor mergedColors(const QColor &colorA, const QColor &colorB, int factor = 50)
{
	const int maxFactor = 100;
	QColor tmp = colorA;
	tmp.setRed((tmp.red() * factor) / maxFactor + (colorB.red() * (maxFactor - factor)) / maxFactor);
	tmp.setGreen((tmp.green() * factor) / maxFactor + (colorB.green() * (maxFactor - factor)) / maxFactor);
	tmp.setBlue((tmp.blue() * factor) / maxFactor + (colorB.blue() * (maxFactor - factor)) / maxFactor);
	return tmp;
}

static void KiwiDarkStyleDrawGradient(QPainter *painter, const QRect &rect, const QColor &gradientStart,
	const QColor &gradientStop, bool contentsPropagated)
{
	QString gradientName;
	gradientName.sprintf("%dx%d-%x-%x", rect.width(), rect.height(), gradientStart.rgba(), gradientStop.rgba());
	QPixmap cache;
	if (!QPixmapCache::find(gradientName, cache)) {
		cache = QPixmap(rect.size());
		if (contentsPropagated)
			cache.fill(Qt::transparent);
		QPainter cachePainter(&cache);
		QRect pixmapRect(0, 0, rect.width(), rect.height());
		int x = pixmapRect.center().x();
		QLinearGradient gradient(x, pixmapRect.top(), x, pixmapRect.bottom());
		gradient.setColorAt(0, gradientStart);
		gradient.setColorAt(1, gradientStop);
		cachePainter.fillRect(pixmapRect, gradient);
		cachePainter.end();
		//         if (UsePixmapCache)
		QPixmapCache::insert(gradientName, cache);
	}
	painter->drawPixmap(rect, cache);
}

QPalette KxDarkStyle::standardPalette() const
{
	// Get palette from parent class
	QPalette palette = QPlastiqueStyle::standardPalette();
	// Override some colors
	palette.setBrush(QPalette::Disabled, QPalette::Button, QColor(80,80,80));
	palette.setBrush(QPalette::Disabled, QPalette::Light, QColor(97,97,97));
	palette.setBrush(QPalette::Disabled, QPalette::Midlight, QColor(59,59,59));
	palette.setBrush(QPalette::Disabled, QPalette::Dark, QColor(37,37,37));
	palette.setBrush(QPalette::Disabled, QPalette::Mid, QColor(45,45,45));
	palette.setBrush(QPalette::Disabled, QPalette::Base, QColor(42,42,42));
	palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, palette.color(QPalette::Disabled, QPalette::Base).lighter(110));
	palette.setBrush(QPalette::Disabled, QPalette::Window, QColor(68,68,68));
	palette.setBrush(QPalette::Disabled, QPalette::Shadow, QColor(0,0,0));
	palette.setBrush(QPalette::Disabled, QPalette::Text, palette.color(QPalette::Disabled, QPalette::Base).lighter(250));

	palette.setBrush(QPalette::Active, QPalette::WindowText, QColor(200,200,200));
	palette.setBrush(QPalette::Active, QPalette::Button, QColor(100,100,100));
	palette.setBrush(QPalette::Active, QPalette::Light, QColor(97,97,97));
	palette.setBrush(QPalette::Active, QPalette::Midlight, QColor(59,59,59));
	palette.setBrush(QPalette::Active, QPalette::Dark, QColor(37,37,37));
	palette.setBrush(QPalette::Active, QPalette::Mid, QColor(45,45,45));
	palette.setBrush(QPalette::Active, QPalette::Text, QColor(200,200,200));
	palette.setBrush(QPalette::Active, QPalette::BrightText, QColor(37,37,37));
	palette.setBrush(QPalette::Active, QPalette::ButtonText, QColor(200,200,200));
	palette.setBrush(QPalette::Active, QPalette::Base, QColor(42,42,42));
	palette.setBrush(QPalette::Active, QPalette::AlternateBase, palette.color(QPalette::Active, QPalette::Base).lighter(110));
	palette.setBrush(QPalette::Active, QPalette::Window, QColor(68,68,68));
	palette.setBrush(QPalette::Active, QPalette::Shadow, QColor(0,0,0));

	palette.setBrush(QPalette::Inactive, QPalette::WindowText, QColor(200,200,200));
	palette.setBrush(QPalette::Inactive, QPalette::Button, QColor(100,100,100));
	palette.setBrush(QPalette::Inactive, QPalette::Light, QColor(97,97,97));
	palette.setBrush(QPalette::Inactive, QPalette::Midlight, QColor(59,59,59));
	palette.setBrush(QPalette::Inactive, QPalette::Dark, QColor(37,37,37));
	palette.setBrush(QPalette::Inactive, QPalette::Mid, QColor(45,45,45));
	palette.setBrush(QPalette::Inactive, QPalette::Text, QColor(200,200,200));
	palette.setBrush(QPalette::Inactive, QPalette::BrightText, QColor(37,37,37));
	palette.setBrush(QPalette::Inactive, QPalette::ButtonText, QColor(200,200,200));
	palette.setBrush(QPalette::Inactive, QPalette::Base, QColor(42,42,42));
	palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, palette.color(QPalette::Inactive, QPalette::Base).lighter(110));
	palette.setBrush(QPalette::Inactive, QPalette::Window, QColor(68,68,68));
	palette.setBrush(QPalette::Inactive, QPalette::Shadow, QColor(0,0,0));

	return palette;
}

void KxDarkStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option,
	QPainter *painter, const QWidget *widget) const
{
	QStyleOption *newOption = const_cast<QStyleOption*>(option);

	if (element == PE_IndicatorBranch)
	{
		// This is a straight copy from qplastiquestyle.cpp whith expensive draws commented out.
		// Once it's fixed in Qt source, this override can be removed.
		{
			QColor borderColor = option->palette.background().color().darker(178);
			QColor alphaCornerColor;
			if (widget) {
				// ### backgroundrole/foregroundrole should be part of the style option
				alphaCornerColor = mergedColors(option->palette.color(widget->backgroundRole()), borderColor);
			} else {
				alphaCornerColor = mergedColors(option->palette.background().color(), borderColor);
			}
			QColor alphaTextColor = mergedColors(option->palette.background().color(), option->palette.text().color());

			//                      int mid_h = option->rect.x() + option->rect.width() / 2;
			//                      int mid_v = option->rect.y() + option->rect.height() / 2;
			//                      int bef_h = mid_h;
			//                      int bef_v = mid_v;
			//                      int aft_h = mid_h;
			//                      int aft_v = mid_v;
			//                      QBrush brush(option->palette.dark().color(), Qt::Dense4Pattern);
			//                      if (option->state & State_Item) {
			//                              if (option->direction == Qt::RightToLeft)
			//                                      painter->fillRect(option->rect.left(), mid_v, bef_h - option->rect.left(), 1, brush);
			//                              else
			//                                      painter->fillRect(aft_h, mid_v, option->rect.right() - aft_h + 1, 1, brush);
			//                      }
			//                      if (option->state & State_Sibling)
			//                              painter->fillRect(mid_h, aft_v, 1, option->rect.bottom() - aft_v + 1, brush);
			//                      if (option->state & (State_Open | State_Children | State_Item | State_Sibling))
			//                              painter->fillRect(mid_h, option->rect.y(), 1, bef_v - option->rect.y(), brush);

			if (option->state & State_Children) {
				painter->save();
				QPoint center = option->rect.center();
				// border
				QRect fullRect(center.x() - 4, center.y() - 4, 9, 9);
				painter->setPen(borderColor);

				const QLine lines[4] = {
					QLine(fullRect.left() + 1, fullRect.top(),
					fullRect.right() - 1, fullRect.top()),
					QLine(fullRect.left() + 1, fullRect.bottom(),
					fullRect.right() - 1, fullRect.bottom()),
					QLine(fullRect.left(), fullRect.top() + 1,
					fullRect.left(), fullRect.bottom() - 1),
					QLine(fullRect.right(), fullRect.top() + 1,
					fullRect.right(), fullRect.bottom() - 1) };
					painter->drawLines(lines, 4);

					// "antialiased" corners
					painter->setPen(alphaCornerColor);
					const QPoint points[4] = {
						fullRect.topLeft(),
						fullRect.topRight(),
						fullRect.bottomLeft(),
						fullRect.bottomRight() };
						painter->drawPoints(points, 4);

						// fill
						//                              QRect adjustedRect = fullRect;
						//                              QRect gradientRect(adjustedRect.left() + 1, adjustedRect.top() + 1,
						//                                                                 adjustedRect.right() - adjustedRect.left() - 1,
						//                                                                 adjustedRect.bottom() - adjustedRect.top() - 1);
						//                              qt_plastique_draw_gradient(painter, gradientRect, baseGradientStartColor, baseGradientStopColor);
						// draw "+" or "-"
						painter->setPen(alphaTextColor);
						painter->drawLine(center.x() - 2, center.y(), center.x() + 2, center.y());
						if (!(option->state & State_Open))
							painter->drawLine(center.x(), center.y() - 2, center.x(), center.y() + 2);
						painter->restore();
			}
		}
		return;
	}

	// Swap window and button colors for tab widget frame
	//if( element == PE_FrameTabWidget ) 
	//{
	//	QPalette p = option->palette;
	//	QBrush windowDisabled = p.window();
	//	QBrush buttonDisabled = p.button();
	//	QBrush windowActive = p.window();
	//	QBrush buttonActive = p.button();
	//	QBrush windowInactive = p.window();
	//	QBrush buttonInactive = p.button();
	//	p.setBrush(QPalette::Disabled, QPalette::Button, windowDisabled);
	//	p.setBrush(QPalette::Disabled, QPalette::Window, buttonDisabled);
	//	p.setBrush(QPalette::Active, QPalette::Button, windowActive);
	//	p.setBrush(QPalette::Active, QPalette::Window, buttonActive);
	//	p.setBrush(QPalette::Inactive, QPalette::Button, windowInactive);
	//	p.setBrush(QPalette::Inactive, QPalette::Window, buttonInactive);
	//	newOption->palette = p; 
	//}

	if (element == PE_IndicatorHeaderArrow)
	{
		if (const QStyleOptionHeader *header = qstyleoption_cast<const QStyleOptionHeader *>(option)) 
		{
			QPixmap pixmap = (header->sortIndicator == QStyleOptionHeader::SortUp) ?
				KxQtHelper::getCachedPixmap("arrowDown.png") :
			KxQtHelper::getCachedPixmap("arrowUp.png");
			painter->drawPixmap(option->rect, pixmap, pixmap.rect());
			return;
		}
	}

	// draw check instead of x
	if( element == PE_IndicatorCheckBox)
	{
		if (const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option)) {

			QColor borderColor = option->palette.background().color().dark(178);

			QColor baseGradientStartColor = option->palette.base().color().dark(101);
			QColor baseGradientStopColor = option->palette.base().color().dark(106);

			QColor highlightedBaseGradientStartColor = option->palette.base().color();
			QColor highlightedBaseGradientStopColor = mergedColors(option->palette.base().color().dark(105), option->palette.highlight().color(), 70);

			QColor highlightedDarkInnerBorderColor = mergedColors(option->palette.button().color(), option->palette.highlight().color(), 35);
			QColor highlightedLightInnerBorderColor = mergedColors(option->palette.button().color(), option->palette.highlight().color(), 58);

			QColor alphaCornerColor = mergedColors(option->palette.background().color(), borderColor);
			QColor alphaTextColor = mergedColors(option->palette.background().color(), option->palette.text().color());
			QColor alphaLightTextColor = mergedColors(option->palette.background().color().light(250), option->palette.text().color().light(250));

			bool hover = (button->state & State_Enabled) && (button->state & State_MouseOver);
			QRect rect = option->rect;

			QPixmap cache;
			QString pixmapName = uniqueName("checkbox", option, option->rect.size());
			if (!QPixmapCache::find(pixmapName, cache)) {
				bool contentsPropagated = widget ? widget->testAttribute(Qt::WA_ContentsPropagated) : false;
				cache = QPixmap(rect.size());
				if (contentsPropagated)
					cache.fill(Qt::transparent);
				QRect pixmapRect(0, 0, rect.width(), rect.height());
				QPainter checkBoxPainter(&cache);

				// border
				checkBoxPainter.setPen(borderColor);
				checkBoxPainter.drawLine(pixmapRect.left() + 1, pixmapRect.top(),
					pixmapRect.right() - 1, pixmapRect.top());
				checkBoxPainter.drawLine(pixmapRect.left() + 1, pixmapRect.bottom(),
					pixmapRect.right() - 1, pixmapRect.bottom());
				checkBoxPainter.drawLine(pixmapRect.left(), pixmapRect.top() + 1,
					pixmapRect.left(), pixmapRect.bottom() - 1);
				checkBoxPainter.drawLine(pixmapRect.right(), pixmapRect.top() + 1,
					pixmapRect.right(), pixmapRect.bottom() - 1);

				// "antialiased" corners
				checkBoxPainter.setPen(alphaCornerColor);
				checkBoxPainter.drawPoint(pixmapRect.topLeft());
				checkBoxPainter.drawPoint(pixmapRect.topRight());
				checkBoxPainter.drawPoint(pixmapRect.bottomLeft());
				checkBoxPainter.drawPoint(pixmapRect.bottomRight());

				// fill background
				QRect adjustedRect = option->rect;
				QRect gradientRect(pixmapRect.left() + 1, pixmapRect.top() + 1,
					pixmapRect.right() - pixmapRect.left() - 1,
					pixmapRect.bottom() - pixmapRect.top() - 1);
				if (hover) {
					KiwiDarkStyleDrawGradient(&checkBoxPainter, gradientRect,
						highlightedBaseGradientStartColor,
						highlightedBaseGradientStopColor,
						contentsPropagated);
				} else {
					KiwiDarkStyleDrawGradient(&checkBoxPainter, gradientRect,
						baseGradientStartColor,
						baseGradientStopColor,
						contentsPropagated);
				}

				// draw highlighted border when hovering
				if (hover) {
					checkBoxPainter.setPen(highlightedDarkInnerBorderColor);
					checkBoxPainter.drawLine(pixmapRect.left() + 1, pixmapRect.bottom() - 1,
						pixmapRect.left() + 1, pixmapRect.top() + 1);
					checkBoxPainter.drawLine(pixmapRect.left() + 1, pixmapRect.top() + 1,
						pixmapRect.right() - 2, pixmapRect.top() + 1);
					checkBoxPainter.setPen(highlightedLightInnerBorderColor);
					checkBoxPainter.drawLine(pixmapRect.left() + 2, pixmapRect.bottom() - 2,
						pixmapRect.left() + 2, pixmapRect.top() + 2);
					checkBoxPainter.drawLine(pixmapRect.left() + 2, pixmapRect.top() + 2,
						pixmapRect.right() - 3, pixmapRect.top() + 2);
					checkBoxPainter.setPen(highlightedDarkInnerBorderColor.dark(110));
					checkBoxPainter.drawLine(pixmapRect.left() + 2, pixmapRect.bottom() - 1,
						pixmapRect.right() - 1, pixmapRect.bottom() - 1);
					checkBoxPainter.drawLine(pixmapRect.right() - 1, pixmapRect.bottom() - 1,
						pixmapRect.right() - 1, pixmapRect.top() + 1);
					checkBoxPainter.setPen(highlightedLightInnerBorderColor.dark(110));
					checkBoxPainter.drawLine(pixmapRect.left() + 3, pixmapRect.bottom() - 2,
						pixmapRect.right() - 2, pixmapRect.bottom() - 2);
					checkBoxPainter.drawLine(pixmapRect.right() - 2, pixmapRect.bottom() - 2,
						pixmapRect.right() - 2, pixmapRect.top() + 2);
				}

				// draw check mark when on
				if ((button->state & (State_On | State_Sunken | State_NoChange))) {
					QImage image((button->state & (State_NoChange | State_Sunken)
						? sKiwiDarkStyleCheckSunken : sKiwiDarkStyleCheck));
					if ((button->state & (State_Sunken | State_NoChange))) {
						image.setColor(0, alphaLightTextColor.rgba());
						image.setColor(1, alphaLightTextColor.light(130).rgba());
						image.setColor(2, alphaLightTextColor.light(110).rgba());
					} else { 
						image.setColor(0, option->palette.text().color().rgba());
						image.setColor(1, alphaTextColor.rgba());
					}
					checkBoxPainter.drawImage(pixmapRect.x() + 2, pixmapRect.y() + 2, image);
				}
				checkBoxPainter.end();
				//      if (UsePixmapCache)
				QPixmapCache::insert(pixmapName, cache);
			}
			painter->drawPixmap(rect.topLeft(), cache);
			return;
		}
	}

	// Alter colors for pressed tool buttons
	if( element == PE_PanelButtonTool) {
		if(option->state & State_Enabled && option->state & State_On) {
			QPalette p = option->palette;
			p.setBrush(QPalette::Active, QPalette::Button, fSelectedToolButtonColor);
			p.setBrush(QPalette::Inactive, QPalette::Button, fSelectedToolButtonColor);
			newOption->palette = p;
		}
	}

	if( element == PE_PanelLineEdit || element == PE_FrameLineEdit 
		|| element == PE_IndicatorRadioButton || element == PE_IndicatorCheckBox ) {
			QPalette p = option->palette;
			QColor lighterBorderColor = p.window().color().lighter(144);
			p.setBrush(QPalette::Disabled, QPalette::Shadow, lighterBorderColor);
			p.setBrush(QPalette::Active, QPalette::Shadow, lighterBorderColor);
			p.setBrush(QPalette::Inactive, QPalette::Shadow, lighterBorderColor);
			newOption->palette = p;
	}

	QPlastiqueStyle::drawPrimitive(element, newOption, painter, widget);
}

void KxDarkStyle::drawControl(ControlElement element, const QStyleOption *option,
	QPainter *painter, const QWidget *widget) const
{
	QStyleOption *newOption = const_cast<QStyleOption*>(option);
	if(element == CE_DockWidgetTitle) {
		QFont font = painter->font();
		QFontInfo fi(font);
		font.setPointSize(fi.pointSize());
		painter->setFont(font);
	} else if(element == CE_FocusFrame) {
		qDrawPlainRect(painter, option->rect, fPanelFocusRectColor);
		return;
	}
	// Completely override hotbox menubars drawing
	//! From QPlastiqueStyle implementation
	else if (element == CE_MenuBarItem &&
		widget->property("isQmayaHotBoxMenuBar").isValid()) 
	{
		QColor borderColor = option->palette.background().color().darker(178);
		// Clamp alpha of background so that fully transparent hobox is still visible
		if (borderColor.alpha() < 100) borderColor.setAlpha(100);
		QColor alphaCornerColor;
		if (widget) {
			// ### backgroundrole/foregroundrole should be part of the style option
			alphaCornerColor = mergedColors(option->palette.color(widget->backgroundRole()), borderColor);
		} else {
			alphaCornerColor = mergedColors(option->palette.background().color(), borderColor);
		}
		QColor gradientStartColor = option->palette.button().color().lighter(104);
		QColor gradientStopColor = option->palette.button().color().darker(105);

		// Draws a menu bar item; File, Edit, Help etc..
		if ((option->state & State_Selected) && (option->state & State_Enabled)) {
			QPixmap cache;
			if (true) {
				cache = QPixmap(option->rect.size());
				// Hotbox menus do not draw their backgrounds, this avoids a 'stacked' look due to alpha-blending
				//                cache.fill(option->palette.background().color().lighter(150));
				cache.fill(QColor(0,0,0,0));
				QRect pixmapRect(0, 0, option->rect.width(), option->rect.height());
				QPainter cachePainter(&cache);
				QRect rect = pixmapRect;
				// outer border and corners
				cachePainter.setPen(borderColor);
				cachePainter.drawRect(rect.adjusted(0, 0, -1, -1));
				cachePainter.setPen(alphaCornerColor);

				const QPoint points[4] = {
					rect.topLeft(),
					rect.topRight(),
					rect.bottomLeft(),
					rect.bottomRight() };
					cachePainter.drawPoints(points, 4);

					// inner border
					if ((option->state & QStyle::State_Sunken) || (option->state & QStyle::State_On))
						cachePainter.setPen(option->palette.button().color().darker(118));
					else
						cachePainter.setPen(gradientStartColor);

					QLine lines[2];
					lines[0] = QLine(rect.left() + 1, rect.top() + 1, rect.right() - 1, rect.top() + 1);
					lines[1] = QLine(rect.left() + 1, rect.top() + 2, rect.left() + 1, rect.bottom() - 2);
					cachePainter.drawLines(lines, 2);

					if ((option->state & QStyle::State_Sunken) || (option->state & QStyle::State_On))
						cachePainter.setPen(option->palette.button().color().darker(114));
					else
						cachePainter.setPen(gradientStopColor.darker(102));
					lines[0] = QLine(rect.left() + 1, rect.bottom() - 1, rect.right() - 1, rect.bottom() - 1);
					lines[1] = QLine(rect.right() - 1, rect.top() + 1, rect.right() - 1, rect.bottom() - 2);
					cachePainter.drawLines(lines, 2);
					cachePainter.end();
			}
			painter->drawPixmap(option->rect.topLeft(), cache);
		} else {
			painter->fillRect(option->rect, QColor(0,0,0,0));
			//            painter->fillRect(option->rect, option->palette.background());
		}

		if (const QStyleOptionMenuItem *mbi = qstyleoption_cast<const QStyleOptionMenuItem *>(option)) {
			QStyleOptionMenuItem newMI = *mbi;
			if (!(option->palette.resolve() & (1 << QPalette::ButtonText))) //KDE uses windowText rather than buttonText for menus
				newMI.palette.setColor(QPalette::ButtonText, newMI.palette.windowText().color());
			QCommonStyle::drawControl(element, &newMI, painter, widget);
		}
		return;
	}
	QPlastiqueStyle::drawControl(element, newOption, painter, widget);
}

void KxDarkStyle::polish(QWidget *widget)
{
	// Call base class firsrt
	QPlastiqueStyle::polish(widget);

	// Swap window and button colors for tab bar
	if( widget->inherits("QTabBar")) 
	{
		QPalette p = standardPalette();
		QBrush windowDisabled = p.window();
		QBrush buttonDisabled = p.button();
		QBrush windowActive = p.window();
		QBrush buttonActive = p.button();
		QBrush windowInactive = p.window();
		QBrush buttonInactive = p.button();
		p.setBrush(QPalette::Disabled, QPalette::Button, windowDisabled);
		p.setBrush(QPalette::Disabled, QPalette::Window, buttonDisabled);
		p.setBrush(QPalette::Active, QPalette::Button, windowActive);
		p.setBrush(QPalette::Active, QPalette::Window, buttonActive);
		p.setBrush(QPalette::Inactive, QPalette::Button, windowInactive);
		p.setBrush(QPalette::Inactive, QPalette::Window, buttonInactive);
		widget->setPalette(p); 
	}
}

int KxDarkStyle::styleHint(StyleHint hint, const QStyleOption *opt, const QWidget *widget,
	QStyleHintReturn *returnData) const
{
	int ret = QPlastiqueStyle::styleHint(hint, opt, widget, returnData);

	// Do not etch text - doesn't look good on dark background
	if(hint == SH_EtchDisabledText) ret = 0;

	// Change default button focus so they don't always still focus from viewport
	if(hint ==  SH_Button_FocusPolicy) ret = Qt::NoFocus;

	// Move slider to LMB click position
	if (hint == SH_Slider_AbsoluteSetButtons) ret = Qt::LeftButton;

	// Make the gridcolor for tables a light grey rather than black
	if(hint == SH_Table_GridLineColor) 
	{
		QPalette p = standardPalette();
		ret = p.color(QPalette::Active, QPalette::Window).rgb(); 
	}

#if defined(QT_MAC_USE_COCOA)
	// Fix focus frame issue on Cocoa
	if(hint == SH_FocusFrame_AboveWidget)
	{
		ret = true;
	}
#endif

	return ret;
}

QSize KxDarkStyle::sizeFromContents(ContentsType ct, const QStyleOption *opt,
	const QSize &csz, const QWidget *widget) const
{
	QSize sz(csz);
	switch (ct) {
		case CT_PushButton:
			// A copy of QWindowStyle's implementation save for the forcing of
			// a minimum button width (we only want dialog buttons to do this).
			if (const QStyleOptionButton *btn = qstyleoption_cast<const QStyleOptionButton *>(opt)) {
				sz = QCommonStyle::sizeFromContents(ct, opt, csz, widget);
				int w = sz.width(),
					h = sz.height();
				int defwidth = 0;
				if (btn->features & QStyleOptionButton::AutoDefaultButton)
					defwidth = 2 * pixelMetric(PM_ButtonDefaultIndicator, btn, widget);
#ifndef QT_QWS_SMALL_PUSHBUTTON
				if (w < 75 + defwidth && !btn->text.isEmpty() && (btn->features & QStyleOptionButton::AutoDefaultButton))
					w = 75 + defwidth;
				if (h < 23 + defwidth)
					h = 23 + defwidth;
#endif
				sz = QSize(w, h);
			}
			break;
		default:
			sz = QPlastiqueStyle::sizeFromContents(ct, opt, csz, widget);
			break;
	}
	return sz;
}

QStyle::SubControl KxDarkStyle::hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option,
	const QPoint &pos, const QWidget *widget) const
{
	SubControl ret = SC_None;
	switch (control) {
#ifndef QT_NO_SCROLLBAR
case CC_ScrollBar:
if (const QStyleOptionSlider *scrollBar = qstyleoption_cast<const QStyleOptionSlider *>(option)) {
	QRect slider = subControlRect(control, scrollBar, SC_ScrollBarSlider, widget);
	if (slider.isValid() && slider.contains(pos)) {
		ret = SC_ScrollBarSlider;
		break;
	}

	QRect scrollBarAddLine = subControlRect(control, scrollBar, SC_ScrollBarAddLine, widget);
	if (scrollBarAddLine.isValid() && scrollBarAddLine.contains(pos)) {
		ret = SC_ScrollBarAddLine;
		break;
	}

	QRect scrollBarSubPage = subControlRect(control, scrollBar, SC_ScrollBarSubPage, widget);
	if (scrollBarSubPage.isValid() && scrollBarSubPage.contains(pos)) {
		ret = SC_ScrollBarSubPage;
		break;
	}

	QRect scrollBarAddPage = subControlRect(control, scrollBar, SC_ScrollBarAddPage, widget);
	if (scrollBarAddPage.isValid() && scrollBarAddPage.contains(pos)) {
		ret = SC_ScrollBarAddPage;
		break;
	}

	QRect scrollBarSubLine = subControlRect(control, scrollBar, SC_ScrollBarSubLine, widget);
	if (scrollBarSubLine.isValid() && scrollBarSubLine.contains(pos)) {
		ret = SC_ScrollBarSubLine;
		break;
	}
}
break;
#endif // QT_NO_SCROLLBAR
default:
break;
	}

	return ret != SC_None ? ret : QWindowsStyle::hitTestComplexControl(control, option, pos, widget);
}

int KxDarkStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
	int ret;
	const int kHotBoxMenuBarVMargin = 4;
	const int kHotBoxMenuBarHMargin = 3;

	switch(metric) {
		case PM_MenuBarVMargin:
			if (widget && widget->property("isQmayaHotBoxMenuBar").isValid()) {
				ret = kHotBoxMenuBarVMargin;
			} else {
				ret = 1;
			}
			break;
		case PM_MenuBarHMargin:
			if (widget && widget->property("isQmayaHotBoxMenuBar").isValid()) {
				ret = kHotBoxMenuBarHMargin;
			} else {
				ret = 0;
			}
			break;
		case PM_MenuBarPanelWidth:
			ret = 0;
			break;
		case PM_FocusFrameVMargin:
			ret = 0;
			break;
		case PM_FocusFrameHMargin:
			ret = 0;
			break;
		default:
			ret = QPlastiqueStyle::pixelMetric(metric, option, widget);
	}

	return ret;
}

QPixmap KxDarkStyle::generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt) const
{
	switch (iconMode) {
		case QIcon::Active: 
			{
				QImage img = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);
				int imgh = img.height();
				int imgw = img.width();
				QRgb pixel;
				for (int y = 0; y < imgh; ++y) {
					for (int x = 0; x < imgw; ++x) {
						pixel = img.pixel(x, y);
						int highLimit = 205; // value above this limit will just max up to 255
						int lowLimit = 30; // value below this limit will not be adjusted
						int adjustment = 255 - highLimit;
						QColor color(pixel);
						int h,s,v;
						color.getHsv(&h,&s,&v);
						if(v > lowLimit) {
							v = (v < highLimit) ? v + adjustment : 255;
						}
						color.setHsv(h,s,v);
						img.setPixel(x, y, qRgba(color.red(), color.green(), color.blue(), qAlpha(pixel)));
					}
				}
				return QPixmap::fromImage(img);
			}
		default:
			break;
	}

	return QPlastiqueStyle::generatedIconPixmap(iconMode, pixmap, opt);
}

QIcon KxDarkStyle::standardIconImplementation( StandardPixmap standardIcon, const QStyleOption * option, const QWidget * widget ) const
{
	QIcon icon;
	QPixmap pixmap;

	switch (standardIcon) {
case QStyle::SP_ArrowBack:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileDialogBack.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}    
case QStyle::SP_ArrowForward:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileDialogForward.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}    
case QStyle::SP_FileDialogToParent:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileDialogToParent.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}    
case QStyle::SP_FileDialogListView:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileDialogListView.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}    
case QStyle::SP_FileDialogDetailedView:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileDialogDetailedView.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_FileDialogNewFolder:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileDialogNewFolder.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DirHomeIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DirHomeIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DirIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DirIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DirLinkIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DirLinkIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_FileIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_ComputerIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_ComputerIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DesktopIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DesktopIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DriveCDIcon:
case SP_DriveDVDIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DriveCDIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DriveNetIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DriveNetIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DriveHDIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DriveHDIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_DriveFDIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_DriveFDIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_FileLinkIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_FileLinkIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
case SP_TrashIcon:
{
	pixmap = KxQtHelper::getCachedPixmap("SP_TrashIcon.png");
	icon.addPixmap(pixmap, QIcon::Normal);
	break;
}
default:
break;
	}

	if (icon.isNull())
		icon = QPlastiqueStyle::standardIconImplementation( standardIcon, option, widget );

	return icon;
}
