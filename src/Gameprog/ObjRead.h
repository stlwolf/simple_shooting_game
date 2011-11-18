#if !defined(___OBJREAD___H)
#define ___OBJREAD___H

/********************************************************
	�C���N���[�h�t�@�C��
********************************************************/

#include "../winsprog/main.h"
#include "ObjData.h"

/********************************************************
	�}�N����`
********************************************************/

/********************************************************
	�O���[�o���ϐ��錾
********************************************************/

extern char *g_pPolfile;					// �|���S���t�@�C���f�[�^

extern OBJ_VARIABLE	   *g_pObj;				// �C���f�b�N�X�^�@�|���S���I�u�W�F�N�g
extern OBJ_VERTEX_BUF  *g_pVerBuf;
extern OBJ_VERTEX_BUF2 *g_pVerBuf2;

/********************************************************
	�֐��v���g�^�C�v�錾
********************************************************/

extern OBJ_VARIABLE *Load_ObjFile( char * );
extern char *Load_TextFile( char *, int * );
extern int Load_VertexData( char *, int, OBJ_VARIABLE * );
extern int BufChk_String( char *, char *, int );

extern OBJ_VERTEX_BUF2 *Set_VertexBuf( OBJ_VARIABLE * );

extern int Obj_Error1( void );

extern void Free_ObjIndex( OBJ_VARIABLE * );
extern void Free_ObjVertex1( OBJ_VERTEX_BUF * );
extern void Free_ObjVertex2( OBJ_VERTEX_BUF2 * );


#endif	// #if !defined(___OBJREAD___H)
