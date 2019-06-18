/*=============================================================================

		�Ə�[ Reticle.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2017/01/18
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
						scanf ��warning�h�~ 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "input.h"
#include "Reticle.h"
#include "bullet.h"
#include "Player.h"
#include "Enemy.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/reticle.png"

#define POLYGON_POS_X ( 5.0f )	//	X
#define POLYGON_POS_Y ( 5.0f )	//	Y

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	���[���h�ϊ��p�ϐ�
	D3DXVECTOR3 Move;	//	�ړ���
	D3DXCOLOR Color;	//	�F
	float Length;	//	����
	bool Use;	//	�g�p�t���O

}RETICLE;

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	���_�̍쐬
HRESULT MakeVertexReticle( LPDIRECT3DDEVICE9 pDevice );

//	���_�̕ύX
void VerTexReticle( VERTEX_3D* pVtx );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferReticle = NULL;	//	���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureReticle = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

RETICLE g_Reticle;	//	���e�B�N���\����

#ifdef _DEBUG

	LPD3DXFONT g_pFontReticle = NULL;

#endif	//	_DEBUG

/*-----------------------------------------------------------------------------
 �֐���:	void InitReticle( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitReticle( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�e�N�X�`���̃G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureReticle ) ) )
	{

		MessageBox( NULL , "[ Reticle.cpp ]\n POLYGON00_TEXTURENAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	���_�̍쐬
	MakeVertexReticle( pDevice );


	//	�\���̏�����

	//	���W
	g_Reticle.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	�g�嗦
	g_Reticle.World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	��]��
	g_Reticle.World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	�ړ���
	g_Reticle.Move = D3DXVECTOR3( 5.0f , 5.0f , 5.0f );

	//	����
	g_Reticle.Length = 30.0f;

	//	�F
	g_Reticle.Color = D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f );

		
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
				&g_pFontReticle );

#endif
}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitReticle( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitReticle( void )
{

	if(g_pVtxBufferReticle != NULL)	//���_�o�b�t�@�̃C���^�[�t�F�[�X�|�C���^�̉��
	{
		g_pVtxBufferReticle -> Release();
		g_pVtxBufferReticle  = NULL;

	}	//	end of if



	if( g_pTextureReticle != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureReticle -> Release();
		g_pTextureReticle = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateReticle( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateReticle( void )
{

	//	���z�A�h���X���擾����|�C���^�ϐ�
	VERTEX_3D* pVtx;


	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferReticle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	//	���_�̕ύX
	VerTexReticle( pVtx );


	g_pVtxBufferReticle -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�


	PLAYER *Player = GetPlayer();

	if( GetKeyboardTrigger( DIK_SPACE ) )
	{

		//	�e�̃Z�b�g
		SetBullet( D3DXVECTOR3( g_Reticle.World.Pos.x , g_Reticle.World.Pos.y - 5.0f , g_Reticle.World.Pos.z ) , Player -> World.Rot , 50.0f );

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawReticle( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawReticle( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�t�s�񂠂�̃��[���h���W�ϊ�
	SetWorld( g_Reticle.World.Pos , g_Reticle.World.Rot , g_Reticle.World.Scl );
	

	//	�p�C�v���C���̃X�g���[��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferReticle , 0 , sizeof( VERTEX_3D ));


	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_3D );


	//	�e�N�X�`���̃Z�b�g
	pDevice -> SetTexture( 0 , g_pTextureReticle );


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );


	//	�|���S���̕`��
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON );


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );


#ifdef _DEBUG
/*
	RECT rect = { 0 , 40 , SCREEN_WIDTH , SCREEN_HEIGHT };
	RECT rect01 = { 0 , 60 , SCREEN_WIDTH , SCREEN_HEIGHT };

	char aStr[ 256 ] , aStr01[ 256 ];

	sprintf( &aStr[ 0 ] , "���e�B�N�����W : %.3f , %.3f , %.3f" , g_Reticle.World.Pos.x , g_Reticle.World.Pos.y , g_Reticle.World.Pos.z );
	sprintf( &aStr01[ 0 ] , "���e�B�N���p�x : %.3f , %.3f , %.3f" , g_Reticle.World.Rot.x , g_Reticle.World.Rot.y , g_Reticle.World.Rot.z );

	g_pFontReticle -> DrawText( NULL ,
						&aStr[ 0 ] ,
						-1 ,
						&rect ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));

	g_pFontReticle -> DrawText( NULL ,
						&aStr01[ 0 ] ,
						-1 ,
						&rect01 ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));
*/
#endif


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexReticle( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice	�f�o�C�X
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexReticle( LPDIRECT3DDEVICE9 pDevice )
{

	// �\���̂̃|�C���^�錾
	VERTEX_3D* pVtx;


	// FAILED�}�N���ŃG���[�`�F�b�N
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_3D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferReticle , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// �o�b�t�@�����b�N���A���z�A�h���X���擾
	g_pVtxBufferReticle -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	//	���_���W�̐ݒ�
	pVtx[ 0 ].pos = D3DXVECTOR3( POLYGON_POS_X , POLYGON_POS_Y  , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( -POLYGON_POS_X  , POLYGON_POS_Y  , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( POLYGON_POS_X , -POLYGON_POS_Y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( -POLYGON_POS_X  , -POLYGON_POS_Y , 0.0f );

	//	�@���̐ݒ�
	pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	pVtx[ 1 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	pVtx[ 2 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
	pVtx[ 3 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );

	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
	pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

	//	�e�N�X�`�����W�̐ݒ�
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferReticle -> Unlock(); //����ȍ~�G��Ă͂����Ȃ�

	return S_OK;


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void VerTexReticle( VERTEX_3D* pVtx )
 ����:		VERTEX_3D* pVtx
 �߂�l:	
 ����:		���_�̕ύX
-----------------------------------------------------------------------------*/
void VerTexReticle( VERTEX_3D* pVtx )
{


	//	���_�F�̐ݒ�
	pVtx[ 0 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Reticle.Color.r , g_Reticle.Color.g , g_Reticle.Color.b , g_Reticle.Color.a );



}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void SetReticle( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXCOLOR Color )
 ����:		D3DXVECTOR3 Pos		���W
			D3DXVECTOR3 Rot		��]��
			D3DXCOLOR Color		�F
 �߂�l:	
 ����:		���e�B�N���̃Z�b�g
-----------------------------------------------------------------------------*/
void SetReticle( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXCOLOR Color )
{

	float rot = Rot.y + 180;

	//	���W
	g_Reticle.World.Pos.x = cosf( D3DXToRadian( Rot.x ) ) * sinf( D3DXToRadian( rot ) ) * g_Reticle.Length + Pos.x;
	g_Reticle.World.Pos.y = sinf( D3DXToRadian( Rot.x ) ) * g_Reticle.Length + Pos.y;
	g_Reticle.World.Pos.z = cosf( D3DXToRadian( Rot.x ) ) * cosf( D3DXToRadian( rot ) ) * g_Reticle.Length + Pos.z;

	//	��]��
	g_Reticle.World.Rot = Rot;

	//	�F
	g_Reticle.Color = Color;

}	//	end of func