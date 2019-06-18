/*=============================================================================

		カメラ[ camera.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/10/19
-------------------------------------------------------------------------------
	■　Update
		2016/10/19
=============================================================================*/
/*-----------------------------------------------------------------------------
						scanf のwarning防止 
-----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_WARNINGS

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include "main.h"
#include "input.h"
#include "camera.h"

#include "fade.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define CAMERA_NEAR ( 1.0f )	//	near
#define CAMERA_FAR ( 4000.0f )	//	far

#define SPEED ( 3.0f )	//	注視点移動量

#define RADIUS ( 0.05f )	//	ラディウスの増加量

#define MOVE_SPEED ( 2.0f )	//	移動量
#define ROT_SPEED ( 0.6f )	//	回転量

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
#ifdef _DEBUG
	LPD3DXFONT g_pFontCamera = NULL;
#endif	//	_DEBUG

float g_fLengthPolygon;	//	対角線
float g_fAnglePolygon;	//	角度シータ


D3DXVECTOR3 g_posEye( 0.0f , 60.0f , -150.0f );	//	カメラ
D3DXVECTOR3 g_posAt( 0.0f , 30.0f , 0.0f );	//	注視点
D3DXVECTOR3 g_vecUp( 0.0f , 1.0f , 0.0f );	//	上の視点

//	ビュー行列の作成
D3DXMATRIX g_mtxView;

/*-----------------------------------------------------------------------------
 関数名:	void InitCamera( void )
 引数:		なし
 戻り値:	なし
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitCamera( void )
{
	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

#ifdef _DEBUG
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
					&g_pFontCamera);
#endif	//	_DEBUG


	//	初期化

	g_fLengthPolygon = -g_posEye.z;

	g_fAnglePolygon = 0;


	//	次のモードの取得
	MODE *Mode = GetNextMode();

	if( *Mode == MODE_TITLE )
	{

		g_posEye = D3DXVECTOR3( 0.0f , 120.0f , -150.0f );	//	カメラ
		g_posAt = D3DXVECTOR3( 0.0f , 30.0f , 0.0f );	//	注視点

	}	//	end of if

	else if( *Mode == MODE_GAME )
	{

		g_posEye = D3DXVECTOR3( 0.0f , 150.0f , -1000.0f );	//	カメラ
		g_posAt = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );	//	注視点

	}	//	end of if

	else if( *Mode == MODE_RESULT )
	{

		g_posEye = D3DXVECTOR3( 0.0f , 80.0f , -150.0f );	//	カメラ
		g_posAt = D3DXVECTOR3( 0.0f , 30.0f , 0.0f );	//	注視点

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitCamera( void )
 引数:		なし
 戻り値:	なし
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitCamera( void )
{
#ifdef _DEBUG

	if( g_pFontCamera != NULL )	//	フォントポインタ開放
	{
		g_pFontCamera -> Release();
		g_pFontCamera = NULL;

	}	//	end of if

#endif	//	_DEBUG

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateCamera( void )
 引数:		なし
 戻り値:	なし
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateCamera( void )
{

#if 0

	//	視点移動
	if( GetKeyboardPress( DIK_W ) )
	{
		g_posEye.x += sin( -g_fAnglePolygon ) * MOVE_SPEED;
		g_posEye.z += cos( -g_fAnglePolygon ) * MOVE_SPEED;

		g_posAt.x += sin( -g_fAnglePolygon ) * MOVE_SPEED;
		g_posAt.z += cos( -g_fAnglePolygon ) * MOVE_SPEED;

	}	//	end of if

	if( GetKeyboardPress( DIK_S ) )
	{
		g_posEye.x -= sin( -g_fAnglePolygon ) * MOVE_SPEED;
		g_posEye.z -= cos( -g_fAnglePolygon ) * MOVE_SPEED;

		g_posAt.x -= sin( -g_fAnglePolygon ) * MOVE_SPEED;
		g_posAt.z -= cos( -g_fAnglePolygon ) * MOVE_SPEED;

	}	//	end of if

	if( GetKeyboardPress( DIK_A ) )
	{
		g_posEye.x -= cos( g_fAnglePolygon ) * MOVE_SPEED;
		g_posEye.z -= sin( g_fAnglePolygon ) * MOVE_SPEED;

		g_posAt.x -= cos( g_fAnglePolygon ) * MOVE_SPEED;
		g_posAt.z -= sin( g_fAnglePolygon ) * MOVE_SPEED;

	}	//	end of if

	if( GetKeyboardPress( DIK_D ) )
	{
		g_posEye.x += cos( g_fAnglePolygon ) * MOVE_SPEED;
		g_posEye.z += sin( g_fAnglePolygon ) * MOVE_SPEED;

		g_posAt.x += cos( g_fAnglePolygon ) * MOVE_SPEED;
		g_posAt.z += sin( g_fAnglePolygon ) * MOVE_SPEED;

	}	//	end of if

	

	//	注視点旋回
	if( GetKeyboardPress( DIK_Q ) )
	{

		g_posAt.x = g_fLengthPolygon * cos( g_fAnglePolygon + D3DX_PI * 0.5f ) + posEye.x;
		g_posAt.z = g_fLengthPolygon * sin( g_fAnglePolygon + D3DX_PI * 0.5f ) + posEye.z;

		g_fAnglePolygon += RADIUS;

	}	//	end of if

	else if( GetKeyboardPress( DIK_E ) )
	{

		g_posAt.x = g_fLengthPolygon * cos( g_fAnglePolygon + D3DX_PI * 0.5f ) + posEye.x;
		g_posAt.z = g_fLengthPolygon * sin( g_fAnglePolygon + D3DX_PI * 0.5f ) + posEye.z;

		g_fAnglePolygon -= RADIUS;

	}	//	end of if


	//	視点旋回
	if( GetKeyboardPress( DIK_Z ) )
	{

		g_posEye.x = g_fLengthPolygon * cos( g_fAnglePolygon + D3DX_PI * 1.5f ) + posAt.x;
		g_posEye.z = g_fLengthPolygon * sin( g_fAnglePolygon + D3DX_PI * 1.5f ) + posAt.z;

		g_fAnglePolygon -= RADIUS;

	}	//	end of if

	else if( GetKeyboardPress( DIK_C ) )
	{

		g_posEye.x = g_fLengthPolygon * cos( g_fAnglePolygon + D3DX_PI * 1.5f ) + posAt.x;
		g_posEye.z = g_fLengthPolygon * sin( g_fAnglePolygon + D3DX_PI * 1.5f ) + posAt.z;

		g_fAnglePolygon += RADIUS;

	}	//	end of if

	//	注視点移動
	if( GetKeyboardPress( DIK_T ) )
	{
		g_posAt.y += SPEED;

	}	//	end of if

	else if( GetKeyboardPress( DIK_B ) )
	{
		g_posAt.y -= SPEED;

	}	//	end of if

	//	カメラ移動
	if( GetKeyboardPress( DIK_Y ) )
	{
		g_posEye.y += SPEED;

	}	//	end of if

	else if( GetKeyboardPress( DIK_N ) )
	{
		g_posEye.y -= SPEED;

	}	//	end of if

#endif

	//	次のモードの取得
	MODE *Mode = GetNextMode();

	bool *FadeFlag = GetFadeFlag();

	if( *Mode == MODE_TITLE && *FadeFlag == false )
	{

		g_posEye.x = g_fLengthPolygon * cos( g_fAnglePolygon + D3DX_PI * 1.5f ) + g_posAt.x;
		g_posEye.z = g_fLengthPolygon * sin( g_fAnglePolygon + D3DX_PI * 1.5f ) + g_posAt.z;

		g_fAnglePolygon += 0.01f;

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawCamera( void )
 引数:		なし
 戻り値:	なし
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawCamera( void )
{

#ifdef _DEBUG

//	DebugCamera();

#endif

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	ワールド座標変換
	//	ワールドマトリクスの作成
	D3DXMATRIX mtxWorld;

	D3DXMatrixIdentity( &mtxWorld );	//	行列を単位行列にする

	//	デバイスにワールド変換行列を設定
	pDevice -> SetTransform( D3DTS_WORLD , &mtxWorld );

	//	ビュー座標変換
	D3DXMatrixLookAtLH( &g_mtxView , &g_posEye , &g_posAt , &g_vecUp );

	//	デバイスにビュー変換行列を設定
	pDevice -> SetTransform( D3DTS_VIEW , &g_mtxView );

	//	プロジェクション変換
	//	プロジェクション行列の作成
	D3DXMATRIX mtxProj;

#if 0
	//	パースペクティブ行列
	D3DXMatrixPerspectiveFovLH( &mtxProj ,
								D3DX_PI / 3.0f ,	//	画角
								( float )SCREEN_WIDTH / ( SCREEN_HEIGHT / 2 ) ,	//	アスペクト比
								CAMERA_NEAR ,	//	near	値を絶対0以下にしない
								CAMERA_FAR );	//	far

#endif

	//	パースペクティブ行列
	D3DXMatrixPerspectiveFovLH( &mtxProj ,
								D3DX_PI / 3.0f ,	//	画角
								( float )SCREEN_WIDTH / SCREEN_HEIGHT ,	//	アスペクト比
								CAMERA_NEAR ,	//	near	値を絶対0以下にしない
								CAMERA_FAR );	//	far

	//	デバイスにプロジェクション変換行列を設定
	pDevice -> SetTransform( D3DTS_PROJECTION , &mtxProj );


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetCamera( D3DXVECTOR3 Pos )
 引数:		D3DXVECTOR3 Pos		座標
 戻り値:	なし
 説明:		カメラのセット
-----------------------------------------------------------------------------*/
void SetCamera( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Move )
{

	//posAt.x = sinf( D3DXToRadian( Rot.y + 180 ) ) * Move.x + Pos.x;
	//posAt.y = Pos.y + 20.0f;
	//posAt.z = cosf( D3DXToRadian( Rot.y + 180 ) ) * Move.z + Pos.z;

	g_posAt.x = Pos.x;
	g_posAt.y = Pos.y + 20.0f;
	g_posAt.z = Pos.z;


	g_posEye.x = Pos.x;
	g_posEye.y = 30.0f;
	g_posEye.z = g_posAt.z - 100.0f;




}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	D3DXMATRIX *GetView( void )
 引数:		なし
 戻り値:	なし
 説明:		ビュー行列の情報取得
-----------------------------------------------------------------------------*/
D3DXMATRIX GetView( void )
{
	return g_mtxView;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DebugCamera( void )
 引数:		なし
 戻り値:	なし
 説明:		カメラの文字表示
-----------------------------------------------------------------------------*/
#ifdef _DEBUG
void DebugCamera( void )
{
	int Height = 15;
	D3DXCOLOR Color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	RECT rect00 = { 0 , Height , SCREEN_WIDTH , SCREEN_HEIGHT };
	RECT rect01 = { 0 , Height + 15 , SCREEN_WIDTH , SCREEN_HEIGHT };
	RECT rect02 = { 0 , Height + 45 , SCREEN_WIDTH , SCREEN_HEIGHT };

	char aStr00[ 256 ] , aStr01[ 256 ] , aStr02[ 256 ];

	sprintf( &aStr00[ 0 ] , "[ カメラ座標 ] ( %f : %f : %f )" , g_posEye.x , g_posEye.y , g_posEye.z );
	sprintf( &aStr01[ 0 ] , "[ 注視点座標 ] ( %f : %f : %f )" , g_posAt.x , g_posAt.y , g_posAt.z );
	sprintf( &aStr02[ 0 ] , "*** カメラ操作 ***" );

	g_pFontCamera -> DrawText( NULL ,
						&aStr00[ 0 ] ,
						-1 ,
						&rect00 ,
						DT_LEFT ,
						Color );

	g_pFontCamera -> DrawText( NULL ,
						&aStr01[ 0 ] ,
						-1 ,
						&rect01 ,
						DT_LEFT ,
						Color );

	g_pFontCamera -> DrawText( NULL ,
						&aStr02[ 0 ] ,
						-1 ,
						&rect02 ,
						DT_LEFT ,
						Color );


}	//	end of func

#endif	//	_DEBUG