
#if !defined(___EXPAND___H)
#define ___EXPAND___H

/********************************************************
	�C���N���[�h�t�@�C��
********************************************************/

#include "../Winsprog/main.h"
#include "glext.h"

/********************************************************
	�}�N����`
********************************************************/

/********************************************************
	�O���[�o���ϐ��錾
********************************************************/

extern PFNGLARRAYELEMENTEXTPROC  wglSwapIntervalEXT;

extern PFNGLGENBUFFERSARBPROC    glGenBuffersARB;		// �o�b�t�@�Ǘ��ԍ��쐬
extern PFNGLBINDBUFFERARBPROC    glBindBufferARB;		// �o�b�t�@�Ǘ��ԍ��I��
extern PFNGLBUFFERDATAARBPROC    glBufferDataARB;		// �f�[�^�ǂݍ���
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;		// �o�b�t�@�폜

/********************************************************
	�֐��v���g�^�C�v�錾
********************************************************/

int Get_GLprocs( void );

#endif	// #if !defined(___EXPAND___H)
