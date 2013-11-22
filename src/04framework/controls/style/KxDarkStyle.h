#ifndef KxDarkStyle_H_
#define KxDarkStyle_H_
#include "../base/HQtGui.h"
#include "../base/KxQtHelper.h"

class KX_DLLEXPORT KxDarkStyle : public QPlastiqueStyle
{
	Q_OBJECT
public:
	QPalette standardPalette()const;
	void polish(QWidget *widget);
	int styleHint(StyleHint hint, const QStyleOption *opt = 0, const QWidget *widget = 0, QStyleHintReturn *returnData = 0)const;
	void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget)const;
	void drawControl(ControlElement element, const QStyleOption *opt, QPainter *painter, const QWidget *widget)const;
	QSize sizeFromContents(ContentsType ct, const QStyleOption *opt, const QSize &contentsSize, const QWidget *widget)const;
	SubControl hitTestComplexControl(ComplexControl control, const QStyleOptionComplex *option, const QPoint &pos, const QWidget *widget)const;
	int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget)const;
	QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *opt)const;

	protected slots:
		QIcon standardIconImplementation(StandardPixmap standardIcon, const QStyleOption *option = 0, const QWidget *widget = 0)const;

private:
	static QColor fSelectedToolButtonColor;
	static QColor fPanelFocusRectColor;
};

#endif
