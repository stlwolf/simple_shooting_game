
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "ObjRead.h"

/**********************************************************
	�O���[�o���ϐ���`
**********************************************************/

char *g_pObjfile;						// �I�u�W�F�N�g�t�@�C���f�[�^

OBJ_VARIABLE	*g_pObj;				// �C���f�b�N�X�^�@�|���S���I�u�W�F�N�g
OBJ_VERTEX_BUF	*g_pVerBuf;
OBJ_VERTEX_BUF2 *g_pVerBuf2;			// ���_�o�b�t�@�^�@�|���S���I�u�W�F�N�g

/**********************************************************
	���_�o�b�t�@�R�s�[�֐�

	�@�����Fpol�f�[�^����ǂݍ��񂾊e�C���f�b�N�X���
			�i�[����Ă���OBJ_VARIABLE�^�ϐ��̃A�h���X
	�߂�l�F�e���_��񂪊i�[���ꂽOBJ_VERTEX_BUF2�^�f�[�^
			�̃A�h���X

	pol�f�[�^����擾�������_�C���f�b�N�X�������Ƃ�
	�I�u�W�F�N�g�̒��_�f�[�^��ݒ肷��֐��B�e�|���S������
	�ɒ��_�E�@��E���_�J���[�E�e�N�X�`���������o����
	OBJ_VERTEX_BUF2�^�f�[�^�Ɋi�[������ŕԂ��B
**********************************************************/

OBJ_VERTEX_BUF2 *Set_VertexBuf( OBJ_VARIABLE *pobj_data )
{

	int ndata_cnt;
	int nindex_no;

	OBJ_POLY *ppol_copy;				// �|���S���̃C���f�b�N�X���

	unsigned char *pcobj_copy;

	OBJ_VERTEX  *pobj_v0, *pobj_v1;		// ���_���
	OBJ_NORMAL  *pobj_n0, *pobj_n1;		// �@�����
	OBJ_COLOR   *pobj_c0, *pobj_c1;		// ���_�J���[���
	OBJ_TEXTURE *pobj_t0, *pobj_t1;		// �e�N�X�`�����

	// �|�C���^������
	g_pObj	   = NULL;
	g_pVerBuf  = NULL;
	g_pVerBuf2 = NULL;

	// �I�u�W�F�N�g���i�[�̈���擾�A�����ď�����
	g_pVerBuf2 = (OBJ_VERTEX_BUF2 *)malloc( sizeof(OBJ_VERTEX_BUF2 ) );
	memset( g_pVerBuf2, 0x00, sizeof(OBJ_VERTEX_BUF2) );

	// ndata_cnt�ɃI�u�W�F�N�g�̍\���|���S�������R�i���_���j����
	ndata_cnt = pobj_data->m_Polcnt * 3;

	g_pVerBuf2->m_nPolmode = GL_C4F_N3F_V3F;					// ���_�z��t�H�[�}�b�g
	g_pVerBuf2->m_nPolcnt  = pobj_data->m_Polcnt;				// �|���S����
	g_pVerBuf2->m_nDatacnt = ndata_cnt;							// ���_�f�[�^��
	g_pVerBuf2->m_nObjsize = sizeof(C4F_N3F_V3F) * ndata_cnt;	// �o�b�t�@�T�C�Y

	// ���_�f�[�^�̈�m��
	g_pVerBuf2->pcObjData = (unsigned char *)malloc( g_pVerBuf2->m_nObjsize );

	pcobj_copy = g_pVerBuf2->pcObjData;							// ���_�f�[�^���i�[����̈�̐擪�A�h���X���R�s�[

	pobj_c1	= pobj_data->pObj_c;								// �I�u�W�F�N�g�̃J���[���i�[�A�h���X���R�s�[
	pobj_n1 = pobj_data->pObj_n;								// �I�u�W�F�N�g�̖@�����i�[�A�h���X���R�s�[
	pobj_v1 = pobj_data->pObj_v;								// �I�u�W�F�N�g�̒��_���W�i�[�A�h���X���R�s�[

	ppol_copy = pobj_data->pObj_pol;							// �|���S���C���f�b�N�X�i�[�ꏊ�̐擪�A�h���X���R�s�[

	// �I�u�W�F�N�g�\���|���S���������[�v����
	for ( int loop1 = 0; loop1 < pobj_data->m_Polcnt; loop1++ ) {

		// �R���_�����[�v����
		for ( int loop2 = 0; loop2 < 3; loop2++ ) {

			// �J���[�����R�s�[
			pobj_c0		= (OBJ_COLOR *)pcobj_copy;				// ���_�f�[�^���i�[����Ă���̈悩��J���[���̈�����o��
			nindex_no	= ppol_copy->m_c[loop2];				// �J���[�C���f�b�N�X�ԍ������o��
			*pobj_c0	= *( pobj_c1 + nindex_no );				// �C���f�b�N�X�ԍ��Ŏw�肳�ꂽ�ꏊ����J���[�������o��
			pcobj_copy += sizeof(OBJ_COLOR);					

			// �@�������R�s�[
			pobj_n0		= (OBJ_NORMAL *)pcobj_copy;				// ���_�f�[�^���i�[����Ă���̈悩��@�����̈�����o��
			nindex_no	= ppol_copy->m_n[loop2];				// �@�����C���f�b�N�X�ԍ������o��
			*pobj_n0	= *( pobj_n1 + nindex_no );				// �C���f�b�N�X�ԍ��Ŏw�肳�ꂽ�ꏊ����@���������o��
			pcobj_copy += sizeof(OBJ_NORMAL);

			// ���_���W���R�s�[
			pobj_v0		= (OBJ_VERTEX *)pcobj_copy;				// ���_�f�[�^���i�[����Ă���̈悩�璸�_���W�̈�����o��
			nindex_no	= ppol_copy->m_v[loop2];				// ���_���W�C���f�b�N�X�ԍ������o��
			*pobj_v0	= *( pobj_v1 + nindex_no );				// �C���f�b�N�X�ԍ��Ŏw�肳�ꂽ�ꏊ���璸�_���W�����o��
			pcobj_copy += sizeof(OBJ_VERTEX);
			
		}

		ppol_copy++;

	}

	return g_pVerBuf2;

}

/**********************************************************
	�|���S���t�@�C���ǂݍ��݊֐�

	�@�����F�ǂݍ��݂��s���t�@�C����
	�߂�l�F�e�L�X�g����擾������������OBJ_VARIABLE�^��
			�f�[�^�̃A�h���X

	�w�肳�ꂽ�t�@�C���̃T�C�Y���̃f�[�^�ǂݍ��ݗ̈���m��
	����B���̗̈�ɑS�C���f�b�N�X�f�[�^���������ށB
	�m�ۂ��ꂽ�̈�̐擪�A�h���X��OBJ_VARIABLE�^�̃|�C���^
	*g_pObj�ɏ������݁A���̃A�h���X��Ԃ��B
**********************************************************/

OBJ_VARIABLE *Load_ObjFile( char *filename )
{

	int ret		   = TRUE;														// ����
	int nfilesize  = 0;															// �I�u�W�F�N�g�t�@�C���T�C�Y

	// �e�|�C���^�̏�����
	g_pObjfile = NULL;
	g_pObj	   = NULL;
	g_pVerBuf  = NULL;
	g_pVerBuf2 = NULL;
	
	// �e�L�X�g�f�[�^�̕��������̗̈���m�ۂ��Ă��̐擪�A�h���X���󂯎��
	g_pObjfile = Load_TextFile( filename, &nfilesize );							// �e�L�X�g�f�[�^�ǂݍ��݊֐�

	if ( g_pObjfile == NULL ) {
		Obj_Error1();
		return FALSE;
	}

	g_pObj = (OBJ_VARIABLE *)malloc( sizeof(OBJ_VARIABLE) );					// �|���S���f�[�^�̈�m��

	if ( g_pObj == NULL ) {
		Obj_Error1();
		return FALSE;
	}

	memset( g_pObj, 0x00, sizeof(OBJ_VARIABLE) );								// g_pObj�Ɋm�ۂ����̈�����O�ŏ�����

	// �e�L�X�g����ǂݍ��񂾃|���S�����\������������o���i�����𐔒l�Ƃ��ēǂݍ��ށj
	ret = Load_VertexData( g_pObjfile, nfilesize, g_pObj );

	if ( ret == NULL ) {														// �f�[�^���o�����s
		Obj_Error1();
		return FALSE;
	}

	free( g_pObjfile );															// �e�L�X�g�̈���J������

	return g_pObj;

}

/**********************************************************
	�e�L�X�g�f�[�^�ǂݍ��݊֐�

	��P�����F�ǂݍ��ނ��s���t�@�C����
	��Q�����F�ǂݍ��񂾃t�@�C���̃T�C�Y���i�[����ϐ��̃A�h���X
	�@�߂�l�F�t�@�C���̕��������m�ۂ��ꂽ�������̈��
			�@�擪�A�h���X��Ԃ��B
	
	�I�u�W�F�N�g�t�@�C���ɏ������܂ꂽ�f�[�^�i������j��
	�ǂݍ���ŁA���̃T�C�Y���̗̈���m�ۂ���B���̐擪
	�A�h���X��Ԃ��B
**********************************************************/

char *Load_TextFile( char *fname, int *filesize )
{

	char   *pcbuf;
	size_t fsize;

	FILE   *fp;

	g_pObj	  = NULL;
	g_pVerBuf = NULL;

	if ( ( fp = fopen( fname, "rt" ) ) == NULL ) { return FALSE; }

	// fseek�֐����g���ăt�@�C���|�C���^���Ō���Ɏ����Ă���
	fseek( fp, 0, SEEK_END );
	
	// ftell�֐����g���ăt�@�C���̐擪����t�@�C���|�C���^�̈ʒu�܂ł̕������𐔂���
	fsize = ftell( fp );
	
	// ��L�Ő��������������̗̈���m��
	pcbuf = (char *)malloc( fsize );

	if ( pcbuf == NULL ) {
		fclose( fp );
		return FALSE;
	}

	// �t�@�C���|�C���^��擪�ɖ߂�
	fseek( fp, 0, SEEK_SET );

	// fread�֐����g����fsize��char�^�̑傫���̕�����pcbuf�Ɋi�[����
	// ���ł�char�^�œǂݍ��񂾐���߂�l�Ƃ���fsize�ɕԂ�
	fsize = fread( pcbuf, sizeof(char), fsize, fp );

	fclose( fp );

	// �ǂݍ���char�^�̕����̐���filesize�i�����Ƃ��ė^���炽�A�h���X�j�̒��ɃZ�b�g����
	*filesize = (int)fsize;

	// �e�L�X�g�Ō��m���ꂽ���s�R�[�h��\0�R�[�h�ɒu��������
	for ( int loop = 0; loop < *filesize; loop++ ) {

		if ( *( pcbuf+ loop ) == 0x0A ) {
			*( pcbuf+ loop ) = 0x00;
		}
	
	}

	return pcbuf;

}

/**********************************************************
	Obj�f�[�^�ϊ��֐�

	��P�����Fpol�t�@�C������ǂݍ��񂾃I�u�W�F�N�g���_
			�@��񂪊i�[����Ă���̈�̐擪�A�h���X
	��Q�����F�ǂݍ��񂾃t�@�C���̍ő啶����
	��R�����F�ǂݍ��񂾒��_��񓙂��i�[����ϐ��̃A�h���X
	�@�߂�l�F�����������ɂ�TRUE��Ԃ��A���s���ɂ�FALSE��
			�@�Ԃ��B

	�t�@�C������ǂݍ��񂾒��_���F�@�����F�J���[���F�|���S��
	���𐔒l�Ƃ��ēǂݍ��݁A�܂��ǂݍ��񂾃T�C�Y���̗̈��
	�m�ۂ������OBJ_VARIABLE�^�f�[�^�Ɋe�����i�[����B
**********************************************************/

int Load_VertexData( char *pcbuf, int nfsize, OBJ_VARIABLE *pobj )
{

	int nv_count;		// ���_��
	int nn_count;		// �@����
    int nc_count;		// �J���[��
    int nt_count;		// �e�N�X�`�����W��
    int npol_count;		// �|���S����
    int npoint;			// �t�@�C���|�C���^���݈ʒu

	OBJ_VERTEX  *pv;    // ���_�f�[�^�p�|�C���^
    OBJ_NORMAL  *pn;    // �@���f�[�^�p�|�C���^
    OBJ_COLOR   *pc;    // �J���[�f�[�^�p�|�C���^
    OBJ_TEXTURE *pt;    // �e�N�X�`���f�[�^�p�|�C���^
    OBJ_POLY    *pp;    // �|���S���f�[�^�p�|�C���^

	// �f�[�^�J�n�s�̌���
	npoint = BufChk_String( "# Data Count", pcbuf, nfsize );

	if ( npoint == FALSE ) { return FALSE; }

	// ���_���F�@�����F�J���[���F�|���S�������o�b�t�@����ǂݍ���ŕϐ��ɑ��
	sscanf( ( pcbuf + npoint ), "%d %d %d %d %d", &nv_count, &nn_count, &nc_count, &nt_count, &npol_count );
	
	// �t�@�C������ǂݍ��񂾃|���S�������I�u�W�F�N�g�f�[�^���ɐݒ�
	pobj->m_Polcnt = npol_count;

	// �|���S���̊e�f�[�^���i�[���邽�߂̗̈���m��
	// OBJ_POLY�\���̂̃T�C�Y�i�e�C���f�b�N�X�����i�[����\���́j��npol_count�i�|���S�����j���̗̈�m��
	pobj->pObj_pol = (OBJ_POLY *)malloc( sizeof(OBJ_POLY) * npol_count );

	// �e�\���̕��̗̈���m�ہi�\���̂̃T�C�Y���e��񐔁j
	pobj->pObj_v =  (OBJ_VERTEX *)malloc( sizeof(OBJ_VERTEX) * nv_count );
	pobj->pObj_n =  (OBJ_NORMAL *)malloc( sizeof(OBJ_NORMAL) * nn_count );
	pobj->pObj_c =   (OBJ_COLOR *)malloc( sizeof(OBJ_COLOR) * nc_count );
	pobj->pObj_t = (OBJ_TEXTURE *)malloc( sizeof(OBJ_TEXTURE) * nt_count );

	if ( ( pobj->pObj_pol == NULL ) || ( pobj->pObj_v == NULL )
		|| ( pobj->pObj_n == NULL ) || ( pobj->pObj_c == NULL )
		|| ( pobj->pObj_t == NULL ) ) { return FALSE; }

	// ���_�f�[�^�J�n�s����	
	npoint = BufChk_String( "# Vertex Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// �e�L�X�g����Ƃ��Ă����f�[�^��pobj�Ƀ|�C���^���g���ĊԐړI�ɑ��
	for ( int loop = 0; loop < nv_count; loop++ ) {

		pv = ( pobj->pObj_v ) + loop;																// �f�[�^���L������Ă���ꏊ��pv�Ɋi�[
		sscanf( ( pcbuf + npoint ), "%f,%f,%f", &pv->m_x, &pv->m_y, &pv->m_z );						// ���̏ꏊ����P�s���̃f�[�^��ǂݍ���
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// �ǂݍ��񂾕��������J�E���g���A���̕�npoint��i�߂�
																									// �܂�A�P�s�i��
	}

	// �@���f�[�^�J�n�s����	
	npoint = BufChk_String( "# Normal Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// �e�L�X�g����Ƃ��Ă����f�[�^��pobj�Ƀ|�C���^���g���ĊԐړI�ɑ��
	for ( int loop = 0; loop < nn_count; loop++ ) {

		pn = ( pobj->pObj_n ) + loop;																// �f�[�^���L������Ă���ꏊ��pv�Ɋi�[
		sscanf( ( pcbuf + npoint ), "%f,%f,%f", &pn->m_x, &pn->m_y, &pn->m_z );						// ���̏ꏊ����P�s���̃f�[�^��ǂݍ���
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// �ǂݍ��񂾕��������J�E���g���A���̕�npoint��i�߂�
																									// �܂�A�P�s�i��
	}

	// �J���[�f�[�^�J�n�s����	
	npoint = BufChk_String( "# Color Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// �e�L�X�g����Ƃ��Ă����f�[�^��pobj�Ƀ|�C���^���g���ĊԐړI�ɑ��
	for ( int loop = 0; loop < nc_count; loop++ ) {

		pc = ( pobj->pObj_c ) + loop;																// �f�[�^���L������Ă���ꏊ��pv�Ɋi�[
		sscanf( ( pcbuf + npoint ), "%f,%f,%f,%f", &pc->m_R, &pc->m_G, &pc->m_B, &pc->m_A );		// ���̏ꏊ����P�s���̃f�[�^��ǂݍ���
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// �ǂݍ��񂾕��������J�E���g���A���̕�npoint��i�߂�
																									// �܂�A�P�s�i��
	}

	// �e�N�X�`���f�[�^�J�n�s����	
	npoint = BufChk_String( "# Texture Data", pcbuf, nfsize );

	if ( npoint == NULL ) { return FALSE; }

	// �e�L�X�g����Ƃ��Ă����f�[�^��pobj�Ƀ|�C���^���g���ĊԐړI�ɑ��
	for ( int loop = 0; loop < nt_count; loop++ ) {

		pt = ( pobj->pObj_t ) + loop;																// �f�[�^���L������Ă���ꏊ��pv�Ɋi�[
		sscanf( ( pcbuf + npoint ), "%f,%f", &pt->m_s, &pt->m_t );									// ���̏ꏊ����P�s���̃f�[�^��ǂݍ���
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// �ǂݍ��񂾕��������J�E���g���A���̕�npoint��i�߂�
																									// �܂�A�P�s�i��
	}

	// �|���S���f�[�^�J�n�s����
    npoint = BufChk_String( "# Polygon Data", pcbuf, nfsize );
    
	if( npoint == FALSE ) { return FALSE; }

    // �|���S���f�[�^��pobj�ɑ��
    for( int loop = 0; loop < npol_count; loop++ ) {

		pp = ( pobj->pObj_pol ) + loop;

        sscanf( ( pcbuf + npoint ), "%d,%d,%d : %d,%d,%d : %d,%d,%d : %d,%d,%d",
						&pp->m_v[0], &pp->m_v[1], &pp->m_v[2],										// ���_�C���f�b�N�X
                        &pp->m_n[0], &pp->m_n[1], &pp->m_n[2],										// �@���C���f�b�N�X
                        &pp->m_c[0], &pp->m_c[1], &pp->m_c[2],										// �J���[�C���f�b�N�X
						&pp->m_t[0], &pp->m_t[1], &pp->m_t[2]  );									// �e�N�X�`���C���f�b�N�X
        
		npoint += (int)strlen( ( pcbuf + npoint ) ) + 1;											// �P�s���ړ�

	}

	return TRUE;

}

/**********************************************************
	�w�蕶���񌟍��֐�

	��P�����F�������镶����
	��Q�����F��������镶���f�[�^�̐擪�A�h���X
	��R�����F��������镶���f�[�^�̃T�C�Y
	�@�߂�l�F�����������ɂ͎w�蕶����܂ł̕�������
			�@�Ԃ�A���s���ɂ�FLASE���Ԃ�B

	��P�����Ƃ��ēn���ꂽ���������������֐��B
	�����Ƃ��ēn���ꂽ�������pcbuf���Ō��ݎw�肳��Ă���
	�ʒu����nchk_size���̕������r����B�Y�������񂪔���
	�����΁A�����܂ł̕�������Ԃ��B
**********************************************************/

int BufChk_String( char *pcstring, char *pcbuf, int nsize )
{

	int nleng;									// �P�s�̕�����
	int npoint = 0;								// �����p�ړ��|�C���^
	int nchk_size;								// �`�F�b�N������

	nchk_size = (int)strlen( pcstring );		// �`�F�b�N���镶�����i�����Ƃ��ēn���ꂽ������j
	
	while ( npoint < nsize ) {

		nleng = (int)strlen( pcbuf ) + 1;		// �P�s�̕�����

		npoint += nleng;						// �����p�ړ��|�C���^��ǂݍ��񂾕��������i�߂�

		// �����Ƃ��ēn���ꂽ�������pcbuf���Ō��ݎw�肳��Ă���ʒu����nchk_size���̕������r����
		// ���̕�������r�������ʁA�����ꍇ�O���Ԃ�B
		if ( memcmp( pcstring, pcbuf, nchk_size ) == 0 ) {
			// ���m�����w�蕶����܂ł̕�������Ԃ�
			return npoint;
		}

		pcbuf += nleng;							// �o�b�t�@�ʒu��ǂݍ��񂾕��������i�߂�

	}

	return FALSE;

}

/**********************************************************
	Obj�t�@�C���ǂݍ��݃G���[�֐�

	�@�����G�Ȃ�
	�߂�l�F����I������TRUE��Ԃ�

	Obj�t�@�C���̓ǂݍ��݂Ɏ��s�������Ɏ��s����֐��B
	Obj�t�@�C���ǂݍ��݂̂��߂Ɋm�ۂ����e�f�[�^�̈�
	OBJ_VARIABLE�EOBJ_VERTEX_BUF�EOBJ_VERTEX_BUF2�^
	�f�[�^���������B
**********************************************************/

int Obj_Error1( void )
{
	
	if ( g_pObjfile != NULL ) {
		free( g_pObjfile );
		g_pObjfile = NULL;
	}

	Free_ObjIndex( g_pObj );				// OBJ_VARIABLE�^�f�[�^����֐�
	g_pObj = NULL;

	Free_ObjVertex1( g_pVerBuf );			// OBJ_VERTEX_BUF�^�f�[�^����֐�
	g_pVerBuf = NULL;

	Free_ObjVertex2( g_pVerBuf2 );			// OBJ_VERTEX_BUF2�^�f�[�^����֐�
	g_pVerBuf2 = NULL;

	return TRUE;

}

/**********************************************************
	OBJ_VARIABLE�^�f�[�^����֐�

	�@�����F�������OBJ_VARIABLE�^�̃f�[�^�ւ̃A�h���X
	�߂�l�F�Ȃ�

	�̈�m�ۂ���OBJ_VARIABLE�^�̃f�[�^���������֐��B
	���̍ہA�e�����o�ϐ��̃o�b�t�@���Ȃ��ɃA�h���X������
	����ꍇ�ɂ͂��̗̈���ɉ������B	
**********************************************************/

void Free_ObjIndex( OBJ_VARIABLE *pobj )
{

	if ( pobj != NULL ) {

		if ( pobj->pObj_v != NULL ) {
			free( pobj->pObj_v );
			pobj->pObj_v = NULL;
		}

		if ( pobj->pObj_n != NULL ) {
			free( pobj->pObj_n );
			pobj->pObj_n = NULL;
		}

		if ( pobj->pObj_c != NULL ) {
			free( pobj->pObj_c );
			pobj->pObj_c = NULL;
		}

		if ( pobj->pObj_t != NULL ) {
			free( pobj->pObj_t );
			pobj->pObj_t = NULL;
		}

		if ( pobj->pObj_pol != NULL ) {
			free( pobj->pObj_pol );
			pobj->pObj_pol = NULL;
		}

		free( pobj );
	
	}

}

/**********************************************************
	OBJ_VERTEX_BUF�^�f�[�^����֐�

	�@�����F�������OBJ_VERTEX_BUF�^�̃f�[�^�ւ̃A�h���X
	�߂�l�F�Ȃ�

	�̈�m�ۂ���OBJ_VERTEX_BUF�^�̃f�[�^���������֐��B
	���̍ہA�e�����o�ϐ��̃o�b�t�@���Ȃ��ɃA�h���X������
	����ꍇ�ɂ͂��̗̈���ɉ������B
**********************************************************/

void Free_ObjVertex1( OBJ_VERTEX_BUF *pobj )
{
    if( pobj != NULL ) {

		if( pobj->pObj_v != NULL ) {
            free( pobj->pObj_v );
            pobj->pObj_v = NULL;
        }

		if( pobj->pObj_n != NULL ) {
            free( pobj->pObj_n );
            pobj->pObj_n = NULL;
        }

        if( pobj->pObj_c != NULL ) {
            free( pobj->pObj_c );
            pobj->pObj_c = NULL;
        }

		if( pobj->pObj_t != NULL ){
            free( pobj->pObj_t );
			pobj->pObj_t = NULL;
        }

        free( pobj );
    
	}

}

/**********************************************************
	OBJ_VERTEX_BUF2�^�f�[�^����֐�

	�@�����F�������OBJ_VERTEX_BUF2�^�̃I�u�W�F�N�g�ւ̃A�h���X
	�߂�l�F�Ȃ�

	�m�ۂ���OBJ_VERTEX_BUF2�^�̃f�[�^�̈���������֐��B
	�A���A���_�f�[�^�̒��g�����݂��鎞�͂��̗̈���ɉ������B
**********************************************************/

void Free_ObjVertex2( OBJ_VERTEX_BUF2 *pobj )
{

	if ( pobj != NULL ) {

		if ( pobj->pcObjData != NULL ) {
			free( pobj->pcObjData );
			pobj->pcObjData = NULL;
		}

		free( pobj );

	}

}
