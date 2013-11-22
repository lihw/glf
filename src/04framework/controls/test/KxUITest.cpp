// KxUITest.h
// The main window of control test.
//
// Copyright 2012 by Bruce Wuu. All rights reserved.
//
// Bruce Wuu brucewuu@gmail.com

#include "KxUITest.h"


#include <controls/KxControl>

static const kwChar* enumNames[] = 
{
    "enum 1",
    "enum 2",
    "enum 3",
    "enum 4",
};

KxUITest::KxUITest(QWidget* parent)
    : QWidget(parent)
    , _propertyEnum(enumNames, 0, 3)
{
    setFixedSize(800, 600);
    //resize(800,600);
    //setProperty("explicitWidthSet",qVariantFromValue(800));
    //setProperty("explicitHeightSet",qVariantFromValue(600));
    //setProperty("fixedSize",qVariantFromValue(true));
    setWindowTitle("Qt Sample Window Tester");
    fFormLayout = new KxFormLayout(this);
    setLayout(fFormLayout);

    fTabWidget = new QWidget(this);
    fTabLayout = new KxTabLayout(fTabWidget,true);
    fTabWidget->setLayout(fTabLayout);
    fDisableAllCheckBox = new KxCheckBox(tr("Disable All"));

    connect(fDisableAllCheckBox,SIGNAL(checkStateChangedToOn()),this,SLOT(qtDisableAll()));
    connect(fDisableAllCheckBox,SIGNAL(checkStateChangedToOff()),this,SLOT(qtEnableAll()));

    fFormLayout->addWidget(fTabWidget);
    fFormLayout->addWidget(fDisableAllCheckBox);

    // set the positin of tablayout in formlayout 
    fFormLayout->attach(fTabWidget,kAttachTop,kAttachForm,0);
    fFormLayout->attach(fTabWidget,kAttachLeft,kAttachForm,0);
    fFormLayout->attach(fTabWidget,fDisableAllCheckBox,kAttachBottom,kAttachControl,10);
    fFormLayout->attach(fTabWidget,kAttachRight,kAttachForm,0);

    fFormLayout->attach(fDisableAllCheckBox,kAttachTop,kAttachNone);
    fFormLayout->attach(fDisableAllCheckBox,kAttachLeft,kAttachForm,10);
    fFormLayout->attach(fDisableAllCheckBox,kAttachBottom,kAttachForm,10);
    fFormLayout->attach(fDisableAllCheckBox,kAttachRight,kAttachForm,10);

        testPropertyWidgets();

    //set the content of tablayout widget 
   // First Tab 
    fFirstTabWidget = new QWidget;
    KxColumnLayout * tab1ColumnLayout = new KxColumnLayout(fFirstTabWidget);
    fFirstTabWidget->setLayout(tab1ColumnLayout);
    tab1ColumnLayout->setColumnAdjustable(true);
    fTabLayout->addWidget(fFirstTabWidget);

    // columnlayout content 
    // framelayout 
    QWidget * frameLayoutWidget1 = new QWidget;
    KxFrameLayout *frameLayout1 = new KxFrameLayout(frameLayoutWidget1);
    frameLayout1->setTitle("static text");
    tab1ColumnLayout->addWidget(frameLayoutWidget1);
    //framelayout content 
    // row layout 1
    QWidget * rowLayoutWidget1 = new QWidget;
    KxRowLayout *rowLayout1 = new KxRowLayout(rowLayoutWidget1);
    rowLayout1->setNumColumns(4);
    frameLayout1->addWidget(rowLayoutWidget1);
    // row layout content 
    // column layout 
    QWidget * columnLayoutWidget1 = new QWidget;
    KxColumnLayout *columnLayout1 = new KxColumnLayout(columnLayoutWidget1);
    rowLayout1->addWidget(columnLayoutWidget1);
    // column layout content 
    KxLabel *label1 = new KxLabel("boldLabelFont");
    label1->setFont(gKxUtils->schemeQFont(KxUtils::kBoldLabelFont));
    columnLayout1->addWidget(label1);
    KxLabel *label2 = new KxLabel("smallBoldLabelFont");
    label2->setFont(gKxUtils->schemeQFont(KxUtils::kSmallBoldLabelFont));
    columnLayout1->addWidget(label2);
    KxLabel *label3 = new KxLabel("tinyBoldLabelFont");
    label3->setFont(gKxUtils->schemeQFont(KxUtils::kTinyBoldLabelFont));
    columnLayout1->addWidget(label3);
    // column layout 2 
    QWidget * columnLayoutWidget2 = new QWidget;
    KxColumnLayout *columnLayout2 = new KxColumnLayout(columnLayoutWidget2);
    rowLayout1->addWidget(columnLayoutWidget2);
    // column layout2 contents 
    KxLabel *label21 =  new KxLabel("plainLabelFont");
    label21->setFont(gKxUtils->schemeQFont(KxUtils::kPlainLabelFont));
    KxLabel *label22 = new KxLabel("smallPlainLabelFont");
    label22->setFont(gKxUtils->schemeQFont(KxUtils::kSmallPlainLabelFont));
    KxLabel *label23 = new KxLabel("");

    columnLayout2->addWidget(label21);
    columnLayout2->addWidget(label22);
    columnLayout2->addWidget(label23);

    /// column layout 3 
    QWidget *columnLayoutWidget3 = new QWidget;
    KxColumnLayout *columnLayout3 = new KxColumnLayout(columnLayoutWidget3);
    rowLayout1->addWidget(columnLayoutWidget3);
    // column layout 3 contents 
    KxLabel *label31 = new KxLabel("obliqueLabelFont");
    label31->setFont(gKxUtils->schemeQFont(KxUtils::kObliqueLabelFont));
    KxLabel *label32 = new KxLabel("smallObliqueLabelFont");
    label32->setFont(gKxUtils->schemeQFont(KxUtils::kSmallObliqueLabelFont));
    KxLabel *label33 = new KxLabel("");

    columnLayout3->addWidget(label31);
    columnLayout3->addWidget(label32);
    columnLayout3->addWidget(label33);

    // column layout 4 
    QWidget *columnLayoutWidget4 = new QWidget;
    KxColumnLayout *columnLayout4 = new KxColumnLayout(columnLayoutWidget4);
    rowLayout1->addWidget(columnLayoutWidget4);
    // column layout 4 content 
    KxLabel *label41 = new KxLabel("fixedWidthFont");
    label41->setFont(gKxUtils->schemeQFont(KxUtils::kFixedWidthFont));
    KxLabel *label42 = new KxLabel("smallFixedWidthFont");
    label42->setFont(gKxUtils->schemeQFont(KxUtils::kSmallFixedWidthFont));
    KxLabel *label43 = new KxLabel("");
    columnLayout4->addWidget(label41);
    columnLayout4->addWidget(label42);
    columnLayout4->addWidget(label43);

     // go back to tab1ColumnLayout 
    // add button 
    QWidget *buttonWidget = new QWidget;
    KxRowLayout *buttonRowLayout = new KxRowLayout(buttonWidget);
    buttonWidget->setLayout(buttonRowLayout);
    tab1ColumnLayout->addWidget(buttonWidget);

    // content of button row layout 
    KxLabel *buttonLabel = new KxLabel("button");
    QPushButton *button = new QPushButton("Button");
    buttonRowLayout->addWidget(buttonLabel);
    buttonRowLayout->addWidget(button);
    buttonRowLayout->setNumColumns(2);
    // symbol Buttons 
    QWidget *symbolWidget = new QWidget;
    KxRowLayout *symbolLayout = new KxRowLayout(symbolWidget);
    symbolWidget->setLayout(symbolLayout);
    symbolLayout->setNumColumns(3);
    KxLabel *symbolLabel = new KxLabel("symbolButton");
    KxSymbolButton *symbolButton1 = new KxSymbolButton;
	symbolButton1->iconTextControl()->setIcon(":/circle.png");
	KxSymbolButton *symbolButton2 = new KxSymbolButton;
	symbolButton2->iconTextControl()->setIcon(":/sphere.png");
    symbolLayout->addWidget(symbolLabel);
    symbolLayout->addWidget(symbolButton1);
	symbolLayout->addWidget(symbolButton2);

    tab1ColumnLayout->addWidget(symbolWidget);

	// checkbox 
	QWidget *checkboxWidget = new QWidget;
	KxRowLayout *checkboxLayout = new KxRowLayout(checkboxWidget);
	checkboxLayout->setNumColumns(2);
	KxLabel *checkboxLabel1 = new KxLabel("checkbox");
	KxCheckBox *checkbox1 = new KxCheckBox("checkBox");
	checkboxLayout->addWidget(checkboxLabel1);
	checkboxLayout->addWidget(checkbox1);
	tab1ColumnLayout->addWidget(checkboxWidget);
    
	//radio buttons 
	QWidget *radiobuttonWidget = new QWidget;
	KxRowLayout *radiobuttonLayout1 = new KxRowLayout(radiobuttonWidget);
	radiobuttonLayout1->setNumColumns(3);
	KxRadioButton *radiobutton1 = new KxRadioButton("radio1");
	KxRadioButton *radiobutton2 = new KxRadioButton("radio2");
	KxLabel *radioLabel1 = new KxLabel("radioButton");
	KxButtonGroup *buttonGroup = new KxButtonGroup;
	buttonGroup->addButton(radiobutton1);
	buttonGroup->addButton(radiobutton2);
	radiobuttonLayout1->addWidget(radioLabel1);
	radiobuttonLayout1->addWidget(radiobutton1);
	radiobuttonLayout1->addWidget(radiobutton2);
	tab1ColumnLayout->addWidget(radiobuttonWidget);

	// radio buttongrp 
	QWidget *radiobuttongrpWidget = new QWidget;
	KxRowLayout *radiobuttongrpLayout1 = new KxRowLayout(radiobuttongrpWidget);
	radiobuttongrpLayout1->setNumColumns(5);
	KxButtonGroup *radiobuttonGroup1  = new KxButtonGroup;
	KxRadioButtonGrp *radiobuttonGrp1 = new KxRadioButtonGrp(NULL,true,4,radiobuttonGroup1);
	radiobuttonGrp1->setLabelText("radiobutton grp");
	radiobuttongrpLayout1->addWidget(radiobuttonGrp1);
	tab1ColumnLayout->addWidget(radiobuttongrpWidget);
	KxRadioButton *radiobuttongrparray[4];
	radiobuttonGrp1->getFields(radiobuttongrparray[0],radiobuttongrparray[1],radiobuttongrparray[2],radiobuttongrparray[3]);
	for(int i=0;i < 4; ++i){
		radiobuttongrparray[i]->setText(QString("radio %1").arg(i+1));
	}

	// float widgets 
    QWidget *floatFrameWidget = new QWidget;
    KxFrameLayout *floatFrameLayout1 = new KxFrameLayout(floatFrameWidget);
    floatFrameLayout1->setTitle("frameLayout");
    floatFrameLayout1->setCollapsible(true);
    tab1ColumnLayout->addWidget(floatFrameWidget);

    QWidget *floatColumnWidget = new QWidget;
    KxColumnLayout *floatColumnLayout1 = new KxColumnLayout(floatColumnWidget);
    floatFrameLayout1->addWidget(floatColumnWidget);

    QWidget *floatColumn_rowWidget = new QWidget;
    KxRowLayout *floatColumn_rowLayout = new KxRowLayout(floatColumn_rowWidget);
    floatColumnLayout1->addWidget(floatColumn_rowWidget);
    KxLabel *floatFieldLabel = new KxLabel("floatField");
    KxFloatField *floatField1 = new KxFloatField;
    floatColumn_rowLayout->setNumColumns(2);
    floatColumn_rowLayout->addWidget(floatFieldLabel);
    floatColumn_rowLayout->addWidget(floatField1);

    //second rowlayout 

    QWidget *floatColumn_rowWidget2 = new QWidget;
    KxRowLayout * floatColumn_rowLayout2 = new KxRowLayout(floatColumn_rowWidget2);
    floatColumnLayout1->addWidget(floatColumn_rowWidget2);
    KxLabel *floatSliderLabel = new KxLabel("floatslider");
    KxFloatSlider *floatSlider1 = new KxFloatSlider;
    floatColumn_rowLayout2->setNumColumns(2);
    floatColumn_rowLayout2->addWidget(floatSliderLabel);
    floatColumn_rowLayout2->addWidget(floatSlider1);
    //third rowlayout 

    QWidget *floatColumn_rowWidget3 = new QWidget;
    KxRowLayout *floatColumn_rowLayout3 = new KxRowLayout(floatColumn_rowWidget3);
    floatColumnLayout1->addWidget(floatColumn_rowWidget3);
    KxFloatField *floatField2 = new KxFloatField;
    KxFloatSlider *floatSlider2 = new KxFloatSlider(Qt::Horizontal);
    KxFloatSliderGrp *floatSliderGrp1 = new KxFloatSliderGrp(NULL,true,floatField2,floatSlider2,false);
    floatSliderGrp1->setLabelText("float field slider");
    floatColumn_rowLayout3->addWidget(floatSliderGrp1);
    floatColumn_rowLayout3->setNumColumns(2);

    
    //fourth rowlayout 
    QWidget *floatColumn_rowWidget4 = new QWidget;
    KxRowLayout *floatColumn_rowLayout4 = new KxRowLayout(floatColumn_rowWidget4);
    floatColumnLayout1->addWidget(floatColumn_rowWidget4);
    KxLabel *floatScrollbarLabel = new KxLabel("floatScrollBar");
    KxFloatScrollBar *floatScrollBar1 =new KxFloatScrollBar;
    floatScrollBar1->setOrientation(Qt::Horizontal);
    floatColumn_rowLayout4->addWidget(floatScrollbarLabel);
    floatColumn_rowLayout4->addWidget(floatScrollBar1);

    //  set parent to tabcolumnlayout 
    QWidget *separatorRowWidget = new QWidget;
    KxRowLayout *separatorRowLayout = new KxRowLayout(separatorRowWidget);
    separatorRowLayout->setNumColumns(2);
    KxLabel *separatorLabel = new KxLabel("sparator");
    QFrame *separator = new QFrame();
    separator->setFixedSize(500,10);
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Plain);
    separator->setLineWidth(3);
    separatorRowLayout->addWidget(separatorLabel);
    separatorRowLayout->addWidget(separator);

    tab1ColumnLayout->addWidget(separatorRowWidget);

    // int widgets 
    QWidget *intWidget = new QWidget;
    KxFrameLayout *intFrameLayout = new KxFrameLayout(intWidget);
    tab1ColumnLayout->addWidget(intWidget);
    intFrameLayout->setTitle("colappsed framelayout");
    intFrameLayout->setCollapsible(true);

    QWidget *intColumnWidget = new QWidget;
    KxColumnLayout *intColumnLayout = new KxColumnLayout(intColumnWidget);
    intFrameLayout->addWidget(intColumnWidget);

    QWidget * intFieldRowWidget = new QWidget;
    KxRowLayout *intFieldRowLayout = new KxRowLayout(intFieldRowWidget);
    intColumnLayout->addWidget(intFieldRowWidget);
    KxLabel *intFieldLabel = new KxLabel("int field");
    KxIntField *intField1 = new KxIntField;
    intFieldRowLayout->addWidget(intFieldLabel);
    intFieldRowLayout->addWidget(intField1);
    intFieldRowLayout->setNumColumns(2);

    QWidget *intSliderRowWidget = new QWidget;
    KxRowLayout *intSliderRowLayout = new KxRowLayout(intSliderRowWidget);
    intColumnLayout->addWidget(intSliderRowWidget);
    intSliderRowLayout->setNumColumns(2);
    KxLabel *intSliderLabel = new KxLabel("int slider");
    KxIntSlider *intSlider = new KxIntSlider(Qt::Horizontal);
    intSliderRowLayout->addWidget(intSliderLabel);
    intSliderRowLayout->addWidget(intSlider);

    QWidget *intScrollbarRowWidget =  new QWidget;
    KxRowLayout *intScrollbarRowLayout = new KxRowLayout(intScrollbarRowWidget);
    intColumnLayout->addWidget(intScrollbarRowWidget);
    intScrollbarRowLayout->setNumColumns(2);
    KxLabel *intScrollbarLabel = new KxLabel("int scrollbar");
    KxIntScrollBar *intscrollbar = new KxIntScrollBar;
    intscrollbar->setOrientation(Qt::Horizontal);
    intScrollbarRowLayout->addWidget(intScrollbarLabel);
    intScrollbarRowLayout->addWidget(intscrollbar);

    //Misc widgets 
    QWidget *miscWidget = new QWidget;
    KxFrameLayout *miscFrameLayout = new KxFrameLayout(miscWidget);
    miscFrameLayout->setTitle("Misc widgets ");
    miscFrameLayout->setCollapsible(true);
    tab1ColumnLayout->addWidget(miscWidget);
    
    QWidget *miscColumnWidget = new QWidget;
    KxColumnLayout *miscColumnLayout = new KxColumnLayout(miscColumnWidget);
    miscFrameLayout->addWidget(miscColumnWidget);

    QWidget *textFieldWidget =new QWidget;
    KxRowLayout *textFieldRowLayout =new KxRowLayout(textFieldWidget);
    miscColumnLayout->addWidget(textFieldWidget);
    textFieldRowLayout->setNumColumns(2);
    KxLabel *textFieldLabel = new KxLabel("text field");
    KxField  * textField = new KxField;
    textField->setText("text goes here");
    textFieldRowLayout->addWidget(textFieldLabel);
    textFieldRowLayout->addWidget(textField);

    QWidget *optionMenuWidget = new QWidget;
    KxRowLayout *optionMenuRowLayout = new KxRowLayout(optionMenuWidget);
    miscColumnLayout->addWidget(optionMenuWidget);
    optionMenuRowLayout->setNumColumns(2);
    KxLabel *optionMenuLabel = new KxLabel("option menu");
    KxOptionMenu *optionMenu = new KxOptionMenu;
    for(int i=0; i < 4; ++i){
        KxMenuItem *menuItem = new KxMenuItem(QString("menuitem %1").arg(i+1),NULL);
        optionMenu->menu()->addAction(menuItem);
        optionMenu->addItem(menuItem->text());
    }
    optionMenuRowLayout->addWidget(optionMenuLabel);
    optionMenuRowLayout->addWidget(optionMenu);

     //progress bar 
    QWidget *progressBarWidget = new QWidget;
    KxRowLayout *progressBarRowLayout = new KxRowLayout(progressBarWidget);
    miscColumnLayout->addWidget(progressBarWidget);
    progressBarRowLayout->setNumColumns(2);
    KxLabel *progressBarLabel = new KxLabel("progress bar ");
    KxProgressBar *progressBar = new KxProgressBar;
    progressBar->setFixedWidth(500);
    progressBar->setRange(0,10);
    progressBar->setValue(2);
    progressBarRowLayout->addWidget(progressBarLabel);
    progressBarRowLayout->addWidget(progressBar);

    /// canvas to be defined 

    // scrollfield 
    KxScrollField *scrollField = new KxScrollField;
    scrollField->setLineWrapMode(QTextEdit::WidgetWidth);
    scrollField->setText("Editable scrollfield width world wrap ");
    tab1ColumnLayout->addWidget(scrollField);





    ///second tab 
    QWidget *secondTabWidget = new QWidget;
    KxColumnLayout *secondTabColumnLayout = new KxColumnLayout(secondTabWidget);
    fTabLayout->addWidget(secondTabWidget);

    // menu for menubar layout 
    QWidget *menubarWidget = new QWidget;
    menubarWidget->setFixedSize(100,20);
    KxMenuBarLayout *menubarLayout = new KxMenuBarLayout(menubarWidget);
    secondTabColumnLayout->addWidget(menubarWidget);
    QMenuBar *menubar  = (QMenuBar*)menubarLayout->menuBar();
    if(menubar == NULL){
        menubar = new QMenuBar;
        menubarLayout->setMenuBar(menubar);
    }
    QMenu *menu1 = new QMenu("File",menubar);
    menu1->setWindowFlags(menu1->windowFlags() | Qt::WindowStaysOnTopHint);
    menu1->setWindowTitle("File");
    menubar->addMenu(menu1);

    for(int i=0; i < 4; ++i){
        KxMenuItem *menuItem = new KxMenuItem(QString("menuitem %1").arg(i+1),NULL);
        menu1->addAction(menuItem);
    }

    //  scrolllayout 

    QWidget *scrollLayoutWidget = new QWidget;
    KxScrollLayout *scrollLayout1 = new KxScrollLayout(scrollLayoutWidget);
   scrollLayoutWidget->setMinimumSize(400,200);
   scrollLayoutWidget->setProperty("explicitWidth",qVariantFromValue(400));
   scrollLayoutWidget->setProperty("explicitHeight",qVariantFromValue(200));

    secondTabColumnLayout->addWidget(scrollLayoutWidget);

    QWidget *scrollLayoutContentWidget =new QWidget;
    KxRowColumnLayout * rowcolumnLayout1 = new KxRowColumnLayout(scrollLayoutContentWidget,KxRowColumnLayout::kFixNumberOfColumns,3);
    for(int i=0;i < 10; ++i){
        KxLabel *label = new KxLabel(QString("item %1").arg(i+1));
        KxIntField *intfiled = new KxIntField;
        KxIntSlider *intslider = new KxIntSlider(Qt::Horizontal);
        rowcolumnLayout1->addWidget(label);
        rowcolumnLayout1->addWidget(intfiled);
        rowcolumnLayout1->addWidget(intslider);
    }
    scrollLayout1->addWidget(scrollLayoutContentWidget);
    

    //nested framelayout 
    QWidget *nestedFrameWidget = new QWidget;
    KxFrameLayout *nestedFrameLayout1 = new KxFrameLayout(nestedFrameWidget);
    secondTabColumnLayout->addWidget(nestedFrameWidget);

    nestedFrameLayout1->setTitle("nested framelayout 1");
    nestedFrameLayout1->setCollapsible(true);

    QWidget *nestedFrameWidget2 = new QWidget;
    {
        KxFrameLayout *nestedFrameLayout2 = new KxFrameLayout(nestedFrameWidget2);
        nestedFrameLayout2->setTitle("nested framelaout 2");
        //nestedFrameLayout2->setCollapsible(true);
        //nestedFrameLayout2->setCollapseState(false);
        KxLabel *nestedLabel = new KxLabel("nested framelayout");
        QPushButton *button2 = new QPushButton("ok") ;
        nestedFrameLayout2->addWidget(nestedLabel);
        nestedFrameLayout2->addWidget(button2);
    }
    
    QWidget *nestedFrameWidget3 = new QWidget;
    {
        KxFrameLayout *nestedFrameLayout3 = new KxFrameLayout(nestedFrameWidget3);
        nestedFrameLayout3->setTitle("nested framelaout 3");
        nestedFrameLayout3->setCollapsible(true);
        nestedFrameLayout3->setCollapseState(false);
        KxLabel *nestedLabel = new KxLabel("nested framelayout");
        QPushButton *button2 = new QPushButton("ok") ;
        nestedFrameLayout3->addWidget(nestedLabel);
        nestedFrameLayout3->addWidget(button2);
    }

    QWidget *nestedFrameWidget4 = new QWidget;
    {
        KxFrameLayout *nestedFrameLayout4 = new KxFrameLayout(nestedFrameWidget4);
        nestedFrameLayout4->setTitle("nested framelaout 3");
        nestedFrameLayout4->setCollapsible(true);
        nestedFrameLayout4->setCollapseState(false);
        KxLabel *nestedLabel = new KxLabel("nested framelayout");
        QPushButton *button2 = new QPushButton("ok") ;
        nestedFrameLayout4->addWidget(nestedLabel);
        nestedFrameLayout4->addWidget(button2);
    }

    nestedFrameLayout1->addWidget(nestedFrameWidget2);
    nestedFrameLayout1->addWidget(nestedFrameWidget3);
    nestedFrameLayout1->addWidget(nestedFrameWidget4);

    // gridlayout 
    QWidget *gridWidget = new QWidget;
    KxGridLayout *gridLayout1 = new KxGridLayout(gridWidget,false,false,true);
    secondTabColumnLayout->addWidget(new KxLabel("gridLayout"));
    secondTabColumnLayout->addWidget(gridWidget);
    gridLayout1->setNumberOfColumns(2);
    gridLayout1->setCellWidth(50);
    gridLayout1->setCellHeight(50);
    for(int i=0; i < 9; ++i){
        QPushButton *b = new QPushButton(QString("button %1").arg(i+1));
        gridLayout1->addWidget(b);
    }
    //rowlayout 
    QWidget *rowLayoutWidget = new QWidget;
    KxRowLayout *rowLayout2 = new KxRowLayout(rowLayoutWidget);
    secondTabColumnLayout->addWidget(new KxLabel("rowLayout"));
    secondTabColumnLayout->addWidget(rowLayoutWidget);
    rowLayout2->setNumColumns(7);
    for(int i=0;i < 15; ++i){
        QPushButton *button = new QPushButton(QString("rowLayout Button %1").arg(i+1));
        rowLayout2->addWidget(button);
    }

    // panel layout to be defined 

    // third tab 
    // icontext controls 
    QWidget *iconTextColumnLayoutWidget =  new QWidget;
    KxColumnLayout *iconTextColumnLayout1 = new KxColumnLayout(iconTextColumnLayoutWidget);
    fTabLayout->addWidget(iconTextColumnLayoutWidget);

    // icontextbutton 
    KxIconTextButton *iconTextButton1 = new KxIconTextButton;
    iconTextButton1->iconTextControl()->setStyle(KxIconTextControl::iconAndTextHorizontal);
    iconTextButton1->iconTextControl()->setIcon(":/sphere.png");
    iconTextButton1->iconTextControl()->setText("iconTextButton");
    iconTextColumnLayout1->addWidget(iconTextButton1);

    // icontextcheckbxo 
    KxIconTextCheckBox *iconTextCheckbox1 = new KxIconTextCheckBox;
    iconTextCheckbox1->iconTextControl()->setStyle(KxIconTextControl::iconAndTextHorizontal);
    iconTextCheckbox1->iconTextControl()->setIcon(":/circle.png");
    iconTextCheckbox1->iconTextControl()->setText("iconTextCheckbox");
    iconTextColumnLayout1->addWidget(iconTextCheckbox1);

    //icontext radio collection 
    QWidget *icontextRadiocollectionWidget = new QWidget;
    KxFrameLayout *icontextRadiocollectionFramelayout = new KxFrameLayout(icontextRadiocollectionWidget);
    iconTextColumnLayout1->addWidget(icontextRadiocollectionWidget);
    icontextRadiocollectionFramelayout->setTitle("iconTextRadioButton");

    QWidget *icontextRadioColumnLayoutWidget = new QWidget;
    KxColumnLayout *icontextRadioColumnLayout = new KxColumnLayout(icontextRadioColumnLayoutWidget);
    icontextRadiocollectionFramelayout->addWidget(icontextRadioColumnLayoutWidget);
    KxButtonGroup *icontextRadiobuttonGroup = new KxButtonGroup;

    KxIconTextRadioButton *r1 = new KxIconTextRadioButton;
    r1->iconTextControl()->setStyle(KxIconTextControl::textOnly);
    r1->iconTextControl()->setText("bubble me ");
    icontextRadiobuttonGroup->addButton(r1);
    icontextRadioColumnLayout->addWidget(r1);


    KxIconTextRadioButton *r2 = new KxIconTextRadioButton;
    r2->iconTextControl()->setStyle(KxIconTextControl::iconOnly);
    r2->iconTextControl()->setIcon(":/circle.png");
    icontextRadiobuttonGroup->addButton(r2);
    icontextRadioColumnLayout->addWidget(r2);

    KxIconTextRadioButton *r3 = new KxIconTextRadioButton;
    r3->iconTextControl()->setStyle(KxIconTextControl::iconAndTextHorizontal);
    r3->iconTextControl()->setText("sphere");
    r3->iconTextControl()->setIcon(":/sphere.png");
    icontextRadiobuttonGroup->addButton(r3);
    icontextRadioColumnLayout->addWidget(r3);

     // icontext static label 
    QWidget *icontextStaticFrameLayoutWidget = new QWidget;
    KxFrameLayout *icontextStaticFrameLayout = new KxFrameLayout(icontextStaticFrameLayoutWidget);
    iconTextColumnLayout1->addWidget(icontextStaticFrameLayoutWidget);
    icontextStaticFrameLayout->setTitle("iconTextStaticLabel");

    QWidget *icontextStaticColumnLayoutWidget =new QWidget;
    KxColumnLayout *icontextStaticColumnLayout = new KxColumnLayout(icontextStaticColumnLayoutWidget);
    icontextStaticFrameLayout->addWidget(icontextStaticColumnLayoutWidget);

    for(int i=0; i < 3; i++){
        KxIconTextStaticLabel *label = new KxIconTextStaticLabel;
        label->setIcon(":/sphere.png");
        label->setStyle(KxIconTextControl::iconAndTextHorizontal);
        label->setText(QString("icontext static text %1 ").arg(i+1));
        icontextStaticColumnLayout->addWidget(label);
    }

    // fourth tab 
    QWidget *groupcontrolsWidget = new QWidget;
    KxColumnLayout *groupcontrolsColumnLayout = new KxColumnLayout(groupcontrolsWidget);
    fTabLayout->addWidget(groupcontrolsWidget);

    //color slider grp  to be defined 

    //colorslider button grp to be defined 
    
    // color index slider grp to be defined 
    
    // int slider grp 
    QWidget *intslidergrpWidget = new QWidget;
    KxRowLayout *intslidergrpRowlayout = new KxRowLayout(intslidergrpWidget);
    groupcontrolsColumnLayout->addWidget(intslidergrpWidget);
    KxIntField *intfiled1 = new KxIntField;
    KxIntSlider *intslider1 = new KxIntSlider(Qt::Horizontal);
    KxIntSliderGrp *intsliderGrp1 = new KxIntSliderGrp(NULL,true,intField1,intslider1);
    intsliderGrp1->setLabelText("intSliderGrp");
    intslidergrpRowlayout->addWidget(intsliderGrp1);

    //float slider button grp 
    QWidget *floatsliderbuttongrpWidget = new QWidget;
    KxRowLayout *floatsliderbuttongrpLayout =  new KxRowLayout(floatsliderbuttongrpWidget);
    groupcontrolsColumnLayout->addWidget(floatsliderbuttongrpWidget);

    KxFloatField *floatfiled1 = new KxFloatField;
    KxFloatSlider *floatslider2 = new KxFloatSlider(Qt::Horizontal);
    KxFloatSliderGrp *floatslidergrp2 = new KxFloatSliderGrp(NULL,true,floatfiled1,floatslider2);
    QPushButton *floatsliderbutton = new QPushButton("floatSliderButton");
    floatslidergrp2->setLabelText("floatSliderButtonGrp");
    floatslidergrp2->addWidgetToGrp(floatsliderbutton);
    floatsliderbuttongrpLayout->addWidget(floatslidergrp2);


    // fifth tab 
    // fieldgrp controls 
     QWidget *groupfiledsWidget = new QWidget;
     KxColumnLayout *groupFieldsColumnLayout = new KxColumnLayout(groupfiledsWidget);
     fTabLayout->addWidget(groupfiledsWidget);

     // attrfilegrp to be defined 

     // attrfilegrp to be defined with attributes 

     // floatFieldGrp 
     QWidget *floatFieldGrpWidget = new QWidget;
     KxRowLayout *floatFieldRowLayout = new KxRowLayout(floatFieldGrpWidget);
     groupFieldsColumnLayout->addWidget(floatFieldGrpWidget);
     floatFieldRowLayout->setNumColumns(2);

     KxLabel *floatfieldLabel = new KxLabel("floatFieldGrp");
     KxFloatFieldGrp *floatFieldGrp1 = new KxFloatFieldGrp;
     floatFieldGrp1->addLabel();
     floatFieldGrp1->setLabelText("floatFiledGrp");
     floatFieldGrp1->setColumnWidth(0, kLabelWidth);
     floatFieldGrp1->setColumnAttachment(0, KxWidgetGrp::kBothHSides, 0);
     floatFieldGrp1->setColumnAlignment(0, Qt::AlignLeft | Qt::AlignVCenter);
     for(int i=1; i < 5; ++i){
         KxFloatField *tempFloatField = new KxFloatField;
         tempFloatField->setDecimals(kDefaultDigits);
         tempFloatField->setDeltaF(kFieldDelta);
         floatFieldGrp1->addField(tempFloatField);
         floatFieldGrp1->setColumnWidth(i, kFieldWidth);
         floatFieldGrp1->setColumnAttachment(i, KxWidgetGrp::kBothHSides, 0);
         floatFieldGrp1->setColumnAlignment(i, Qt::AlignRight | Qt::AlignVCenter);
         tempFloatField->setValueF((double)i);
     }
     floatFieldRowLayout->addWidget(floatfieldLabel);
     floatFieldRowLayout->addWidget(floatFieldGrp1);
        
     // int field grp 
     QWidget *intFieldGrpWidget = new QWidget;
     KxRowLayout *intFieldGrpRowLayout = new KxRowLayout(intFieldGrpWidget);
     intFieldGrpRowLayout->setNumColumns(2);
     groupFieldsColumnLayout->addWidget(intFieldGrpWidget);

     KxLabel *intFieldLabel1 = new KxLabel("intFieldGrp");
     KxIntFieldGrp *intFieldGrp1 = new KxIntFieldGrp;
     intFieldGrp1->addLabel();
     intFieldGrp1->setLabelText("intFieldGrp");
     intFieldGrp1->setColumnWidth(0,kLabelWidth);
     intFieldGrp1->setColumnAttachment(0,KxWidgetGrp::kBothHSides,0);
     intFieldGrp1->setColumnAlignment(0,Qt::AlignLeft | Qt::AlignVCenter);
     for(int i=1; i < 5; ++i){
         KxIntField *tempIntField = new KxIntField;
         intFieldGrp1->addField(tempIntField);
         intFieldGrp1->setColumnWidth(i,kFieldWidth);
         intFieldGrp1->setColumnAttachment(i,KxWidgetGrp::kBothHSides,0);
         intFieldGrp1->setColumnAlignment(i,Qt::AlignLeft | Qt::AlignVCenter);
     }
     intFieldGrpRowLayout->addWidget(intFieldLabel1);
     intFieldGrpRowLayout->addWidget(intFieldGrp1);

     //textfield grp 
     QWidget *textFieldGrpWidget = new QWidget;
     KxRowLayout *textFieldGrpRowLayout =new KxRowLayout(textFieldGrpWidget);
     textFieldGrpRowLayout->setNumColumns(2);
     groupFieldsColumnLayout->addWidget(textFieldGrpWidget);

     KxLabel *textFieldGrpLabel1 = new KxLabel("textFieldGrp");
     KxTextFieldGrp *textFieldGrp1 = new KxTextFieldGrp(NULL,true);
     textFieldGrp1->kxField()->setText("left");
     KxTextFieldGrp *textFieldGrp2 = new KxTextFieldGrp(NULL,true);
     textFieldGrp2->kxField()->setText("middle");
     KxTextFieldGrp *textFieldGrp3 = new KxTextFieldGrp(NULL,true);
     textFieldGrp3->kxField()->setText("right");

     textFieldGrpRowLayout->addWidget(textFieldGrpLabel1);
     textFieldGrpRowLayout->addWidget(textFieldGrp1);
     textFieldGrpRowLayout->addWidget(textFieldGrp2);
     textFieldGrpRowLayout->addWidget(textFieldGrp3);


     //textFieldButtonGrp 
    QWidget *textFieldButtonGrpWidget = new QWidget;
    KxRowLayout *textFieldButtonGrpRowLayout = new KxRowLayout(textFieldButtonGrpWidget);
    groupFieldsColumnLayout->addWidget(textFieldButtonGrpWidget);

    KxLabel *textFieldButtonGrpLabel = new KxLabel("textFieldButtonGrp");
    KxTextFieldGrp *textFieldButtonGrp1 = new KxTextFieldGrp;
    QPushButton *textFieldButton1 = new QPushButton("T1");
    textFieldButtonGrp1->addWidgetToGrp(textFieldButton1);
    textFieldButtonGrp1->kxField()->setText("above");

    KxTextFieldGrp* textFieldButtonGrp2 = new KxTextFieldGrp;
    QPushButton *textFieldButton2 = new QPushButton("T2");
    textFieldButtonGrp2->addWidgetToGrp(textFieldButton2);
    textFieldButtonGrp2->kxField()->setText("below");

    textFieldButtonGrpRowLayout->addWidget(textFieldButtonGrpLabel);
    textFieldButtonGrpRowLayout->addWidget(textFieldButtonGrp1);
    textFieldButtonGrpRowLayout->addWidget(textFieldButtonGrp2);
    
    //checkboxGrp 
    QWidget *checkBoxGrpWidget = new QWidget;
    KxRowLayout *checkBoxGrpRowLayout  =new KxRowLayout(checkBoxGrpWidget);
    groupFieldsColumnLayout->addWidget(checkBoxGrpWidget);

    KxLabel * checkBoxGrpLabel = new KxLabel("checkBoxGrp");
    KxCheckBoxGrp *checkBoxGrp1 =new KxCheckBoxGrp(NULL,false,3);
    KxCheckBox *checkBoxArray[4];
    checkBoxGrp1->getFields(checkBoxArray[0],checkBoxArray[1],checkBoxArray[2],checkBoxArray[3]);
    checkBoxArray[0]->setCheckState(Qt::Checked);
    checkBoxArray[0]->setText("on");
    checkBoxArray[1]->setText("off");
    checkBoxArray[2]->setText("on");
    checkBoxArray[2]->setChecked(true);

    checkBoxGrpRowLayout->addWidget(checkBoxGrpLabel);
    checkBoxGrpRowLayout->addWidget(checkBoxGrp1);

    //   symbolcheckbox 
    QWidget *symbolCheckBoxWidget = new QWidget;
    KxRowLayout *symbolCheckBoxRowLayout = new KxRowLayout(symbolCheckBoxWidget);
    groupFieldsColumnLayout->addWidget(symbolCheckBoxWidget);
    KxLabel *symbolCheckBoxLabel = new KxLabel("symbolCheckBox");
    KxSymbolCheckBox *symbolCheckBox1 = new KxSymbolCheckBox;
    symbolCheckBox1->iconTextControl()->setStyle(KxIconTextControl::iconOnly);
    symbolCheckBox1->iconTextControl()->setIcon(":/sphere.png");
    symbolCheckBox1->iconTextControl()->setSelectionIcon(":/circle.png");
    symbolCheckBoxRowLayout->addWidget(symbolCheckBoxLabel);
    symbolCheckBoxRowLayout->addWidget(symbolCheckBox1);

    //shelfButton 
    QWidget *shelfButtonWidget = new QWidget;
    KxRowLayout *shelfButtonRowLayout = new KxRowLayout(shelfButtonWidget);
    groupFieldsColumnLayout->addWidget(shelfButtonWidget);
    KxLabel *shelfButtonLabel = new KxLabel("shelfButton");
    KxShelfButton *shelfButton1 =new KxShelfButton;
    shelfButton1->iconTextControl()->setStyle(KxIconTextControl::iconAndTextVertical);
    shelfButton1->iconTextControl()->setIcon(":/circle.png");
    shelfButton1->iconTextControl()->setText("circle");

    KxShelfButton *shelfButton2 =new KxShelfButton;
    shelfButton2->iconTextControl()->setStyle(KxIconTextControl::iconAndTextVertical);
    shelfButton2->iconTextControl()->setIcon(":/sphere.png");
    shelfButton2->iconTextControl()->setText("sphere");

    shelfButtonRowLayout->addWidget(shelfButtonLabel);
    shelfButtonRowLayout->addWidget(shelfButton1);
    shelfButtonRowLayout->addWidget(shelfButton2);

    //toolbutton
    QWidget *toolButtonWidget =  new QWidget;
    KxRowLayout *toolButtonRowLayout =new KxRowLayout(toolButtonWidget);
    groupFieldsColumnLayout->addWidget(toolButtonWidget);

    KxLabel *toolButtonLabel =new KxLabel("toolButton");
    KxToolRadioButton *toolButton1 = new KxToolRadioButton;
    toolButton1->setIcon(QIcon(":/sphere.png"));
    KxToolRadioButton * toolButton2 = new KxToolRadioButton;
    toolButton2->setIcon(QIcon(":/circle.png"));
    KxButtonGroup *toolButtonCluster = new KxButtonGroup;
    toolButtonCluster->addButton(toolButton1);
    toolButtonCluster->addButton(toolButton2);

    toolButtonRowLayout->addWidget(toolButtonLabel);
    toolButtonRowLayout->addWidget(toolButton1);
    toolButtonRowLayout->addWidget(toolButton2);

    
    // add the misc tabs 
    QWidget *tableWidget = new QWidget;
    KxColumnLayout * tableColumnLayout =new KxColumnLayout(tableWidget);
    fTabLayout->addWidget(tableWidget);

    //picture 
    QWidget *pictureWidget = new QWidget;
    KxRowLayout *pictureRowLayout =new KxRowLayout(pictureWidget);
    tableColumnLayout->addWidget(pictureWidget);
    KxLabel *pictureLabel = new KxLabel("picture");
    KxStaticPicture *picture1 = new KxStaticPicture;
    picture1->setPicture(":/test.png");

    pictureRowLayout->addWidget(pictureLabel);
    pictureRowLayout->addWidget(picture1);

    //table 


    //tree 
    


    fTabLayout->setTabLabel(1,"Basic Controls");
    fTabLayout->setTabLabel(2,"Layouts ...");
    fTabLayout->setTabLabel(3,"icontext controls");
    fTabLayout->setTabLabel(4,"Group Controls");
    fTabLayout->setTabLabel(5,"Group and Fields");
    fTabLayout->setTabLabel(6,"Table Trees ");
    fTabLayout->setTabLabel(7,"3D controls");
    fTabLayout->setTabLabel(0,"property controls");

    fTabLayout->selectWidget(7);
}

void KxUITest::testPropertyWidgets()
{
    QWidget* pageWidget = new QWidget;
    fTabLayout->addWidget(pageWidget);

    // The main layout 
    KxRowLayout* rowLayout = new KxRowLayout(pageWidget);
    rowLayout->setNumColumns(2);
    rowLayout->setColumnAlignment(0, Qt::AlignLeft | Qt::AlignTop);
    rowLayout->setColumnAlignment(1, Qt::AlignLeft | Qt::AlignTop);
    rowLayout->setColumnAdjustable(0);
    rowLayout->setColumnAdjustable(1);
    QWidget* rowWidget;

    // The left pane.
    rowWidget = new QWidget();
    rowLayout->addWidget(rowWidget);

    KxColumnLayout* columnLayout = new KxColumnLayout(rowWidget);
    rowWidget->setLayout(columnLayout);
    columnLayout->setColumnAdjustable(true);

    QWidget* frameWidget = new QWidget;
    columnLayout->addWidget(frameWidget);

    KxFrameLayout* frame = new KxFrameLayout(frameWidget);
    frame->setTitle(tr("Basic Property"));
    frame->setCollapsible(true);

    // Frame content
    QWidget* frameContentWidget = new QWidget;
    frame->addWidget(frameContentWidget);
    QFormLayout* formLayout = new QFormLayout(frameContentWidget);
    formLayout->setSpacing(2);
    
    QWidget* propertyWidget;
    propertyWidget = _propertyBool.createQtWidget(QString("Bool"));
    formLayout->addRow(new KxLabel(QString("Bool:")), propertyWidget); 
    _propertyEnum.bindQtWidget(propertyWidget);
    propertyWidget = _propertyFloat.createQtWidget(QString("Float"));
    formLayout->addRow(new KxLabel(QString("Float:")), propertyWidget); 
    propertyWidget = _propertyInt.createQtWidget(QString("Int"));
    formLayout->addRow(new KxLabel(QString("Int:")), propertyWidget); 
    propertyWidget = _propertyUint.createQtWidget(QString("Uint"));
    formLayout->addRow(new KxLabel(QString("Uint:")), propertyWidget); 
    propertyWidget = _propertyString.createQtWidget(QString("String"));
    formLayout->addRow(new KxLabel(QString("String:")), propertyWidget); 
    propertyWidget = _propertyEnum.createQtWidget(QString("Enum"));
    formLayout->addRow(new KxLabel(QString("Enum:")), propertyWidget); 
    _propertyEnum.bindQtWidget(propertyWidget);
    
    QWidget* colorWidget = _propertyColor.createQtWidget(QString("Color"));
    columnLayout->addWidget(colorWidget);

    QWidget* srtMatrixWidget = _propertySRTMatrix.createQtWidget(QString("SRTMatrix"));
    columnLayout->addWidget(srtMatrixWidget);
    

    // The right pane.
    rowWidget = new QWidget();
    rowLayout->addWidget(rowWidget);

    frame = new KxFrameLayout(rowWidget);
    frame->setTitle(tr("Content"));
    frame->setCollapsible(true);

    frameContentWidget = new QWidget;
    frame->addWidget(frameContentWidget);
    formLayout = new QFormLayout(frameContentWidget);
    formLayout->setSpacing(2);

    _boolValueLabel = new KxLabel;
    _floatValueLabel = new KxLabel;
    _intValueLabel = new KxLabel;
    _uintValueLabel = new KxLabel;
    _stringValueLabel = new KxLabel;
    _srtmatrixValueLabel = new KxLabel;
    _colorValueLabel = new KxLabel;
    _enumValueLabel = new KxLabel;
    
    formLayout->addRow(new KxLabel(QString("Bool:")), _boolValueLabel); 
    formLayout->addRow(new KxLabel(QString("Float:")), _floatValueLabel); 
    formLayout->addRow(new KxLabel(QString("Int:")), _intValueLabel); 
    formLayout->addRow(new KxLabel(QString("Uint:")), _uintValueLabel); 
    formLayout->addRow(new KxLabel(QString("String:")), _stringValueLabel); 
    formLayout->addRow(new KxLabel(QString("SRTMatrix:")), _srtmatrixValueLabel); 
    formLayout->addRow(new KxLabel(QString("Color:")), _colorValueLabel); 
    formLayout->addRow(new KxLabel(QString("Enum:")), _enumValueLabel); 

    startTimer(500);
}

void KxUITest::showText(const QString &text)
{
    KxMessageBox messagebox(KxMessageBox::Information,"tips",text);
    messagebox.exec();
}

void KxUITest::qtDisableAll()
{
   showText("disable all");
}

void KxUITest::qtEnableAll()
{
    showText("enable all");
}

KxUITest::~KxUITest()
{

}
    
void KxUITest::timerEvent(QTimerEvent* event)
{
    if (_propertyBool.getBool())
    {
        _boolValueLabel->setText(tr("Yes"));
    }
    else
    {
        _boolValueLabel->setText(tr("No"));
    }

    QString value;

    value = QString("%1").arg(_propertyInt.getInt());
    _intValueLabel->setText(value);
    
    value = QString("%1").arg(_propertyUint.getUint());
    _uintValueLabel->setText(value);
    
    value = QString("%1").arg(_propertyFloat.getFloat());
    _floatValueLabel->setText(value);
    
    _stringValueLabel->setText(QString(_propertyString.getString().c_str()));

    value = QString("%1 %2 %3 %4 %5 %6 %7 %8 %9")
        .arg(_propertySRTMatrix.getTranslationX(), 4)
        .arg(_propertySRTMatrix.getTranslationY(), 4)
        .arg(_propertySRTMatrix.getTranslationZ(), 4)
        
        .arg(kwRadianToDegree(_propertySRTMatrix.getRotationX()), 4)
        .arg(kwRadianToDegree(_propertySRTMatrix.getRotationY()), 4)
        .arg(kwRadianToDegree(_propertySRTMatrix.getRotationZ()), 4)
        
        .arg(_propertySRTMatrix.getScalingX(), 4)
        .arg(_propertySRTMatrix.getScalingY(), 4)
        .arg(_propertySRTMatrix.getScalingZ(), 4);
    _srtmatrixValueLabel->setText(value);
    
    value = QString("%1 %2 %3 %4")
        .arg(_propertyColor.getR(), 4)
        .arg(_propertyColor.getG(), 4)
        .arg(_propertyColor.getB(), 4)
        .arg(_propertyColor.getA(), 4);
    _colorValueLabel->setText(value);
    
    value = QString(_propertyEnum.getEnumName(_propertyEnum.getEnum()));
    _enumValueLabel->setText(value);

}
