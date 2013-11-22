#ifndef KxBasicCursors_H_
#define KxBasicCursors_H_

#include "../base/HQtGui.h"
#include "KxCursorShape.h"
//
//
QString leftArrow = "leftArrow.png,11,9";
QString eyeDropper = "colorPickCursor.png,3,30";
QString question	= "question.png,16,25";
QString crossHair = "defaultCrossHair.png,16,16";
QString dubCrossHair = "defaultDoubleCrossHair.png,16,16";
QString iBeam = "textBeam.png,16,16";
QString hand = "defaultHand.png,14,16";
QString inv = "invisibleCursor.png,1,1";
QString lasso = "lasso.png,11,21";
QString busy = "waitBusy.png,16,16";
QString rmb = "rmbMenu.png,7,5";
//
const KxCursorShape kDefaultCursor					(leftArrow);
const KxCursorShape kDefaultCrossHairCursor			(crossHair);
const KxCursorShape kDefaultDoubleCrossHairCursor	(dubCrossHair);
const KxCursorShape kDefaultIBeamCursor				(iBeam);
const KxCursorShape kDefaultQuestionArrowCursor		(question);
const KxCursorShape kDefaultHandCursor				(hand);
const KxCursorShape kInvisibleCursor					(inv);
const KxCursorShape kDefaultEyeDropperCursor			(eyeDropper);
const KxCursorShape kLassoCursor						(lasso);
const KxCursorShape kRMBMenuCursor					(rmb);

#endif