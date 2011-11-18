
/**********************************************************
	インクルードヘッダー
**********************************************************/

#include "Gamemain.h"

/**********************************************************
	グローバル変数
**********************************************************/

// ＦＰＳ処理用変数
DWORD fpsChk1;
DWORD fpsChk2;

int fpsCnt;

float g_Stringx = -100.0f, g_Stringy = 120.0f;		   // 自機ターゲット数の文字表示位置
float g_eStringx = -100.0f, g_eStringy = -220.0f;      // 対戦相手ターゲット数の文字表示位置

OBJ_VARIABLE	*g_pObjData;

OBJ_VERTEX_BUF  *g_pObjBuf;
OBJ_VERTEX_BUF2 *g_pObjBuf2;

GLuint g_nVerbufNo[3];

GLUquadricObj  *globj;

// フラグ構造体変数
FLAG g_Flag;

TTF_TEXT g_Ttftext[6];								// 文字表示用TTF変数

/**********************************************************
	OpenGL初期化関数

	　引数：なし
	戻り値：処理成功時にはTRUEを、処理失敗時にはFALSEを返す。

	OpenGLを使用して描画を行うために必要な初期化を行う。
	更にpolデータをテキストデータから読み込んで数値データ
	に変換する。そのデータを元に各頂点データの領域を確保し、
	またオブジェクトの頂点情報を頂点バッファから読み込む
	ように設定する。
**********************************************************/

int Init_OpenGL( void )
{

	int ret;
	int nbufsize;

	ret = Get_GLprocs();													// 拡張機能取得関数

	if ( ret == FALSE ) {
		MessageBox( NULL, "OpenGL Expand Error", "Error", MB_OK );
		return FALSE;
	}

	wglSwapIntervalEXT(0);													// V-sync待ち解除

	glEnable( GL_DEPTH_TEST );

	globj = gluNewQuadric();

	// 指定したテキストファイルからオブジェクトデータを読み込む
	g_pObjData = Load_ObjFile( OBJFileName[0] );							// ポリゴンファイル読み込み関数

	if ( g_pObjData == NULL ) { return FALSE; }								// ポリゴンファイル読み込み失敗処理

	g_pObjBuf2 = Set_VertexBuf( g_pObjData );								// ポリゴンデータから頂点データを取得					

	if ( g_pObjBuf2 == NULL ) { return FALSE; }								// 頂点データ取得失敗

	Free_ObjIndex( g_pObjData );											// ポリゴンデータ解放
	g_pObjData = NULL;

	// 頂点バッファ設定
	glGenBuffersARB( 3, &g_nVerbufNo[0] );									// バッファ管理番号取得

	glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_nVerbufNo[0] );					// バッファ管理番号選択

	nbufsize = g_pObjBuf2->m_nObjsize;										// バッファサイズ代入

	glBufferDataARB( GL_ARRAY_BUFFER_ARB, nbufsize, g_pObjBuf2->pcObjData, GL_STATIC_DRAW_ARB );

	// 頂点配列　読み出しアドレス
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, g_nVerbufNo[0] );					// バッファ管理番号選択

	glInterleavedArrays( g_pObjBuf2->m_nPolmode, 0, NULL );					// 頂点バッファから描画

	glClearColor( 0.0f, 0.0f, 0.0f, 2.0f );									// 背景色クリア

	int i = 0;

	i = ttfTextSysInit( &hWnd );											// 文字表示ライブラリ初期化

	memset( &g_Ttftext, 0x00, sizeof(TTF_TEXT) );							// 文字表示構造体初期化
	
	for ( int i = 0; i < 6; i++ ) {
		ttfTextDefault( &g_Ttftext[i] );									// 文字表示用初期化関数
	}

	return TRUE;

}

/**********************************************************
	ゲーム初期化関数

	　引数：なし
	戻り値：処理成功時にTRUEを返す。

	各初期化関数、マップ・プレイヤー・ターゲットの初期化
	関数を実行する。フラグ構造体のフラグをリセット。
**********************************************************/

BOOL Init_Game( void )
{

	// マップ初期化関数
	Map_Init();

	// プレイヤー初期化関数
	Player_Init();

	// ターゲット初期化関数
	Target_Init();

	// オープニング・エンディングフラグを0に初期化
	g_Flag.m_OpeningFlag = 0;
	g_Flag.m_EndingFlag	 = 0;

	return TRUE;
}

/**********************************************************
	ゲームメイン関数

	　引数：表示を行うウィンドウのハンドル
	戻り値：処理成功時にはTRUEを返し、失敗にはFALSEを返す。

	ゲーム本体の関数。オープニング・エンディングフラグが
	１の場合、それぞれの関数を実行して表示を行う。
	それらの関数が実行されていなければ各オブジェクトの
	移動・描画処理関数を実行する。ここでＦＰＳ処理も行う。
**********************************************************/

BOOL Game_Main( HWND hWnd )
{

	int ctime;

	// エンディングフラグが1の時
	if ( g_Flag.m_EndingFlag == 1 ) {
				
		// エンディング画面を表示する
		if ( FALSE == Ending_Display() ) {
			// エンディング関数内でＥＳＣキーが押された場合
			return FALSE;
		}

		// エンディング画面表示後、フラグが0に設定されている場合
		// （エンディング関数内でＥＮＴＥＲキーが押された場合）
		if ( g_Flag.m_EndingFlag == 0 ) {

			// 画面の塗りつぶし
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

			// 再度、ゲーム初期化関数を実行する
			Init_Game();
	
			// ＦＰＳ処理を行うための再設定
			fpsChk1 = timeGetTime();
			fpsCnt = 0;
		
		}
		// エンディング関数内で何も押されない場合
		else { return TRUE; }

	}

	// オープニングフラグが初期値（0)の場合
	if ( g_Flag.m_OpeningFlag == 0 ) {
		
		// オープニング画面を表示する
		if ( FALSE == Opening_Display() ) {
			// オープニング関数内でＥＳＣキーが押された場合
			return FALSE;
		}

		// オープニング画面表示後フラグがそのままの場合関数を抜ける
		if ( g_Flag.m_OpeningFlag == 0 ) {
			return TRUE;
		}

	}

	// ＦＰＳ処理のための設定
	fpsChk2 = timeGetTime();
	ctime = (int)( fpsChk2 - fpsChk1 );

	// ＦＰＳ処理
	if ( ctime > FPS ) {
		
		// キー入力を取得する
		Input_Device();

		// 移動処理メイン関数
		Move_Main();

		// 移動処理等を設定時間以上かけて行っていなければ実行
		if ( ctime < ( FPS * 2 ) ) {

			WGL_Begin();								// OpenGL描画開始

			Display_Main();								// 描画処理メイン関数

			WGL_End();									// OpenGL描画終了

			fpsChk1 = fpsChk2;							// 描画後の時刻をfpsChk1に設定

		}
		else {
			fpsChk1 += FPS;
		}
			
	}

	// 移動計算・表示等が全て終了した時点でターゲットが存在しなければエンディングフラグを立てる
	// 通信→相手のターゲット数が０の場合も追加
	if ( g_Ereamap.m_TargetNum == 0 || g_Ereamap.m_EnemyTargetNum == 0 ) {
		g_Flag.m_EndingFlag = 1;
	}

	// CPU使用率を下げる
	SleepEx( 1, TRUE );									// １ミリ秒

	return TRUE;

}

/**********************************************************
	移動処理メイン関数

	引数：なし　戻り値：なし

	各オブジェクトの移動処理関連の関数を実行する。
	現段階ではプレイヤーとプレイヤー弾の移動処理関数を
	実行する。
**********************************************************/

void Move_Main( void )
{

	Player_Move();
	PAttack_Move();

}

/**********************************************************
	メイン描画処理関数

	引数：なし　戻り値：なし

	各オブジェクトの描画処理関連の関数を実行する。
	プレイヤー・プレイヤー弾・ターゲット・ワイヤー壁の描画
	と残りターゲット数の文字を表示している。
**********************************************************/

void Display_Main( void )
{

	// ライト関連設定
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	// ディスプレイバッファをクリア
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// プレイヤー描画関数
	Player_Display();

	// プレイヤー弾描画関数
	PAttack_Display();

	// ターゲット描画関数
	Target_Display();

	glDisable( GL_LIGHTING );

	// 文字表示用初期化関数
	//ttfTextDefault( &g_Ttftext[5] );

	// 残りターゲット数の文字表示を行う
	ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_HEIGHT, 25 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[5], "プレイヤー残りターゲット数　%d", g_Ereamap.m_TargetNum );
    ttfTextPrint     ( &g_Ttftext[5], g_Stringx, g_Stringy + 20 );

	ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_HEIGHT, 25 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[5], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[5], "対戦相手の残りターゲット数　%d", g_Ereamap.m_EnemyTargetNum );
    ttfTextPrint     ( &g_Ttftext[5], g_eStringx, g_eStringy - 20 );

	// ワイヤー壁描画関数
	WireWall_Disp( ( g_Ereamap.m_x * 2.0f ), 
				   ( g_Ereamap.m_y * 2.0f ), 
				   ( g_Ereamap.m_z * 2.0f ), 20.0 );

	glFlush();																// 強制描画
	SwapBuffers( hDC );														// バッファの切り替え

}

/**********************************************************
	オープニング関数

	　引数：なし
	戻り値：ＥＮＴＥＲキーが押されればTRUEを返し、ＥＳＣ
			キーが押されればFALSEを返す

	オープニング画面（文字）を表示する。
	ＥＮＴＥＲかＥＳＣキーが押されればそれぞれ結果を返す。
**********************************************************/

BOOL Opening_Display( void )
{

	// 文字拡大用変数
	TTF_SCALE ttfsc = { 2.0f, 2.0f };

	// キー入力取得
	Input_Device();

	WGL_Begin();														// OpenGL描画開始
	
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// オープニング文字表示
	ttfTextParameteri( &g_Ttftext[0], TTF_TEXT_HEIGHT, 40 );
    ttfTextParameteri( &g_Ttftext[0], TTF_TEXT_DISP_SIZE_X, 1024 );
    ttfTextParameteri( &g_Ttftext[0], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextFrontColor( &g_Ttftext[0], 0, 0, 255, 100 );
    ttfTextString    ( &g_Ttftext[0], "タマタマシューティングゲーム" );
    ttfTextPrint     ( &g_Ttftext[0], g_Stringx - 180, g_Stringy - 40 );

	ttfTextParameteri( &g_Ttftext[1], TTF_TEXT_HEIGHT, 35 );
    ttfTextParameteri( &g_Ttftext[1], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[1], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[1], "ＥＮＴＥＲキーでスタート" );
    ttfTextPrint     ( &g_Ttftext[1], g_Stringx - 110, g_Stringy - 200 );

	glFlush();															// 強制描画
	SwapBuffers( hDC );													// バッファの切り替え

	WGL_End();															// OpenGL描画終了

	// ＥＮＴＥＲキーが押されていれば実行
	if ( g_nKeyData & KEY_ENTER ) {
		// オープニングフラグを1に設定後、ＦＰＳ処理のための値を再設定
		g_Flag.m_OpeningFlag = 1;
		fpsChk1 = timeGetTime();
		fpsCnt = 0;
		return TRUE;
	}
	// ＥＳＣキーが押されると実行
	if ( g_nKeyData & KEY_ESC ) {
		return FALSE;
	}

	return TRUE;

}

/**********************************************************
	エンディング関数

	　引数：なし
	戻り値：処理が通常通り終了すればTRUEを返し、ＥＳＣ
			キーが押されればFALSEを返す

	エンディング画面（文字）を表示する。
	通常通り実行されればTRUEを返すが、途中でＥＮＴＥＲキー
	が押されていた場合はエンディングフラグを0にする。
	また、ＥＳＣキーが押されればFALSEを返す。
**********************************************************/

BOOL Ending_Display( void )
{

	// キー入力取得
	Input_Device();

	WGL_Begin();														// OpenGL描画開始

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// エンディング文字表示
	ttfTextParameteri( &g_Ttftext[2], TTF_TEXT_HEIGHT, 60 );
    ttfTextParameteri( &g_Ttftext[2], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[2], TTF_TEXT_DISP_SIZE_Y, 64 );

	if ( g_Ereamap.m_TargetNum == 0 ) {

		ttfTextString( &g_Ttftext[2], "あなたの勝利です" );
		ttfTextPrint( &g_Ttftext[2], g_Stringx - 120, g_Stringy - 20 );

	}
	else if ( g_Ereamap.m_EnemyTargetNum == 0 )	{

		ttfTextString( &g_Ttftext[2], "あなたの敗北です" );
		ttfTextPrint( &g_Ttftext[2], g_Stringx - 120, g_Stringy - 20 );

	}
	else if ( g_Ereamap.m_EnemyTargetNum == 99 && g_Ereamap.m_TargetNum == 0 ) {
		
		ttfTextString( &g_Ttftext[2], "ゲーム終了" );
		ttfTextPrint( &g_Ttftext[2], g_Stringx - 60, g_Stringy - 20 );
	
	}

	ttfTextParameteri( &g_Ttftext[3], TTF_TEXT_HEIGHT, 30 );
    ttfTextParameteri( &g_Ttftext[3], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[3], TTF_TEXT_DISP_SIZE_Y, 64 );	
	ttfTextString    ( &g_Ttftext[3], "ＥＮＴＥＲキーで再スタート" );
    ttfTextPrint     ( &g_Ttftext[3], g_Stringx - 80, g_Stringy - 180 );

	ttfTextParameteri( &g_Ttftext[4], TTF_TEXT_HEIGHT, 30 );
    ttfTextParameteri( &g_Ttftext[4], TTF_TEXT_DISP_SIZE_X, 512 );
    ttfTextParameteri( &g_Ttftext[4], TTF_TEXT_DISP_SIZE_Y, 64 );
	ttfTextString    ( &g_Ttftext[4], "ＥＳＣキーで終了" );
    ttfTextPrint     ( &g_Ttftext[4], g_Stringx - 30, g_Stringy - 230 );

	glFlush();															// 強制描画
	SwapBuffers( hDC );													// バッファの切り替え

	WGL_End();															// OpenGL描画終了

	// ＥＮＴＥＲキーが押されれば実行
	if ( g_nKeyData & KEY_ENTER ) {
		// エンディングフラグを0に設定
		g_Flag.m_EndingFlag = 0;
	}
	// ＥＳＣキーが押されれば実行
	if ( g_nKeyData & KEY_ESC ) {
		return FALSE;
	}

	return TRUE;

}

/**********************************************************
	OpenGL描画初期化処理関数

	　引数：なし
	戻り値；処理が成功すればTRUEを返し、失敗時にはFALSEを
			返す。

	OpenGLを使って描画するための準備を行う関数。デバイス
	コンテキストとレンタリングコンテキストを指定してそれ
	を結びつける。
**********************************************************/

int WGL_Begin()
{

	// デバイスコンテキストを取得
	hDC = GetDC( hWnd );

	if ( hDC == 0 ) { return FALSE; }

	// レンタリングコンテキストの指定
	wglMakeCurrent( hDC, hRC );

	return TRUE;

}

/**********************************************************
	OpenGL描画終了時処理関数

	　引数：なし
	戻り値；処理が成功すればTRUEを返し、失敗時にはFALSEを
			返す。

	OpenGLを使った描画が終了した時に実行する関数。
	デバイスコンテキストとレンタリングコンテキストの
	結びつけを解除する。
**********************************************************/

int WGL_End()
{

	if ( hDC == 0 ) { return FALSE; }

	// レンタリングコンテキストを解放
	wglMakeCurrent( NULL, NULL );

	// デバイスコンテキストを解放
	ReleaseDC( hWnd, hDC );
	hDC = 0;

	return TRUE;

}
