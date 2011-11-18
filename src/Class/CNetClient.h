#if !defined(__CNETCLIENT__H)
#define __CNETCLIENT__H

/************************************************
	ヘッダーファイル
************************************************/

// C用ヘッダー
#include <stdio.h>

// winsock用に必要なヘッダとライブラリ
#include <winsock.h>
#pragma comment( lib, "ws2_32.lib" )

// boost関連ヘッダー
#include <boost/ref.hpp>
#include <boost/thread.hpp>

#include "../Gameprog/Field.h"
#include "../Gameprog/Target.h"

// クラス宣言
class CSendThread;
class CRecvThread;

/************************************************
	列挙体定義
************************************************/

// パケット用コード
enum PACKET_TYPE : int {

	START_PACKET = 1,	// 通信開始パケット
	END_PACKET,			// 通信終了パケット
	DATA_PACKET,		// 通常パケット

	NON_PACKET			// その他のパケット

};

/************************************************
	構造体定義
************************************************/

// パケットサイズ
#define PACKET_SIZE (20)

// データ送受信用パケット
struct PACKET {
	
	// パケットの種類
	int m_nType;

	// パケットサイズ
	int m_nLen;

	// 送信者ID
	int m_nSender;

	// 受信者ID
	int m_nSendTo;

	// データ
	int m_nData;
	
};

/************************************************
	クラス定義：ネットワーク接続用クラス
************************************************/

class CNetClient {

private:

	// winsock用変数
	WSAData m_wsadata;	

	// 送受信用ソケット
	SOCKET m_RecvSocket;
	SOCKET m_SendSocket;

	// 送受信スレッドクラスへのポインタ
	CSendThread *m_pSendThread;
	CRecvThread *m_pRecvThread;

	// 自分のクライアントID
	int m_nMyID;

	// 対戦相手のID
	int m_nEnemyID;

public:

	// mutex用オブジェクト
	boost::mutex SendMutex;
	boost::mutex RecvMutex;
	boost::mutex OutMutex;

	// コンストラクタ・デストラクタ関数
	CNetClient();
	~CNetClient();

	// 終了処理関数
	void End();

	// サーバ接続関数
	bool ServerConnect();

	// IPファイル読み込み関数
	bool ReadIPFile( char *filename, char *in_ip );

	/* getterとsetter */

	SOCKET getSendSocket() { return this->m_SendSocket; }
	SOCKET getRecvSocket() { return this->m_RecvSocket; }

	int getMyID() { return this->m_nMyID; } 
	void setMyID( int in_id ) { this->m_nMyID = in_id; } 

	int getEnemyID() { return this->m_nEnemyID; } 
	void setEnemyID( int in_id ) { this->m_nEnemyID = in_id; } 

	/* ここまで */

};

/************************************************
	クラス定義：送信用スレッドクラス
************************************************/

class CSendThread {

private:

	// ネットワーククライアントクラスへのポインタ
	CNetClient *m_cNetClient;

	// 終了判定
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

	PACKET m_pPacket;

public:

	// 関数オブジェクトのoperator()()
	void operator()();

	// コンストラクタ・デストラクタ関数
	CSendThread( CNetClient* in_client );
	~CSendThread();

	// 終了処理関数
	void End();

};

/************************************************
	クラス定義：受信用スレッドクラス
************************************************/

class CRecvThread {

private:

	// ネットワーククライアントクラスへのポインタ
	CNetClient *m_cNetClient;

	// 終了判定
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

public:

	// 関数オブジェクトのoperator()()
	void operator()();

	// コンストラクタ・デストラクタ関数
	CRecvThread( CNetClient* in_client );
	~CRecvThread();

	// 終了処理関数
	void End();

};

#endif