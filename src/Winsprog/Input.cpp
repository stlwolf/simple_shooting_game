
/*********************************************************
	インクルードファイル
*********************************************************/

#include "main.h"						// アプリケーション全体に関る定義
#include "Input.h"

/**********************************************************
	グローバル変数
**********************************************************/

int g_nKeyData     = 0;											// リアルタイムなキー情報
int g_nKeyEdgeData = 0;											// キーのエッジ情報

int naKeyData[][2] = {
	{ VK_NUMPAD8, KEY_UP      },	// 上方向（テンキー８）
	{ VK_NUMPAD2, KEY_DOWN    },	// 下方向（テンキー２）
	{ VK_NUMPAD4, KEY_LEFT    },	// 左方向（テンキー４）
	{ VK_NUMPAD6, KEY_RIGHT   },	// 右方向（テンキー６）
	{ VK_NUMPAD1, KEY_ROTL	  },	// テンキー１
	{ VK_NUMPAD3, KEY_ROTR    },	// テンキー３
	{        'Z', KEY_BUTTON1 },	// Ｚキー
	{		 'X', KEY_BUTTON2 },	// Ｘキー
	{  VK_RETURN, KEY_ENTER	  },	// ＥＮＴＥＲキー
	{  VK_ESCAPE, KEY_ESC	  },	// ＥＣＳキー
	{          0, 0           }		// whileループ終了チェック
};

/**********************************************************
	入力処理関数

	out	:	g_nKeyData		---	bit 0: Up			0x01
								bit 1: Down			0x02
								bit 2: Left			0x04
								bit 3: Right		0x08
								bit 4: Button 0		0x10
								bit 5: Button 1		0x20
	
	g_nKeyEdgeData	---	上記のエッジ情報

	　引数：なし
	戻り値：なし

	押されたキー情報を取得する関数。押されたキーから入力
	情報データを作り、更にそれからエッジデータを作成する。
**********************************************************/

void Input_Device( void )
{
	int	nKeyCode;												// GetAsyncKeyStateに渡す仮想キーコード
	int	nCount = 0;												// ループカウンタ兼naKeyData用index
	
	static int nOld = 0;										// 前回の出力データ
	
	nOld = g_nKeyData;											// 前回の出力データを覚えておく
	
	g_nKeyData = 0;												// 今回の出力データを初期化
	
	while ( nKeyCode = naKeyData[nCount][0] ) {					// Key Dataが0になるまでループ
		
		if ( GetAsyncKeyState(nKeyCode) & 0x8000 ) {			// 最上位ビットが立っていればキーが押されている
			g_nKeyData |= naKeyData[nCount][1];					// 押されたキーに対応するビットを1にする
		}
		++nCount;												// 次のKey Dataに進める
	}
	g_nKeyEdgeData = ( g_nKeyData ^ nOld ) & g_nKeyData;		// エッジデータを作る
}
