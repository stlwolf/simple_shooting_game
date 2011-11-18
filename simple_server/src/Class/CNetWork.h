#if !defined(__CNETWORK__H)
#define __CNETWORK__H

// �w�b�_�[�t�@�C��
#include "../main.h"

// winsock�ɕK�v�ȃw�b�_�ƃ��C�u����
#include <winsock.h>
#pragma comment( lib, "ws2_32.lib" )

// boost�֘A�w�b�_�[
#include <boost/ref.hpp>
#include <boost/thread.hpp>

// STL�w�b�_�[
#include <deque>

// ���O��Ԓ�`
using namespace std;

// �N���X�錾
class CNetWork;
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
	int m_cType;

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
	�N���X��`�F�l�b�g���[�N����M�p�N���X
************************************************/

class CNetWork {

private:

	// �ő�\�P�b�g��
	static const int MAX_SOCKET = 21;

	// �\�P�b�g�ʐM�p
	WSAData m_wsadata; 

	// �T�[�o��IP�A�h���X
	HOSTENT *m_serverip;

	// �����t���O
	volatile bool m_continuing;

	// ����M�p�\�P�b�g
	SOCKET m_SendSocket[MAX_SOCKET];
	SOCKET m_RecvSocket[MAX_SOCKET];

	// ����M�X���b�h�N���X
	CSendThread *m_cSendThread[MAX_SOCKET];
	CRecvThread *m_cRecvThread[MAX_SOCKET];

	// �p�P�b�g�p�L���[
	deque<PACKET> m_PacketQueue;

public:

	// mutex�I�u�W�F�N�g
	boost::mutex SendMutex;
	boost::mutex RecvMutex;
	boost::mutex OutMutex;

	// �R���X�g���N�^�E�f�X�g���N�^�֐�
	CNetWork();
	~CNetWork();

	// �ڑ��v����t�֐�
	bool ListenStart();

	/* getter��setter */

	SOCKET getSendSocket( int in_no ) { return this->m_SendSocket[in_no]; }
	SOCKET getRecvSocket( int in_no ) { return this->m_RecvSocket[in_no]; }
	
	deque<PACKET> getQueue() { return this->m_PacketQueue; }
	void setBackQueue( PACKET in_pack ) { this->m_PacketQueue.push_back(in_pack); }
	void setFrontQueue( PACKET in_pack ) { this->m_PacketQueue.push_front(in_pack); }
	void PopFrontQueue() { this->m_PacketQueue.pop_front(); }
	void PopBackQueue() { this->m_PacketQueue.pop_back(); }

	/* �����܂� */

};
	
/************************************************
	�N���X��`�F���M�p�X���b�h�N���X
************************************************/

class CSendThread {

private:

	// �e�̃l�b�g���[�N�N���X�ւ̃|�C���^
	CNetWork *m_cNetWork;

	// �X���b�h�ԍ�
	int m_nThreadNo;

	// �I������t���O
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

	// �����Ɋ��蓖�Ă�ꂽ���M�p�\�P�b�g�ԍ�
	SOCKET m_SendSocket;

	// �i�b��j�ŐV�̃p�P�b�g�p�ϐ�
	PACKET m_Packet;

public:

	// �֐��I�u�W�F�N�g��operator()()
	void operator()();

	// �R���X�g���N�^�E�f�X�g���N�^�֐�
	CSendThread( CNetWork* in_net, SOCKET in_socket, int in_no );

	// �I�������֐�
	void End();

};

/************************************************
	�N���X��`�F��M�p�X���b�h�N���X
************************************************/

class CRecvThread {

private:

	// �e�̃l�b�g���[�N�N���X�ւ̃|�C���^
	CNetWork *m_cNetWork;

	// �X���b�h�ԍ�
	int m_nThreadNo;

	// �I������t���O
	bool* m_bEndFlag1;
	bool* m_bEndFlag2;

	// �����Ɋ��蓖�Ă�ꂽ��M�p�\�P�b�g�ԍ�
	SOCKET m_RecvSocket;

	// �i�b��j�ŐV�̃p�P�b�g�p�ϐ�
	PACKET m_Packet;

public:

	// �֐��I�u�W�F�N�g��operator()()
	void operator()();

	// �R���X�g���N�^�E�f�X�g���N�^�֐�
	CRecvThread( CNetWork* in_net, SOCKET in_socket, int in_no );

	// �I�������֐�
	void End();

};

#endif