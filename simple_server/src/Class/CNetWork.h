#if !defined(__CNETWORK__H)
#define __CNETWORK__H

// ヘッダーファイル
#include "../main.h"

// winsockに必要なヘッダとライブラリ
#include <winsock.h>
#pragma comment( lib, "ws2_32.lib" )

// boost関連ヘッダー
#include <boost/ref.hpp>
#include <boost/thread.hpp>

// STLヘッダー
#include <deque>

// 名前空間定義
using namespace std;

// クラス宣言
class CNetWork;
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
	int m_cType;

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
	クラス定義：ネットワーク送受信用クラス
************************************************/

class CNetWork {

private:

	// 最大ソケット数
	static const int MAX_SOCKET = 21;

	// ソケット通信用
	WSAData m_wsadata; 

	// サーバ側IPアドレス
	HOSTENT *m_serverip;

	// 生存フラグ
	volatile bool m_continuing;

	// 送受信用ソケット
	SOCKET m_SendSocket[MAX_SOCKET];
	SOCKET m_RecvSocket[MAX_SOCKET];

	// 送受信スレッドクラス
	CSendThread *m_cSendThread[MAX_SOCKET];
	CRecvThread *m_cRecvThread[MAX_SOCKET];

	// パケット用キュー
	deque<PACKET> m_PacketQueue;

public:

	// mutexオブジェクト
	boost::mutex SendMutex;
	boost::mutex RecvMutex;
	boost::mutex OutMutex;

	// コンストラクタ・デストラクタ関数
	CNetWork();
	~CNetWork();

	// 接続要求受付関数
	bool ListenStart();

	/* getterとsetter */

	SOCKET getSendSocket( int in_no ) { return this->m_SendSocket[in_no]; }
	SOCKET getRecvSocket( int in_no ) { return this->m_RecvSocket[in_no]; }
	
	deque<PACKET> getQueue() { return this->m_PacketQueue; }
	void setBackQueue( PACKET in_pack ) { this->m_PacketQueue.push_back(in_pack); }
	void setFrontQueue( PACKET in_pack ) { this->m_PacketQueue.push_front(in_pack); }
	void PopFrontQueue() { this->m_PacketQueue.pop_front(); }
	void PopBackQueue() { this->m_PacketQueue.pop_back(); }

	/* ここまで */

};
	
/************************************************
	クラス定義：送信用スレッドクラス
************************************************/

class CSendThread {

private:

	// 親のネットワーククラスへのポインタ
	CNetWork *m_cNetWork;

	// スレッド番号
	int m_nThreadNo;

	// 終了判定フラグ
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

	// 自分に割り当てられた送信用ソケット番号
	SOCKET m_SendSocket;

	// （暫定）最新のパケット用変数
	PACKET m_Packet;

public:

	// 関数オブジェクトのoperator()()
	void operator()();

	// コンストラクタ・デストラクタ関数
	CSendThread( CNetWork* in_net, SOCKET in_socket, int in_no );

	// 終了処理関数
	void End();

};

/************************************************
	クラス定義：受信用スレッドクラス
************************************************/

class CRecvThread {

private:

	// 親のネットワーククラスへのポインタ
	CNetWork *m_cNetWork;

	// スレッド番号
	int m_nThreadNo;

	// 終了判定フラグ
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

	// 自分に割り当てられた受信用ソケット番号
	SOCKET m_RecvSocket;

	// （暫定）最新のパケット用変数
	PACKET m_Packet;

public:

	// 関数オブジェクトのoperator()()
	void operator()();

	// コンストラクタ・デストラクタ関数
	CRecvThread( CNetWork* in_net, SOCKET in_socket, int in_no );

	// 終了処理関数
	void End();

};

#endif