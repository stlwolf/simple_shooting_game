
/************************************************
	�w�b�_�[�t�@�C��
************************************************/

#include "./CNetwork.h"

/************************************************
	���O���
************************************************/

using namespace std;

/************************************************
	CNetWork�N���X�F�R���X�g���N�^�֐�
************************************************/

CNetWork::CNetWork()
{

	// �����o�ϐ�������
	this->m_continuing = true;

	for ( int ct = 0; ct < CNetWork::MAX_SOCKET; ct++ ) {
		
		this->m_SendSocket[ct] = INVALID_SOCKET;
		this->m_RecvSocket[ct] = INVALID_SOCKET;

		this->m_cSendThread[ct] = NULL;
		this->m_cRecvThread[ct] = NULL;

	}

	char buf[1024];

	cout << "SERVER PROGRAM START" << endl;

	// �\�P�b�g�g�p�J�n����
	WSAStartup( MAKEWORD( 1, 1 ), &this->m_wsadata );

	// �z�X�g���擾
	gethostname( buf, sizeof(buf) );

	// �z�X�g������IP�A�h���X���擾
	this->m_serverip = gethostbyname(buf);

	// IP�A�h���X�̕\��
	cout << "SERVER IP ADDRESS : ";
	printf( "%u.%u.%u.%u \n", (unsigned char)this->m_serverip->h_addr_list[0][0],
							  (unsigned char)this->m_serverip->h_addr_list[0][1],
							  (unsigned char)this->m_serverip->h_addr_list[0][2],
							  (unsigned char)this->m_serverip->h_addr_list[0][3] );

}

/************************************************
	CNetWork�N���X�F�f�X�g���N�^�֐�
************************************************/

CNetWork::~CNetWork()
{

	// �\�P�b�g�����
	for ( int ct = 1; ct < MAX_SOCKET; ct++ ) {

		// �\�P�b�g�z�������łȂ����̂�������
		if ( this->m_RecvSocket[ct] != INVALID_SOCKET ) {
			closesocket(this->m_RecvSocket[ct]);
			this->m_RecvSocket[ct] = INVALID_SOCKET;
		}

		// ��L�Ɠ���
		if ( this->m_SendSocket[ct] != INVALID_SOCKET ) {
			closesocket(this->m_SendSocket[ct]);
			this->m_SendSocket[ct] = INVALID_SOCKET;
		}

	}

	// �󂯕t���p�̃\�P�b�g�����
	closesocket(this->m_RecvSocket[0]);
	closesocket(this->m_SendSocket[0]);

	// �e�X���b�h�̍ŏI�I������
	for ( int ct = 0; ct < MAX_SOCKET; ct++ ) {

		// �X���b�h�N���X�z�������łȂ����̂�������
		if ( this->m_cSendThread[ct] != NULL ) {
			this->m_cSendThread[ct]->End();
			delete (this->m_cSendThread[ct]);
		}

		// ��L�ɓ���
		if ( this->m_cRecvThread[ct] != NULL ) {
			this->m_cRecvThread[ct]->End();
			delete (this->m_cRecvThread[ct]);
		}

	}

	// �\�P�b�g�I������
	WSACleanup();

}

/************************************************
	CNetWork�N���X�F�ڑ��v����t�֐�
************************************************/

bool CNetWork::ListenStart()
{

		// ���[�J���ϐ�
		int len, cheak;

		SOCKET rclient;
		SOCKET sclient;

		struct sockaddr_in rsaddr;
		struct sockaddr_in rcaddr;
		struct sockaddr_in ssaddr;
		struct sockaddr_in scaddr;

		// ��M�\�P�b�g�̍쐬
		if ( ( this->m_RecvSocket[0] = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
			return false;
		}

		// ���M�\�P�b�g�̍쐬
		if ( ( this->m_SendSocket[0] = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
			return false;
		}

		// 0�ŏ�����
		memset( (char *)&rsaddr, 0, sizeof(rsaddr) );
		memset( (char *)&rcaddr, 0, sizeof(rcaddr) );
		memset( (char *)&ssaddr, 0, sizeof(ssaddr) );
		memset( (char *)&scaddr, 0, sizeof(scaddr) );

		// �o�C���h�O�̏���
		rsaddr.sin_family = AF_INET;
		rsaddr.sin_addr.s_addr = INADDR_ANY;
		rsaddr.sin_port = htons(50000);

		ssaddr.sin_family = AF_INET;
		ssaddr.sin_addr.s_addr = INADDR_ANY;
		ssaddr.sin_port = htons(50001);

		// ��M�\�P�b�g���o�C���h
		if ( ( bind( this->m_RecvSocket[0], (struct sockaddr *)&rsaddr, sizeof(rsaddr) ) ) == SOCKET_ERROR ) {
			return false;
		}

		// ���M�\�P�b�g���o�C���h
		if ( ( bind( this->m_SendSocket[0], (struct sockaddr *)&ssaddr, sizeof(ssaddr) ) ) == SOCKET_ERROR ) {
			return false;
		}

		// ��M�\�P�b�g�����X����Ԃɂ���
		if ( ( listen( this->m_RecvSocket[0], 10 ) ) == SOCKET_ERROR ) {
			return false;
		}

		// ���M�\�P�b�g�����X����Ԃɂ���
		if ( ( listen( this->m_SendSocket[0], 10 ) ) == SOCKET_ERROR ) {
			return false;
		}

		{
			boost::mutex::scoped_lock lk(this->OutMutex);
			cout << "�\�P�b�g�ڑ��v���̎�t�J�n" << endl;
		}

		while ( this->m_continuing ) {

			// �T�C�Y���擾
			len = sizeof(rcaddr);
			
			// ����M�\�P�b�g�̐ڑ�����p�t���O
			cheak = 0; 

			// ��M��t�\�P�b�g�ւ̐ڑ���҂�
			rclient = accept( this->m_RecvSocket[0], (struct sockaddr *)&rcaddr, &len );

			// �G���[�����i�Ȃ�炩�̗��R�Ŏ�t�����s�j
			if ( rclient == INVALID_SOCKET ) {
				return false;
			}

			// �T�C�Y���擾
			len = sizeof(rcaddr);

			// ���M��t�\�P�b�g�ւ̐ڑ���҂�
			sclient = accept( this->m_SendSocket[0], (struct sockaddr *)&scaddr, &len );

			// �G���[�����i�Ȃ�炩�̗��R�Ŏ�t�����s
			if ( sclient == INVALID_SOCKET ) {
				return false;
			}

			// �\�P�b�g�z����̋�̕�����T���ă��[�v
			for ( int ct = 1; ct < CNetWork::MAX_SOCKET; ct++ ) {

				// �󂯕t��������M�\�P�b�g����t�\�P�b�g����؂�ւ���
				if ( this->m_RecvSocket[ct] == INVALID_SOCKET ) {

					boost::mutex::scoped_lock lk(this->OutMutex);

					cout << "��M�\�P�b�g�ڑ��m�F �\�P�b�g�ԍ� = " << rclient << " �N���C�A���gNO = "  << ct << endl;
					
					// �\�P�b�g�̃R�s�[
					this->m_RecvSocket[ct] = rclient;

					// �X���b�h�����
					this->m_cRecvThread[ct] = new CRecvThread( this, rclient, ct );

					// �`�F�b�N�p�ϐ����v���X�P
					cheak++;

				}

				if ( this->m_SendSocket[ct] == INVALID_SOCKET ) {

					boost::mutex::scoped_lock lk(this->OutMutex);

					cout << "���M�\�P�b�g�ڑ��m�F �\�P�b�g�ԍ� = " << sclient << " �N���C�A���gNO = "  << ct << endl;
					
					// �\�P�b�g�̃R�s�[
					this->m_SendSocket[ct] = sclient;

					// �X���b�h�����
					this->m_cSendThread[ct] = new CSendThread( this, sclient, ct );

					// �`�F�b�N�p�ϐ����v���X�P
					cheak++;

				}

				// �\�P�b�g�ڑ��E�X���b�h�����������������m�F
				if ( cheak == 2 ) { 
				
					// ����M�X���b�h�̊J�n
					boost::thread thread1(*this->m_cRecvThread[ct]);
					boost::thread thread2(*this->m_cSendThread[ct]);

					break; 

				}
				// �Ȃ�炩�̃G���[�ő���M�p�̃\�P�b�g�̂ǂ��炩�����s
				else if ( cheak == 1 ) {
					return false;
				}

			}
		
		}

		// ��{�I�Ɏ�t���͊O�ɂ͖߂�Ȃ�
		return true;

}

/************************************************
	CSendThread�N���X�F�R���X�g���N�^�֐�

	�����P�F�e�l�b�g���[�N�N���X�ւ̃|�C���^
	�����Q�F�����̑��M�p�\�P�b�g�ԍ�
	�����R�F�����̃X���b�hNO
************************************************/

CSendThread::CSendThread( CNetWork *in_net, SOCKET in_socket, int in_no )
{

	// �����o�ϐ�������
	this->m_cNetWork = in_net;
	this->m_nThreadNo = in_no;

	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;

	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;

	this->m_SendSocket = in_socket; 

}

/************************************************
	CSendThread�N���X�F���M�X���b�h�֐�
************************************************/

void CSendThread::operator ()()
{

	PACKET packet;

	while ( true ) {

		{
			// mutex�I�u�W�F�N�g�ɂ�郍�b�N
			boost::mutex::scoped_lock lk(this->m_cNetWork->SendMutex);

			// End()�֐�����EndFlag1��true�ɂȂ��Ă�����X���b�h�I������
			if ( *this->m_bEndFlag1 ) {
				// End�֐����Ŗ������[�v�ɓ���Ȃ��悤��EndFlag2��true�ɂ���
				*this->m_bEndFlag2 = true;
				break;
			}
			
		}

		{
			boost::mutex::scoped_lock lk(this->m_cNetWork->OutMutex);

			// �p�P�b�g�p�L���[����łȂ���Ύ��s
			if ( !this->m_cNetWork->getQueue().empty() ) {

				// �L���[�̐擪�����o��
				packet = this->m_cNetWork->getQueue().front();

				// ���o�����p�P�b�g���ŏ��̃p�P�b�g�̏ꍇ�i���M������߂�p�P�b�g�Ȃ̂ōŏ��Ɋm�F�j
				if ( packet.m_cType == START_PACKET ) {

					// �X���b�hNO�������ꍇ�Ɏ������̃p�P�b�g�Ɣ��f
					// �p�P�b�g�̃f�[�^�����Ɋi�[����Ă���̂��X���b�hNO�Ɠ����ꍇ�iRecv�X���b�h�Ńf�[�^�����ɃX���b�hNO���i�[���Ă�j
					if ( packet.m_nData == this->m_nThreadNo ) {

						// ���M��ɃX���b�h�ԍ����i�[�B�N���C�A���g���ł�START_PACKET�̏ꍇ�A���M����N���C�A���gID�ɂ���
						packet.m_cType = START_PACKET;
						packet.m_nLen = PACKET_SIZE;
						// ���M��i���������ł̓N���C�A���gID�ɂȂ�j�ɃX���b�hNO���i�[�i�Ή��������s���j
						packet.m_nSendTo = this->m_nThreadNo;

						cout << "�N���C�A���gID��ʒm ID = " << packet.m_nSendTo << endl;

						// �N���C�A���g�Ƀp�P�b�g�𑗐M
						int len = send( this->m_SendSocket, (char *)&packet, PACKET_SIZE, 0 );

						// �p�P�b�g�p�L���[����擪�̃p�P�b�g���폜
						this->m_cNetWork->PopFrontQueue();

					}

				}
				else {
				
					// �L���[������o�����p�P�b�g�̑��M�悪�����̃X���b�hNO�Ɠ����Ȃ珈�����s��
					if ( packet.m_nSendTo == this->m_nThreadNo ) {
						
						// �p�P�b�g���f�[�^�p�P�b�g�Ȃ珈��
						if ( packet.m_cType == DATA_PACKET ) {

							// ���M��ɂ��̂܂ܑ���
							send( this->m_SendSocket, (char *)&packet, PACKET_SIZE, 0 );

							cout << "�X���b�hNO = " << this->m_nThreadNo << " �f�[�^�p�P�b�g = " << packet.m_nSender << " ���� " << packet.m_nSendTo << " ���� "<< endl;
							cout << "�f�[�^���e = " << packet.m_nData << endl; 

							// �擪�̃p�P�b�g���폜
							this->m_cNetWork->PopFrontQueue();

						}

						// �p�P�b�g���ʐM�I���p�P�b�g�Ȃ珈��
						if ( packet.m_cType == END_PACKET ){
							
							// ���M��ɂ��̂܂ܑ���
							send( this->m_SendSocket, (char *)&packet, PACKET_SIZE, 0 );

							// �p�P�b�g�p�L���[�̐擪�̃p�P�b�g���폜
							this->m_cNetWork->PopFrontQueue();

							cout << "�X���b�hNO = " << this->m_nThreadNo << " �I�����������s�i���M�X���b�h�j" << endl;

							// �������[�v�ɂȂ�Ȃ��悤�Ƀt���O��true�ɂ���
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
	CSendThread�N���X�F�I�������֐�
************************************************/

void CSendThread::End()
{

	// �܂�EndFlag1��true�ɂ���
	{	
		boost::mutex::scoped_lock lk(this->m_cNetWork->SendMutex);
		*this->m_bEndFlag1 = true;
	}

	// ���M�X���b�h�֐��Ȃ���EndFlag2��true�ɂȂ�܂ő҂�
	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetWork->SendMutex);

		// ���M�X���b�h���I�������甲����iEndFlag2��true�ɂȂ�����j
		if ( this->m_bEndFlag2 ) {
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

	�����P�F�e�l�b�g���[�N�N���X�ւ̃|�C���^
	�����Q�F�����̎�M�p�\�P�b�g�ԍ�
	�����R�F�����̃X���b�hNO
************************************************/

CRecvThread::CRecvThread( CNetWork *in_net, SOCKET in_socket, int in_no )
{

	// �����o�ϐ�������
	this->m_cNetWork = in_net;
	this->m_nThreadNo = in_no;

	this->m_bEndFlag1 = new bool;
	this->m_bEndFlag2 = new bool;

	*this->m_bEndFlag1 = false;
	*this->m_bEndFlag2 = false;

	this->m_RecvSocket = in_socket;

}

/************************************************
	CRecvThread�N���X�F��M�X���b�h�֐�
************************************************/

void CRecvThread::operator()()
{

	int len;	
	PACKET packet;

	while ( true ) {

		{
			// mutex�I�u�W�F�N�g�ɂ�郍�b�N
			boost::mutex::scoped_lock lk(this->m_cNetWork->RecvMutex);

			// End()�֐�����EndFlag1��true�ɂȂ��Ă�����X���b�h�I������
			if ( *this->m_bEndFlag1 ) {
				// End�֐����Ŗ������[�v�ɓ���Ȃ��悤��true�ɂ���
				*this->m_bEndFlag2 = true;
				break;
			}
		}

		// �p�P�b�g������
		len = 0;

		packet.m_cType = NON_PACKET;
		packet.m_nLen = 0;
		packet.m_nData = 0;
		packet.m_nSender = 0;
		packet.m_nSendTo = 0;
		
		// �p�P�b�g�������Ă���̂�҂�
		len = recv( this->m_RecvSocket, (char *)&packet, PACKET_SIZE, 0 );

		// �����Ă����ŏ��̃p�P�b�g�̏ꍇ�̏���
		if ( packet.m_cType == START_PACKET ) {
			// �p�P�b�g�̃f�[�^�����ɃX���b�hNO���i�[�i����NO�͂��̂܂܃N���C�A���gID�ɂȂ�B����ɃN���C�A���gID�ő��M���𔻒f�j
			packet.m_nData = this->m_nThreadNo;
		}
		// �ʐM�I���p�P�b�g�������Ă����ꍇ
		else if ( packet.m_cType == END_PACKET ) {
		
			// �f�[�^���ɑ��M����i�[
			packet.m_nData = packet.m_nSendTo;
			// ���M��𑗐M���ɂ���
			packet.m_nSendTo = packet.m_nSender;

		}

		// �p�P�b�g�̃R�s�[
		this->m_Packet = packet;

		{
			boost::mutex::scoped_lock lk(this->m_cNetWork->OutMutex);
		
			// �p�P�b�g�p�L���[��PUSH�i�L���[�̌�납��j
			this->m_cNetWork->setBackQueue(this->m_Packet);

			cout << "��M�p�P�b�g��� �^�C�v = " << packet.m_cType << " �f�[�^�� = " << packet.m_nData << endl;
			cout << "���M�� ID = " << packet.m_nSender << " ���M�� ID = " << packet.m_nSendTo << endl;			
			cout << "��M�o�C�g�� = " << len << endl;
		}

		// �ʐM�I���p�P�b�g�̏ꍇ�̓X���b�h���I��������
		if ( packet.m_cType == END_PACKET ) {

			boost::mutex::scoped_lock lk(this->m_cNetWork->OutMutex);

			cout << "�N���C�A���gID = " << packet.m_nSender << " ����I���ʒm���󂯂��i��M�X���b�h�j" << endl;

			// �������[�v�ɂȂ�Ȃ��悤�Ƀt���O��true�ɂ���
			*this->m_bEndFlag2 = true;

			break;
		
		}

		Sleep(10);

	}

}

/************************************************
	CRecvThread�N���X�F�I�������֐�
************************************************/

void CRecvThread::End()
{

	// �܂�EndFlag1��true�ɂ���
	{	
		boost::mutex::scoped_lock lk(this->m_cNetWork->RecvMutex);
		*this->m_bEndFlag1 = true;
	}

	// ��M�X���b�h�֐��Ȃ���EndFlag2��true�ɂȂ�܂ő҂�
	while ( true ) {

		boost::mutex::scoped_lock lk(this->m_cNetWork->RecvMutex);

		// ���M�X���b�h���I�������甲����iEndFlag2��true�ɂȂ�����j
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
