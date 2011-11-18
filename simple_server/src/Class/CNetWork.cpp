
/************************************************
	ヘッダーファイル
************************************************/

#include "./CNetwork.h"

/************************************************
	名前空間
************************************************/

using namespace std;

/************************************************
	CNetWorkクラス：コンストラクタ関数
************************************************/

CNetWork::CNetWork()
{

	// メンバ変数初期化
	this->m_continuing = true;

	for ( int ct = 0; ct < CNetWork::MAX_SOCKET; ct++ ) {
		
		this->m_SendSocket[ct] = INVALID_SOCKET;
		this->m_RecvSocket[ct] = INVALID_SOCKET;

		this->m_cSendThread[ct] = NULL;
		this->m_cRecvThread[ct] = NULL;

	}

	char buf[1024];

	cout << "SERVER PROGRAM START" << endl;

	// ソケット使用開始処理
	WSAStartup( MAKEWORD( 1, 1 ), &this->m_wsadata );

	// ホスト名取得
	gethostname( buf, sizeof(buf) );

	// ホスト名からIPアドレスを取得
	this->m_serverip = gethostbyname(buf);

	// IPアドレスの表示
	cout << "SERVER IP ADDRESS : ";
	printf( "%u.%u.%u.%u \n", (unsigned char)this->m_serverip->h_addr_list[0][0],
							  (unsigned char)this->m_serverip->h_addr_list[0][1],
							  (unsigned char)this->m_serverip->h_addr_list[0][2],
							  (unsigned char)this->m_serverip->h_addr_list[0][3] );

}

/************************************************
	CNetWorkクラス：デストラクタ関数
************************************************/

CNetWork::~CNetWork()
{

	// ソケットを閉じる
	for ( int ct = 1; ct < MAX_SOCKET; ct++ ) {

		// ソケット配列内が空でないものだけ閉じる
		if ( this->m_RecvSocket[ct] != INVALID_SOCKET ) {
			closesocket(this->m_RecvSocket[ct]);
			this->m_RecvSocket[ct] = INVALID_SOCKET;
		}

		// 上記と同じ
		if ( this->m_SendSocket[ct] != INVALID_SOCKET ) {
			closesocket(this->m_SendSocket[ct]);
			this->m_SendSocket[ct] = INVALID_SOCKET;
		}

	}

	// 受け付け用のソケットを閉じる
	closesocket(this->m_RecvSocket[0]);
	closesocket(this->m_SendSocket[0]);

	// 各スレッドの最終終了処理
	for ( int ct = 0; ct < MAX_SOCKET; ct++ ) {

		// スレッドクラス配列内が空でないものだけ閉じる
		if ( this->m_cSendThread[ct] != NULL ) {
			this->m_cSendThread[ct]->End();
			delete (this->m_cSendThread[ct]);
		}

		// 上記に同じ
		if ( this->m_cRecvThread[ct] != NULL ) {
			this->m_cRecvThread[ct]->End();
			delete (this->m_cRecvThread[ct]);
		}

	}

	// ソケット終了処理
	WSACleanup();

}

/************************************************
	CNetWorkクラス：接続要求受付関数
************************************************/

bool CNetWork::ListenStart()
{

		// ローカル変数
		int len, cheak;

		SOCKET rclient;
		SOCKET sclient;

		struct sockaddr_in rsaddr;
		struct sockaddr_in rcaddr;
		struct sockaddr_in ssaddr;
		struct sockaddr_in scaddr;

		// 受信ソケットの作成
		if ( ( this->m_RecvSocket[0] = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
			return false;
		}

		// 送信ソケットの作成
		if ( ( this->m_SendSocket[0] = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
			return false;
		}

		// 0で初期化
		memset( (char *)&rsaddr, 0, sizeof(rsaddr) );
		memset( (char *)&rcaddr, 0, sizeof(rcaddr) );
		memset( (char *)&ssaddr, 0, sizeof(ssaddr) );
		memset( (char *)&scaddr, 0, sizeof(scaddr) );

		// バインド前の準備
		rsaddr.sin_family = AF_INET;
		rsaddr.sin_addr.s_addr = INADDR_ANY;
		rsaddr.sin_port = htons(50000);

		ssaddr.sin_family = AF_INET;
		ssaddr.sin_addr.s_addr = INADDR_ANY;
		ssaddr.sin_port = htons(50001);

		// 受信ソケットをバインド
		if ( ( bind( this->m_RecvSocket[0], (struct sockaddr *)&rsaddr, sizeof(rsaddr) ) ) == SOCKET_ERROR ) {
			return false;
		}

		// 送信ソケットをバインド
		if ( ( bind( this->m_SendSocket[0], (struct sockaddr *)&ssaddr, sizeof(ssaddr) ) ) == SOCKET_ERROR ) {
			return false;
		}

		// 受信ソケットをリスン状態にする
		if ( ( listen( this->m_RecvSocket[0], 10 ) ) == SOCKET_ERROR ) {
			return false;
		}

		// 送信ソケットをリスン状態にする
		if ( ( listen( this->m_SendSocket[0], 10 ) ) == SOCKET_ERROR ) {
			return false;
		}

		{
			boost::mutex::scoped_lock lk(this->OutMutex);
			cout << "ソケット接続要求の受付開始" << endl;
		}

		while ( this->m_continuing ) {

			// サイズを取得
			len = sizeof(rcaddr);
			
			// 送受信ソケットの接続判定用フラグ
			cheak = 0; 

			// 受信受付ソケットへの接続を待つ
			rclient = accept( this->m_RecvSocket[0], (struct sockaddr *)&rcaddr, &len );

			// エラー処理（なんらかの理由で受付を失敗）
			if ( rclient == INVALID_SOCKET ) {
				return false;
			}

			// サイズを取得
			len = sizeof(rcaddr);

			// 送信受付ソケットへの接続を待つ
			sclient = accept( this->m_SendSocket[0], (struct sockaddr *)&scaddr, &len );

			// エラー処理（なんらかの理由で受付を失敗
			if ( sclient == INVALID_SOCKET ) {
				return false;
			}

			// ソケット配列内の空の部分を探してループ
			for ( int ct = 1; ct < CNetWork::MAX_SOCKET; ct++ ) {

				// 受け付けた送受信ソケットを受付ソケットから切り替える
				if ( this->m_RecvSocket[ct] == INVALID_SOCKET ) {

					boost::mutex::scoped_lock lk(this->OutMutex);

					cout << "受信ソケット接続確認 ソケット番号 = " << rclient << " クライアントNO = "  << ct << endl;
					
					// ソケットのコピー
					this->m_RecvSocket[ct] = rclient;

					// スレッドを作る
					this->m_cRecvThread[ct] = new CRecvThread( this, rclient, ct );

					// チェック用変数をプラス１
					cheak++;

				}

				if ( this->m_SendSocket[ct] == INVALID_SOCKET ) {

					boost::mutex::scoped_lock lk(this->OutMutex);

					cout << "送信ソケット接続確認 ソケット番号 = " << sclient << " クライアントNO = "  << ct << endl;
					
					// ソケットのコピー
					this->m_SendSocket[ct] = sclient;

					// スレッドを作る
					this->m_cSendThread[ct] = new CSendThread( this, sclient, ct );

					// チェック用変数をプラス１
					cheak++;

				}

				// ソケット接続・スレッド生成が成功したか確認
				if ( cheak == 2 ) { 
				
					// 送受信スレッドの開始
					boost::thread thread1(*this->m_cRecvThread[ct]);
					boost::thread thread2(*this->m_cSendThread[ct]);

					break; 

				}
				// なんらかのエラーで送受信用のソケットのどちらかが失敗
				else if ( cheak == 1 ) {
					return false;
				}

			}
		
		}

		// 基本的に受付中は外には戻らない
		return true;

}

/************************************************
	CSendThreadクラス：コンストラクタ関数

	引数１：親ネットワーククラスへのポインタ
	引数２：自分の送信用ソケット番号
	引数３：自分のスレッドNO
************************************************/

CSendThread::CSendThread( CNetWork *in_net, SOCKET in_socket, int in_no )
{

	// メンバ変数初期化
	this->m_cNetWork = in_net;
	this->m_nThreadNo = in_no;

	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;

	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;

	this->m_SendSocket = in_socket; 

}

/************************************************
	CSendThreadクラス：送信スレッド関数
************************************************/

void CSendThread::operator ()()
{

	PACKET packet;

	while ( true ) {

		{
			// mutexオブジェクトによるロック
			boost::mutex::scoped_lock lk(this->m_cNetWork->SendMutex);

			// End()関数内でEndFlag1がtrueになっていたらスレッド終了準備
			if ( *this->m_bEndFlag1 ) {
				// End関数内で無限ループに入らないようにEndFlag2をtrueにする
				*this->m_bEndFlag2 = true;
				break;
			}
			
		}

		{
			boost::mutex::scoped_lock lk(this->m_cNetWork->OutMutex);

			// パケット用キューが空でなければ実行
			if ( !this->m_cNetWork->getQueue().empty() ) {

				// キューの先頭を取り出す
				packet = this->m_cNetWork->getQueue().front();

				// 取り出したパケットが最初のパケットの場合（送信先を決めるパケットなので最初に確認）
				if ( packet.m_cType == START_PACKET ) {

					// スレッドNOが同じ場合に自分宛のパケットと判断
					// パケットのデータ部分に格納されているのがスレッドNOと同じ場合（Recvスレッドでデータ部分にスレッドNOを格納してる）
					if ( packet.m_nData == this->m_nThreadNo ) {

						// 送信先にスレッド番号を格納。クライアント側ではSTART_PACKETの場合、送信先をクライアントIDにする
						packet.m_cType = START_PACKET;
						packet.m_nLen = PACKET_SIZE;
						// 送信先（向こう側ではクライアントIDになる）にスレッドNOを格納（対応ずけを行う）
						packet.m_nSendTo = this->m_nThreadNo;

						cout << "クライアントIDを通知 ID = " << packet.m_nSendTo << endl;

						// クライアントにパケットを送信
						int len = send( this->m_SendSocket, (char *)&packet, PACKET_SIZE, 0 );

						// パケット用キューから先頭のパケットを削除
						this->m_cNetWork->PopFrontQueue();

					}

				}
				else {
				
					// キューから取り出したパケットの送信先が自分のスレッドNOと同じなら処理を行う
					if ( packet.m_nSendTo == this->m_nThreadNo ) {
						
						// パケットがデータパケットなら処理
						if ( packet.m_cType == DATA_PACKET ) {

							// 送信先にそのまま送る
							send( this->m_SendSocket, (char *)&packet, PACKET_SIZE, 0 );

							cout << "スレッドNO = " << this->m_nThreadNo << " データパケット = " << packet.m_nSender << " から " << packet.m_nSendTo << " 宛へ "<< endl;
							cout << "データ内容 = " << packet.m_nData << endl; 

							// 先頭のパケットを削除
							this->m_cNetWork->PopFrontQueue();

						}

						// パケットが通信終了パケットなら処理
						if ( packet.m_cType == END_PACKET ){
							
							// 送信先にそのまま送る
							send( this->m_SendSocket, (char *)&packet, PACKET_SIZE, 0 );

							// パケット用キューの先頭のパケットを削除
							this->m_cNetWork->PopFrontQueue();

							cout << "スレッドNO = " << this->m_nThreadNo << " 終了処理を実行（送信スレッド）" << endl;

							// 無限ループにならないようにフラグをtrueにする
							*this->m_bEndFlag2 = true;
							
							break;

						}

					}

				}
			
			}
		}

		Sleep(100);

	}

}

/************************************************
	CSendThreadクラス：終了処理関数
************************************************/

void CSendThread::End()
{

	// まずEndFlag1をtrueにする
	{	
		boost::mutex::scoped_lock lk(this->m_cNetWork->SendMutex);
		*this->m_bEndFlag1 = true;
	}

	// 送信スレッド関数ないでEndFlag2がtrueになるまで待つ
	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetWork->SendMutex);

		// 送信スレッドが終了したら抜ける（EndFlag2がtrueになったら）
		if ( this->m_bEndFlag2 ) {
			break;
		}

		// CPUのスリープ処理
		Sleep(100);

	}

	// Flagをdeleteする
	delete(this->m_bEndFlag1);
	delete(this->m_bEndFlag2);

	this->m_bEndFlag1 = NULL;
	this->m_bEndFlag2 = NULL;

}

/************************************************
	CRecvThreadクラス：コンストラクタ関数

	引数１：親ネットワーククラスへのポインタ
	引数２：自分の受信用ソケット番号
	引数３：自分のスレッドNO
************************************************/

CRecvThread::CRecvThread( CNetWork *in_net, SOCKET in_socket, int in_no )
{

	// メンバ変数初期化
	this->m_cNetWork = in_net;
	this->m_nThreadNo = in_no;

	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;

	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;

	this->m_RecvSocket = in_socket;

}

/************************************************
	CRecvThreadクラス：受信スレッド関数
************************************************/

void CRecvThread::operator()()
{

	int len;	
	PACKET packet;

	while ( true ) {

		{
			// mutexオブジェクトによるロック
			boost::mutex::scoped_lock lk(this->m_cNetWork->RecvMutex);

			// End()関数内でEndFlag1がtrueになっていたらスレッド終了準備
			if ( *this->m_bEndFlag1 ) {
				// End関数内で無限ループに入らないようにtrueにする
				*this->m_bEndFlag2 = true;
				break;
			}
		}

		// パケット初期化
		len = 0;

		packet.m_cType = NON_PACKET;
		packet.m_nLen = 0;
		packet.m_nData = 0;
		packet.m_nSender = 0;
		packet.m_nSendTo = 0;
		
		// パケットが送られてくるのを待つ
		len = recv( this->m_RecvSocket, (char *)&packet, PACKET_SIZE, 0 );

		// 送られてきた最初のパケットの場合の処理
		if ( packet.m_cType == START_PACKET ) {
			// パケットのデータ部分にスレッドNOを格納（このNOはそのままクライアントIDになる。さらにクライアントIDで送信宛を判断）
			packet.m_nData = this->m_nThreadNo;
		}
		// 通信終了パケットが送られてきた場合
		else if ( packet.m_cType == END_PACKET ) {
		
			// データ部に送信先を格納
			packet.m_nData = packet.m_nSendTo;
			// 送信先を送信元にする
			packet.m_nSendTo = packet.m_nSender;

		}

		// パケットのコピー
		this->m_Packet = packet;

		{
			boost::mutex::scoped_lock lk(this->m_cNetWork->OutMutex);
		
			// パケット用キューにPUSH（キューの後ろから）
			this->m_cNetWork->setBackQueue(this->m_Packet);

			cout << "受信パケット情報 タイプ = " << packet.m_cType << " データ部 = " << packet.m_nData << endl;
			cout << "送信者 ID = " << packet.m_nSender << " 送信先 ID = " << packet.m_nSendTo << endl;			
			cout << "受信バイト数 = " << len << endl;
		}

		// 通信終了パケットの場合はスレッドを終了させる
		if ( packet.m_cType == END_PACKET ) {

			boost::mutex::scoped_lock lk(this->m_cNetWork->OutMutex);

			cout << "クライアントID = " << packet.m_nSender << " から終了通知を受けた（受信スレッド）" << endl;

			// 無限ループにならないようにフラグをtrueにする
			*this->m_bEndFlag2 = true;

			break;
		
		}

		Sleep(10);

	}

}

/************************************************
	CRecvThreadクラス：終了処理関数
************************************************/

void CRecvThread::End()
{

	// まずEndFlag1をtrueにする
	{	
		boost::mutex::scoped_lock lk(this->m_cNetWork->RecvMutex);
		*this->m_bEndFlag1 = true;
	}

	// 受信スレッド関数ないでEndFlag2がtrueになるまで待つ
	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetWork->RecvMutex);

		// 送信スレッドが終了したら抜ける（EndFlag2がtrueになったら）
		if ( *this->m_bEndFlag2 ) {
			break;
		}

		// CPUのスリープ処理
		Sleep(100);

	}

	// Flagのdelete処理
	delete(this->m_bEndFlag1);
	delete(this->m_bEndFlag2);

	this->m_bEndFlag1 = NULL;
	this->m_bEndFlag2 = NULL;

}
