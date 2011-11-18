#if !defined( __MAIN__H)
#define __MAIN__H

/**********************************************************
	インクルードヘッダ
**********************************************************/

// Ｃ言語関連ヘッダー
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <conio.h>
#include <math.h>
#include <time.h>
#include <mbstring.h>

// ウインドウプログラム関連ヘッダー
#include <windows.h>

#	pragma comment ( lib, "winmm.lib" )

// OpenGL関連ヘッダー
//#if GLUT
//	#include <GL/glut.h>							// GLUT宣言 
//#else
	#include <GL/gl.h>								// OpenGLヘッダファイル
	#include <GL/glu.h>
	#pragma comment ( lib, "opengl32.lib" )			// link with Microsoft OpenGL lib
	#pragma comment ( lib, "glu32.lib" )			// link with Microsoft OpenGL Utility lib
//#endif

#include "../GL_Utility/glext.h"

// 文字出力用設定
#include "../GL_Utility/ttftext.h"
#pragma comment( lib, "ttftext_r.lib" )

// ゲームプログラムヘッダー
#include "Input.h"
#include "MatrixCal.h"

#include "../Gameprog/GameMain.h"

/**********************************************************
	グローバル変数定義
**********************************************************/

extern HDC		hDC;								// デバイスコンテキスト
extern HGLRC	hRC;								// OpenGLデバイスコンテキスト
extern HWND		hWnd;								// ウインドウハンドル

extern int g_CxFrameSize;							// ウインドウ枠の幅
extern int g_CyFrameSize;							// ウインドウ枠の高さ

extern int g_CyCaptionSize;							// タイトルバーの高さ

extern int g_CxScreenSize;							// デスクトップの幅
extern int g_CyScreenSize;							// デスクトップの高さ

extern char OBJFileName[2][20];						// オブジェクトファイル名

/**********************************************************
	定数定義
**********************************************************/

#define PI	  (3.1415926535897932384626433832795)	// 円周率
#define FPS	  (1000 / 60)							// FPS値

#define WHITE (1 << 0)
#define RED   (1 << 1)
#define GREEN (1 << 2)
#define BULE  (1 << 3)

/**********************************************************
	プロトタイプ宣言
**********************************************************/

extern int APIENTRY WinMain( HINSTANCE, HINSTANCE, LPSTR, int );
extern LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );
extern void Reshape_Window( int ,int );

extern HWND Init_Window( char *, int, int, int, int );
extern int Init_GLWindow( void );
extern int End_Win( void );

#endif