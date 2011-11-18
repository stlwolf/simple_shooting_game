
/**********************************************************
	インクルードファイル
**********************************************************/

#include "main.h"
#include "../Gameprog/Gamemain.h"

/**********************************************************
	グローバル変数定義
**********************************************************/

HDC		hDC;								// デバイスコンテキスト
HGLRC	hRC;								// OpenGLデバイスコンテキスト
HWND	hWnd;								// ウインドウハンドル

int g_CxFrameSize;							// ウインドウ枠の幅
int g_CyFrameSize;							// ウインドウ枠の高さ

int g_CyCaptionSize;						// タイトルバーの高さ

int g_CxScreenSize;							// デスクトップの幅
int g_CyScreenSize;							// デスクトップの高さ

char OBJFileName[2][20] = {					// オブジェクトファイル名
        "Dat/dat1.pol",
        "Dat/dat2.pol"
};

/**********************************************************
	メッセージプロシージャ関数

	第１引数：ウィンドウハンドル
	第２引数：ウィンドウメッセージ
	第３引数：副パラメータ
	第４引数：副パラメータ
	　戻り値：プロシージャで処理できないメッセージ処理の結果

	作成したウインドウに対して送られたメッセージを処理する。
	ＥＳＣキーが押された時の終了処理とウィンドウサイズが
	変更された時の処理を行う。
**********************************************************/

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg ) {

		case WM_PAINT:											// 再表示
			break;

		case WM_SIZE:											// サイズ変更
			
			WGL_Begin();										// OpenGL描画開始
			Reshape_Window( LOWORD(lParam), HIWORD(lParam) );	// ウインドウサイズ変更関数 
			WGL_End();											// OpenGL描画終了

			break;

		case WM_CHAR:
        
			switch ( wParam ) {
				
				case  27:										// ＥＳＣキーが押された時の処理
				case 'q':
				case 'Q':
				
					PostQuitMessage(0);
					break;

				case ' ':
					break;
			}
			break;

		case WM_CLOSE:
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );

}

/**********************************************************
	ウインドウサイズ変更関数

	第１引数：変更されたウインドウの横幅
	第２引数：変更されたウインドウの縦幅
	　戻り値：なし

	ウインドウサイズが変更された時に実行される関数。
	新しい横幅・縦幅を使って新しいプロジェクション行列を
	作成・設定する。
**********************************************************/

void Reshape_Window( int width, int height ) 
{

	float w = (float)width;								// 更新後のウインドウ横幅
	float h = (float)height;							// 更新後のウインドウ高さ

	glViewport( 0, 0, width, height );					// ビューポート設定

	glMatrixMode( GL_PROJECTION );						// プロジェクション行列に設定
	
	glLoadIdentity();									// 行列初期化
	gluPerspective( 45.0f, w / h, 1.0f, 500.0f );

	glMatrixMode( GL_MODELVIEW );						// モデルビュー行列に設定

	glLoadIdentity();									// 行列初期化

}

/**********************************************************
	ウインドウ初期化関数

	第１引数：タイトルバーに表示する文字列
	第２引数：作成するウインドウのＸ座標
	第３引数：作成するウインドウのＹ座標
	第４引数：作成するウインドウの横幅
	第５引数：作成するウインドウの高さ
	　戻り値：作成したウインドウのハンドル

	ウインドウズＡＰＩを使ってウインドウ本体を作成する。
	引数として渡せれた情報を元にサイズ等を決定して作成。
	登録処理の際に２重起動をしていないかチェックしている。
**********************************************************/

HWND Init_Window( char *title, int x, int y, int width, int height )
{

	WNDCLASSEX	wc;
	
	static HINSTANCE hInstance = 0;

	g_CxFrameSize		= GetSystemMetrics( SM_CXFRAME );				// ウインドウ枠の幅を取得
	g_CyFrameSize		= GetSystemMetrics( SM_CYFRAME );				// ウインドウ枠の高さを取得

	g_CyCaptionSize	= GetSystemMetrics( SM_CYCAPTION );					// タイトルバーの高さを取得

	g_CxScreenSize	= GetSystemMetrics( SM_CXSCREEN );					// デスクトップの幅を取得
	g_CyScreenSize	= GetSystemMetrics( SM_CYSCREEN );					// デスクトップの高さを取得

	// ウインドウインスタンスが０ならば登録処理（二重起動防止処理）
	if ( hInstance == 0 ) {

		hInstance		 = GetModuleHandle( NULL );						// インスタンスの取得

		wc.cbSize		 = sizeof(wc);									// ウインドウクラスのサイズ
		wc.style		 = CS_HREDRAW | CS_VREDRAW;						// ウインドウスタイル
		wc.lpfnWndProc	 = WindowProc;									// メッセージ処理関数の登録
		wc.cbClsExtra	 = 0;
		wc.cbWndExtra	 = 0;
		wc.hInstance	 = hInstance;									// インスタンスの登録
		wc.hIcon		 = LoadIcon( NULL, IDI_WINLOGO );				// 標準アイコン登録
		wc.hIconSm		 = LoadIcon( NULL, IDI_APPLICATION );			// 小さいアイコン登録
		wc.hCursor		 = LoadCursor( (HINSTANCE)NULL, IDC_ARROW );	// カーソル登録
		wc.hbrBackground = NULL;										// ウインドウ背景色
		wc.lpszMenuName	 = NULL;										// メニュー設定（NULLは使用しない）
		wc.lpszClassName = "OpenGL";									// ウインドウクラスの名前

		// ココがウインドウクラス登録処理
		if ( !RegisterClassEx( &wc ) ){
			
			// ウインドウクラス登録エラー時の表示
			MessageBox( NULL, "RegisterClass() Failed: Cannot register window class", "Error", MB_OK );
			return NULL;

		}
	}

	width  += ( g_CxFrameSize * 2 );										//　widthサイズ＋（フレーム枠の幅＊２）
	height += ( g_CyFrameSize * 2 ) + g_CyCaptionSize;						// heightサイズ＋（フレーム枠の高さ＊２）＋タイトルバー高さ

	// ウインドウ作成処理
	hWnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,								// 拡張ウインドウサイズ
						   "OpenGL",														// 登録されているクラス名
						    title,															// ウインドウ名（タイトルバー名）
						   WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,			// ウインドウスタイル
						   x,																// ウインドウ横方向位置
						   y,																// ウインドウ縦方向位置
						   width,															// ウインドウの幅
						   height,															// ウインドウの高さ
						   NULL,															// オーナーウインドウのハンドル
						   NULL,															// メニューハンドルまたは子ウインドウID
						   hInstance,														// ウインドウに関連付けるインスタンス
						   NULL																// 追加情報
						   );

	// ウインドウ作成が正常に処理されたかのチェック
	if ( hWnd == NULL ) {

		// ウインドウ作成エラー時の表示
		MessageBox( NULL, "CreateWindow() Failed: Cannot create a window.", "Error", MB_OK );
		return NULL;
	
	}

	return hWnd;

}

/**********************************************************
	OpenGL初期化関数

	　引数：なし
	戻り値：処理成功時にはTRUE（1)を返し、失敗時には0（FALSE）
			を返す。

	ウインドウ作成後にOpenGLを使用できるように初期化を行う
	関数。画面のピクセルフォーマットを取得してOpenGLが使用
	可能かどうか調べる。使用可ならばレンタリングコンテキスト
	を作成する。
**********************************************************/

int Init_GLWindow( void )
{

	int pf;

	PIXELFORMATDESCRIPTOR pfd;

    pfd.nSize           = sizeof(PIXELFORMATDESCRIPTOR);	// この構造体のサイズ
    pfd.nVersion        = 1;								// 構造体のバージョン
    pfd.dwFlags         = PFD_DRAW_TO_WINDOW				// ウィンドウに直接描画するときに指定
                        | PFD_SUPPORT_OPENGL				// OpenGLをサポート
                        | PFD_DOUBLEBUFFER;					// バッファ設定
															// PFD_DOUBLEBUFFER  : ダブルバッファ
															// NULL              : シングルバッファ
															//                     シングルバッファの時は
															//                     SwapBuffersをしてはいけない。
    pfd.iPixelType      = PFD_TYPE_RGBA;					// ピクセルデータのタイプを指定
    pfd.cColorBits      = 32;								// カラーバッファビット数
    pfd.cRedBits        = 8;								// RGBA の R ビット数
    pfd.cRedShift       = 0;								// R ビットシフト設定
    pfd.cGreenBits      = 8;								// RGBA の G ビット数
    pfd.cGreenShift     = 0;								// G ビットシフト設定
    pfd.cBlueBits       = 8;								// RGBA の B ビット数
    pfd.cBlueShift      = 0;								// B ビットシフト設定
    pfd.cAlphaBits      = 8;								// RGBA の A ビット数
    pfd.cAlphaShift     = 0;								// A ビットシフト設定
    pfd.cAccumBits      = 0;								// アキュムレーションバッファのビット数
    pfd.cAccumRedBits   = 0;								// アキュムレーションバッファの Rビット数
    pfd.cAccumGreenBits = 0;								// アキュムレーションバッファの Gビット数
    pfd.cAccumBlueBits  = 0;								// アキュムレーションバッファの Bビット数
    pfd.cAccumAlphaBits = 0;								// アキュムレーションバッファの Aビット数
    pfd.cDepthBits      = 32;								// ディプスバッファビット数
    pfd.cStencilBits    = 0;								// ステンシルバッファビット数
    pfd.cAuxBuffers     = 0;								// 補助バッファのビット数（未使用）
    pfd.iLayerType      = PFD_MAIN_PLANE;					// レイヤータイプ
    pfd.bReserved       = 0;								// オーバーレイとアンダーレイの数（未使用）
    pfd.dwLayerMask     = 0;								// レイヤマスク（未使用）
    pfd.dwVisibleMask   = 0;								// ビジブルマスク（未使用）
    pfd.dwDamageMask    = 0;								// ダメージマスク（未使用）

	hDC = GetDC( hWnd );									// デバイスコンテキスト取得

	pf	= ChoosePixelFormat( hDC, &pfd );					// ピクセルIDの取得

	if ( pf == 0 ) {
		MessageBox(NULL, "ChoosePixelFormat() Failed: Cannot find a suitable pixel format.", "Error", MB_OK);
        return 0;
	}

	// デバイスコンテキストにピクセルフォ－マットを設定する
	if ( SetPixelFormat( hDC, pf, &pfd ) == FALSE ) {
		MessageBox(NULL, "SetPixelFormat() Failed: Cannot set format specified.", "Error", MB_OK );
        return 0;
	}

	// ピクセルIDからフォーマット特性を取得
	DescribePixelFormat( hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd );

	// １６色または２５６色時の処理
	if ( pfd.dwFlags & PFD_NEED_PALETTE || pfd.iPixelType == PFD_TYPE_COLORINDEX ) {
		MessageBox( NULL, "256 Color un no", "Error", MB_OK );
		return 0;
	}

	hRC = wglCreateContext( hDC );				// レンタリングコンテキストの作成

	ReleaseDC( hWnd, hDC );						// デバイスコンテキストの解放

	return TRUE;

}
 
/**********************************************************
	終了処理関数

	　引数：なし
	戻り値：通常通り処理が終了すれば0が返る。

	ウインドウを削除する時に実行される関数。デバイス
	コンテキスト・レンタリングコンテキストの解放削除
	を行う。ウインドウも削除する。
**********************************************************/

int End_Win( void )
{

	wglMakeCurrent( NULL, NULL );				// レンタリングコンテキスト指定解除
	ReleaseDC( hWnd, hDC );						// デバイスコンテキストの解放
	wglDeleteContext( hRC );					// レンタリングコンテキスト削除
	DestroyWindow( hWnd );						// ウインドウ削除

	return 0;

}

/**********************************************************
	メイン関数
**********************************************************/

int APIENTRY WinMain( HINSTANCE hCurrentInst, HINSTANCE hPreviousInst, LPSTR lpszCmdLine, int nCmdShow )
{

	int ret;
	bool com;
	
	TIMECAPS tc;												// ＦＰＳ処理用タイム変数

	MSG	msg;													// ウインメッセージ処理用変数

	hWnd = Init_Window( "OpenGL Window", 0, 0, 640, 480 );		// ウインドウ初期化関数
	
	if ( hWnd == NULL ) {
		End_Win();												// 終了処理関数
		exit(1);												// エラー終了
	}

	ret = Init_GLWindow();										// OpenGL初期化関数

	if ( ret == 0 ) {
		End_Win();												// 終了処理関数
		exit(1);												// エラー終了
	}

	WGL_Begin();												// OpenGL描画開始
	Init_OpenGL();
	WGL_End();													// OpenGL描画終了

	srand( (unsigned)time( NULL ) );

	Init_Game();

	ShowWindow( hWnd, SW_SHOW );								// ウインドウの表示モード設定
	UpdateWindow( hWnd );										// ウインドウの表示

	// ＦＰＳ処理開始にあたっての初期化
	timeGetDevCaps( &tc, sizeof(TIMECAPS) );					// 実行可能な分解能取得
	timeBeginPeriod( tc.wPeriodMin );							// timeGettime()の精度を上げる

	fpsChk1 = timeGetTime();
	fpsCnt = 0;

	/* 通信の接続処理をこの間で行う */

	CNetClient* cnet = NULL;

	// インスタンスの作成
	cnet = new CNetClient();

	// 作成に成功すれば実行
	if ( cnet != NULL ) {
		
		// サーバへの接続用要求を行う
		com = cnet->ServerConnect();

		// 接続に失敗した場合、インスタンスも破棄
		if ( com == false ) {
			delete(cnet);
			cnet = NULL;
		}

	}

	/* ここまで */

	while( 1 ) {

		if ( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) == FALSE ) {

				if ( Game_Main( hWnd ) == FALSE ) {

					// 通信接続が成功してば終了処理（エンディング関数内でEscが押されたときはここ処理）
					if ( cnet != NULL ) {
						// 終了処理関数（スレッド関連）
						cnet->End();
						// デストラクタ内でソケットのクローズを行う
						delete(cnet);
					}

				DestroyWindow( hWnd );							// ウィンドウを破棄
				return 0;										// GameMain関数が正常終了しなかったら強制終了
			}

		}
		else {

			if ( GetMessage( &msg, NULL, 0, 0 ) == FALSE ) { break; }

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		
		}
	
	}

	timeEndPeriod( tc.wPeriodMin );								// timeGetTime()の精度を戻す

	End_Win();													// 終了処理関数

	/* 通信処理関連の後片付け */

	// 通信接続が成功してば終了処理
	if ( cnet != NULL ) {
	
		// 終了処理関数（スレッド関連）
		cnet->End();

		// デストラクタ内でソケットのクローズを行う
		delete(cnet);
	
	}

	/* ここまで */

	return 0;

}
