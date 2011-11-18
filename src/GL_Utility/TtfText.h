/*----------------------------------------------------------------------*/
/*   Copyright (C) 2006 by DEA                                          */
/*   TrueTypeFont 文字出力 プログラム                                   */
/*                                                                      */
/*   Ver 1.0  : テクスチャを使用して描画                                */
/*----------------------------------------------------------------------*/
#ifndef _MY_TTF_TEXT_INCLUDE
#define _MY_TTF_TEXT_INCLUDE

#if defined __cplusplus
extern "C" {
#endif


#define TTF_MAX_STRING  256                     // 一度に表示できる最大文字数

//-----------------------------------------------------------------------------
// 構造体設定
typedef struct
{
    float x, y;
} TTF_SCALE;

typedef struct {
    float r, g, b, a;
} TTF_RGBA;

// フォント表示用構造体
typedef struct {
    int             Init_f;                     // 初期化フラグ

    SIZE            DispSize;                   // ユーザー設定表示サイズ

    GLuint          TexNo;                      // テクスチャ管理番号
    SIZE            ImgSize;                    // 画像のサイズ
    int             Image_f;                    // 文字画像作成フラグ      0:未作成  1:作成済み
    unsigned char   *Image;                     // 画像データ（文字を絵に変換したデータ）

    int             Front_f;                    // 文字表示フラグ
    TTF_RGBA        Front_rgba;                 // 文字色

    int             Back_f;                     // 背景表示フラグ
    TTF_RGBA        Back_rgba;                  // 背景色

    TTF_SCALE       Scale;                      // 拡大縮小値

    int             lFont_f;                    // 論理フォント作成フラグ  0:未作成  1:作成済み
    LOGFONT         lFont;                      // フォントの書式
    HFONT           hFont;                      // 論理フォント

    char            string[TTF_MAX_STRING+1];   // 表示文字（テキスト）
} TTF_TEXT;


//-----------------------------------------------------------------------------
// フォントデータ設定変更用
// ttfTextParameterv・ttfTextParameteri 関数で使用するコマンド
enum
{
    TTF_TEXT_DEF,               // デフォルト設定       v:i
    TTF_TEXT_FONT,              // 論理フォント作成     v:i

    TTF_TEXT_ALL,               // 全データ書き換え     v:

    TTF_TEXT_DISP_SIZE,         // サイズ指定           v:
    TTF_TEXT_DISP_SIZE_X,       // サイズ指定 横幅       :i
    TTF_TEXT_DISP_SIZE_Y,       // サイズ指定 縦幅       :i

    TTF_TEXT_SCALE,             // 拡大縮小             v:

    TTF_TEXT_BACK,              // 背景設定              :i
    TTF_TEXT_BACK_COLOR,        // 背景色設定           v:

    TTF_TEXT_FRONT,             // 文字設定              :i
    TTF_TEXT_FRONT_COLOR,       // 文字色設定           v:


    TTF_TEXT_HEIGHT,            // 文字高さ設定          :i
    TTF_TEXT_WIDTH,             // 文字幅設定            :i
    TTF_TEXT_WEIGHT,            // 文字の太さ設定        :i
    TTF_TEXT_ITALIC,            // イタリック文字設定    :i
    TTF_TEXT_UNDERLINE,         // 下線設定              :i
    TTF_TEXT_STRIKEOUT,         // 訂正線設定            :i
    TTF_TEXT_NAME,              // フォント名設定       v:
};


//-----------------------------------------------------------------------------
// プロトタイプ宣言

// フォントシステム初期化
extern  int ttfTextSysInit( HWND *hwnd );

// デフォルト値設定用
extern  int ttfTextDefault( TTF_TEXT *ttfText );

// フォントデータ設定変更用
extern  void ttfTextParameterv( TTF_TEXT *ttfText, int setcom, void *vp );
extern  void ttfTextParameteri( TTF_TEXT *ttfText, int setcom, int   vp );

// 文字色変更処理
extern  void ttfTextFrontColor( TTF_TEXT *ttfText, float r, float g, float b, float a );

// 背景色変更処理
extern  void ttfTextBackColor( TTF_TEXT *ttfText, float r, float g, float b, float a );

// 文字列設定用関数
extern  void ttfTextString( TTF_TEXT *ttfText, const char *fmt, ... );

// 表示用関数
extern  void ttfTextPrint( TTF_TEXT *ttfText, float kx, float ky );


#if defined __cplusplus
} // end of extern "C"
#endif

#endif