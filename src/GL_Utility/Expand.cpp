
/**********************************************************
	インクルードヘッダー
**********************************************************/

#include "Expand.h"

/**********************************************************
	グローバル変数定義
**********************************************************/

PFNGLARRAYELEMENTEXTPROC  wglSwapIntervalEXT	  = NULL;

PFNGLGENBUFFERSARBPROC    glGenBuffersARB       = NULL;		// バッファ管理番号作成
PFNGLBINDBUFFERARBPROC    glBindBufferARB       = NULL;		// バッファ管理番号選択
PFNGLBUFFERDATAARBPROC    glBufferDataARB       = NULL;		// データ読み込み
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB    = NULL;		// バッファ削除

/**********************************************************
	拡張機能取得関数

	　引数：なし
	戻り値：処理成功時にTRUEを返し、失敗時にはFALSEを返す。

	OpenGLの拡張関数をグラフッィクボードが使用可能ならば
	取得しTRUEを返す。もし対応していない場合はエラーを返す。
**********************************************************/

int Get_GLprocs()
{

	wglSwapIntervalEXT = (PFNGLARRAYELEMENTEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
	
	if ( !wglSwapIntervalEXT ) { return FALSE; }

    // 拡張関数設定
    glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)    wglGetProcAddress( "glGenBuffersARB" );
    glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)    wglGetProcAddress( "glBindBufferARB" );
    glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)    wglGetProcAddress( "glBufferDataARB" );
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress( "glDeleteBuffersARB" );
    
	if ( !glGenBuffersARB    ) { return FALSE; }
	if ( !glBindBufferARB    ) { return FALSE; }
	if ( !glBufferDataARB    ) { return FALSE; }
	if ( !glDeleteBuffersARB ) { return FALSE; }

    return TRUE;

}
