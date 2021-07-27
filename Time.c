/*=============================================================================

		タイム[ Time.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Time.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/number000.png"
#define POLYGON01_TEXTURENAME "data/TEXTURE/GAME/Time.png"

#define MAX_TEXTURE ( 2 )	//	最大テクスチャ数

#define SCORE_DIGIT ( 2 )	//	何桁の数字の表示

//#define FRAME ( 0.06f )	//	フレーム数
#define FRAME ( 60 )	//	フレーム数

#define TIME_MIN ( 0 )	//	タイムの最小値
#define TIME_COLORCHANGE ( 10 )	//	色を変える時間

#define SECOND ( 90 )	//	秒数


#define POLYGON01_X ( SCREEN_WIDTH * 0.36f )	//	タイム文字のX
#define POLYGON01_Y ( -10.0f )					//	タイム文字のY
#define POLYGON01_WIDTH  ( 150.0f )				//	タイム文字のWidth
#define POLYGON01_HEIGHT ( 75.0f )				//	タイム文字のHeight

#define POLYGON00_X ( SCREEN_WIDTH * 0.5f )		//	タイムのX
#define POLYGON00_Y ( 50.0f )					//	タイムのY
#define POLYGON00_WIDTH  ( 70.0f )				//	タイムのWidth
#define POLYGON00_HEIGHT ( 70.0f )				//	タイムのHeight

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice );

//	テクスチャの変更
void VerTexTime( VERTEX_2D* pVtx , int Cnt , int num );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTime = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTime[ MAX_TEXTURE ] = { NULL };	//	テクスチャインターフェース

D3DXCOLOR g_TimeColor;	//	色

int g_Time;	//	タイム

bool g_TimeFlag;	//	タイムフラグ

/*-----------------------------------------------------------------------------
 関数名:	void InitTime( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitTime( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureTime[ 0 ] ) ) )
	{
		MessageBox( NULL , "[ Time.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTextureTime[ 1 ] ) ) )
	{
		MessageBox( NULL , "[ Time.cpp ]\n POLYGON01_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexTime( pDevice );


	//	変数初期化

	//	色
	g_TimeColor = D3DXCOLOR( 255 , 255 , 255 , 255 );

	//	タイム
	g_Time = SECOND * FRAME;

	//	タイムフラグ
	g_TimeFlag = false;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitTime( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitTime( void )
{

	if(g_pVtxBufferTime != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferTime -> Release();
		g_pVtxBufferTime  = NULL;

	}	//	end of if


	for( int CntTime = 0 ; CntTime < MAX_TEXTURE ; CntTime++ )
	{

		if( g_pTextureTime[ CntTime ] != NULL )	//	テクスチャポリゴン開放
		{
			g_pTextureTime[ CntTime ] -> Release();
			g_pTextureTime[ CntTime ] = NULL;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateTime( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateTime( void )
{

	int num;	//	数字を表示する変数
	int value = g_Time / FRAME;


	//	仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferTime -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );

	for( int Cnt = 0 ; Cnt < SCORE_DIGIT ; Cnt++ )
	{

		num = value % 10;
		value /= 10;

		//	テクスチャの変更
		VerTexTime( pVtx , Cnt , num );

	}	//	end of for


	g_pVtxBufferTime -> Unlock(); //これ以降触れてはいけない


	//	タイムが0以下になったら
	if( g_Time <= 0 )
	{

		g_Time = TIME_MIN;

		g_TimeFlag = true;

	}	//	end of if

	else
	{

		g_Time--;	//	タイムを減らす

		//	タイムが一定の値を下回ったら
		if( g_Time / FRAME < TIME_COLORCHANGE )
		{

			//	色変更
			g_TimeColor.r = 255;
			g_TimeColor.g = 0;
			g_TimeColor.b = 0;

		}	//	end of if
	
	}	//	end of else


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawTime( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawTime( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTime , 0 , sizeof( VERTEX_2D ) );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureTime[ 0 ] );


	for( int CntTime = 0 ; CntTime < SCORE_DIGIT ; CntTime++ )
	{

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , CntTime * NUM_VERTEX , NUM_POLYGON);

	}	//	end of for


	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureTime[ 1 ] );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , SCORE_DIGIT * NUM_VERTEX , NUM_POLYGON);


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTime( LPDIRECT3DDEVICE9 pDevice )
{

	int num;	//	数字を表示する変数
	int value = g_Time / FRAME;	//	タイムの値


	//	仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * SCORE_DIGIT * MAX_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTime , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferTime -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	for( int Cnt = 0 ; Cnt < SCORE_DIGIT ; Cnt++ )
	{

		num = value % 10;
		value /= 10;


		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( POLYGON00_X                   - Cnt * POLYGON00_WIDTH , POLYGON00_Y                    , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( POLYGON00_X + POLYGON00_WIDTH - Cnt * POLYGON00_WIDTH , POLYGON00_Y                    , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( POLYGON00_X                   - Cnt * POLYGON00_WIDTH , POLYGON00_Y + POLYGON00_HEIGHT , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( POLYGON00_X + POLYGON00_WIDTH - Cnt * POLYGON00_WIDTH , POLYGON00_Y + POLYGON00_HEIGHT , 0.0f );

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
		pVtx[ 0 ].tex = D3DXVECTOR2( 0.1f * num        , 0 );
		pVtx[ 1 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 0 );
		pVtx[ 2 ].tex = D3DXVECTOR2( 0.1f * num        , 1 );
		pVtx[ 3 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 1 );

		pVtx += 4;	//	ポインタをずらす

	}	//	end of for


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( POLYGON01_X                   , POLYGON01_Y                    , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( POLYGON01_X + POLYGON01_WIDTH , POLYGON01_Y                    , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( POLYGON01_X                   , POLYGON01_Y + POLYGON01_HEIGHT , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( POLYGON01_X + POLYGON01_WIDTH , POLYGON01_Y + POLYGON01_HEIGHT , 0.0f );

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


	g_pVtxBufferTime -> Unlock(); //これ以降触れてはいけない

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexTime( VERTEX_2D* pVtx , int Cnt , int num )
 引数:		VERTEX_2D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				タイムのカウンタ
			int num				テクスチャをずらす変数
 戻り値:	
 説明:		テクスチャの変更
-----------------------------------------------------------------------------*/
void VerTexTime( VERTEX_2D* pVtx , int Cnt , int num )
{

	pVtx += Cnt * NUM_VERTEX;


	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_TimeColor.r , g_TimeColor.g , g_TimeColor.b , g_TimeColor.a );

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0.1f * num        , 0 );
	pVtx[ 1 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 0 );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0.1f * num        , 1 );
	pVtx[ 3 ].tex = D3DXVECTOR2( 0.1f * num + 0.1f , 1 );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	int *GetTime( void )
 引数:		
 戻り値:	return &g_Time;
 説明:		タイム情報の取得
-----------------------------------------------------------------------------*/
bool *GetTime( void )
{

	return &g_TimeFlag;

}	//	end of func
