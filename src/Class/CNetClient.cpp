
/************************************************
	ヘッダーファイル
************************************************/

#include "CNetClient.h"

/************************************************
	CNetClientクラス：コンストラクタ関数
************************************************/

CNetClient::CNetClient()
{

	// メンバ変数の初期化
	this->m_SendSocket = INVALID_SOCKET;
	this->m_RecvSocket = INVALID_SOCKET;

	this->m_pSendThread = NULL;
	this->m_pRecvThread = NULL;

	// この時点でのクライアントIDは0
	this->m_nMyID = 0;
	
	// この時点での対戦相手IDは0
	this->m_nEnemyID = 0;

	// ソケット使用開始処理
	WSAStartup( MAKEWORD( 1, 1 ), &this->m_wsadata );

}

/************************************************
	CNetClientクラス：デストラクタ関数
************************************************/

CNetClient::~CNetClient()
{

	// ソケットをクローズ
	closesocket(this->m_RecvSocket);
	closesocket(this->m_SendSocket);

	this->m_SendSocket = INVALID_SOCKET;
	this->m_RecvSocket = INVALID_SOCKET;

	// ソケット使用終了処理
	WSACleanup();

}

/************************************************
	CNetClientクラス：IPファイル読み込み関数
	
	引数１：IPアドレスが書かれたファイルのパス
	引数２：IPアドレスを書き込む配列へのポインタ
************************************************/

bool CNetClient::ReadIPFile( char* filename, char *in_ip )
{

	FILE* fp;
	size_t fsize;
	char temp[20];

	// 指定されたファイルをテキストモードでオープン
	if ( ( fp = fopen( filename, "rt" ) ) == NULL ) { return false; }

	// fseek関数を使ってファイルポインタを最後尾に持ってくる
	fseek( fp, 0, SEEK_END );
	
	// ftell関数を使ってファイルの先頭からファイルポインタの位置までの文字数を数える
	fsize = ftell( fp );
	
	// ファイルポインタを先頭に戻す
	fseek( fp, 0, SEEK_SET );

	// ファイルからIPアドレスを読み込む（???.???.???.???）
	fsize = fread( temp, sizeof(char), fsize, fp );

	// 末尾に\0を挿入
	temp[fsize] = '\0';

	// 読み込んだIPアドレスをコピー
	strcpy( in_ip, temp );

	fclose( fp );

	return true;

}

/************************************************
	CNetClientクラス：サーバ接続関数
************************************************/

bool CNetClient::ServerConnect()
{

	bool ret;
	char ip[20];

	// IPファイルからIPアドレスを取得
	ret= this->ReadIPFile( "./DAT/ip.txt", ip );

	if ( ret == false ) {
		return false;
	}

	// ソケット通信用変数
	// 送信用と受信用
	struct sockaddr_in rserver;
	struct sockaddr_in sserver;

	// 0で初期化
	memset( (char *)&rserver, 0, sizeof(rserver) );
	memset( (char *)&sserver, 0, sizeof(sserver) );

	// ソケットの作成
	this->m_SendSocket = socket( AF_INET, SOCK_STREAM, 0 );
	this->m_RecvSocket = socket( AF_INET, SOCK_STREAM, 0 );

	// 接続先指定用構造体の準備
	sserver.sin_family  = AF_INET;
	sserver.sin_port	= htons(50000);
	sserver.sin_addr.s_addr = inet_addr(ip);
	
	// 固定版
	//sserver.sin_addr.s_addr = inet_addr("192.168.2.2");

	rserver.sin_family  = AF_INET;
	rserver.sin_port	= htons(50001);

	rserver.sin_addr.s_addr = inet_addr(ip);
	
	// 固定版
	//rserver.sin_addr.s_addr = inet_addr("192.168.2.2");

	char debug_string_buf[100];
	sprintf( debug_string_buf, "接続開始...\n" );
	OutputDebugString( debug_string_buf );

	// サーバの50000ポートの受付に接続要求
	if ( connect( this->m_SendSocket, (struct sockaddr *)&sserver, sizeof(sserver) ) != 0 ) {

		// 接続が不可能な場合はFalseを返す
		char debug_string_buf[100];
		sprintf( debug_string_buf, "送信ソケット・コネクト失敗" );
		OutputDebugString( debug_string_buf );

		return false;
		
	}

	// サーバの50001ポートの受付に接続要求
	if ( connect( this->m_RecvSocket, (struct sockaddr *)&rserver, sizeof(rserver) ) != 0 ) {

		// 接続が不可能な場合はFalseを返す
		char debug_string_buf1[100];
		sprintf( debug_string_buf1, "受信ソケット・コネクト失敗" );
		OutputDebugString( debug_string_buf1 );

		return false;
		
	}

	char debug_string_buf2[100];
	sprintf( debug_string_buf2, "接続成功...\n" );
	OutputDebugString( debug_string_buf2 );

	// 送信スレッドクラスをインスタンス化
	this->m_pSendThread = new CSendThread(this);

	// 受信スレッドクラスをインスタンス化
	this->m_pRecvThread = new CRecvThread(this);

	// スレッド開始
	boost::thread thread1(*this->m_pSendThread);
	boost::thread thread2(*this->m_pRecvThread);

	return true;

}

/************************************************
	CNetClientクラス：クライアント終了処理関数
************************************************/

void CNetClient::End()
{

	if ( this->m_pSendThread != NULL ) {

		// delete前の終了処理
		this->m_pSendThread->End();
	
		// スレッドをdelete
		delete(this->m_pSendThread);

		this->m_pSendThread = NULL;

	}

	if ( this->m_pRecvThread != NULL ) {

		// delete前の終了処理
		this->m_pRecvThread->End();

		// スレッドをdelete
		delete(this->m_pRecvThread);

		this->m_pRecvThread = NULL;

	}

}

/************************************************
	CSendThreadクラス：コンストラクタ関数
************************************************/

CSendThread::CSendThread( CNetClient *in_client )
{

	// メンバ変数初期化
	this->m_cNetClient = in_client;
	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;
	
	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;

	// 通信開始のパケットとして初期化
	this->m_pPacket.m_nType = START_PACKET;
	this->m_pPacket.m_nLen = PACKET_SIZE;
	this->m_pPacket.m_nData = 0;
	this->m_pPacket.m_nSender = this->m_cNetClient->getMyID();
	this->m_pPacket.m_nSendTo = 0;

}

/************************************************
	CSendThreadクラス：デストラクタ関数
************************************************/

CSendThread::~CSendThread()
{
}

/************************************************
	CSendThreadクラス：送信スレッド関数
************************************************/

void CSendThread::operator ()()
{

	int len = 0;
	
	int target = MAX_TARGET;

	// 最初のパケット(START_PACKET)を送る（これに対応して、サーバがクライアントIDを通知）
	len = send( this->m_cNetClient->getSendSocket(), (char*)&this->m_pPacket, this->m_pPacket.m_nLen, 0 );

	while ( true ) {

		{
			// mutexオブジェクトによるロック
			boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);

			// End()関数が実行されていればEndFlag1がtrueになっている
			if ( *this->m_bEndFlag1 ) {
				// End関数内で無限ループに入らないようにEndFlag2をtrueにする
				*this->m_bEndFlag2 = true;
				break;			
			}
		}
		
		// 自機のターゲット数が0になるか（パケットをターゲット数分送ればtargetは0になる）
		// 相手のターゲット数が初期状態でも実行
		if ( target == 0 || g_Ereamap.m_EnemyTargetNum == 99 ) {
			
			// 自機のターゲットが0になった後に値が初期の値に戻っていたなら、targetも初期化、
			if ( g_Ereamap.m_TargetNum == MAX_TARGET ) {
				target = MAX_TARGET;
			}
				
		} 
		
		// 現在のターゲット数が減っていたら実行
		if ( g_Ereamap.m_TargetNum < target ) {

			// mutexでロック
			boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);

			// データパケットを作る
			this->m_pPacket.m_nType = DATA_PACKET;
			// 自機の残りターゲット数をデータ部に格納
			this->m_pPacket.m_nData = g_Ereamap.m_TargetNum;
			// クライアントIDを送信元に設定
			this->m_pPacket.m_nSender = this->m_cNetClient->getMyID();
			// 対戦相手のIDを送信先に設定
			this->m_pPacket.m_nSendTo = this->m_cNetClient->getEnemyID();

			// 残りのターゲット数を通知
			len = send( this->m_cNetClient->getSendSocket(), (char*)&this->m_pPacket, this->m_pPacket.m_nLen, 0 );

			// ターゲットカウンタをマイナス
			target--;

		}

		Sleep(100);

	}

}

/************************************************
	CSendThreadクラス：終了処理関数
************************************************/

void CSendThread::End()
{

	{

		PACKET packet;

		// 通信終了パケットを作成
		packet.m_nType = END_PACKET;
		packet.m_nSender = this->m_cNetClient->getMyID();
		packet.m_nSendTo = this->m_cNetClient->getEnemyID();
		packet.m_nLen = PACKET_SIZE;
		packet.m_nData = 99;

		// 終了パケットを送信		
		send( this->m_cNetClient->getSendSocket(), (char*)&packet, PACKET_SIZE, 0 );

	}

	{	
		boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);
	
		// EndFlag1をtrueにして、送信スレッドの終了処理に移行可能にする
		*this->m_bEndFlag1 = true;
	}

	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);

		// 送信スレッド内でEndFlag2がtrueになるまでループ
		if ( *this->m_bEndFlag2 ) {
			break;
		}

		// CPUのスリープ処理
		Sleep(100);

	}

	// Flagをdelete処理
	delete(this->m_bEndFlag1);
	delete(this->m_bEndFlag2);

	this->m_bEndFlag1 = NULL;
	this->m_bEndFlag2 = NULL;

}

/************************************************
	CRecvThreadクラス：コンストラクタ関数
************************************************/

CRecvThread::CRecvThread( CNetClient *in_client )
{

	// メンバ変数初期化
	this->m_cNetClient = in_client;
	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;

	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;
	
}

/************************************************
	CRecvThreadクラス：デストラクタ関数
************************************************/

CRecvThread::~CRecvThread()
{
}

/************************************************
	CRecvThreadクラス：受信スレッド関数
************************************************/

void CRecvThread::operator ()()
{

	int len = 0;;
	PACKET packet; 

	while ( true ) {

		{
			// mutexオブジェクトによるロック
			boost::mutex::scoped_lock lk(this->m_cNetClient->RecvMutex);

			// End()関数が実行されていればEndFlag1がtrueになっている
			if ( *this->m_bEndFlag1 ) {
				// End関数内で無限ループに入らないようにEndFlag2をtrueにする
				*this->m_bEndFlag2 = true;
				break;		
			}
		}

		{

			// サーバからパケットを受信
			len = recv( this->m_cNetClient->getRecvSocket(), (char *)&packet, PACKET_SIZE, 0 );

			// サーバからのパケットがスタートパケット時にはクライアントIDと対戦相手IDを設定
			if ( packet.m_nType == START_PACKET ) {

				{
					boost::mutex::scoped_lock lk(this->m_cNetClient->OutMutex);

					// パケットの送信先を取得
					int id = packet.m_nSendTo;

					// サーバから送られてきた送信先をクライアントIDにする
					this->m_cNetClient->setMyID(id);

					// 自分のIDから対戦相手のIDを決める
					if ( id % 2 == 1 ) {
					
						// クライアントIDが奇数なら対戦相手IDを＋１の値に設定
						id++; 
						this->m_cNetClient->setEnemyID(id);

					}
					else {

						// クライアントIDが偶数なら対戦相手IDを－１の値に設定
						id--;
						this->m_cNetClient->setEnemyID(id);

					}
				}

			}
			// 送られてきたパケットが通信終了パケットの場合
			else if ( packet.m_nType == END_PACKET ) {

				// mutexオブジェクトでロック
				boost::mutex::scoped_lock lk(this->m_cNetClient->OutMutex);

				char debug_string_buf2[100];
				sprintf( debug_string_buf2, "受信スレッド終了処理に入る\n" );
				OutputDebugString( debug_string_buf2 );
			
				// End関数で無限ループに入らないようにEndFlag2をtrueにする
				*this->m_bEndFlag2 = true;

				// スレッドを終了させる
				break;

			}
			else {

				// mutexオブジェクトでロック
				boost::mutex::scoped_lock lk(this->m_cNetClient->OutMutex);

				// データ部分のターゲット数を取得
				int target = packet.m_nData;

				// 対戦相手の残りターゲット数に設定
				g_Ereamap.m_EnemyTargetNum = target;

#if 0
				char debug_string_buf2[100];
				sprintf( debug_string_buf2, "受信パケット タイプ = %d, 送信元 = %d, 送信先 = %d\n", packet.m_nType, packet.m_nSender, packet.m_nSendTo );
				OutputDebugString( debug_string_buf2 );
#endif

			}
		}

	}
				
}

/************************************************
	CRecvThreadクラス：終了処理関数
************************************************/

void CRecvThread::End()
{

	{	
		boost::mutex::scoped_lock lk(this->m_cNetClient->RecvMutex);

		// EndFlag1をtrueにして、送信スレッドの終了処理に移行可能にする		
		*this->m_bEndFlag1 = true;
	}

	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetClient->RecvMutex);
		
		// 受信スレッド内でEndFlag2がtrueになるまでループ
		if ( *this->m_bEndFlag2 ) {
			break;
		}

		// CPUのスリープ処理
		Sleep(100);

	}

	// Flagをdelete処理
	delete(this->m_bEndFlag1);
	delete(this->m_bEndFlag2);

	this->m_bEndFlag1 = NULL;
	this->m_bEndFlag2 = NULL;

}
