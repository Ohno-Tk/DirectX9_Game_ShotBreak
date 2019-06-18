/*=============================================================================

		�v���C���[[ Player.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/12/29
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
						scanf ��warning�h�~ 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "MeshWall.h"
#include "Player.h"
#include "Shadow.h"
#include "fade.h"
#include "Reticle.h"
#include "Building.h"
#include "Utility.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Player/CowBoy.x"

#define MAX_MAT ( 8 )	//	���f���̍ő�}�e���A����

#define ROT_SPEED ( 1.5f )	//	��]��

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	�L�[�{�[�h�ł̑���
void KeyboardOperate( void );

//	���̂Ƃ̓����蔻��
void CollisionObject( void );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshPlayer;	//	���b�V���C���^�[�t�F�[�X�|�C���^

LPD3DXBUFFER g_pBufferMatPlayer;	//	�}�e���A�����

DWORD g_nNumMatPlayer;	//	�}�e���A����

LPDIRECT3DTEXTURE9 g_pTexturePlayer[ MAX_MAT ] = { NULL };//	�e�N�X�`���C���^�[�t�F�[�X

D3DXMATERIAL* pMatPlayer;	//	�}�e���A���|�C���^

LPD3DXFONT g_pFontPlayer = NULL;	//	�t�H���g�|�C���^

PLAYER g_Player;	//	�v���C���[�\����

#ifdef _DEBUG

	LPD3DXFONT g_pFontModel = NULL;

#endif	//	_DEBUG

/*-----------------------------------------------------------------------------
 �֐���:	void InitPlayer( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitPlayer( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();


	HRESULT hr;

	hr = D3DXLoadMeshFromX( MODEL_NAME ,			//	�t�@�C����
							D3DXMESH_MANAGED,
							pDevice,				//	�f�o�C�X
							NULL,					//	�אڃo�b�t�@
							&g_pBufferMatPlayer,		//	�}�e���A�������i�[
							NULL,
							&g_nNumMatPlayer,		//	�}�e���A����
							&g_pMeshPlayer );		//	���b�V��

	//	���f���̃G���[�`�F�b�N
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ Player.cpp ]\n MODEL_NAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	�}�e���A�����̃|�C���^�Ƃ��ăo�b�t�@�̃A�h���X���擾
	pMatPlayer = ( D3DXMATERIAL* )g_pBufferMatPlayer -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatPlayer ; i++ )
	{
		if( pMatPlayer[ i ].pTextureFilename != NULL )
		{

			//	�e�N�X�`���̃G���[�`�F�b�N
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatPlayer[ i ].pTextureFilename , &g_pTexturePlayer[ i ]  ) ) )
			{
				MessageBox( NULL , "���f���̉摜�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for



	//	�\���̏�����

	if( *Mode == MODE_TITLE )
	{

		//	���W
		g_Player.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 60.0f );

		//	��]��
		g_Player.World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	}	//	end of if

	else if( *Mode == MODE_GAME )
	{

		//	���W
		g_Player.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , -1000.0f );

		//	��]��
		g_Player.World.Rot = D3DXVECTOR3( 0.0f , 180.0f , 0.0f );

	}	//	end of else if

	//	�g�嗦
	g_Player.World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	�O�̍��W
	g_Player.OldPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	�ړ���
	g_Player.Move = D3DXVECTOR3( 5.0f , 5.0f , 5.0f );

	//	��]��
	g_Player.RotSpeed = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	���a
	g_Player.Radius = 5.0f;

	//	�g�p�t���O
	g_Player.Use = true;


	//	�e�̃Z�b�g
	SetShadow( g_Player.World.Pos , 15.0f , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );


#ifdef _DEBUG
	//	�t�H���g�̐ݒ�
	D3DXCreateFont( pDevice ,
				18 ,	//	��������
				0 ,	//	������
				0 ,	//	�����̑���
				0 ,	//	�~�b�v�}�b�v
				FALSE ,	//	�C�^���b�N
				SHIFTJIS_CHARSET ,
				OUT_DEFAULT_PRECIS ,
				DEFAULT_QUALITY ,
				DEFAULT_PITCH ,
				"Terminal" ,
				&g_pFontModel );
#endif


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitPlayer( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitPlayer( void )
{

	if( g_pMeshPlayer != NULL )	//	���b�V���C���^�[�t�F�[�X�|�C���^
	{

		g_pMeshPlayer -> Release();
		g_pMeshPlayer = NULL;

	}	//	end of if

	if( g_pBufferMatPlayer != NULL )	//	�}�e���A�����J��
	{

		g_pBufferMatPlayer -> Release();
		g_pBufferMatPlayer = NULL;

	}	//	end of if

}	//	end of func
 
/*-----------------------------------------------------------------------------
 �֐���:	void UpdataPlayer( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdatePlayer( void )
{

	//	���̃��[�h�̎擾
	MODE *Mode = GetNextMode();


	if( *Mode == MODE_GAME )
	{

		//	�O��̍��W���擾
		g_Player.OldPos = g_Player.World.Pos;


		//	�L�[�{�[�h�ł̑���
		KeyboardOperate();

		//	���̂Ƃ̓����蔻��
		CollisionObject();


		//	�J�����̃Z�b�g
		SetCamera( g_Player.World.Pos , g_Player.World.Rot , g_Player.Move );

		//	���e�B�N���̃Z�b�g
		SetReticle( D3DXVECTOR3( g_Player.World.Pos.x , g_Player.World.Pos.y + 25.0f , g_Player.World.Pos.z ) , g_Player.World.Rot , D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ) );

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawPlayer( void )
 ����:		
 �߂�l:	
 ����:		���f���̕`��
-----------------------------------------------------------------------------*/
void DrawPlayer( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	���݃f�o�C�X�ɐݒ肳��Ă���}�e���A�����̎擾
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	if( g_Player.Use == true)
	{

		//	�t�s��Ȃ��̃��[���h���W�ϊ�
		SetWorld( g_Player.World.Pos , g_Player.World.Rot , g_Player.World.Scl );


		for( int i = 0 ; i < ( int )g_nNumMatPlayer ; i++ )
		{
			//	�f�o�C�X�Ƀ}�e���A����ݒ�
			pDevice -> SetMaterial( &pMatPlayer[ i ].MatD3D );

			//	�e�N�X�`���ݒ�
			pDevice -> SetTexture( 0 , g_pTexturePlayer[ i ] );


			//	�`��
			g_pMeshPlayer -> DrawSubset( i );

		}	//	end of for

	}	//	end of if

	pDevice -> SetMaterial( &matDef );

#ifdef _DEBUG

	RECT rect = { 0 , 40 , SCREEN_WIDTH , SCREEN_HEIGHT };
	RECT rect01 = { 0 , 60 , SCREEN_WIDTH , SCREEN_HEIGHT };

	char aStr[ 256 ] , aStr01[ 256 ];

	sprintf( &aStr[ 0 ] , "�v���C���[���W : %.3f , %.3f , %.3f" , g_Player.World.Pos.x , g_Player.World.Pos.y , g_Player.World.Pos.z );
	sprintf( &aStr01[ 0 ] , "�v���C���[�p�x : %.3f , %.3f , %.3f" , g_Player.World.Rot.x , g_Player.World.Rot.y , g_Player.World.Rot.z );

	g_pFontModel -> DrawText( NULL ,
						&aStr[ 0 ] ,
						-1 ,
						&rect ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));

	g_pFontModel -> DrawText( NULL ,
						&aStr01[ 0 ] ,
						-1 ,
						&rect01 ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));

#endif



}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void KeyboardOperate( void )
 ����:		
 �߂�l:	
 ����:		�L�[�{�[�h�ł̑���
-----------------------------------------------------------------------------*/
void KeyboardOperate( void )
{

	//	�e�̏��擾
	SHADOW *Shadow = GetShadow( 0 );

	//	���b�V���E�H�[�����擾
	MESHWALL *MeshWall = GetMeshWall();



	if( GetKeyboardPress( DIK_W ) )
	{

		g_Player.World.Pos.z += g_Player.Move.z;

		Shadow -> World.Pos.z += g_Player.Move.z;		


		//	�ړI�n�ɂǂ��炪�߂�������
		if( g_Player.World.Rot.y > 0 )
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , 180.0f , 0.0f ) , 0.1f );

		}	//	end of if

		else
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , -180.0f , 0.0f ) , 0.1f );


		}	//	end of else
	

	}	//	end of if

	if( GetKeyboardPress( DIK_S ) )
	{

		g_Player.World.Pos.z -= g_Player.Move.z;

		Shadow -> World.Pos.z -= g_Player.Move.z;


		//	�ړI�n�ɂǂ��炪�߂�������
		if( g_Player.World.Rot.y > 180 )
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , 360.0f , 0.0f ) , 0.1f );

		}	//	end of if

		else
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , 0.0f , 0.0f ) , 0.1f );

		}	//	end of else


	}	//	end of if


	if( GetKeyboardPress( DIK_A ) )
	{

		g_Player.World.Pos.x -= g_Player.Move.x;

		Shadow -> World.Pos.x -= g_Player.Move.x;


		//	�ړI�n�ɂǂ��炪�߂�������
		if( g_Player.World.Rot.y > -90 )
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , 90.0f , 0.0f ) , 0.1f );

		}	//	end of if

		else
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , -270.0f , 0.0f ) , 0.1f );

		}	//	end of else
		

	}	//	end of if


	if( GetKeyboardPress( DIK_D ) )
	{

		g_Player.World.Pos.x += g_Player.Move.x;

		Shadow -> World.Pos.x += g_Player.Move.x;


		//	�ړI�n�ɂǂ��炪�߂�������
		if( g_Player.World.Rot.y > 90 )
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , 270.0f , 0.0f ) , 0.1f );


		}	//	end of if

		else
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) , 0.1f );

		}	//	end of else


	}	//	end of if


	//	�p�x���P�W�O�x�𒴂�����
	if( g_Player.World.Rot.y > 180 )
	{

		//	-�P�V�X�x�ɂ���
		g_Player.World.Rot.y -= 360;

	}	//	end of if

	//	�p�x��-�P�W�O�x�𒴂�����
	else if( g_Player.World.Rot.y < -180 )
	{

		//	�P�V�X�x�ɂ���
		g_Player.World.Rot.y += 360;

	}	//	end of else if

	
	if( GetKeyboardPress( DIK_DOWNARROW ) )
	{

		g_Player.World.Rot.x += ROT_SPEED;

	}	//	end of if

	if( GetKeyboardPress( DIK_UPARROW ) )
	{

		g_Player.World.Rot.x -= ROT_SPEED;

	}	//	end of if

	if( GetKeyboardPress( DIK_LEFTARROW ) )
	{

		g_Player.World.Rot.y += ROT_SPEED;

	}	//	end of if

	if( GetKeyboardPress( DIK_RIGHTARROW ) )
	{

		g_Player.World.Rot.y -= ROT_SPEED;

	}	//	end of if

/*
	//	�e����
	if( GetKeyboardTrigger( DIK_SPACE ) )
	{

		//	�e�̃Z�b�g
		SetBullet( D3DXVECTOR3( g_Player.World.Pos.x , g_Player.World.Pos.y + 15.0f , g_Player.World.Pos.z ) , g_Player.World.Rot , 10.0f );

		//	���y�Đ�
		PlaySound( SOUND_LABEL_SE_SHOT );

	}	//	end of if
	*/

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void CollisionObject( void )
 ����:		
 �߂�l:	
 ����:		���̂Ƃ̓����蔻��
-----------------------------------------------------------------------------*/
void CollisionObject( void )
{

	//	�e�̏��擾
	SHADOW *Shadow = GetShadow( 0 );

	//	���b�V���E�H�[�����擾
	MESHWALL *MeshWall = GetMeshWall();


	//	�ǂƂ̔���
	if( g_Player.World.Pos.z + 10.0f > MeshWall[ 0 ].World.Pos.z )
	{

		g_Player.World.Pos = g_Player.OldPos;

		Shadow -> World.Pos = g_Player.OldPos;

	}	//	end of if

	else if( g_Player.World.Pos.z - 10.0f < MeshWall[ 1 ].World.Pos.z )
	{

		g_Player.World.Pos = g_Player.OldPos;

		Shadow -> World.Pos = g_Player.OldPos;

	}	//	end of else if

	else if( g_Player.World.Pos.x - 10.0f < MeshWall[ 3 ].World.Pos.x )
	{

		g_Player.World.Pos = g_Player.OldPos;

		Shadow -> World.Pos = g_Player.OldPos;

	}	//	end of else if

	else if( g_Player.World.Pos.x + 10.0f > MeshWall[ 2 ].World.Pos.x )
	{

		g_Player.World.Pos = g_Player.OldPos;

		Shadow -> World.Pos = g_Player.OldPos;

	}	//	end of else if



	//	�������̎擾
	BUILDING *Building = GetBuilding();

	for( int CntBuilding = 0 ; CntBuilding < MAX_BUILDING ; CntBuilding++ , Building++ )
	{
		//	���Ƌ��̓����蔻��
		if( SphereCollision( D3DXVECTOR3( g_Player.World.Pos.x , g_Player.World.Pos.y + 10.0f , g_Player.World.Pos.z ) , D3DXVECTOR3( Building -> World.Pos.x , Building -> World.Pos.y + 30.0f , Building -> World.Pos.z ) , g_Player.Radius , Building -> Radius ) )
		{

			g_Player.World.Pos = g_Player.OldPos;

			Shadow -> World.Pos = g_Player.OldPos;

		}	//	end of if

	}	//	end of for

	

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	PLAYER *GetPlayer( void )
 ����:		
 �߂�l:	return &g_Player;
 ����:		�v���C���[���̎擾
-----------------------------------------------------------------------------*/
PLAYER *GetPlayer( void )
{

	return &g_Player;

}	//	end of func