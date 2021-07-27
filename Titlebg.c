/*=============================================================================

		タイトル[ Titlebg.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2017/01/06
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Titlebg.h"
#include "fade.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/TITLE/ButtomRogo .png"
#define POLYGON01_TEXTURENAME "data/TEXTURE/TITLE/Title.png"

#define MAX_TEXTURE ( 2 )	//	最大テクスチャ数

#define FADE_RATE ( 1.0f / 80 )	//	どのくらいでフェードさせるのか

#define MIN_ALPHA ( 0.5f )	//	α値の最小値

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR2 Pos;	//	座標
	D3DXVECTOR2 Size;	//	大きさ
	D3DXCOLOR Color;	//	色

}TITLE;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexTitle( LPDIRECT3DDEVICE9 pDevice );

//	頂点の変更
void VerTexTitle( VERTEX_2D* pVtx , int Cnt );


/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTitle = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTitle[ MAX_TEXTURE ] = { NULL };	//	テクスチャインターフェース

TITLE g_Title[ MAX_TEXTURE ];	//	タイトル構造体情報

FADE g_TitleFade;	//	タイトル列挙

/*-----------------------------------------------------------------------------
 関数名:	void InitTitleBg( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitTitleBg( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureTitle[ 0 ] ) ) )
	{

		MessageBox( NULL , "[ Titlebg.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTextureTitle[ 1 ] ) ) )
	{

		MessageBox( NULL , "[ Titlebg.cpp ]\n POLYGON01_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if



	//	頂点の作成
	MakeVertexTitle( pDevice );


	//	構造体初期化

	//	座標
	g_Title[ 0 ].Pos = D3DXVECTOR2( SCREEN_WIDTH - 520.0f , SCREEN_HEIGHT * 0.8f );

	//	大きさ
	g_Title[ 0 ].Size = D3DXVECTOR2( 260.0f , 80.0f );

	//	色
	g_Title[ 0 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	タイトルロゴ
	//	座標
	g_Title[ 1 ].Pos = D3DXVECTOR2( SCREEN_WIDTH * 0.2f , -10.0f );

	//	大きさ
	g_Title[ 1 ].Size = D3DXVECTOR2( 500.0f , 260.0f );

	//	色
	g_Title[ 1 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	変数初期化
	g_TitleFade = FADE_IN;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitTitleBg( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitTitleBg( void )
{

	if(g_pVtxBufferTitle != NULL)	//頂点バッファのインターフェースポインタの解放
	{

		g_pVtxBufferTitle -> Release();
		g_pVtxBufferTitle  = NULL;

	}	//	end of if


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		if( g_pTextureTitle[ Cnt ] != NULL )	//	テクスチャポリゴン開放
		{

			g_pTextureTitle[ Cnt ] -> Release();
			g_pTextureTitle[ Cnt ] = NULL;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateTitleBg( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateTitleBg( void )
{

	// 仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferTitle -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		//	頂点の変更
		VerTexTitle( pVtx , Cnt );

	}	//	end of for


	g_pVtxBufferTitle -> Unlock(); //これ以降触れてはいけない


	if( g_TitleFade == FADE_IN )	//	フェードイン時の処理
	{

		g_Title[ 0 ].Color.a -= FADE_RATE;	//	a値を減算して後ろの画面を浮かび上がらせる

		//	フェードインの終了
		if( g_Title[ 0 ].Color.a < MIN_ALPHA )
		{

			g_Title[ 0 ].Color.a = MIN_ALPHA;
			g_TitleFade = FADE_OUT;

		}	//	end od if

	}	//	end of if

	else if( g_TitleFade == FADE_OUT )	//	フェードアウト時の処理
	{

		g_Title[ 0 ].Color.a += FADE_RATE;	//	a値を加算して後ろの画面を消していく

		//	フェードアウトの終了
		if( g_Title[ 0 ].Color.a > 1.0f )
		{

			g_Title[ 0 ].Color.a = 1.0f;
			g_TitleFade = FADE_IN;	//	フェードインに処理の切り替え

		}	//	end of if

	}	//	end of else if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawTitleBg( void )
 引数:		なし
 戻り値:	なし
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawTitleBg( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTitle , 0 , sizeof( VERTEX_2D ) );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureTitle[ 0 ] );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureTitle[ 1 ] );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexTitle( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTitle( LPDIRECT3DDEVICE9 pDevice )
{

	// 仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTitle , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferTitle -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );


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

		pVtx += 4;	//	pVtxをずらす

	}	//	end of for


	g_pVtxBufferTitle -> Unlock(); //これ以降触れてはいけない


	return S_OK;


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexTitle( VERTEX_2D* pVtx , int Cnt )
 引数:		VERTEX_2D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				カウンタ
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexTitle( VERTEX_2D* pVtx , int Cnt )
{

	//	pVtxをCnt分ずらす
	pVtx += Cnt * NUM_VERTEX;


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Title[ Cnt ].Pos.x                         , g_Title[ Cnt ].Pos.y                         , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Title[ Cnt ].Pos.x + g_Title[ Cnt ].Size.x , g_Title[ Cnt ].Pos.y                         , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Title[ Cnt ].Pos.x                         , g_Title[ Cnt ].Pos.y + g_Title[ Cnt ].Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Title[ Cnt ].Pos.x + g_Title[ Cnt ].Size.x , g_Title[ Cnt ].Pos.y + g_Title[ Cnt ].Size.y , 0.0f );

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Title[ Cnt ].Color.r , g_Title[ Cnt ].Color.g , g_Title[ Cnt ].Color.b , g_Title[ Cnt ].Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Title[ Cnt ].Color.r , g_Title[ Cnt ].Color.g , g_Title[ Cnt ].Color.b , g_Title[ Cnt ].Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Title[ Cnt ].Color.r , g_Title[ Cnt ].Color.g , g_Title[ Cnt ].Color.b , g_Title[ Cnt ].Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Title[ Cnt ].Color.r , g_Title[ Cnt ].Color.g , g_Title[ Cnt ].Color.b , g_Title[ Cnt ].Color.a );

}	//	end of func
