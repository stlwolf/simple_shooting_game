
/**********************************************************
	�C���N���[�h�w�b�_�[
**********************************************************/

#include "Expand.h"

/**********************************************************
	�O���[�o���ϐ���`
**********************************************************/

PFNGLARRAYELEMENTEXTPROC  wglSwapIntervalEXT	  = NULL;

PFNGLGENBUFFERSARBPROC    glGenBuffersARB       = NULL;		// �o�b�t�@�Ǘ��ԍ��쐬
PFNGLBINDBUFFERARBPROC    glBindBufferARB       = NULL;		// �o�b�t�@�Ǘ��ԍ��I��
PFNGLBUFFERDATAARBPROC    glBufferDataARB       = NULL;		// �f�[�^�ǂݍ���
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB    = NULL;		// �o�b�t�@�폜

/**********************************************************
	�g���@�\�擾�֐�

	�@�����F�Ȃ�
	�߂�l�F������������TRUE��Ԃ��A���s���ɂ�FALSE��Ԃ��B

	OpenGL�̊g���֐����O���t�b�B�N�{�[�h���g�p�\�Ȃ��
	�擾��TRUE��Ԃ��B�����Ή����Ă��Ȃ��ꍇ�̓G���[��Ԃ��B
**********************************************************/

int Get_GLprocs()
{

	wglSwapIntervalEXT = (PFNGLARRAYELEMENTEXTPROC)wglGetProcAddress( "wglSwapIntervalEXT" );
	
	if ( !wglSwapIntervalEXT ) { return FALSE; }

    // �g���֐��ݒ�
    glGenBuffersARB    = (PFNGLGENBUFFERSARBPROC)    wglGetProcAddress( "glGenBuffersARB" );
    glBindBufferARB    = (PFNGLBINDBUFFERARBPROC)    wglGetProcAddress( "glBindBufferARB" );
    glBufferDataARB    = (PFNGLBUFFERDATAARBPROC)    wglGetProcAddress( "glBufferDataARB" );
    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC) wglGetProcAddress( "glDeleteBuffersARB" );
    
	if ( !glGenBuffersARB    ) { return FALSE; }
	if ( !glBindBufferARB    ) { return FALSE; }
	if ( !glBufferDataARB    ) { return FALSE; }
	if ( !glDeleteBuffersARB ) { return FALSE; }

    return TRUE;

}
