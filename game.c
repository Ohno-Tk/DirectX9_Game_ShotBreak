/*=============================================================================

		�Q�[��[ game.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/12/15
-------------------------------------------------------------------------------
	���@Update
=============================================================================*/
/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "camera.h"
#include "MeshField.h"
#include "MeshWall.h"
#include "skydome.h"
#include "Shadow.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyFly.h"
#include "Building.h"
#include "bullet.h"
#include "Effect.h"
#include "explosion.h"
#include "Score.h"
#include "Time.h"
#include "CountDown.h"
#include "Pause.h"
#include "Reticle.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 �֐���:	void InitGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		������
-----------------------------------------------------------------------------*/
void InitGame( void )
{

	InitCamera();	//	�J�����̏�����

	InitMeshField();		//	���b�V���t�B�[���h�̏�����

	InitMeshWall();		//	���b�V���E�H�[���̏�����

	InitSkydome();		//	�X�J�C�h�[���̏�����

	InitShadow();	//	�e�̏�����

	InitPlayer();		//	�v���C���[�̏�����

	InitEnemy();		//	�G�̏�����

	InitEnemyFly();		//	�G( ���V�^�C�v )�̏�����

	InitBuilding();		//	�����̏�����

	InitBullet();	//	�e�̏�����

	InitEffect();	//	�G�t�F�N�g�̏�����

	InitExplosin();		//	�����̏�����

	InitScore();		//	�X�R�A�̏�����

	InitTime();		//	�^�C���̏�����

	InitCountDown();		//	�J�E���g�_�E���̏�����

	InitPause();		//	�|�[�Y�̏�����

	InitReticle();	//	�Ə��̏�����

	//	���y�Đ�
	PlaySound( SOUND_LABEL_BGM_GAME );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitGame( void )
{

	UninitMeshField();		//	���b�V���t�B�[���h�̏I��

	UninitMeshWall();	//	���b�V���E�H�[���̏I��

	UninitSkydome();		//	�X�J�C�h�[���̏I��

	UninitShadow();	//	�e�̏I��

	UninitPlayer();	//	�v���C���[�̏I��

	UninitEnemy();	//	�G�̏I��

	UninitEnemyFly();	//	�G( ���V�^�C�v )�̏I��

	UninitBuilding();	//	�����̏I��

	UninitBullet();	//	�e�̏I��

	UninitEffect();	//	�G�t�F�N�g�̏I��

	UninitExplosin();	//	�����̏I��

	UninitScore();	//	�X�R�A�̏I��

	UninitTime();	//	�^�C���̏I��

	UninitCountDown();	//	�J�E���g�_�E���̏I��

	UninitPause();	//	�|�[�Y�̏I��

	UninitReticle();	//	�Ə��̏I��

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateGame( void )
{

	//	�X�^�[�g�������̏��擾
	bool *Start = GetStart();

	//	�^�C�����̎擾
	bool *Time = GetTime();

	//	�|�[�Y�t���O���̎擾
	bool *Pause = GetPauseFlag();


	if( *Start == true && *Pause == false )
	{

		UpdateMeshField();		//	���b�V���t�B�[���h�̍X�V

		UpdateMeshWall();	//	���b�V���E�H�[���̍X�V

		UpdateSkydome();		//	�X�J�C�h�[���̍X�V

		UpdateShadow();	//	�e�̍X�V

		UpdatePlayer();	//	�v���C���[�̍X�V

		UpdateEnemy();	//	�G�̍X�V

		UpdateEnemyFly();	//	�G( ���V�^�C�v )�̍X�V

		UpdateBuilding();	//	�����̍X�V

		UpdateBullet();	//	�e�̍X�V

		UpdateEffect();	//	�G�t�F�N�g�̍X�V

		UpdateExplosin();	//	�����̍X�V

		UpdateScore();	//	�X�R�A�̍X�V

		UpdateTime();	//	�^�C���̍X�V

		UpdateReticle();	//	�Ə��̍X�V


	}	//	end of if

	UpdateCountDown();	//	�J�E���g�_�E���̍X�V

	UpdatePause();	//	�|�[�Y�̍X�V


	//	��ʑJ��
	if( *Time == true )
	{
		
		//	�t�F�[�h�̃Z�b�g
		SetFade( FADE_OUT , MODE_RESULT );

		//	���Ԗڂ̉����~�߂�
		StopSound( SOUND_LABEL_BGM_GAME );

	}	//	end of if
	//if( GetKeyboardTrigger( DIK_RETURN ) )
	//{
	//	SetFade( FADE_OUT , MODE_RESULT );

	//	//	���Ԗڂ̉����~�߂�
	//	StopSound( SOUND_LABEL_BGM_GAME );

	//}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawGame( void )
 ����:		�Ȃ�
 �߂�l:	�Ȃ�
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawGame( void )
{

	DrawMeshField();		//	���b�V���t�B�[���h�̕`��

	DrawSkydome();		//	�X�J�C�h�[���̕`��

	DrawMeshWall();		//	���b�V���E�H�[���̕`��

	DrawShadow();	//	�e�̕`��

	DrawPlayer();		//	�v���C���[�̕`��

	DrawEnemy();		//	�G�̕`��

	DrawEnemyFly();		//	�G( ���V�^�C�v )�̕`��

	DrawBuilding();		//	�����̕`��

	DrawBullet();	//	�e�̕`��

	DrawEffect();	//	�G�t�F�N�g�̕`��

	DrawExplosin();		//	�����̕`��

	DrawScore();		//	�X�R�A�̕`��

	DrawTime();		//	�^�C���̕`��

	DrawCountDown();		//	�J�E���g�_�E���̕`��

	DrawPause();		//	�|�[�Y�̕`��

	DrawReticle();	//	�Ə��̕`��

}	//	end of func