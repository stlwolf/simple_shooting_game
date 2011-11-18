#if !defined(__OBJDATA__H)
#define __OBJDATA__H

/**********************************************************
	インクルードファイル
**********************************************************/

#include "../Winsprog/MatrixCal.h"

/**********************************************************
	オブジェクトデータ用構造体
**********************************************************/

// 頂点座標
typedef struct {
	float m_x, m_y, m_z;
} OBJ_VERTEX;

// 法線座標
typedef struct {
	float m_x, m_y, m_z;
} OBJ_NORMAL;

// 頂点カラー
typedef struct {
    float m_R, m_G, m_B, m_A;
} OBJ_COLOR;

// テクスチャ座標
typedef struct {
    float m_s, m_t;
} OBJ_TEXTURE;

// ポリゴン情報（１つのポリゴンが持つ各構成情報をインデックス番号で指定保持）
typedef struct {
    int   m_v[3];				// 頂点インデックス
    int   m_n[3];				// 法線インデックス
    int   m_c[3];				// カラーインデックス
    int   m_t[3];				// テクスチャインデックス
} OBJ_POLY;

// オブジェクトデータ構造体（１つのオブジェクトを構成する情報のアドレスを保持？）
typedef struct {
	int		    m_Polcnt;		// 構成ポリゴン数
	OBJ_VERTEX  *pObj_v;		// 頂点バッファ
	OBJ_NORMAL  *pObj_n;		// 法線バッファ
	OBJ_COLOR   *pObj_c;		// カラーバッファ
	OBJ_TEXTURE *pObj_t;		// テクスチャバッファ
	OBJ_POLY	*pObj_pol;		// ポリゴン構成情報バッファ
} OBJ_VARIABLE;

typedef struct {
	int		    m_Polcnt;		// 構成ポリゴン数
	OBJ_VERTEX  *pObj_v;		// 頂点バッファ
	OBJ_NORMAL  *pObj_n;		// 法線バッファ
	OBJ_COLOR   *pObj_c;		// カラーバッファ
	OBJ_TEXTURE *pObj_t;		// テクスチャバッファ
} OBJ_VERTEX_BUF;

typedef struct {
    int           m_nPolmode;	// ポリゴンのモード 
    int           m_nPolcnt;	// ポリゴン数
    int           m_nDatacnt;   // 頂点データ数
    int           m_nObjsize;	// バッファデータサイズ
    unsigned char *pcObjData;	// 頂点情報
} OBJ_VERTEX_BUF2;

typedef struct {
    union {
        struct {
            float m_x, m_y;
        };
        struct {
            float m_s, m_t;				// OpenGL=strq : GLSL=stpq
        };
        float   m_v[2];
    };
} VEC2;

typedef struct {
    union {
        struct {
            float m_x, m_y, m_z;
        };
        struct {
            float m_r, m_g, m_b;
        };
        struct {
            float m_s, m_t, m_p;		// OpenGL=strq : GLSL=stpq
        };
        float   m_v[3];
    };
} VEC3;

typedef struct {
    union {
        struct {
            float m_x, m_y, m_z, m_w;
        };
        struct {
            float m_r, m_g, m_b, m_a;
        };
        struct {
            float m_s, m_t, m_p, m_q;	// OpenGL=strq : GLSL=stpq
        };
        float   m_v[4];
    };
} VEC4;

typedef struct {
    unsigned char m_R, m_G, m_B, m_A;
} VEC4UB;

// 頂点配列フォーマット

typedef struct {
	VECTOR4 m_c;
    VECTOR3 m_n;
    VECTOR3 m_v;
} C4F_N3F_V3F;

#endif
