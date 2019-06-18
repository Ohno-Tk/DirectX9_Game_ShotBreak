/*=============================================================================

		�e[ bullet.cpp ]

-------------------------------------------------------------------------------
	���@�����
		�����

	���@�쐬��
		2016/11/24
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <math.h>
#include "main.h"
#include "input.h"
#include "sound.h"
#include "bullet.h"
#include "Utility.h"
#include "explosion.h"
#include "MeshWall.h"
#include "Enemy.h"
#include "EnemyFly.h"
#include "Score.h"
#include "Shadow.h"
#include "Effect.h"

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/bullet000.png"

#define MAX_BULLET ( 100 )	//	�e�ő吔

#define POLYGON_X ( 5.0f )	//	�|���S���̂w
#define POLYGON_Y ( 5.0f )	//	�|���S���̂x

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
typedef struct
{
	WORLD World;	//	���[���h�ϊ��p�ϐ�
	D3DXVECTOR3 Move;	//	�ړ���
	int LIfe;			//	����
	float length;	//	����
	float Radius;	//	���a
	bool Use;	//	�g�p�t���O

}BULLET;

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

//	���_�̍쐬
HRESULT MakeVertexBullet( LPDIRECT3DDEVICE9 pDevice );

//	�����蔻��
void Collision( int CntBullet );

/*-----------------------------------------------------------------------------
	�O���[�o���ϐ�
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferBullet = NULL;	//	���_�o�b�t�@�C���^�t�F�[�X�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;//	�e�N�X�`���C���^�[�t�F�[�X

BULLET g_Bullet[ MAX_BULLET ];	//	�e�̍\����

/*-----------------------------------------------------------------------------
 �֐���:	void InitBullet( void )
 ����:		
 �߂�l:	
 ����:		������
-----------------------------------------------------------------------------*/
void InitBullet( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�G���[�`�F�b�N
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureBullet  ) ) )
	{
		MessageBox( NULL , "[ bullet.cpp ]\n POLYGON00_TEXTURENAME\n�̓ǂݍ��݂��ł��܂���ł���" , "�x��" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	���_�̍쐬
	MakeVertexBullet( pDevice );


	//	�\���̏�����
	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{
		//	���W
		g_Bullet[ Cnt ].World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�g�嗦
		g_Bullet[ Cnt ].World.Scl =  D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

		//	��]��
		g_Bullet[ Cnt ].World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	�ړ���
		g_Bullet[ Cnt ].Move = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

		//	����
		g_Bullet[ Cnt ].LIfe = 0;

		//	����
		g_Bullet[ Cnt ].length = 1.0f;

		//	���a
		g_Bullet[ Cnt ].Radius = 30.0f;

		//	�g�p�t���O
		g_Bullet[ Cnt ].Use = false;

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UninitBullet( void )
 ����:		
 �߂�l:	
 ����:		�I��
-----------------------------------------------------------------------------*/
void UninitBullet( void )
{

	if( g_pVtxBufferBullet != NULL )	//	���_�o�b�t�@�C���^�t�F�[�X�J��
	{
		g_pVtxBufferBullet -> Release();
		g_pVtxBufferBullet = NULL;

	}	//	end of if

	if( g_pTextureBullet != NULL )	//	�e�N�X�`���|���S���J��
	{
		g_pTextureBullet -> Release();
		g_pTextureBullet = NULL;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void UpdateBullet( void )
 ����:		
 �߂�l:	
 ����:		�X�V
-----------------------------------------------------------------------------*/
void UpdateBullet( void )
{

	//	�e�̏��擾
	SHADOW *Shadow = GetShadow( 1 );


	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		if( g_Bullet[ Cnt ].Use == true )
		{			

			//	���W�ړ�
			g_Bullet[ Cnt ].World.Pos += g_Bullet[ Cnt ].Move;

			//	�^�[�Q�b�g�Ƃ̓����蔻��
			Collision( Cnt );

			//	�G�t�F�N�g�̃Z�b�g
			SetEffect( g_Bullet[ Cnt ].World.Pos , D3DXCOLOR( 255 , 0 , 0 , 255 ) , 5 , 15.0f );


		}	//	end of if

	}	//	end of for


}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void DrawBullet( void )
 ����:		
 �߂�l:	
 ����:		�`��
-----------------------------------------------------------------------------*/
void DrawBullet( void )
{

	//	�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	�p�C�v���C���̃X�g���[��
	pDevice -> SetStreamSource( 0 , g_pVtxBufferBullet , 0 , sizeof( VERTEX_3D ));

	//	���_�t�H�[�}�b�g�̐ݒ�
	pDevice -> SetFVF( FVF_VERTEX_3D );

	//	�e�N�X�`���̃Z�b�g
	pDevice -> SetTexture( 0 , g_pTextureBullet );

	//	Z�o�b�t�@
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , FALSE );


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , FALSE );



	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		if( g_Bullet[ Cnt ].Use == true )
		{

			//	�t�s�񂠂�̃��[���h���W�ϊ�
			SetWorldInv( g_Bullet[ Cnt ].World.Pos , g_Bullet[ Cnt ].World.Rot , g_Bullet[ Cnt ].World.Scl );

			//	�|���S���̕`��
			pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX , NUM_POLYGON );

		}	//	end of if

	}	//	end of for


	//	�S�̂̃��C�g��L���ɂ���
	pDevice -> SetRenderState( D3DRS_LIGHTING , TRUE );

	//	���ɖ߂�
	pDevice -> SetRenderState( D3DRS_ZENABLE , D3DZB_TRUE );
	pDevice -> SetRenderState( D3DRS_ZWRITEENABLE , TRUE );

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	HRESULT MakeVertexBullet( LPDIRECT3DDEVICE9 pDevice )
 ����:		LPDIRECT3DDEVICE9 pDevice	�f�o�C�X
 �߂�l:	�ǂ��ꍇ	return S_OK;
			�_���ȏꍇ	return E_FAIL;
 ����:		���_�̍쐬
-----------------------------------------------------------------------------*/
HRESULT MakeVertexBullet( LPDIRECT3DDEVICE9 pDevice )
{

	VERTEX_3D* pVtx;	//	���z�A�h���X���擾����|�C���^�ϐ�


	//	���_�o�b�t�@�̐���
	if( FAILED( pDevice -> CreateVertexBuffer( sizeof( VERTEX_3D ) * NUM_VERTEX * MAX_BULLET , D3DUSAGE_WRITEONLY , FVF_VERTEX_3D , D3DPOOL_MANAGED , &g_pVtxBufferBullet , NULL ) ) )
	{
		return E_FAIL;
	}	//	end of if


	//	�o�b�t�@�����b�N�����z�A�h���X���擾����
	g_pVtxBufferBullet -> Lock( 0 , 0 , (void**)&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		//	���_���W�̐ݒ�
		pVtx[ 0 ].pos = D3DXVECTOR3( -POLYGON_X , POLYGON_Y  , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( POLYGON_X  , POLYGON_Y  , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( -POLYGON_X , -POLYGON_Y  , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( POLYGON_X  , -POLYGON_Y  , 0.0f );


		//	�@���̐ݒ�
		pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 1 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 2 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );
		pVtx[ 3 ].normal = D3DXVECTOR3( 0.0f , 0.0f , -1.0f );

		//	���_�F�̐ݒ�
		pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
		pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

		//	�e�N�X�`�����W�̐ݒ�
		pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

		pVtx += 4;	//	pVtx�����炷

	}	//	end of for


	//	�o�b�t�@�̃A�����b�N
	g_pVtxBufferBullet -> Unlock();

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void SetBullet( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , float Length )
 ����:		D3DXVECTOR3 Pos		���W
			D3DXVECTOR3 Rot		��]��
			float Length		����
 �߂�l:	
 ����:		�e�̃Z�b�g
-----------------------------------------------------------------------------*/
void SetBullet( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , float Length )
{

	float rot = Rot.y + 180;

	for( int Cnt = 0 ; Cnt < MAX_BULLET ; Cnt++ )
	{

		if( g_Bullet[ Cnt ].Use == false )
		{

			//	���W
			//g_Bullet[ CntBullet ].World.Pos.x = cosf( D3DXToRadian( Rot.x ) ) * sinf( D3DXToRadian( rot ) ) * Length + Pos.x;
			//g_Bullet[ CntBullet ].World.Pos.y = sinf( D3DXToRadian( Rot.x ) ) * Length + Pos.y;
			//g_Bullet[ CntBullet ].World.Pos.z = cosf( D3DXToRadian( Rot.x ) ) * cosf( D3DXToRadian( rot ) ) * Length + Pos.z;

			g_Bullet[ Cnt ].World.Pos.x = Pos.x;
			g_Bullet[ Cnt ].World.Pos.y = Pos.y;
			g_Bullet[ Cnt ].World.Pos.z = Pos.z;


			//	�ړ���
			g_Bullet[ Cnt ].Move.x = cosf( D3DXToRadian( Rot.x ) ) * sinf( D3DXToRadian( rot ) ) * Length;
			g_Bullet[ Cnt ].Move.y = sinf( D3DXToRadian( Rot.x ) ) * Length;
			g_Bullet[ Cnt ].Move.z = cosf( D3DXToRadian( Rot.x ) ) * cosf( D3DXToRadian( rot ) ) * Length;

			//	����
			g_Bullet[ Cnt ].LIfe = 100;

			//	�g�p�t���O
			g_Bullet[ Cnt ].Use = true;

			break;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 �֐���:	void Collision( int Cnt )
 ����:		int Cnt	�e�\���̂̃J�E���^
 �߂�l:	
 ����:		�����蔻��
-----------------------------------------------------------------------------*/
void Collision( int Cnt )
{

	//	�X�R�A���̎擾
	int *Score = GetScore();

	//	�^�[�Q�b�g���̎擾
	ENEMY *Enemy = GetEnemyNor();
	ENEMY *EnemyFly = GetEnemyFly();

	//	���b�V���E�H�[�����擾
	MESHWALL *MeshWall = GetMeshWall();


	for( int CntEnemy = 0 ; CntEnemy < MAX_ENEMY ; CntEnemy++ , Enemy++ )
	{

		if( Enemy -> Use == true )
		{

			//	���̓����蔻��
			if( SphereCollision( g_Bullet[ Cnt ].World.Pos , D3DXVECTOR3( Enemy -> World.Pos.x , Enemy -> World.Pos.y + 20.0f , Enemy -> World.Pos.z ) , g_Bullet[ Cnt ].Radius , Enemy -> Radius ) )
			{

				//	�G�̎g�p�t���O��OFF�ɂ���
				Enemy -> Use = false;

				//	�e�̎g�p�t���O��OFF�ɂ���
				g_Bullet[ Cnt ].Use = false;

				//	�^�[�Q�b�g�̉e�������i +1�̓v���C���[�����ɍ���Ă��邩�� �j
				IndexShadow( CntEnemy + 1 );


				//	�X�R�A�̉��Z
				*Score += 50;

				//	�����̃Z�b�g
				SetExplosin( D3DXVECTOR3( Enemy -> World.Pos.x , Enemy -> World.Pos.y + 20.0f , Enemy -> World.Pos.z ) , 30.0f );

				//	���y�Đ�
				PlaySound( SOUND_LABEL_SE_HIT );

			}	//	end of if

		}	//	end of if
				
	}	//	end of for

	for( int CntEnemyFly = 0 ; CntEnemyFly < MAX_ENEMY_FLY ; CntEnemyFly++ , EnemyFly++ )
	{

		if( EnemyFly -> Use == true )
		{

			//	���̓����蔻��
			if( SphereCollision( g_Bullet[ Cnt ].World.Pos , D3DXVECTOR3( EnemyFly -> World.Pos.x , EnemyFly -> World.Pos.y + 20.0f , EnemyFly -> World.Pos.z ) , g_Bullet[ Cnt ].Radius , EnemyFly -> Radius ) )
			{

				//	�G�̎g�p�t���O��OFF�ɂ���
				EnemyFly -> Use = false;

				//	�e�̎g�p�t���O��OFF�ɂ���
				g_Bullet[ Cnt ].Use = false;

				//	�^�[�Q�b�g�̉e�������i +1�̓v���C���[�����ɍ���Ă��邩�� �j
				IndexShadow( CntEnemyFly + 11 );


				//	�X�R�A�̉��Z
				*Score += 100;

				//	�����̃Z�b�g
				SetExplosin( D3DXVECTOR3( EnemyFly -> World.Pos.x , EnemyFly -> World.Pos.y + 20.0f , EnemyFly -> World.Pos.z ) , 30.0f );

				//	���y�Đ�
				PlaySound( SOUND_LABEL_SE_HIT );

			}

		}

	}	//	end of for

	//	���b�V���E�H�[���Ƃ̓����蔻��

	//	�y������
	if( g_Bullet[ Cnt ].World.Pos.z + 20.0f > MeshWall[ 0 ].World.Pos.z || g_Bullet[ Cnt ].World.Pos.z - 20.0f < MeshWall[ 1 ].World.Pos.z )
	{

		g_Bullet[ Cnt ].Use = false;

	}	//	end of if

	//	X������
	if( g_Bullet[ Cnt ].World.Pos.x + 20.0f > MeshWall[ 2 ].World.Pos.x || g_Bullet[ Cnt ].World.Pos.x - 20.0f < MeshWall[ 3 ].World.Pos.x )
	{

		g_Bullet[ Cnt ].Use = false;

	}	//	end of if

}

/*-----------------------------------------------------------------------------
 �֐���:	BULLET *GetBullet( void )
 ����:		
 �߂�l:	return &g_Bullet[ 0 ];
 ����:		�e���擾
-----------------------------------------------------------------------------*/
BULLET *GetBullet( void )
{
	return &g_Bullet[ 0 ];

}	//	end of func