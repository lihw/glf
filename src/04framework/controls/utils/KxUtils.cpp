#include "KxUtils.h"

#if defined(OSWin_)
#include <Windows.h>
#endif

KxUtils * gKxUtils = NULL; 

//	The scheme symbol names for fonts.
//
char * schemeFontNameTable[] = {
    "boldLabelFont",
    "smallBoldLabelFont",
    "tinyBoldLabelFont",
    "plainLabelFont",
    "smallPlainLabelFont",
    "obliqueLabelFont",
    "smallObliqueLabelFont",
    "fixedWidthFont",
    "smallFixedWidthFont"
};

struct SchemeFontProperties {
    int pixelSize;
    bool bold;
    bool italic;
    bool underline;
    bool strikeOut;
    bool fixedPitch;
    QString family;
};

#if defined(OSMac_)
static SchemeFontProperties schemeFonts[] = {
    //	{	pixel,	bold,	italic,	underline,	strikeOut,	fixedPitch,	family }
    {	11,		true,	false,	false,		false,		false,		"Lucida Grande"},	//"boldLabelFont",
    {	10,		true,	false,	false,		false,		false,		"Lucida Grande"},	//"smallBoldLabelFont",
    {	9,		true,	false,	false,		false,		false,		"Lucida Grande"},	//"tinyBoldLabelFont",
    {	11,		false,	false,	false,		false,		false,		"Lucida Grande"},	//"plainLabelFont",
    {	10,		false,	false,	false,		false,		false,		"Lucida Grande"},	//"smallPlainLabelFont",
    {	11,		false,	true,	false,		false,		false,		"Lucida Grande"},	//"obliqueLabelFont",
    {	10,		false,	true,	false,		false,		false,		"Lucida Grande"},	//"smallObliqueLabelFont",
    {	12,		false,	false,	false,		false,		true,		"Courier"},	//"fixedWidthFont",
    {	10,		false,	false,	false,		false,		true,		"Courier"}	//"smallFixedWidthFont"
};
#elif defined(OSWin_)
static SchemeFontProperties schemeFonts[] = {
    //	{	pixel,	bold,	italic,	underline,	strikeOut,	fixedPitch,	family }
    {	11,		true,	false,	false,		false,		false,		"Tahoma"},	//"boldLabelFont",
    {	10,		true,	false,	false,		false,		false,		"Tahoma"},	//"smallBoldLabelFont",
    {	9,		true,	false,	false,		false,		false,		"Tahoma"},	//"tinyBoldLabelFont",
    {	11,		false,	false,	false,		false,		false,		"Tahoma"},	//"plainLabelFont",
    {	10,		false,	false,	false,		false,		false,		"Tahoma"},	//"smallPlainLabelFont",
    {	11,		false,	true,	false,		false,		false,		"Tahoma"},	//"obliqueLabelFont",
    {	10,		false,	true,	false,		false,		false,		"Tahoma"},	//"smallObliqueLabelFont",
    {	12,		false,	false,	false,		false,		true,		"Courier New"},	//"fixedWidthFont",
    {	11,		false,	false,	false,		false,		true,		"Courier New"}	//"smallFixedWidthFont"
};
#else
static SchemeFontProperties schemeFonts[] = {
    //	{	pixel,	bold,	italic,	underline,	strikeOut,	fixedPitch,	family }
    {	11,		true,	false,	false,		false,		false,		"Arial"},	//"boldLabelFont",
    {	10,		true,	false,	false,		false,		false,		"Arial"},	//"smallBoldLabelFont",
    {	9,		true,	false,	false,		false,		false,		"Arial"},	//"tinyBoldLabelFont",
    {	11,		false,	false,	false,		false,		false,		"Arial"},	//"plainLabelFont",
    {	10,		false,	false,	false,		false,		false,		"Arial"},	//"smallPlainLabelFont",
    {	11,		false,	true,	false,		false,		false,		"Arial"},	//"obliqueLabelFont",
    {	10,		false,	true,	false,		false,		false,		"Arial"},	//"smallObliqueLabelFont",
    {	12,		false,	false,	false,		false,		true,		"Courier New"},	//"fixedWidthFont",
    {	10,		false,	false,	false,		false,		true,		"Courier New"}	//"smallFixedWidthFont"
};
#endif


#if defined(OSWin_)

// Font initialization values 
static LOGFONT schemeFontDescription[] = {
    {-9,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// boldLabelFont
    {-8,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// smallboldLabelFont
    {-7,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// tinyboldLabelFont
    {-9,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// plainLabelFont
    {-8,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// smallplainLabelFont (used for controls, careful!)
    {-9,0,0,0,FW_NORMAL,TRUE,FALSE,FALSE,ANSI_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// obliqueLabelFont
    {-8,0,0,0,FW_NORMAL,TRUE,FALSE,FALSE,ANSI_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS
    ,L"MS Sans Serif"},			// smallobliqueLabelFont
    {-10,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_SWISS
    ,L"Courier New"},			// fixedwidthFont
    {-8,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,
    OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,FIXED_PITCH|FF_SWISS
    ,L"Courier New"} 			// smallfixedwidthFont
};

#elif defined(OSMac_)

//FMFontList sMacFontList[] = {
//    {kThemeEmphasizedSystemFont, -1, bold, 12, NULL},		// boldLabelFont
//    {kThemeSmallEmphasizedSystemFont, -1, bold, 10, NULL},		// smallBoldLabelFont
//    {kThemeCurrentPortFont, -1, bold, 9, NULL},		// tinyBoldLabelFont
//    {kThemeSystemFont, -1, normal, 12, NULL},	// plainLabelFont
//    {kThemeSmallSystemFont, -1, normal, 10, NULL},	// smallPlainLabelFont
//    {kThemeCurrentPortFont, -1, italic, 12, NULL},	// obliqueLabelFont
//    {kThemeCurrentPortFont, -1, italic, 10, NULL},	// smallObliqueLabelFont
//    {kThemeCurrentPortFont, -1, normal, 12, "\pCourier"},	// fixedWidthFont
//    {kThemeCurrentPortFont, -1, normal, 9, "\pCourier"}		// smallFixedWidthFont
//};
//
#endif



KxUtils::KxUtils()
{
    gKxUtils = this;

}

KxUtils::~KxUtils()
{

}


QColor KxUtils::schemeQColor(KxSchemeColor color) const
//
//	Description:
//		Return a scheme color as QColor.
//
{
    QPalette p = qApp->palette();
    // Get colors from application style or define custom
    switch(color) {
        case kBasicBackground: return p.color(QPalette::Active, QPalette::Base);
        case kTextForeground: return p.color(QPalette::Active, QPalette::WindowText);
        case kTextBackground: return p.color(QPalette::Active, QPalette::Base);
        case kTextFieldBackground: return p.color(QPalette::Active, QPalette::Base);
        case kReadOnlyBackground: return p.color(QPalette::Active, QPalette::Window);
        case kTextSelectedBackground: return p.color(QPalette::Active, QPalette::Highlight);
        case kTextSelectedBackground2: return (QColor::fromRgbF(0.29f,0.32f,0.34f));
        case kTextSelectedForeground: return p.color(QPalette::Active, QPalette::HighlightedText);
        case kDisabledTextForeground: return p.color(QPalette::Disabled, QPalette::Text);
        case kLayerAdjustmentTextForeground: return QColor(229, 105, 41);
        case kScrolledListBackground: return p.color(QPalette::Active, QPalette::Base);
        case kScrollBarTroughColor: return p.color(QPalette::Active, QPalette::Base);
        case kScrollBarControlBackground: return p.color(QPalette::Active, QPalette::Button);
        case kButtonBackground: return p.color(QPalette::Active, QPalette::Button);
        case kSelectFillColor: return p.color(QPalette::Active, QPalette::Highlight);
        case kSelectColor: return p.color(QPalette::Active, QPalette::HighlightedText);
        case kCheckColor: return p.color(QPalette::Active, QPalette::Button);
        case kRadioColor: return p.color(QPalette::Active, QPalette::Button);
        case kBlueSelectBackgroundColor1: return QColor(56,132,196);
        case kBlueSelectBackgroundColor2: return QColor(129,167,193);
        case kIndicatorBackground: return QColor(125,125,0);
        case kWarningColor: return QColor(0,0,128);
        case kErrorColor: return QColor(255,100,100);
        case kInformationColor: return QColor(255,255,255);
        case kAlternateBackground1: return QColor(255,255,255);
        case kAlternateBackground2: return QColor(241,203,165);
        case kAlternateBackground3: return QColor(174,240,215);
        case kAlternateBackground4: return QColor(241,203,165);
        case kAlternateBackground5: return QColor(241,203,165);
        case kAlternateBackground6: return QColor(241,203,165);
        case kConnectedBackground: return QColor(241,203,165);
        case kChannelBoxLabelBackground: return QColor(205,205,205);
        case kMultiBackground: return QColor(186,150,190);
        case kDrawingAreaBackground: return QColor(255,255,255);
        case kDrawingAreaContrastColor1: return QColor(255,100,100);
        case kDrawingAreaContrastColor2: return QColor(0,0,255);
        case kDrawingAreaContrastColor3: return QColor(0,255,0);
        case kDrawingAreaContrastColor4: return QColor(255,0,255);
        case kHighlightColor1: return QColor(255,255,0);
        case kHighlightColor2: return QColor(0,0,255);
        case kHighlightColor3: return QColor(0,255,0);
        case kHighlightColor4: return QColor(255,0,0);
        case kHighlightColor5: return QColor(255,0,0);
        case kHighlightColor6: return QColor(255,0,0);
        case kHighlightColor7: return QColor(255,0,0);
        case kHighlightColor8: return QColor(255,0,0);
        case kRedColor: return Qt::red;
        case kGreenColor: return Qt::green;
        case kBlueColor: return Qt::blue;
        case kOrangeColor: return QColor(255,128,64);
        case kYellowColor: return Qt::yellow;
        case kPurpleColor: return Qt::magenta;
        case kBrownColor: return QColor(128,64,0);
        case kWindowManagerBackground: return QColor(255,255,255);
        case kWindowManagerActiveBackground: return QColor(255,255,255);
        case kWindowManagerForeground: return QColor(0,0,0);
        case kWindowManagerActiveForeground: return QColor(0,0,0);
        default:
            break;
    }
    // Should not ever reach this code.
    return Qt::black;
}



QFont KxUtils::schemeQFont(TschemeFont font) const
{
    Q_ASSERT(kInvalidSchemeFont != font);

    SchemeFontProperties properties = schemeFonts[font];
    QFont qFont;
    qFont.setPixelSize(properties.pixelSize);
    qFont.setBold(properties.bold);
    qFont.setItalic(properties.italic);
    qFont.setUnderline(properties.underline);
    qFont.setStrikeOut(properties.strikeOut);
    qFont.setFixedPitch(properties.fixedPitch);
    qFont.setFamily(properties.family);
    return qFont;
}
