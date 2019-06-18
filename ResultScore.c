/*=============================================================================

		リザルトスコア[ ResultScore.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2017/01/06
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "ResultScore.h"
#include "Score.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/number000.png"
#define POLYGON01_TEXTURENAME "data/TEXTURE/RESULT/Score.png"

#define SCORE_DIGIT ( 4 )	//	何桁の数字の表示

#define MAX_TEXTURE ( 2 )	//	最大テクスチャ数


#define POLYGON00_X ( SCREEN_WIDTH * 0.63f )		//	スコアのX
#define POLYGON00_Y ( SCREEN_HEIGHT * 0.45f )		//	スコアのY
#define POLYGON00_WIDTH  ( 100.0f )	//	スコアのWidth
#define POLYGON00_HEIGHT ( 100.0f )	//	スコアのHeight

#define POLYGON01_X ( SCREEN_WIDTH * 0.38f )		//	スコア文字のX
#define POLYGON01_Y ( SCREEN_HEIGHT * 0.25f )		//	スコア文字のY
#define POLYGON01_WIDTH  ( 200.0f )	//	スコア文字のWidth
#define POLYGON01_HEIGHT ( 100.0f )	//	スコア文字のHeight

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
HRESULT MakeVertexResultScore( LPDIRECT3DDEVICE9 pDevice );


/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferResultScore = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResultScore[ MAX_TEXTURE ] = { NULL };	//	テクスチャインターフェース

/*-----------------------------------------------------------------------------
 関数名:	void InitResultScore( void )
 引数:		なし
 戻り値:	なし
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitResultScore( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureResultScore[ 0 ] ) ) )
	{

		MessageBox( NULL , "[ ResultScore.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTextureResultScore[ 1 ] ) ) )
	{

		MessageBox( NULL , "[ ResultScore.cpp ]\n POLYGON01_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexResultScore( pDevice );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitResultScore( void )
 引数:		なし
 戻り値:	なし
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitResultScore( void )
{

	if(g_pVtxBufferResultScore != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferResultScore -> Release();
		g_pVtxBufferResultScore  = NULL;

	}	//	end of if



	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		if( g_pTextureResultScore[ Cnt ] != NULL )	//	テクスチャポリゴン開放
		{
			g_pTextureResultScore[ Cnt ] -> Release();
			g_pTextureResultScore[ Cnt ] = NULL;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateResultScore( void )
 引数:		なし
 戻り値:	なし
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateResultScore( void )
{

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawResultScore( void )
 引数:		なし
 戻り値:	なし
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawResultScore( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferResultScore , 0 , sizeof( VERTEX_2D ) );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureResultScore[ 0 ] );


	for( int Cnt = 0 ; Cnt < SCORE_DIGIT ; Cnt++ )
	{

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , Cnt * NUM_VERTEX , NUM_POLYGON);

	}	//	end of for


	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureResultScore[ 1 ] );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , SCORE_DIGIT * NUM_VERTEX , NUM_POLYGON);


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexResultScore( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexResultScore( LPDIRECT3DDEVICE9 pDevice )
{

	int num;	//	数字を表示する変数
	int *value = GetScore();


	// 仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * SCORE_DIGIT * MAX_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferResultScore , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferResultScore -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );

	for( int Cnt = 0 ; Cnt < SCORE_DIGIT ; Cnt++ )
	{

		num = *value % 10;
		*value /= 10;

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


	g_pVtxBufferResultScore -> Unlock(); //これ以降触れてはいけない

	return S_OK;

}	//	end of func