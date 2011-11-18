
/**********************************************************
	インクルードファイル
**********************************************************/

#include "ObjRead.h"

/**********************************************************
	グローバル変数定義
**********************************************************/

char *g_pObjfile;						// オブジェクトファイルデータ

OBJ_VARIABLE	*g_pObj;				// インデックス型　ポリゴンオブジェクト
OBJ_VERTEX_BUF	*g_pVerBuf;
OBJ_VERTEX_BUF2 *g_pVerBuf2;			// 頂点バッファ型　ポリゴンオブジェクト

/**********************************************************
	頂点バッファコピー関数

	　引数：polデータから読み込んだ各インデックス情報が
			格納されているOBJ_VARIABLE型変数のアドレス
	戻り値：各頂点情報が格納されたOBJ_VERTEX_BUF2型データ
			のアドレス

	polデータから取得した頂点インデックス情報をもとに
	オブジェクトの頂点データを設定する関数。各ポリゴンごと
	に頂点・法泉・頂点カラー・テクスチャ情報を取り出して
	OBJ_VERTEX_BUF2型データに格納した上で返す。
**********************************************************/

OBJ_VERTEX_BUF2 *Set_VertexBuf( OBJ_VARIABLE *pobj_data )
{

	int ndata_cnt;
	int nindex_no;

	OBJ_POLY *ppol_copy;				// ポリゴンのインデックス情報

	unsigned char *pcobj_copy;

	OBJ_VERTEX  *pobj_v0, *pobj_v1;		// 頂点情報
	OBJ_NORMAL  *pobj_n0, *pobj_n1;		// 法線情報
	OBJ_COLOR   *pobj_c0, *pobj_c1;		// 頂点カラー情報
	OBJ_TEXTURE *pobj_t0, *pobj_t1;		// テクスチャ情報

	// ポインタ初期化
	g_pObj	   = NULL;
	g_pVerBuf  = NULL;
	g_pVerBuf2 = NULL;

	// オブジェクト情報格納領域を取得、そして初期化
	g_pVerBuf2 = (OBJ_VERTEX_BUF2 *)malloc( sizeof(OBJ_VERTEX_BUF2 ) );
	memset( g_pVerBuf2, 0x00, sizeof(OBJ_VERTEX_BUF2) );

	// ndata_cntにオブジェクトの構成ポリゴン数＊３（頂点数）を代入
	ndata_cnt = pobj_data->m_Polcnt * 3;

	g_pVerBuf2->m_nPolmode = GL_C4F_N3F_V3F;					// 頂点配列フォーマット
	g_pVerBuf2->m_nPolcnt  = pobj_data->m_Polcnt;				// ポリゴン数
	g_pVerBuf2->m_nDatacnt = ndata_cnt;							// 頂点データ数
	g_pVerBuf2->m_nObjsize = sizeof(C4F_N3F_V3F) * ndata_cnt;	// バッファサイズ

	// 頂点データ領域確保
	g_pVerBuf2->pcObjData = (unsigned char *)malloc( g_pVerBuf2->m_nObjsize );

	pcobj_copy = g_pVerBuf2->pcObjData;							// 頂点データを格納する領域の先頭アドレスをコピー

	pobj_c1	= pobj_data->pObj_c;								// オブジェクトのカラー情報格納アドレスをコピー
	pobj_n1 = pobj_data->pObj_n;								// オブジェクトの法線情報格納アドレスをコピー
	pobj_v1 = pobj_data->pObj_v;								// オブジェクトの頂点座標格納アドレスをコピー

	ppol_copy = pobj_data->pObj_pol;							// ポリゴンインデックス格納場所の先頭アドレスをコピー

	// オブジェクト構成ポリゴン数分ループする
	for ( int loop1 = 0; loop1 < pobj_data->m_Polcnt; loop1++ ) {

		// ３頂点分ループする
		for ( int loop2 = 0; loop2 < 3; loop2++ ) {

			// カラー情報をコピー
			pobj_c0		= (OBJ_COLOR *)pcobj_copy;				// 頂点データが格納されている領域からカラー情報領域を取り出す
			nindex_no	= ppol_copy->m_c[loop2];				// カラーインデックス番号を取り出す
			*pobj_c0	= *( pobj_c1 + nindex_no );				// インデックス番号で指定された場所からカラー情報を取り出す
			pcobj_copy += sizeof(OBJ_COLOR);					

			// 法線情報をコピー
			pobj_n0		= (OBJ_NORMAL *)pcobj_copy;				// 頂点データが格納されている領域から法線情報領域を取り出す
			nindex_no	= ppol_copy->m_n[loop2];				// 法線情報インデックス番号を取り出す
			*pobj_n0	= *( pobj_n1 + nindex_no );				// インデックス番号で指定された場所から法線情報を取り出す
			pcobj_copy += sizeof(OBJ_NORMAL);

			// 頂点座標をコピー
			pobj_v0		= (OBJ_VERTEX *)pcobj_copy;				// 頂点データが格納されている領域から頂点座標領域を取り出す
			nindex_no	= ppol_copy->m_v[loop2];				// 頂点座標インデックス番号を取り出す
			*pobj_v0	= *( pobj_v1 + nindex_no );				// インデックス番号で指定された場所から頂点座標を取り出す
			pcobj_copy += sizeof(OBJ_VERTEX);
			
		}

		ppol_copy++;

	}

	return g_pVerBuf2;

}

/**********************************************************
	ポリゴンファイル読み込み関数

	　引数：読み込みを行うファイル名
	戻り値：テキストから取得した情報を持つOBJ_VARIABLE型の
			データのアドレス

	指定されたファイルのサイズ分のデータ読み込み領域を確保
	する。その領域に全インデックスデータを書き込む。
	確保された領域の先頭アドレスをOBJ_VARIABLE型のポインタ
	*g_pObjに書き込み、そのアドレスを返す。
**********************************************************/

OBJ_VARIABLE *Load_ObjFile( char *filename )
{

	int ret		   = TRUE;														// 結果
	int nfilesize  = 0;															// オブジェクトファイルサイズ

	// 各ポインタの初期化
	g_pObjfile = NULL;
	g_pObj	   = NULL;
	g_pVerBuf  = NULL;
	g_pVerBuf2 = NULL;
	
	// テキストデータの文字数分の領域を確保してその先頭アドレスを受け取る
	g_pObjfile = Load_TextFile( filename, &nfilesize );							// テキストデータ読み込み関数

	if ( g_pObjfile == NULL ) {
		Obj_Error1();
		return FALSE;
	}

	g_pObj = (OBJ_VARIABLE *)malloc( sizeof(OBJ_VARIABLE) );					// ポリゴンデータ領域確保

	if ( g_pObj == NULL ) {
		Obj_Error1();
		return FALSE;
	}

	memset( g_pObj, 0x00, sizeof(OBJ_VARIABLE) );								// g_pObjに確保した領域内を０で初期化

	// テキストから読み込んだポリゴンを構成する情報を取り出す（文字を数値として読み込む）
	ret = Load_VertexData( g_pObjfile, nfilesize, g_pObj );

	if ( ret == NULL ) {														// データ取り出し失敗
		Obj_Error1();
		return FALSE;
	}

	free( g_pObjfile );															// テキスト領域を開放する

	return g_pObj;

}

/**********************************************************
	テキストデータ読み込み関数

	第１引数：読み込むを行うファイル名
	第２引数：読み込んだファイルのサイズを格納する変数のアドレス
	　戻り値：ファイルの文字数分確保されたメモリ領域の
			　先頭アドレスを返す。
	
	オブジェクトファイルに書き込まれたデータ（文字列）を
	読み込んで、そのサイズ分の領域を確保する。その先頭
	アドレスを返す。
**********************************************************/

char *Load_TextFile( char *fname, int *filesize )
{

	char   *pcbuf;
	size_t fsize;

	FILE   *fp;

	g_pObj	  = NULL;
	g_pVerBuf = NULL;

	if ( ( fp = fopen( fname, "rt" ) ) == NULL ) { return FALSE; }

	// fseek関数を使ってファイルポインタを最後尾に持ってくる
	fseek( fp, 0, SEEK_END );
	
	// ftell関数を使ってファイルの先頭からファイルポインタの位置までの文字数を数える
	fsize = ftell( fp );
	
	// 上記で数えた文字数分の領域を確保
	pcbuf = (char *)malloc( fsize );

	if ( pcbuf == NULL ) {
		fclose( fp );
		return FALSE;
	}

	// ファイルポインタを先頭に戻す
	fseek( fp, 0, SEEK_SET );

	// fread関数を使ってfsize個のchar型の大きさの文字をpcbufに格納する
	// ついでにchar型で読み込んだ数を戻り値としてfsizeに返す
	fsize = fread( pcbuf, sizeof(char), fsize, fp );

	fclose( fp );

	// 読み込んだchar型の文字の数をfilesize（引数として与えらたアドレス）の中にセットする
	*filesize = (int)fsize;

	// テキストで検知された改行コードを\0コードに置き換える
	for ( int loop = 0; loop < *filesize; loop++ ) {

		if ( *( pcbuf+ loop ) == 0x0A ) {
			*( pcbuf+ loop ) = 0x00;
		}
	
	}

	return pcbuf;

}

/**********************************************************
	Objデータ変換関数

	第１引数：polファイルから読み込んだオブジェクト頂点
			　情報が格納されている領域の先頭アドレス
	第２引数：読み込んだファイルの最大文字数
	第３引数：読み込んだ頂点情報等を格納する変数のアドレス
	　戻り値：処理成功時にはTRUEを返し、失敗時にはFALSEを
			　返す。

	ファイルから読み込んだ頂点数：法線数：カラー数：ポリゴン
	数を数値として読み込み、また読み込んだサイズ分の領域を
	確保した上でOBJ_VARIABLE型データに各情報を格納する。
**********************************************************/

int Load_VertexData( char *pcbuf, int nfsize, OBJ_VARIABLE *pobj )
{

	int nv_count;		// 頂点数
	int nn_count;		// 法線数
    int nc_count;		// カラー数
    int nt_count;		// テクスチャ座標数
    int npol_count;		// ポリゴン数
    int npoint;			// ファイルポインタ現在位置

	OBJ_VERTEX  *pv;    // 頂点データ用ポインタ
    OBJ_NORMAL  *pn;    // 法線データ用ポインタ
    OBJ_COLOR   *pc;    // カラーデータ用ポインタ
    OBJ_TEXTURE *pt;    // テクスチャデータ用ポインタ
    OBJ_POLY    *pp;    // ポリゴンデータ用ポインタ

	// データ開始行の検索
	npoint = BufChk_String( "# Data Count", pcbuf, nfsize );

	if ( npoint == FALSE ) { return FALSE; }

	// 頂点数：法線数：カラー数：ポリゴン数をバッファから読み込んで変数に代入
	sscanf( ( pcbuf + npoint ), "%d %d %d %d %d", &nv_count, &nn_count, &nc_count, &nt_count, &npol_count );
	
	// ファイルから読み込んだポリゴン数をオブジェクトデータ内に設定
	pobj->m_Polcnt = npol_count;

	// ポリゴンの各データを格納するための領域を確保
	// OBJ_POLY構造体のサイズ（各インデックス情報を格納する構造体）＊npol_count（ポリゴン数）分の領域確保
	pobj->pObj_pol = (OBJ_POLY *)malloc( sizeof(OBJ_POLY) * npol_count );

	// 各構造体分の領域を確保（構造体のサイズ＊各情報数）
	pobj->pObj_v =  (OBJ_VERTEX *)malloc( sizeof(OBJ_VERTEX) * nv_count );
	pobj->pObj_n =  (OBJ_NORMAL *)malloc( sizeof(OBJ_NORMAL) * nn_count );
	pobj->pObj_c =   (OBJ_COLOR *)malloc( sizeof(OBJ_COLOR) * nc_count );
	pobj->pObj_t = (OBJ_TEXTURE *)malloc( sizeof(OBJ_TEXTURE) * nt_count );

	if ( ( pobj->pObj_pol == NULL ) || ( pobj->pObj_v == NULL )
		|| ( pobj->pObj_n == NULL ) || ( pobj->pObj_c == NULL )
		|| ( pobj->pObj_t == NULL ) ) { return FALSE; }

	// 頂点データ開始行検索	
	npoint = BufChk_String( "# Vertex Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// テキストからとってきたデータをpobjにポインタを使って間接的に代入
	for ( int loop = 0; loop < nv_count; loop++ ) {

		pv = ( pobj->pObj_v ) + loop;																// データが記入されている場所をpvに格納
		sscanf( ( pcbuf + npoint ), "%f,%f,%f", &pv->m_x, &pv->m_y, &pv->m_z );						// その場所から１行分のデータを読み込む
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// 読み込んだ文字数をカウントし、その分npointを進める
																									// つまり、１行進む
	}

	// 法線データ開始行検索	
	npoint = BufChk_String( "# Normal Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// テキストからとってきたデータをpobjにポインタを使って間接的に代入
	for ( int loop = 0; loop < nn_count; loop++ ) {

		pn = ( pobj->pObj_n ) + loop;																// データが記入されている場所をpvに格納
		sscanf( ( pcbuf + npoint ), "%f,%f,%f", &pn->m_x, &pn->m_y, &pn->m_z );						// その場所から１行分のデータを読み込む
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// 読み込んだ文字数をカウントし、その分npointを進める
																									// つまり、１行進む
	}

	// カラーデータ開始行検索	
	npoint = BufChk_String( "# Color Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// テキストからとってきたデータをpobjにポインタを使って間接的に代入
	for ( int loop = 0; loop < nc_count; loop++ ) {

		pc = ( pobj->pObj_c ) + loop;																// データが記入されている場所をpvに格納
		sscanf( ( pcbuf + npoint ), "%f,%f,%f,%f", &pc->m_R, &pc->m_G, &pc->m_B, &pc->m_A );		// その場所から１行分のデータを読み込む
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// 読み込んだ文字数をカウントし、その分npointを進める
																									// つまり、１行進む
	}

	// テクスチャデータ開始行検索	
	npoint = BufChk_String( "# Texture Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// テキストからとってきたデータをpobjにポインタを使って間接的に代入
	for ( int loop = 0; loop < nt_count; loop++ ) {

		pt = ( pobj->pObj_t ) + loop;																// データが記入されている場所をpvに格納
		sscanf( ( pcbuf + npoint ), "%f,%f", &pt->m_s, &pt->m_t );									// その場所から１行分のデータを読み込む
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// 読み込んだ文字数をカウントし、その分npointを進める
																									// つまり、１行進む
	}

	// ポリゴンデータ開始行検索
    npoint = BufChk_String( "# Polygon Data", pcbuf, nfsize );
    
	if( npoint == FALSE ) { return FALSE; }

    // ポリゴンデータをpobjに代入
    for( int loop = 0; loop < npol_count; loop++ ) {

		pp = ( pobj->pObj_pol ) + loop;

        sscanf( ( pcbuf + npoint ), "%d,%d,%d : %d,%d,%d : %d,%d,%d : %d,%d,%d",
						&pp->m_v[0], &pp->m_v[1], &pp->m_v[2],										// 頂点インデックス
                        &pp->m_n[0], &pp->m_n[1], &pp->m_n[2],										// 法線インデックス
                        &pp->m_c[0], &pp->m_c[1], &pp->m_c[2],										// カラーインデックス
						&pp->m_t[0], &pp->m_t[1], &pp->m_t[2]  );									// テクスチャインデックス
        
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// １行分移動

	}

	return TRUE;

}

/**********************************************************
	指定文字列検索関数

	第１引数：検索する文字列
	第２引数：検索される文字データの先頭アドレス
	第３引数：検索される文字データのサイズ
	　戻り値：処理成功時には指定文字列までの文字数が
			　返り、失敗時にはFLASEが返る。

	第１引数として渡された文字列を検索する関数。
	引数として渡された文字列とpcbuf内で現在指定されている
	位置からnchk_size分の文字を比較する。該当文字列が発見
	されれば、そこまでの文字数を返す。
**********************************************************/

int BufChk_String( char *pcstring, char *pcbuf, int nsize )
{

	int nleng;									// １行の文字数
	int npoint = 0;								// 検索用移動ポインタ
	int nchk_size;								// チェック文字数

	nchk_size = (int)strlen( pcstring );		// チェックする文字数（引数として渡された文字列）
	
	while ( npoint < nsize ) {

		nleng = (int)strlen( pcbuf ) + 1;		// １行の文字数

		npoint += nleng;						// 検索用移動ポインタを読み込んだ文字数分進める

		// 引数として渡された文字列とpcbuf内で現在指定されている位置からnchk_size分の文字を比較する
		// その文字列を比較した結果、同じ場合０が返る。
		if ( memcmp( pcstring, pcbuf, nchk_size ) == 0 ) {
			// 検知した指定文字列までの文字数を返す
			return npoint;
		}

		pcbuf += nleng;							// バッファ位置を読み込んだ文字数分進める

	}

	return FALSE;

}

/**********************************************************
	Objファイル読み込みエラー関数

	　引数；なし
	戻り値：正常終了時にTRUEを返す

	Objファイルの読み込みに失敗した時に実行する関数。
	Objファイル読み込みのために確保した各データ領域
	OBJ_VARIABLE・OBJ_VERTEX_BUF・OBJ_VERTEX_BUF2型
	データを解放する。
**********************************************************/

int Obj_Error1( void )
{
	
	if ( g_pObjfile != NULL ) {
		free( g_pObjfile );
		g_pObjfile = NULL;
	}

	Free_ObjIndex( g_pObj );				// OBJ_VARIABLE型データ解放関数
	g_pObj = NULL;

	Free_ObjVertex1( g_pVerBuf );			// OBJ_VERTEX_BUF型データ解放関数
	g_pVerBuf = NULL;

	Free_ObjVertex2( g_pVerBuf2 );			// OBJ_VERTEX_BUF2型データ解放関数
	g_pVerBuf2 = NULL;

	return TRUE;

}

/**********************************************************
	OBJ_VARIABLE型データ解放関数

	　引数：解放するOBJ_VARIABLE型のデータへのアドレス
	戻り値：なし

	領域確保したOBJ_VARIABLE型のデータを解放する関数。
	その際、各メンバ変数のバッファ情報ないにアドレスが存在
	する場合にはその領域を先に解放する。	
**********************************************************/

void Free_ObjIndex( OBJ_VARIABLE *pobj )
{

	if ( pobj != NULL ) {

		if ( pobj->pObj_v != NULL ) {
			free( pobj->pObj_v );
			pobj->pObj_v = NULL;
		}

		if ( pobj->pObj_n != NULL ) {
			free( pobj->pObj_n );
			pobj->pObj_n = NULL;
		}

		if ( pobj->pObj_c != NULL ) {
			free( pobj->pObj_c );
			pobj->pObj_c = NULL;
		}

		if ( pobj->pObj_t != NULL ) {
			free( pobj->pObj_t );
			pobj->pObj_t = NULL;
		}

		if ( pobj->pObj_pol != NULL ) {
			free( pobj->pObj_pol );
			pobj->pObj_pol = NULL;
		}

		free( pobj );
	
	}

}

/**********************************************************
	OBJ_VERTEX_BUF型データ解放関数

	　引数：解放するOBJ_VERTEX_BUF型のデータへのアドレス
	戻り値：なし

	領域確保したOBJ_VERTEX_BUF型のデータを解放する関数。
	その際、各メンバ変数のバッファ情報ないにアドレスが存在
	する場合にはその領域を先に解放する。
**********************************************************/

void Free_ObjVertex1( OBJ_VERTEX_BUF *pobj )
{
    if( pobj != NULL ) {

		if( pobj->pObj_v != NULL ) {
            free( pobj->pObj_v );
            pobj->pObj_v = NULL;
        }

		if( pobj->pObj_n != NULL ) {
            free( pobj->pObj_n );
            pobj->pObj_n = NULL;
        }

        if( pobj->pObj_c != NULL ) {
            free( pobj->pObj_c );
            pobj->pObj_c = NULL;
        }

		if( pobj->pObj_t != NULL ){
            free( pobj->pObj_t );
			pobj->pObj_t = NULL;
        }

        free( pobj );
    
	}

}

/**********************************************************
	OBJ_VERTEX_BUF2型データ解放関数

	　引数：解放するOBJ_VERTEX_BUF2型のオブジェクトへのアドレス
	戻り値：なし

	確保したOBJ_VERTEX_BUF2型のデータ領域を解放する関数。
	但し、頂点データの中身が存在する時はその領域を先に解放する。
**********************************************************/

void Free_ObjVertex2( OBJ_VERTEX_BUF2 *pobj )
{

	if ( pobj != NULL ) {

		if ( pobj->pcObjData != NULL ) {
			free( pobj->pcObjData );
			pobj->pcObjData = NULL;
		}

		free( pobj );

	}

}
