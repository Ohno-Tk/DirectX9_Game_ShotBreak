/*=============================================================================

		カウントダウン[ CountDown.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "CountDown.h"
#include "sound.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/CountDown_Start.png"
#define POLYGON01_TEXTURENAME "data/TEXTURE/GAME/CountDown_1.png"
#define POLYGON02_TEXTURENAME "data/TEXTURE/GAME/CountDown_2.png"
#define POLYGON03_TEXTURENAME "data/TEXTURE/GAME/CountDown_3.png"

#define MAX_TEXTURE ( 4 )	//	テクスチャの最大数


#define COUNTDOWN_X ( SCREEN_WIDTH * 0.35f )		//	スコアのX
#define COUNTDOWN_Y ( SCREEN_HEIGHT * 0.4f )		//	スコアのY
#define COUNTDOWN_WIDTH  ( 250.0f )	//	スコアのWidth
#define COUNTDOWN_HEIGHT ( 150.0f )	//	スコアのHeight

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/
typedef enum
{
	COUNTDOWN_GO = 0,
	COUNTDOWN_1,
	COUNTDOWN_2,
	COUNTDOWN_3,
	COUNTDOWN_MAX,

}COUNTDOWN;

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexCountDown( LPDIRECT3DDEVICE9 pDevice );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferCountDown = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCountDown[ MAX_TEXTURE ] = { NULL };//	テクスチャインターフェース


COUNTDOWN g_CountDown = COUNTDOWN_3;	// カウントダウンの列挙

int g_CountDown_TexNo;	// テクスチャNO
int g_CountDown_Time;	// タイマーカウンタ
int g_CountDown_Frame;	// フレームカウンタ

bool g_Start;			// スタートしたかどうか


/*-----------------------------------------------------------------------------
 関数名:	void InitCountDown( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitCountDown( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureCountDown[ 0 ] ) ) )
	{
		MessageBox( NULL , "[ CountDown.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTextureCountDown[ 1 ] ) ) )
	{
		MessageBox( NULL , "[ CountDown.cpp ]\n POLYGON01_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON02_TEXTURENAME , &g_pTextureCountDown[ 2 ] ) ) )
	{
		MessageBox( NULL , "[ CountDown.cpp ]\n POLYGON02_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON03_TEXTURENAME , &g_pTextureCountDown[ 3 ] ) ) )
	{
		MessageBox( NULL , "[ CountDown.cpp ]\n POLYGON03_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexCountDown( pDevice );


	// 変数初期化
	g_CountDown_TexNo = (int)g_CountDown;

	g_CountDown_Time = (int)g_CountDown;

	g_CountDown_Frame = 0;

	g_Start = false;


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitCountDown( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitCountDown( void )
{

	if(g_pVtxBufferCountDown != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferCountDown -> Release();
		g_pVtxBufferCountDown  = NULL;

	}	//	end of if


	for( int CntCountDown = 0 ; CntCountDown < MAX_TEXTURE ; CntCountDown++ )
	{

		if( g_pTextureCountDown[ CntCountDown ] != NULL )	//	テクスチャポリゴン開放
		{
			g_pTextureCountDown[ CntCountDown ] -> Release();
			g_pTextureCountDown[ CntCountDown ] = NULL;

		}	//	end of if

	}	//	end of for


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateCountDown( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateCountDown( void )
{

	int num = 0;


	if( g_CountDown_Time != 0 )
	{

		g_CountDown_Frame++;

		if( g_CountDown_Frame == 60 )
		{

			g_CountDown_Frame = 0;
			g_CountDown_TexNo--;
			g_CountDown_Time--;

			if( g_CountDown_TexNo == COUNTDOWN_GO )
			{

				PlaySound( SOUND_LABEL_SE_COUNTDOWN02 );	//	音楽再生

			}
			else
			{

				PlaySound( SOUND_LABEL_SE_COUNTDOWN01 );	//	音楽再生

			}

		}	//	end of if

	}	//	end of if

	//ゲームスタート
	else
	{

		// 60フレームだけ表示
		if( g_CountDown_Frame != 60 )
		{

			g_CountDown_Frame++;

		}	//	end of if

		else
		{

			g_Start = true;

		}	//	end of else

	}	//	end of else


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawCountDown( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawCountDown( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferCountDown , 0 , sizeof( VERTEX_2D ) );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureCountDown[ g_CountDown_TexNo ] );


	if( g_Start == false )
	{

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);

	}	//	end of if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexCountDown( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexCountDown( LPDIRECT3DDEVICE9 pDevice )
{

	// 構造体のポインタ宣言
	VERTEX_2D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferCountDown , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferCountDown -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( COUNTDOWN_X                   , COUNTDOWN_Y                    , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( COUNTDOWN_X + COUNTDOWN_WIDTH , COUNTDOWN_Y                    , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( COUNTDOWN_X                   , COUNTDOWN_Y + COUNTDOWN_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( COUNTDOWN_X + COUNTDOWN_WIDTH , COUNTDOWN_Y + COUNTDOWN_HEIGHT , 0.0f );

	//	座標変換済み頂点フラグの設定
	pVtx[ 0 ].rhw = 1.0f;
	pVtx[ 1 ].rhw = 1.0f;
	pVtx[ 2 ].rhw = 1.0f;
	pVtx[ 3 ].rhw = 1.0f;

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 1 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 2 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );
	pVtx[ 3 ].color = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );


	g_pVtxBufferCountDown -> Unlock(); //これ以降触れてはいけない

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	bool *GetStart( void )
 引数:		
 戻り値:	return &g_Start;
 説明:		スタートしたかの情報取得
-----------------------------------------------------------------------------*/
bool *GetStart( void )
{

	return &g_Start;

}	//	end of func
