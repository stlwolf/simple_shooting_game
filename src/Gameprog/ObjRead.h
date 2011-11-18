#if !defined(___OBJREAD___H)
#define ___OBJREAD___H

/********************************************************
	インクルードファイル
********************************************************/

#include "../winsprog/main.h"
#include "ObjData.h"

/********************************************************
	マクロ定義
********************************************************/

/********************************************************
	グローバル変数宣言
********************************************************/

extern char *g_pPolfile;					// ポリゴンファイルデータ

extern OBJ_VARIABLE	   *g_pObj;				// インデックス型　ポリゴンオブジェクト
extern OBJ_VERTEX_BUF  *g_pVerBuf;
extern OBJ_VERTEX_BUF2 *g_pVerBuf2;

/********************************************************
	関数プロトタイプ宣言
********************************************************/

extern OBJ_VARIABLE *Load_ObjFile( char * );
extern char *Load_TextFile( char *, int * );
extern int Load_VertexData( char *, int, OBJ_VARIABLE * );
extern int BufChk_String( char *, char *, int );

extern OBJ_VERTEX_BUF2 *Set_VertexBuf( OBJ_VARIABLE * );

extern int Obj_Error1( void );

extern void Free_ObjIndex( OBJ_VARIABLE * );
extern void Free_ObjVertex1( OBJ_VERTEX_BUF * );
extern void Free_ObjVertex2( OBJ_VERTEX_BUF2 * );


#endif	// #if !defined(___OBJREAD___H)
