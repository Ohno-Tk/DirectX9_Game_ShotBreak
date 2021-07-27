/*=============================================================================

		チュートリアル[ Tutorialbg.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2017/01/11
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Tutorialbg.h"
#include "fade.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/TITLE/ButtomRogo .png"
#define POLYGON01_TEXTURENAME "data/TEXTURE/TUTORIAL/sousa.png"
#define POLYGON02_TEXTURENAME "data/TEXTURE/TUTORIAL/Bg.jpg"
#define POLYGON03_TEXTURENAME "data/TEXTURE/TUTORIAL/操作説明.png"
#define POLYGON04_TEXTURENAME "data/TEXTURE/TUTORIAL/画面.jpg"
#define POLYGON05_TEXTURENAME "data/TEXTURE/TUTORIAL/ゲーム画面.png"

#define MAX_TEXTURE ( 6 )	//	最大テクスチャ数

#define FADE_RATE ( 1.0f / 60 )	//	どのくらいでフェードさせるのか

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

}TUTORIAL;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice );

//	頂点の変更
void VerTexTutorial( VERTEX_2D* pVtx , int Cnt );


/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferTutorial = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorial[ MAX_TEXTURE ] = { NULL };	//	テクスチャインターフェース

TUTORIAL g_Tutorial[ MAX_TEXTURE ];	//	チュートリアル構造体情報

FADE g_TutorialFade;	//	チュートリアル列挙

bool g_Flag;

/*-----------------------------------------------------------------------------
 関数名:	void InitTutorialBg( void )
 引数:		なし
 戻り値:	なし
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitTutorialBg( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTextureTutorial[ 0 ] ) ) )
	{

		MessageBox( NULL , "[ Tutorialbg.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTextureTutorial[ 1 ] ) ) )
	{

		MessageBox( NULL , "[ Tutorialbg.cpp ]\n POLYGON01_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON02_TEXTURENAME , &g_pTextureTutorial[ 2 ] ) ) )
	{

		MessageBox( NULL , "[ Tutorialbg.cpp ]\n POLYGON02_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON03_TEXTURENAME , &g_pTextureTutorial[ 3 ] ) ) )
	{

		MessageBox( NULL , "[ Tutorialbg.cpp ]\n POLYGON03_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON04_TEXTURENAME , &g_pTextureTutorial[ 4 ] ) ) )
	{

		MessageBox( NULL , "[ Tutorialbg.cpp ]\n POLYGON04_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON05_TEXTURENAME , &g_pTextureTutorial[ 5 ] ) ) )
	{

		MessageBox( NULL , "[ Tutorialbg.cpp ]\n POLYGON05_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if



	//	頂点の作成
	MakeVertexTutorial( pDevice );


	//	構造体初期化

	//	座標
	g_Tutorial[ 0 ].Pos = D3DXVECTOR2( SCREEN_WIDTH - 520.0f , SCREEN_HEIGHT * 0.83f );

	//	大きさ
	g_Tutorial[ 0 ].Size = D3DXVECTOR2( 260.0f , 80.0f );

	//	色
	g_Tutorial[ 0 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	座標
	g_Tutorial[ 1 ].Pos = D3DXVECTOR2( 0.0f , 100.0f );

	//	大きさ
	g_Tutorial[ 1 ].Size = D3DXVECTOR2( SCREEN_WIDTH , 400.0f );

	//	色
	g_Tutorial[ 1 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	座標
	g_Tutorial[ 2 ].Pos = D3DXVECTOR2( 0.0f , 0.0f );
				
	//	大きさ	 
	g_Tutorial[ 2 ].Size = D3DXVECTOR2( SCREEN_WIDTH , SCREEN_HEIGHT );
				
	//	色	   
	g_Tutorial[ 2 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	座標
	g_Tutorial[ 3 ].Pos = D3DXVECTOR2( 300.0f , -10.0f );
				
	//	大きさ	 
	g_Tutorial[ 3 ].Size = D3DXVECTOR2( 200.0f , 150.0f );
				
	//	色	   
	g_Tutorial[ 3 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	座標
	g_Tutorial[ 4 ].Pos = D3DXVECTOR2( 0.0f , 100.0f );

	//	大きさ
	g_Tutorial[ 4 ].Size = D3DXVECTOR2( SCREEN_WIDTH , 400.0f );

	//	色
	g_Tutorial[ 4 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );


	//	座標
	g_Tutorial[ 5 ].Pos = D3DXVECTOR2( 300.0f , -10.0f );
				
	//	大きさ	 
	g_Tutorial[ 5 ].Size = D3DXVECTOR2( 200.0f , 150.0f );
				
	//	色	   
	g_Tutorial[ 5 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );



	//	変数初期化
	g_TutorialFade = FADE_IN;

	g_Flag = false;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitTutorialBg( void )
 引数:		なし
 戻り値:	なし
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitTutorialBg( void )
{

	if(g_pVtxBufferTutorial != NULL)	//頂点バッファのインターフェースポインタの解放
	{

		g_pVtxBufferTutorial -> Release();
		g_pVtxBufferTutorial  = NULL;

	}	//	end of if


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		if( g_pTextureTutorial[ Cnt ] != NULL )	//	テクスチャポリゴン開放
		{

			g_pTextureTutorial[ Cnt ] -> Release();
			g_pTextureTutorial[ Cnt ] = NULL;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateTutorialBg( void )
 引数:		なし
 戻り値:	なし
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateTutorialBg( void )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferTutorial -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		//	頂点の変更
		VerTexTutorial( pVtx , Cnt );

	}	//	end of for


	g_pVtxBufferTutorial -> Unlock(); //これ以降触れてはいけない



	if( g_TutorialFade == FADE_IN )	//	フェードイン時の処理
	{

		g_Tutorial[ 0 ].Color.a -= FADE_RATE;	//	a値を減算して後ろの画面を浮かび上がらせる

		//	フェードインの終了
		if( g_Tutorial[ 0 ].Color.a < 0.0f )
		{

			g_Tutorial[ 0 ].Color.a = 0.0f;
			g_TutorialFade = FADE_OUT;

		}	//	end od if

	}	//	end of if

	else if( g_TutorialFade == FADE_OUT )	//	フェードアウト時の処理
	{

		g_Tutorial[ 0 ].Color.a += FADE_RATE;	//	a値を加算して後ろの画面を消していく

		//	フェードアウトの終了
		if( g_Tutorial[ 0 ].Color.a > 1.0f )
		{

			g_Tutorial[ 0 ].Color.a = 1.0f;
			g_TutorialFade = FADE_IN;	//	フェードインに処理の切り替え

		}	//	end of if

	}	//	end of else if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawTutorialBg( void )
 引数:		なし
 戻り値:	なし
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawTutorialBg( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferTutorial , 0 , sizeof( VERTEX_2D ) );

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureTutorial[ 2 ] );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8 , NUM_POLYGON);

	//	テクスチャのセット
	pDevice -> SetTexture( 0 , g_pTextureTutorial[ 0 ] );

	//	ポリゴンの描画
	pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);


	if( g_Flag == false )
	{

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTextureTutorial[ 1 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTextureTutorial[ 3 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12 , NUM_POLYGON);

	}	//	end of if

	else
	{

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTextureTutorial[ 4 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16 , NUM_POLYGON);

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTextureTutorial[ 5 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 20 , NUM_POLYGON);

	}


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexTutorial( LPDIRECT3DDEVICE9 pDevice )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferTutorial , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	//バッファをロックし仮想アドレスを取得する
	g_pVtxBufferTutorial -> Lock( 0 , 0 ,	( void** )&pVtx , 0 );


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


	g_pVtxBufferTutorial -> Unlock(); //これ以降触れてはいけない


	return S_OK;


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexTutorial( VERTEX_2D* pVtx , int Cnt )
 引数:		VERTEX_2D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				カウンタ
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexTutorial( VERTEX_2D* pVtx , int Cnt )
{

	//	pVtxをCnt分ずらす
	pVtx += Cnt * NUM_VERTEX;


	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Tutorial[ Cnt ].Pos.x                            , g_Tutorial[ Cnt ].Pos.y                                    , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Tutorial[ Cnt ].Pos.x + g_Tutorial[ Cnt ].Size.x , g_Tutorial[ Cnt ].Pos.y                                    , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Tutorial[ Cnt ].Pos.x                            , g_Tutorial[ Cnt ].Pos.y + g_Tutorial[ Cnt ].Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Tutorial[ Cnt ].Pos.x + g_Tutorial[ Cnt ].Size.x , g_Tutorial[ Cnt ].Pos.y + g_Tutorial[ Cnt ].Size.y , 0.0f );

	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Tutorial[ Cnt ].Color.r , g_Tutorial[ Cnt ].Color.g , g_Tutorial[ Cnt ].Color.b , g_Tutorial[ Cnt ].Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Tutorial[ Cnt ].Color.r , g_Tutorial[ Cnt ].Color.g , g_Tutorial[ Cnt ].Color.b , g_Tutorial[ Cnt ].Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Tutorial[ Cnt ].Color.r , g_Tutorial[ Cnt ].Color.g , g_Tutorial[ Cnt ].Color.b , g_Tutorial[ Cnt ].Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Tutorial[ Cnt ].Color.r , g_Tutorial[ Cnt ].Color.g , g_Tutorial[ Cnt ].Color.b , g_Tutorial[ Cnt ].Color.a );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	bool *GetFlag( void )
 引数:		
 戻り値:	
 説明:		
-----------------------------------------------------------------------------*/
bool *GetFlag( void )
{

	return &g_Flag;

}	//	end of func
