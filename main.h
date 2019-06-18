#ifndef _MAIN_H_
#define _MAIN_H_

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <windows.h>
#include <d3dx9.h>

#pragma comment( lib , "d3d9.lib" )
#define DIRECTINPUT_VERSION ( 0x0800 )
#include <dinput.h>
#include <XAudio2.h>
	
#pragma comment( lib , "d3dx9.lib" )
#pragma comment( lib , "dxguid.lib" )
#pragma comment( lib , "winmm.lib" )	//	システム時刻取得に必要
#pragma comment( lib , "dinput8.lib" )	

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define SCREEN_WIDTH   ( 800 )				//	ウインドウの幅
#define SCREEN_HEIGHT  ( 600 )				//	ウインドウの高さ

//	頂点フォーマット
#define FVF_VERTEX_2D ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1 )
#define FVF_VERTEX_3D ( D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_NORMAL )

#define NUM_VERTEX ( 4 )	//	頂点数
#define NUM_POLYGON ( 2 )	//	ポリゴン数

/*-----------------------------------------------------------------------------
	列挙
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
	構造体
-----------------------------------------------------------------------------*/
//	頂点構造体（上のフォーマットの型に合わせること）
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;	//	座標変換済み頂点フラグ（DirectXにしかない仕様）必ず1.0fを入れる
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

//	ワールド座標変換用構造体
typedef struct
{
	D3DXVECTOR3 Pos;	//	座標
	D3DXVECTOR3 Rot;	//	回転量
	D3DXVECTOR3 Scl;	//	拡大率
}WORLD;


//	関数ポインタ
typedef void (*ModeFunc)(void);

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

void SetWorldInv( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl );	//	逆行列ありのワールド座標変換
void SetWorld( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl );	//	逆行列なしのワールド座標変換

LPDIRECT3DDEVICE9 GetDevice( void );	//	D3DDeviceの取得

MODE *GetMode( void );	//	モードの取得

void SetMode( MODE mode );	//	モードのセット

#endif