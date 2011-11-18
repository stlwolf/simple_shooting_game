
/************************************************
	�w�b�_�[�t�@�C��
************************************************/

#include "CNetClient.h"

/************************************************
	CNetClient�N���X�F�R���X�g���N�^�֐�
************************************************/

CNetClient::CNetClient()
{

	// �����o�ϐ��̏�����
	this->m_SendSocket = INVALID_SOCKET;
	this->m_RecvSocket = INVALID_SOCKET;

	this->m_pSendThread = NULL;
	this->m_pRecvThread = NULL;

	// ���̎��_�ł̃N���C�A���gID��0
	this->m_nMyID = 0;
	
	// ���̎��_�ł̑ΐ푊��ID��0
	this->m_nEnemyID = 0;

	// �\�P�b�g�g�p�J�n����
	WSAStartup( MAKEWORD( 1, 1 ), &this->m_wsadata );

}

/************************************************
	CNetClient�N���X�F�f�X�g���N�^�֐�
************************************************/

CNetClient::~CNetClient()
{

	// �\�P�b�g���N���[�Y
	closesocket(this->m_RecvSocket);
	closesocket(this->m_SendSocket);

	this->m_SendSocket = INVALID_SOCKET;
	this->m_RecvSocket = INVALID_SOCKET;

	// �\�P�b�g�g�p�I������
	WSACleanup();

}

/************************************************
	CNetClient�N���X�FIP�t�@�C���ǂݍ��݊֐�
	
	�����P�FIP�A�h���X�������ꂽ�t�@�C���̃p�X
	�����Q�FIP�A�h���X���������ޔz��ւ̃|�C���^
************************************************/

bool CNetClient::ReadIPFile( char* filename, char *in_ip )
{

	FILE* fp;
	size_t fsize;
	char temp[20];

	// �w�肳�ꂽ�t�@�C�����e�L�X�g���[�h�ŃI�[�v��
	if ( ( fp = fopen( filename, "rt" ) ) == NULL ) { return false; }

	// fseek�֐����g���ăt�@�C���|�C���^���Ō���Ɏ����Ă���
	fseek( fp, 0, SEEK_END );
	
	// ftell�֐����g���ăt�@�C���̐擪����t�@�C���|�C���^�̈ʒu�܂ł̕������𐔂���
	fsize = ftell( fp );
	
	// �t�@�C���|�C���^��擪�ɖ߂�
	fseek( fp, 0, SEEK_SET );

	// �t�@�C������IP�A�h���X��ǂݍ��ށi???.???.???.???�j
	fsize = fread( temp, sizeof(char), fsize, fp );

	// ������\0��}��
	temp[fsize] = '\0';

	// �ǂݍ���IP�A�h���X���R�s�[
	strcpy( in_ip, temp );

	fclose( fp );

	return true;

}

/************************************************
	CNetClient�N���X�F�T�[�o�ڑ��֐�
************************************************/

bool CNetClient::ServerConnect()
{

	bool ret;
	char ip[20];

	// IP�t�@�C������IP�A�h���X���擾
	ret= this->ReadIPFile( "./DAT/ip.txt", ip );

	if ( ret == false ) {
		return false;
	}

	// �\�P�b�g�ʐM�p�ϐ�
	// ���M�p�Ǝ�M�p
	struct sockaddr_in rserver;
	struct sockaddr_in sserver;

	// 0�ŏ�����
	memset( (char *)&rserver, 0, sizeof(rserver) );
	memset( (char *)&sserver, 0, sizeof(sserver) );

	// �\�P�b�g�̍쐬
	this->m_SendSocket = socket( AF_INET, SOCK_STREAM, 0 );
	this->m_RecvSocket = socket( AF_INET, SOCK_STREAM, 0 );

	// �ڑ���w��p�\���̂̏���
	sserver.sin_family  = AF_INET;
	sserver.sin_port	= htons(50000);
	sserver.sin_addr.s_addr = inet_addr(ip);
	
	// �Œ��
	//sserver.sin_addr.s_addr = inet_addr("192.168.2.2");

	rserver.sin_family  = AF_INET;
	rserver.sin_port	= htons(50001);

	rserver.sin_addr.s_addr = inet_addr(ip);
	
	// �Œ��
	//rserver.sin_addr.s_addr = inet_addr("192.168.2.2");

	char debug_string_buf[100];
	sprintf( debug_string_buf, "�ڑ��J�n...\n" );
	OutputDebugString( debug_string_buf );

	// �T�[�o��50000�|�[�g�̎�t�ɐڑ��v��
	if ( connect( this->m_SendSocket, (struct sockaddr *)&sserver, sizeof(sserver) ) != 0 ) {

		// �ڑ����s�\�ȏꍇ��False��Ԃ�
		char debug_string_buf[100];
		sprintf( debug_string_buf, "���M�\�P�b�g�E�R�l�N�g���s" );
		OutputDebugString( debug_string_buf );

		return false;
		
	}

	// �T�[�o��50001�|�[�g�̎�t�ɐڑ��v��
	if ( connect( this->m_RecvSocket, (struct sockaddr *)&rserver, sizeof(rserver) ) != 0 ) {

		// �ڑ����s�\�ȏꍇ��False��Ԃ�
		char debug_string_buf1[100];
		sprintf( debug_string_buf1, "��M�\�P�b�g�E�R�l�N�g���s" );
		OutputDebugString( debug_string_buf1 );

		return false;
		
	}

	char debug_string_buf2[100];
	sprintf( debug_string_buf2, "�ڑ�����...\n" );
	OutputDebugString( debug_string_buf2 );

	// ���M�X���b�h�N���X���C���X�^���X��
	this->m_pSendThread = new CSendThread(this);

	// ��M�X���b�h�N���X���C���X�^���X��
	this->m_pRecvThread = new CRecvThread(this);

	// �X���b�h�J�n
	boost::thread thread1(*this->m_pSendThread);
	boost::thread thread2(*this->m_pRecvThread);

	return true;

}

/************************************************
	CNetClient�N���X�F�N���C�A���g�I�������֐�
************************************************/

void CNetClient::End()
{

	if ( this->m_pSendThread != NULL ) {

		// delete�O�̏I������
		this->m_pSendThread->End();
	
		// �X���b�h��delete
		delete(this->m_pSendThread);

		this->m_pSendThread = NULL;

	}

	if ( this->m_pRecvThread != NULL ) {

		// delete�O�̏I������
		this->m_pRecvThread->End();

		// �X���b�h��delete
		delete(this->m_pRecvThread);

		this->m_pRecvThread = NULL;

	}

}

/************************************************
	CSendThread�N���X�F�R���X�g���N�^�֐�
************************************************/

CSendThread::CSendThread( CNetClient *in_client )
{

	// �����o�ϐ�������
	this->m_cNetClient = in_client;
	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;
	
	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;

	// �ʐM�J�n�̃p�P�b�g�Ƃ��ď�����
	this->m_pPacket.m_nType = START_PACKET;
	this->m_pPacket.m_nLen = PACKET_SIZE;
	this->m_pPacket.m_nData = 0;
	this->m_pPacket.m_nSender = this->m_cNetClient->getMyID();
	this->m_pPacket.m_nSendTo = 0;

}

/************************************************
	CSendThread�N���X�F�f�X�g���N�^�֐�
************************************************/

CSendThread::~CSendThread()
{
}

/************************************************
	CSendThread�N���X�F���M�X���b�h�֐�
************************************************/

void CSendThread::operator ()()
{

	int len = 0;
	
	int target = MAX_TARGET;

	// �ŏ��̃p�P�b�g(START_PACKET)�𑗂�i����ɑΉ����āA�T�[�o���N���C�A���gID��ʒm�j
	len = send( this->m_cNetClient->getSendSocket(), (char*)&this->m_pPacket, this->m_pPacket.m_nLen, 0 );

	while ( true ) {

		{
			// mutex�I�u�W�F�N�g�ɂ�郍�b�N
			boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);

			// End()�֐������s����Ă����EndFlag1��true�ɂȂ��Ă���
			if ( *this->m_bEndFlag1 ) {
				// End�֐����Ŗ������[�v�ɓ���Ȃ��悤��EndFlag2��true�ɂ���
				*this->m_bEndFlag2 = true;
				break;			
			}
		}
		
		// ���@�̃^�[�Q�b�g����0�ɂȂ邩�i�p�P�b�g���^�[�Q�b�g���������target��0�ɂȂ�j
		// ����̃^�[�Q�b�g����������Ԃł����s
		if ( target == 0 || g_Ereamap.m_EnemyTargetNum == 99 ) {
			
			// ���@�̃^�[�Q�b�g��0�ɂȂ�����ɒl�������̒l�ɖ߂��Ă����Ȃ�Atarget���������A
			if ( g_Ereamap.m_TargetNum == MAX_TARGET ) {
				target = MAX_TARGET;
			}
				
		} 
		
		// ���݂̃^�[�Q�b�g���������Ă�������s
		if ( g_Ereamap.m_TargetNum < target ) {

			// mutex�Ń��b�N
			boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);

			// �f�[�^�p�P�b�g�����
			this->m_pPacket.m_nType = DATA_PACKET;
			// ���@�̎c��^�[�Q�b�g�����f�[�^���Ɋi�[
			this->m_pPacket.m_nData = g_Ereamap.m_TargetNum;
			// �N���C�A���gID�𑗐M���ɐݒ�
			this->m_pPacket.m_nSender = this->m_cNetClient->getMyID();
			// �ΐ푊���ID�𑗐M��ɐݒ�
			this->m_pPacket.m_nSendTo = this->m_cNetClient->getEnemyID();

			// �c��̃^�[�Q�b�g����ʒm
			len = send( this->m_cNetClient->getSendSocket(), (char*)&this->m_pPacket, this->m_pPacket.m_nLen, 0 );

			// �^�[�Q�b�g�J�E���^���}�C�i�X
			target--;

		}

		Sleep(100);

	}

}

/************************************************
	CSendThread�N���X�F�I�������֐�
************************************************/

void CSendThread::End()
{

	{

		PACKET packet;

		// �ʐM�I���p�P�b�g���쐬
		packet.m_nType = END_PACKET;
		packet.m_nSender = this->m_cNetClient->getMyID();
		packet.m_nSendTo = this->m_cNetClient->getEnemyID();
		packet.m_nLen = PACKET_SIZE;
		packet.m_nData = 99;

		// �I���p�P�b�g�𑗐M		
		send( this->m_cNetClient->getSendSocket(), (char*)&packet, PACKET_SIZE, 0 );

	}

	{	
		boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);
	
		// EndFlag1��true�ɂ��āA���M�X���b�h�̏I�������Ɉڍs�\�ɂ���
		*this->m_bEndFlag1 = true;
	}

	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetClient->SendMutex);

		// ���M�X���b�h����EndFlag2��true�ɂȂ�܂Ń��[�v
		if ( *this->m_bEndFlag2 ) {
			break;
		}

		// CPU�̃X���[�v����
		Sleep(100);

	}

	// Flag��delete����
	delete(this->m_bEndFlag1);
	delete(this->m_bEndFlag2);

	this->m_bEndFlag1 = NULL;
	this->m_bEndFlag2 = NULL;

}

/************************************************
	CRecvThread�N���X�F�R���X�g���N�^�֐�
************************************************/

CRecvThread::CRecvThread( CNetClient *in_client )
{

	// �����o�ϐ�������
	this->m_cNetClient = in_client;
	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;

	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;
	
}

/************************************************
	CRecvThread�N���X�F�f�X�g���N�^�֐�
************************************************/

CRecvThread::~CRecvThread()
{
}

/************************************************
	CRecvThread�N���X�F��M�X���b�h�֐�
************************************************/

void CRecvThread::operator ()()
{

	int len = 0;;
	PACKET packet; 

	while ( true ) {

		{
			// mutex�I�u�W�F�N�g�ɂ�郍�b�N
			boost::mutex::scoped_lock lk(this->m_cNetClient->RecvMutex);

			// End()�֐������s����Ă����EndFlag1��true�ɂȂ��Ă���
			if ( *this->m_bEndFlag1 ) {
				// End�֐����Ŗ������[�v�ɓ���Ȃ��悤��EndFlag2��true�ɂ���
				*this->m_bEndFlag2 = true;
				break;		
			}
		}

		{

			// �T�[�o����p�P�b�g����M
			len = recv( this->m_cNetClient->getRecvSocket(), (char *)&packet, PACKET_SIZE, 0 );

			// �T�[�o����̃p�P�b�g���X�^�[�g�p�P�b�g���ɂ̓N���C�A���gID�Ƒΐ푊��ID��ݒ�
			if ( packet.m_nType == START_PACKET ) {

				{
					boost::mutex::scoped_lock lk(this->m_cNetClient->OutMutex);

					// �p�P�b�g�̑��M����擾
					int id = packet.m_nSendTo;

					// �T�[�o���瑗���Ă������M����N���C�A���gID�ɂ���
					this->m_cNetClient->setMyID(id);

					// ������ID����ΐ푊���ID�����߂�
					if ( id % 2 == 1 ) {
					
						// �N���C�A���gID����Ȃ�ΐ푊��ID���{�P�̒l�ɐݒ�
						id++; 
						this->m_cNetClient->setEnemyID(id);

					}
					else {

						// �N���C�A���gID�������Ȃ�ΐ푊��ID���|�P�̒l�ɐݒ�
						id--;
						this->m_cNetClient->setEnemyID(id);

					}
				}

			}
			// �����Ă����p�P�b�g���ʐM�I���p�P�b�g�̏ꍇ
			else if ( packet.m_nType == END_PACKET ) {

				// mutex�I�u�W�F�N�g�Ń��b�N
				boost::mutex::scoped_lock lk(this->m_cNetClient->OutMutex);

				char debug_string_buf2[100];
				sprintf( debug_string_buf2, "��M�X���b�h�I�������ɓ���\n" );
				OutputDebugString( debug_string_buf2 );
			
				// End�֐��Ŗ������[�v�ɓ���Ȃ��悤��EndFlag2��true�ɂ���
				*this->m_bEndFlag2 = true;

				// �X���b�h���I��������
				break;

			}
			else {

				// mutex�I�u�W�F�N�g�Ń��b�N
				boost::mutex::scoped_lock lk(this->m_cNetClient->OutMutex);

				// �f�[�^�����̃^�[�Q�b�g�����擾
				int target = packet.m_nData;

				// �ΐ푊��̎c��^�[�Q�b�g���ɐݒ�
				g_Ereamap.m_EnemyTargetNum = target;

#if 0
				char debug_string_buf2[100];
				sprintf( debug_string_buf2, "��M�p�P�b�g �^�C�v = %d, ���M�� = %d, ���M�� = %d\n", packet.m_nType, packet.m_nSender, packet.m_nSendTo );
				OutputDebugString( debug_string_buf2 );
#endif

			}
		}

	}
				
}

/************************************************
	CRecvThread�N���X�F�I�������֐�
************************************************/

void CRecvThread::End()
{

	{	
		boost::mutex::scoped_lock lk(this->m_cNetClient->RecvMutex);

		// EndFlag1��true�ɂ��āA���M�X���b�h�̏I�������Ɉڍs�\�ɂ���		
		*this->m_bEndFlag1 = true;
	}

	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetClient->RecvMutex);
		
		// ��M�X���b�h����EndFlag2��true�ɂȂ�܂Ń��[�v
		if ( *this->m_bEndFlag2 ) {
			break;
		}

		// CPU�̃X���[�v����
		Sleep(100);

	}

	// Flag��delete����
	delete(this->m_bEndFlag1);
	delete(this->m_bEndFlag2);

	this->m_bEndFlag1 = NULL;
	this->m_bEndFlag2 = NULL;

}
