#ifndef KxExport_H_
#define KxExport_H_

//	Symbol Visibility On Windows
//	----------------------------
//
//	On Windows, global symbols must be marked for export when referenced by
//	code in the DLL where the symbol is defined, and marked for import when
//	referenced by code from other DLLs.
//
//	To make this work, each DLL has two preprocessor macros which are specific
//	to it.  One is only defined when source files which belong to that DLL are
//	compiled. It usually has a name ending in _DLL (e.g. MY_DLL).
//
//	The second macro is defined to export symbols when the first macro
//	is present and to import symbols when it isn't. It usually has a name
//	ending in _EXPORT (e.g. MY_EXPORT).
//
//	Each class which a DLL wants to export should be marked in its header
//	file with the second macro. E.g:
//
//		class MY_EXPORT MyClass
//		{ ... };
//
//	
//	Symbol Visibility On Linux & OS X
//	---------------------------------
//
//	By default, all global symbols on Linux and OS X are exported from the
//	shared libraries in which they are defined. However, a large number of
//	those symbols are never used outside of the shared lib. Exporting them
//	simply bloats the lib's symbol table and increases dynamic link times,
//
//	When AW_HIDE_SYMBOLS is defined, global symbols in shared libraries
//	will by default be marked 'hidden' meaning that they will only be
//	accessible from within the shared library in which they are defined and
//	will not be exported.
//
//	To have a global symbol exported, the DLL_PUBLIC macro must be used. E.g:
//
//		class DLL_PUBLIC MyClass
//		{ ... };
//
//	This would cause the shared lib to export all of 'MyClass's global symbols.
//
//	To prevent the export of specific symbols within an otherwise exported
//	class, the DLL_PRIVATE must be used. E.g:
//	
//		class DLL_PUBLIC MyClass
//		{
//		public:
//						void exportedFunc();
//			DLL_PRIVATE void nonExportedFunc();
//		};
//
//	'exportedFunc' will be exported because the class is marked DLL_PUBLIC,
//	but 'nonExportedFunc' will not be exported because it is explicitly marked
//	as DLL_PRIVATE.
//
//	When AW_HIDE_SYMBOLS is not defined, DLL_PUBLIC and DLL_PRIVATE become
//	empty macros and we get the default behaviour.
//
//	Unlike Windows, the macro defining a symbol's visibility does not change
//	depending upon whether you're compiling code which is in the same DLL
//	as the symbol. That means that we can simply mark each exported class
//	with DLL_PUBLIC rather than having a separate macro for each DLL.
//
//	However, the same source code is used for Windows builds and including both
//	DLL_PUBLIC and the Windows dll-specific macro quickly gets tiresome:
//
//		class DLL_PUBLIC MY_EXPORT MyClass
//		{ ... };
//
//	So we instead re-use the Windows macros on Linux and OS X by defining
//	them all to be the same as DLL_PUBLIC. That allows us to mark a class
//	for export simply by providing the dll-specific macro:
//
//		class MY_EXPORT MyClass
//		{ ... };
//
//	Note that on Windows we don't use anything equivalent to DLL_PRIVATE, so on
//	Windows builds that macro is defined to be empty.
//#define  OSWin_
//#define  Kx_DLL
//
//#ifndef DLL_PUBLIC
//# if defined(OSWin_)
//#   define DLL_PUBLIC                                        G
//#   define DLL_PRIVATE
//# else
//#   define DLL_PUBLIC __attribute__ ((visibility("default")))
//#   define DLL_PRIVATE __attribute__ ((visibility("hidden")))
//# endif
//#endif
//
//#if defined(OSWin_)
//#if defined(Kx_DLL)
//#define KX_DLLEXPORT _declspec(dllexport)
//#else 
////#define KX_DLLEXPORT _declspec(dllimport)
//#endif
//#else
//#define KX_DLLEXPORT DLL_PUBLIC
//#endif
#include <QtCore/qglobal.h>
//#define  OSWin_

//#ifdef KX_CONTROLS_LIB//
//# define KX_DLLEXPORT Q_DECL_EXPORT
//#else
//# define KX_DLLEXPORT Q_DECL_IMPORT
//#endif
#define KX_DLLEXPORT 

#endif
