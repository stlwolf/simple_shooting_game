#if !defined(__CNETCLIENT__H)
#define __CNETCLIENT__H

/************************************************
	�w�b�_�[�t�@�C��
************************************************/

// C�p�w�b�_�[
#include <stdio.h>

// winsock�p�ɕK�v�ȃw�b�_�ƃ��C�u����
#include <winsock.h>
#pragma comment( lib, "ws2_32.lib" )

// boost�֘A�w�b�_�[
#include <boost/ref.hpp>
#include <boost/thread.hpp>

#include "../Gameprog/Field.h"
#include "../Gameprog/Target.h"

// �N���X�錾
class CSendThread;
class CRecvThread;

/************************************************
	�񋓑̒�`
************************************************/

// �p�P�b�g�p�R�[�h
enum PACKET_TYPE : int {

	START_PACKET = 1,	// �ʐM�J�n�p�P�b�g
	END_PACKET,			// �ʐM�I���p�P�b�g
	DATA_PACKET,		// �ʏ�p�P�b�g

	NON_PACKET			// ���̑��̃p�P�b�g

};

/************************************************
	�\���̒�`
************************************************/

// �p�P�b�g�T�C�Y
#define PACKET_SIZE (20)

// �f�[�^����M�p�p�P�b�g
struct PACKET {
	
	// �p�P�b�g�̎��
	int m_nType;

	// �p�P�b�g�T�C�Y
	int m_nLen;

	// ���M��ID
	int m_nSender;

	// ��M��ID
	int m_nSendTo;

	// �f�[�^
	int m_nData;
	
};

/************************************************
	�N���X��`�F�l�b�g���[�N�ڑ��p�N���X
************************************************/

class CNetClient {

private:

	// winsock�p�ϐ�
	WSAData m_wsadata;	

	// ����M�p�\�P�b�g
	SOCKET m_RecvSocket;
	SOCKET m_SendSocket;

	// ����M�X���b�h�N���X�ւ̃|�C���^
	CSendThread *m_pSendThread;
	CRecvThread *m_pRecvThread;

	// �����̃N���C�A���gID
	int m_nMyID;

	// �ΐ푊���ID
	int m_nEnemyID;

public:

	// mutex�p�I�u�W�F�N�g
	boost::mutex SendMutex;
	boost::mutex RecvMutex;
	boost::mutex OutMutex;

	// �R���X�g���N�^�E�f�X�g���N�^�֐�
	CNetClient();
	~CNetClient();

	// �I�������֐�
	void End();

	// �T�[�o�ڑ��֐�
	bool ServerConnect();

	// IP�t�@�C���ǂݍ��݊֐�
	bool ReadIPFile( char *filename, char *in_ip );

	/* getter��setter */

	SOCKET getSendSocket() { return this->m_SendSocket; }
	SOCKET getRecvSocket() { return this->m_RecvSocket; }

	int getMyID() { return this->m_nMyID; } 
	void setMyID( int in_id ) { this->m_nMyID = in_id; } 

	int getEnemyID() { return this->m_nEnemyID; } 
	void setEnemyID( int in_id ) { this->m_nEnemyID = in_id; } 

	/* �����܂� */

};

/************************************************
	�N���X��`�F���M�p�X���b�h�N���X
************************************************/

class CSendThread {

private:

	// �l�b�g���[�N�N���C�A���g�N���X�ւ̃|�C���^
	CNetClient *m_cNetClient;

	// �I������
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

	PACKET m_pPacket;

public:

	// �֐��I�u�W�F�N�g��operator()()
	void operator()();

	// �R���X�g���N�^�E�f�X�g���N�^�֐�
	CSendThread( CNetClient* in_client );
	~CSendThread();

	// �I�������֐�
	void End();

};

/************************************************
	�N���X��`�F��M�p�X���b�h�N���X
************************************************/

class CRecvThread {

private:

	// �l�b�g���[�N�N���C�A���g�N���X�ւ̃|�C���^
	CNetClient *m_cNetClient;

	// �I������
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

public:

	// �֐��I�u�W�F�N�g��operator()()
	void operator()();

	// �R���X�g���N�^�E�f�X�g���N�^�֐�
	CRecvThread( CNetClient* in_client );
	~CRecvThread();

	// �I�������֐�
	void End();

};

#endif