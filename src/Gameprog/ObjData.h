#if !defined(__OBJDATA__H)
#define __OBJDATA__H

/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "../Winsprog/MatrixCal.h"

/**********************************************************
	�I�u�W�F�N�g�f�[�^�p�\����
**********************************************************/

// ���_���W
typedef struct {
	float m_x, m_y, m_z;
} OBJ_VERTEX;

// �@�����W
typedef struct {
	float m_x, m_y, m_z;
} OBJ_NORMAL;

// ���_�J���[
typedef struct {
    float m_R, m_G, m_B, m_A;
} OBJ_COLOR;

// �e�N�X�`�����W
typedef struct {
    float m_s, m_t;
} OBJ_TEXTURE;

// �|���S�����i�P�̃|���S�������e�\�������C���f�b�N�X�ԍ��Ŏw��ێ��j
typedef struct {
    int   m_v[3];				// ���_�C���f�b�N�X
    int   m_n[3];				// �@���C���f�b�N�X
    int   m_c[3];				// �J���[�C���f�b�N�X
    int   m_t[3];				// �e�N�X�`���C���f�b�N�X
} OBJ_POLY;

// �I�u�W�F�N�g�f�[�^�\���́i�P�̃I�u�W�F�N�g���\��������̃A�h���X��ێ��H�j
typedef struct {
	int		    m_Polcnt;		// �\���|���S����
	OBJ_VERTEX  *pObj_v;		// ���_�o�b�t�@
	OBJ_NORMAL  *pObj_n;		// �@���o�b�t�@
	OBJ_COLOR   *pObj_c;		// �J���[�o�b�t�@
	OBJ_TEXTURE *pObj_t;		// �e�N�X�`���o�b�t�@
	OBJ_POLY	*pObj_pol;		// �|���S���\�����o�b�t�@
} OBJ_VARIABLE;

typedef struct {
	int		    m_Polcnt;		// �\���|���S����
	OBJ_VERTEX  *pObj_v;		// ���_�o�b�t�@
	OBJ_NORMAL  *pObj_n;		// �@���o�b�t�@
	OBJ_COLOR   *pObj_c;		// �J���[�o�b�t�@
	OBJ_TEXTURE *pObj_t;		// �e�N�X�`���o�b�t�@
} OBJ_VERTEX_BUF;

typedef struct {
    int           m_nPolmode;	// �|���S���̃��[�h 
    int           m_nPolcnt;	// �|���S����
    int           m_nDatacnt;   // ���_�f�[�^��
    int           m_nObjsize;	// �o�b�t�@�f�[�^�T�C�Y
    unsigned char *pcObjData;	// ���_���
} OBJ_VERTEX_BUF2;

typedef struct {
    union {
        struct {
            float m_x, m_y;
        };
        struct {
            float m_s, m_t;				// OpenGL=strq : GLSL=stpq
        };
        float   m_v[2];
    };
} VEC2;

typedef struct {
    union {
        struct {
            float m_x, m_y, m_z;
        };
        struct {
            float m_r, m_g, m_b;
        };
        struct {
            float m_s, m_t, m_p;		// OpenGL=strq : GLSL=stpq
        };
        float   m_v[3];
    };
} VEC3;

typedef struct {
    union {
        struct {
            float m_x, m_y, m_z, m_w;
        };
        struct {
            float m_r, m_g, m_b, m_a;
        };
        struct {
            float m_s, m_t, m_p, m_q;	// OpenGL=strq : GLSL=stpq
        };
        float   m_v[4];
    };
} VEC4;

typedef struct {
    unsigned char m_R, m_G, m_B, m_A;
} VEC4UB;

// ���_�z��t�H�[�}�b�g

typedef struct {
	VECTOR4 m_c;
    VECTOR3 m_n;
    VECTOR3 m_v;
} C4F_N3F_V3F;

#endif
