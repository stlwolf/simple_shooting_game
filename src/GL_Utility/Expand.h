
#if !defined(___EXPAND___H)
#define ___EXPAND___H

/********************************************************
	インクルードファイル
********************************************************/

#include "../Winsprog/main.h"
#include "glext.h"

/********************************************************
	マクロ定義
********************************************************/

/********************************************************
	グローバル変数宣言
********************************************************/

extern PFNGLARRAYELEMENTEXTPROC  wglSwapIntervalEXT;

extern PFNGLGENBUFFERSARBPROC    glGenBuffersARB;		// バッファ管理番号作成
extern PFNGLBINDBUFFERARBPROC    glBindBufferARB;		// バッファ管理番号選択
extern PFNGLBUFFERDATAARBPROC    glBufferDataARB;		// データ読み込み
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;		// バッファ削除

/********************************************************
	関数プロトタイプ宣言
********************************************************/

int Get_GLprocs( void );

#endif	// #if !defined(___EXPAND___H)
