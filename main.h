#ifndef _MAIN_H_
#define _MAIN_H_

/*-----------------------------------------------------------------------------
	�w�b�_�t�@�C��
-----------------------------------------------------------------------------*/
#include <windows.h>
#include <d3dx9.h>

#pragma comment( lib , "d3d9.lib" )
#define DIRECTINPUT_VERSION ( 0x0800 )
#include <dinput.h>
#include <XAudio2.h>
	
#pragma comment( lib , "d3dx9.lib" )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "winmm.lib" )	//	�V�X�e�������擾�ɕK�v
#pragma comment( lib , "dinput8.lib" )	

/*-----------------------------------------------------------------------------
	�萔��`
-----------------------------------------------------------------------------*/
#define SCREEN_WIDTH   ( 800 )				//	�E�C���h�E�̕�
#define SCREEN_HEIGHT  ( 600 )				//	�E�C���h�E�̍���

//	���_�t�H�[�}�b�g
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL )

#define NUM_VERTEX ( 4 )	//	���_��
#define NUM_POLYGON ( 2 )	//	�|���S����

/*-----------------------------------------------------------------------------
	��
-----------------------------------------------------------------------------*/
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE;

/*-----------------------------------------------------------------------------
	�\����
-----------------------------------------------------------------------------*/
//	���_�\���́i��̃t�H�[�}�b�g�̌^�ɍ��킹�邱�Ɓj
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;	//	���W�ϊ��ςݒ��_�t���O�iDirectX�ɂ����Ȃ��d�l�j�K��1.0f������
	D3DCOLOR color;
	D3DXVECTOR2 tex;
}VERTEX_2D;

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 tex;
}VERTEX_3D;

//	���[���h���W�ϊ��p�\����
typedef struct
{
	D3DXVECTOR3 Pos;	//	���W
	D3DXVECTOR3 Rot;	//	��]��
	D3DXVECTOR3 Scl;	//	�g�嗦
}WORLD;


//	�֐��|�C���^
typedef void (*ModeFunc)(void);

/*-----------------------------------------------------------------------------
	�v���g�^�C�v�錾
-----------------------------------------------------------------------------*/

void SetWorldInv( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl );	//	�t�s�񂠂�̃��[���h���W�ϊ�
void SetWorld( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl );	//	�t�s��Ȃ��̃��[���h���W�ϊ�

LPDIRECT3DDEVICE9 GetDevice( void );	//	D3DDevice�̎擾

MODE *GetMode( void );	//	���[�h�̎擾

void SetMode( MODE mode );	//	���[�h�̃Z�b�g

#endif