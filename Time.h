/*=============================================================================

		�^�C��[ Time.h ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	��d�C���N���[�h�h�~
-----------------------------------------------------------------------------*/
#ifndef _TIME_H_
#define _TIME_H_

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
void InitTime( void );		//	�^�C���̏�����
void UninitTime( void );	//	�^�C���̏I��
void UpdateTime( void );	//	�^�C���̍X�V
void DrawTime( void );		//	�^�C���̕`��

bool *GetTime( void );	//	�^�C�����̎擾

#endif