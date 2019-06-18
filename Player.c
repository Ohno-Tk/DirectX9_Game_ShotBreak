/*=============================================================================

		プレイヤー[ Player.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/12/29
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
						scanf のwarning防止 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	ヘッダファイル
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
	定数定義
-----------------------------------------------------------------------------*/
#define MODEL_NAME "data/MODEL/Player/CowBoy.x"

#define MAX_MAT ( 8 )	//	モデルの最大マテリアル数

#define ROT_SPEED ( 1.5f )	//	回転量

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	キーボードでの操作
void KeyboardOperate( void );

//	物体との当たり判定
void CollisionObject( void );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPD3DXMESH g_pMeshPlayer;	//	メッシュインターフェースポインタ

LPD3DXBUFFER g_pBufferMatPlayer;	//	マテリアル情報

DWORD g_nNumMatPlayer;	//	マテリアル数

LPDIRECT3DTEXTURE9 g_pTexturePlayer[ MAX_MAT ] = { NULL };//	テクスチャインターフェース

D3DXMATERIAL* pMatPlayer;	//	マテリアルポインタ

LPD3DXFONT g_pFontPlayer = NULL;	//	フォントポインタ

PLAYER g_Player;	//	プレイヤー構造体

#ifdef _DEBUG

	LPD3DXFONT g_pFontModel = NULL;

#endif	//	_DEBUG

/*-----------------------------------------------------------------------------
 関数名:	void InitPlayer( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitPlayer( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	次のモードの取得
	MODE *Mode = GetNextMode();


	HRESULT hr;

	hr = D3DXLoadMeshFromX( MODEL_NAME ,			//	ファイル名
							D3DXMESH_MANAGED,
							pDevice,				//	デバイス
							NULL,					//	隣接バッファ
							&g_pBufferMatPlayer,		//	マテリアル情報を格納
							NULL,
							&g_nNumMatPlayer,		//	マテリアル数
							&g_pMeshPlayer );		//	メッシュ

	//	モデルのエラーチェック
	if( FAILED( hr ) )
	{

		MessageBox( NULL , "[ Player.cpp ]\n MODEL_NAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	マテリアル情報のポインタとしてバッファのアドレスを取得
	pMatPlayer = ( D3DXMATERIAL* )g_pBufferMatPlayer -> GetBufferPointer();

	for( int i = 0 ; i < ( int )g_nNumMatPlayer ; i++ )
	{
		if( pMatPlayer[ i ].pTextureFilename != NULL )
		{

			//	テクスチャのエラーチェック
			if( FAILED( D3DXCreateTextureFromFile( pDevice , pMatPlayer[ i ].pTextureFilename , &g_pTexturePlayer[ i ]  ) ) )
			{
				MessageBox( NULL , "モデルの画像の読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

			}	//	end of if

		}	//	end of if

	}	//	end of for



	//	構造体初期化

	if( *Mode == MODE_TITLE )
	{

		//	座標
		g_Player.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , 60.0f );

		//	回転量
		g_Player.World.Rot = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	}	//	end of if

	else if( *Mode == MODE_GAME )
	{

		//	座標
		g_Player.World.Pos = D3DXVECTOR3( 0.0f , 0.0f , -1000.0f );

		//	回転量
		g_Player.World.Rot = D3DXVECTOR3( 0.0f , 180.0f , 0.0f );

	}	//	end of else if

	//	拡大率
	g_Player.World.Scl = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	前の座標
	g_Player.OldPos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );

	//	移動量
	g_Player.Move = D3DXVECTOR3( 5.0f , 5.0f , 5.0f );

	//	回転量
	g_Player.RotSpeed = D3DXVECTOR3( 1.0f , 1.0f , 1.0f );

	//	半径
	g_Player.Radius = 5.0f;

	//	使用フラグ
	g_Player.Use = true;


	//	影のセット
	SetShadow( g_Player.World.Pos , 15.0f , D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f ) );


#ifdef _DEBUG
	//	フォントの設定
	D3DXCreateFont( pDevice ,
				18 ,	//	文字高さ
				0 ,	//	文字幅
				0 ,	//	文字の太さ
				0 ,	//	ミップマップ
				FALSE ,	//	イタリック
				SHIFTJIS_CHARSET ,
				OUT_DEFAULT_PRECIS ,
				DEFAULT_QUALITY ,
				DEFAULT_PITCH ,
				"Terminal" ,
				&g_pFontModel );
#endif


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitPlayer( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitPlayer( void )
{

	if( g_pMeshPlayer != NULL )	//	メッシュインターフェースポインタ
	{

		g_pMeshPlayer -> Release();
		g_pMeshPlayer = NULL;

	}	//	end of if

	if( g_pBufferMatPlayer != NULL )	//	マテリアル情報開放
	{

		g_pBufferMatPlayer -> Release();
		g_pBufferMatPlayer = NULL;

	}	//	end of if

}	//	end of func
 
/*-----------------------------------------------------------------------------
 関数名:	void UpdataPlayer( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdatePlayer( void )
{

	//	次のモードの取得
	MODE *Mode = GetNextMode();


	if( *Mode == MODE_GAME )
	{

		//	前回の座標を取得
		g_Player.OldPos = g_Player.World.Pos;


		//	キーボードでの操作
		KeyboardOperate();

		//	物体との当たり判定
		CollisionObject();


		//	カメラのセット
		SetCamera( g_Player.World.Pos , g_Player.World.Rot , g_Player.Move );

		//	レティクルのセット
		SetReticle( D3DXVECTOR3( g_Player.World.Pos.x , g_Player.World.Pos.y + 25.0f , g_Player.World.Pos.z ) , g_Player.World.Rot , D3DXCOLOR( 0.0f , 1.0f , 0.0f , 1.0f ) );

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawPlayer( void )
 引数:		
 戻り値:	
 説明:		モデルの描画
-----------------------------------------------------------------------------*/
void DrawPlayer( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	現在デバイスに設定されているマテリアル情報の取得
	D3DMATERIAL9 matDef;
	pDevice -> GetMaterial( &matDef );


	if( g_Player.Use == true)
	{

		//	逆行列なしのワールド座標変換
		SetWorld( g_Player.World.Pos , g_Player.World.Rot , g_Player.World.Scl );


		for( int i = 0 ; i < ( int )g_nNumMatPlayer ; i++ )
		{
			//	デバイスにマテリアルを設定
			pDevice -> SetMaterial( &pMatPlayer[ i ].MatD3D );

			//	テクスチャ設定
			pDevice -> SetTexture( 0 , g_pTexturePlayer[ i ] );


			//	描画
			g_pMeshPlayer -> DrawSubset( i );

		}	//	end of for

	}	//	end of if

	pDevice -> SetMaterial( &matDef );

#ifdef _DEBUG

	RECT rect = { 0 , 40 , SCREEN_WIDTH , SCREEN_HEIGHT };
	RECT rect01 = { 0 , 60 , SCREEN_WIDTH , SCREEN_HEIGHT };

	char aStr[ 256 ] , aStr01[ 256 ];

	sprintf( &aStr[ 0 ] , "プレイヤー座標 : %.3f , %.3f , %.3f" , g_Player.World.Pos.x , g_Player.World.Pos.y , g_Player.World.Pos.z );
	sprintf( &aStr01[ 0 ] , "プレイヤー角度 : %.3f , %.3f , %.3f" , g_Player.World.Rot.x , g_Player.World.Rot.y , g_Player.World.Rot.z );

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
 関数名:	void KeyboardOperate( void )
 引数:		
 戻り値:	
 説明:		キーボードでの操作
-----------------------------------------------------------------------------*/
void KeyboardOperate( void )
{

	//	影の情報取得
	SHADOW *Shadow = GetShadow( 0 );

	//	メッシュウォール情報取得
	MESHWALL *MeshWall = GetMeshWall();



	if( GetKeyboardPress( DIK_W ) )
	{

		g_Player.World.Pos.z += g_Player.Move.z;

		Shadow -> World.Pos.z += g_Player.Move.z;		


		//	目的地にどちらが近いか判定
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


		//	目的地にどちらが近いか判定
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


		//	目的地にどちらが近いか判定
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


		//	目的地にどちらが近いか判定
		if( g_Player.World.Rot.y > 90 )
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , 270.0f , 0.0f ) , 0.1f );


		}	//	end of if

		else
		{

			D3DXVec3Lerp( &g_Player.World.Rot , &g_Player.World.Rot , &D3DXVECTOR3( 0.0f , -90.0f , 0.0f ) , 0.1f );

		}	//	end of else


	}	//	end of if


	//	角度が１８０度を超えたら
	if( g_Player.World.Rot.y > 180 )
	{

		//	-１７９度にする
		g_Player.World.Rot.y -= 360;

	}	//	end of if

	//	角度が-１８０度を超えたら
	else if( g_Player.World.Rot.y < -180 )
	{

		//	１７９度にする
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
	//	弾発射
	if( GetKeyboardTrigger( DIK_SPACE ) )
	{

		//	弾のセット
		SetBullet( D3DXVECTOR3( g_Player.World.Pos.x , g_Player.World.Pos.y + 15.0f , g_Player.World.Pos.z ) , g_Player.World.Rot , 10.0f );

		//	音楽再生
		PlaySound( SOUND_LABEL_SE_SHOT );

	}	//	end of if
	*/

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void CollisionObject( void )
 引数:		
 戻り値:	
 説明:		物体との当たり判定
-----------------------------------------------------------------------------*/
void CollisionObject( void )
{

	//	影の情報取得
	SHADOW *Shadow = GetShadow( 0 );

	//	メッシュウォール情報取得
	MESHWALL *MeshWall = GetMeshWall();


	//	壁との判定
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



	//	建物情報の取得
	BUILDING *Building = GetBuilding();

	for( int CntBuilding = 0 ; CntBuilding < MAX_BUILDING ; CntBuilding++ , Building++ )
	{
		//	球と球の当たり判定
		if( SphereCollision( D3DXVECTOR3( g_Player.World.Pos.x , g_Player.World.Pos.y + 10.0f , g_Player.World.Pos.z ) , D3DXVECTOR3( Building -> World.Pos.x , Building -> World.Pos.y + 30.0f , Building -> World.Pos.z ) , g_Player.Radius , Building -> Radius ) )
		{

			g_Player.World.Pos = g_Player.OldPos;

			Shadow -> World.Pos = g_Player.OldPos;

		}	//	end of if

	}	//	end of for

	

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	PLAYER *GetPlayer( void )
 引数:		
 戻り値:	return &g_Player;
 説明:		プレイヤー情報の取得
-----------------------------------------------------------------------------*/
PLAYER *GetPlayer( void )
{

	return &g_Player;

}	//	end of func