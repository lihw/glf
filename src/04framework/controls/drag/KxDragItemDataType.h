#ifndef KxDragItemDataType_H_
#define KxDragItemDataType_H_

typedef int KxDragItemDataType;

#define kAppToSysBaseTypeNum	10000
#define kUnknownSystemType		 9999

// External or System Types that we recognize
#define 	kTextDragType				kAppToSysBaseTypeNum
#define		kFileNameDragType	  		kTextDragType + 1
#define		kLastExternalDragType  		kFileNameDragType

// Internal Types
#define    	kPrimitiveDragType			kLastExternalDragType + 1

#endif