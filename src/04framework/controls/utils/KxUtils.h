#ifndef KxUtils_H_
#define KxUtils_H_

#include "../base/HQtGui.h"

class QColor;
class QFont;

class KX_DLLEXPORT KxUtils  
{
public:
    enum KxSchemeColor {

        //	Control (Widget) colors.
        //
        kInvalidSchemeColor = -1,
        kBasicBackground = 0,
        kTextForeground,
        kTextBackground,
        kTextFieldBackground,
        kReadOnlyBackground,
        kTextSelectedBackground,
        kTextSelectedBackground2,
        kTextSelectedForeground,
        kDisabledTextForeground,
        kLayerAdjustmentTextForeground,
        kScrolledListBackground,
        kScrollBarTroughColor,
        kScrollBarControlBackground,
        kButtonBackground,
        kSelectFillColor,
        kSelectColor,
        kCheckColor,
        kRadioColor,
        kBlueSelectBackgroundColor1,
        kBlueSelectBackgroundColor2,

        //	Information colors.
        //
        kIndicatorBackground,
        kWarningColor,
        kErrorColor,
        kInformationColor,

        //	Alternate background colors.
        //
        kAlternateBackground1,
        kAlternateBackground2,
        kAlternateBackground3,
        kAlternateBackground4,
        kAlternateBackground5,
        kAlternateBackground6,

        // more discriptive names
        kConnectedBackground,
        kChannelBoxLabelBackground,
        kMultiBackground,

        //	Drawing area colors.
        //
        kDrawingAreaBackground,
        kDrawingAreaContrastColor1,
        kDrawingAreaContrastColor2,
        kDrawingAreaContrastColor3,
        kDrawingAreaContrastColor4,

        //	Highlight colors.
        //
        kHighlightColor1,
        kHighlightColor2,
        kHighlightColor3,
        kHighlightColor4,
        kHighlightColor5,
        kHighlightColor6,
        kHighlightColor7,
        kHighlightColor8,

        //	Primary and secondary colors (brown too?).
        //
        kRedColor,
        kGreenColor,
        kBlueColor,
        kOrangeColor,
        kYellowColor,
        kPurpleColor,
        kBrownColor,

        //	Window manager colors.
        //
        kWindowManagerBackground,
        kWindowManagerActiveBackground,
        kWindowManagerForeground,
        kWindowManagerActiveForeground,

        //	Ensure that this is the last item.
        //
        kSchemeColorCount
    };

    // Scheme font classes

    enum TschemeFont {
        kInvalidSchemeFont = -1,
        kBoldLabelFont = 0,
        kSmallBoldLabelFont,
        kTinyBoldLabelFont,
        kPlainLabelFont,
        kSmallPlainLabelFont,
        kObliqueLabelFont,
        kSmallObliqueLabelFont,
        kFixedWidthFont,
        kSmallFixedWidthFont,

        //	Ensure that this is the last item.
        //
        kSchemeFontCount,

        // synonyms
        kObjectFont = kPlainLabelFont,
        kConnectedFont = kSmallObliqueLabelFont,
        kStaticFont = kSmallPlainLabelFont
    };
    // Scheme font attribute settings (weight, pitch)

    enum TschemeFontWeight {
        kInvalidFontWeight = -1,
        kFontWeightNormal = 0,
        kFontWeightBold
    };
    enum TschemeFontPitch {
        kInvalidFontPitch = -1,
        kFontPitchVariable = 0,
        kFontPitchFixed
    };
    enum TschemeFontStyle {
        kInvalidFontStyle = -1,
        kFontStyleNormal = 0,
        kFontStyleItalic
    };


public:
    KxUtils();
    virtual ~KxUtils();

    QColor			schemeQColor	(KxSchemeColor)		const;
    QFont			schemeQFont		(TschemeFont)		const;

};
extern KX_DLLEXPORT KxUtils * gKxUtils;


#endif