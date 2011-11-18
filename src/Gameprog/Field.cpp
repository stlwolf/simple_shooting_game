
/**********************************************************
	�C���N���[�h�t�@�C��
**********************************************************/

#include "Field.h"

/**********************************************************
	�O���[�o���ϐ���`
**********************************************************/

EREA g_Ereamap;

/**********************************************************
	�}�b�v�������֐�

	�����F�Ȃ��@�߂�l�F�Ȃ�

	�}�b�v�̕\���ɕK�v�ȕϐ���������������֐��B
	�}�b�v�̕\���͈͂Əo���^�[�Q�b�g�������������Ă���B
**********************************************************/

void Map_Init( void )
{

	// �G���A�̕\���͈͂̏������ݒ�
	g_Ereamap.m_x = 80;
	g_Ereamap.m_y = 60;
	g_Ereamap.m_z = 80;

	// �G���A�}�b�v�ϐ��ɓo��^�[�Q�b�g����ݒ�
	g_Ereamap.m_TargetNum = MAX_TARGET;

	g_Ereamap.m_EnemyTargetNum = 99;

}

/**********************************************************
	���C���[�Ǎ쐬�֐�

	��P�����F���C���[���쐬����w���W�l
	��Q�����F���C���[���쐬����x���W�l
	��R�����F���C���[���쐬����y���W�l
	��S�����F���C���[������
	�@�߂�l�F�Ȃ�

	�O�𒆐S�Ƃ��āA�����Ƃ��ēn���ꂽ�w�E�x�E�y���W�̒l
	�͈̔͂��͂ނ悤�Ȕ���\������B
**********************************************************/

void WireWall_Disp( float x, float y, float z, float div )
{

    int loop;
    float st_x, st_y, st_z;
    float p_x, p_y, p_z;

    st_x = x / 2.0f;
    st_y = y / 2.0f;
    st_z = z / 2.0f;

    glEnable(GL_BLEND);

    // ���C���[�t���[���̃W���M�����p
    glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);

    for( loop = 0; loop <= div; loop++ ) {

        p_x = x / div * loop;
        p_y = y / div * loop;
        p_z = z / div * loop;

        //�|���S���̔��\��
        glBegin( GL_LINES  );
            glColor3f( 1.0f, 0.0f, 0.0f );  glVertex3f( st_x,  st_y,  st_z-p_z); glVertex3f(-st_x,  st_y,  st_z-p_z);
            glColor3f( 0.0f, 1.0f, 0.0f );  glVertex3f(-st_x,  st_y,  st_z-p_z); glVertex3f(-st_x, -st_y,  st_z-p_z);
            glColor3f( 0.0f, 0.0f, 1.0f );  glVertex3f(-st_x, -st_y,  st_z-p_z); glVertex3f( st_x, -st_y,  st_z-p_z);
            glColor3f( 1.0f, 1.0f, 0.0f );  glVertex3f( st_x, -st_y,  st_z-p_z); glVertex3f( st_x,  st_y,  st_z-p_z);
        glEnd();

        glBegin( GL_LINES );
            glColor3f( 1.0f, 1.0f, 0.0f );  glVertex3f( st_x,  st_y-p_y,  st_z); glVertex3f( st_x,  st_y-p_y, -st_z);
            glColor3f( 0.0f, 1.0f, 1.0f );  glVertex3f( st_x,  st_y-p_y, -st_z); glVertex3f(-st_x,  st_y-p_y, -st_z);
            glColor3f( 0.0f, 1.0f, 0.0f );  glVertex3f(-st_x,  st_y-p_y, -st_z); glVertex3f(-st_x,  st_y-p_y,  st_z);
            glColor3f( 1.0f, 0.0f, 1.0f );  glVertex3f(-st_x,  st_y-p_y,  st_z); glVertex3f( st_x,  st_y-p_y,  st_z);
        glEnd();

        glBegin( GL_LINES  );
            glColor3f( 1.0f, 0.0f, 1.0f );  glVertex3f( st_x-p_x,  st_y,  st_z); glVertex3f( st_x-p_x, -st_y,  st_z);
            glColor3f( 0.0f, 0.0f, 1.0f );  glVertex3f( st_x-p_x, -st_y,  st_z); glVertex3f( st_x-p_x, -st_y, -st_z);
            glColor3f( 0.0f, 1.0f, 1.0f );  glVertex3f( st_x-p_x, -st_y, -st_z); glVertex3f( st_x-p_x,  st_y, -st_z);
            glColor3f( 1.0f, 0.0f, 0.0f );  glVertex3f( st_x-p_x,  st_y, -st_z); glVertex3f( st_x-p_x,  st_y,  st_z);
        glEnd();

    }

    glDisable(GL_BLEND);

}
