/*=============================================================================

		ポーズ[ Pause.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2017/01/16
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "input.h"
#include "fade.h"
#include "Pause.h"
#include "sound.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define POLYGON00_TEXTURENAME "data/TEXTURE/GAME/pauseBg.jpg"	//	ポーズ背景
#define POLYGON01_TEXTURENAME "data/TEXTURE/GAME/continue.png"	//	コンティニュー
#define POLYGON02_TEXTURENAME "data/TEXTURE/GAME/quit.png"	//	終了
#define POLYGON03_TEXTURENAME "data/TEXTURE/GAME/pauseBg1.jpg"	//	ポーズ背景1
#define POLYGON04_TEXTURENAME "data/TEXTURE/GAME/pausescroll.png"	//	ポーズスクロール

#define MAX_TEXTURE ( 5 )	//	最大テクスチャ数

#define MIN_ALPHA ( 0.5f )	//	α値の最小値

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/
typedef enum
{
	TYPE_NONE = 0,
	TYPE_CONTINUE,	//	コンティニュー
	TYPE_QUIT,		//	終了
	TYPE_MAX

}TYPE_PAUSE;

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/
typedef struct
{
	D3DXVECTOR2 Pos;	//	座標
	D3DXVECTOR2 Size;	//	大きさ
	D3DXCOLOR Color;	//	色

}PAUSE;

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/

//	頂点の作成
HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice );

//	頂点の変更
void VerTexPause( VERTEX_2D* pVtx , int Cnt );

//	UV値の変更
void VerTexUVPause( VERTEX_2D* pVtx , int Cnt );

//	キーボードでの移動
void PauseCursorKeyBoard( void );

//	遷移
void PauseTransition( void );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufferPause = NULL;	//	頂点バッファインタフェースへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePause[ MAX_TEXTURE ] = { NULL };//	テクスチャインターフェース

PAUSE g_Pause[ MAX_TEXTURE ];	//	ポーズ構造体

TYPE_PAUSE g_TypePause;	//	ポーズ列挙

D3DXVECTOR2 g_UV_Scroll;	//	UV座標

bool g_PauseFlag;	//	ポーズフラグ

/*-----------------------------------------------------------------------------
 関数名:	void InitPause( void )
 引数:		
 戻り値:	
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitPause( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//	テクスチャのエラーチェック
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON00_TEXTURENAME , &g_pTexturePause[ 0 ] ) ) )
	{

		MessageBox( NULL , "[ Pause.cpp ]\n POLYGON00_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if
	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON01_TEXTURENAME , &g_pTexturePause[ 1 ] ) ) )
	{

		MessageBox( NULL , "[ Pause.cpp ]\n POLYGON01_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON02_TEXTURENAME , &g_pTexturePause[ 2 ] ) ) )
	{

		MessageBox( NULL , "[ Pause.cpp ]\n POLYGON02_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON03_TEXTURENAME , &g_pTexturePause[ 3 ] ) ) )
	{

		MessageBox( NULL , "[ Pause.cpp ]\n POLYGON03_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if

	if( FAILED( D3DXCreateTextureFromFile(  pDevice , POLYGON04_TEXTURENAME , &g_pTexturePause[ 4 ] ) ) )
	{

		MessageBox( NULL , "[ Pause.cpp ]\n POLYGON04_TEXTURENAME\nの読み込みができませんでした" , "警告" , MB_OK | MB_ICONHAND );

	}	//	end of if


	//	頂点の作成
	MakeVertexPause( pDevice );


	//	構造体初期化

	for( int CntPause = 0 ; CntPause < MAX_TEXTURE ; CntPause++ )
	{

		//	色
		g_Pause[ CntPause ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

	}	//	end of for


	//	ポーズ背景
	//	座標
	g_Pause[ 0 ].Pos = D3DXVECTOR2( 0.0f , 0.0f );

	//	大きさ
	g_Pause[ 0 ].Size = D3DXVECTOR2( SCREEN_WIDTH , SCREEN_HEIGHT );

	//	色
	g_Pause[ 0 ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f );


	//	コンティニュー
	//	座標
	g_Pause[ TYPE_CONTINUE ].Pos = D3DXVECTOR2( 330.0f , 80.0f );

	//	大きさ
	g_Pause[ TYPE_CONTINUE ].Size = D3DXVECTOR2( 200.0f , 100.0f );


	//	終了
	//	座標
	g_Pause[ TYPE_QUIT ].Pos = D3DXVECTOR2( g_Pause[ TYPE_CONTINUE ].Pos.x , g_Pause[ TYPE_CONTINUE ].Pos.y + 325.0f );

	//	大きさ
	g_Pause[ TYPE_QUIT ].Size = g_Pause[ TYPE_CONTINUE ].Size;

	//	色
	g_Pause[ TYPE_QUIT ].Color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 0.5f );


	//	ポーズ背景1
	//	座標
	g_Pause[ 3 ].Pos = D3DXVECTOR2( SCREEN_WIDTH * 0.3f , SCREEN_HEIGHT * 0.05f );

	//	大きさ
	g_Pause[ 3 ].Size = D3DXVECTOR2( 350.0f , 500.0f );


	//	ポーズスクロール
	//	座標
	g_Pause[ 4 ].Pos = D3DXVECTOR2( SCREEN_WIDTH * 0.31f , SCREEN_HEIGHT * 0.08f );

	//	大きさ
	g_Pause[ 4 ].Size = D3DXVECTOR2( 330.0f , 475.0f );


	//	列挙初期化
	g_TypePause = TYPE_CONTINUE;


	//	変数初期化

	//	UV座標
	g_UV_Scroll = D3DXVECTOR2( 0.0f , 0.0f );

	//	ポーズフラグ
	g_PauseFlag = false;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitPause( void )
 引数:		
 戻り値:	
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitPause( void )
{

	if(g_pVtxBufferPause != NULL)	//頂点バッファのインターフェースポインタの解放
	{
		g_pVtxBufferPause -> Release();
		g_pVtxBufferPause  = NULL;

	}	//	end of if


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		if( g_pTexturePause[ Cnt ] != NULL )	//	テクスチャポリゴン開放
		{
			g_pTexturePause[ Cnt ] -> Release();
			g_pTexturePause[ Cnt ] = NULL;

		}	//	end of if

	}	//	end of for

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdatePause( void )
 引数:		
 戻り値:	
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdatePause( void )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferPause -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		//	ポーズ中だったら
		if( g_PauseFlag == true )
		{

			//	頂点の変更
			VerTexPause( pVtx , Cnt );

			//	UV値の変更
			VerTexUVPause( pVtx , MAX_TEXTURE - 1 );

		}	//	end of if

	}	//	end of for


	g_pVtxBufferPause -> Unlock(); //これ以降触れてはいけない


	if( GetKeyboardTrigger( DIK_P ) )
	{

		//	ポーズフラグをONにする
		g_PauseFlag = true;

		//	音楽再生
		PlaySound( SOUND_LABEL_SE_MENU );

	}	//	end of if


	//	ポーズ中だったら
	if( g_PauseFlag == true )
	{

		//	キーボードでの移動
		PauseCursorKeyBoard();


		//	遷移
		PauseTransition();


		//	テクスチャアニメーション
		g_UV_Scroll.x += 0.004f;
		g_UV_Scroll.y += 0.004f;

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawPause( void )
 引数:		
 戻り値:	
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawPause( void )
{

	//	デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//	頂点フォーマットの設定
	pDevice -> SetFVF( FVF_VERTEX_2D );

	//	ストリームを設定する
	pDevice -> SetStreamSource( 0 , g_pVtxBufferPause , 0 , sizeof( VERTEX_2D ) );


	//	ポーズ中だったら
	if( g_PauseFlag == true )
	{

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTexturePause[ 0 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 0 , NUM_POLYGON);

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTexturePause[ 3 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 12 , NUM_POLYGON);

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTexturePause[ 4 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 16 , NUM_POLYGON);

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTexturePause[ 1 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 4 , NUM_POLYGON);

		//	テクスチャのセット
		pDevice -> SetTexture( 0 , g_pTexturePause[ 2 ] );

		//	ポリゴンの描画
		pDevice -> DrawPrimitive( D3DPT_TRIANGLESTRIP , 8 , NUM_POLYGON);

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void PauseCursorKeyBoard( void )
 引数:		
 戻り値:	
 説明:		キーボードでの移動
-----------------------------------------------------------------------------*/
void PauseCursorKeyBoard( void )
{

	if( GetKeyboardTrigger( DIK_W ) )
	{

		if( g_TypePause == TYPE_QUIT )
		{

			//	タイプの切り替え
			g_TypePause = TYPE_CONTINUE;


			//	α値の変更
			g_Pause[ TYPE_CONTINUE ].Color.a = 1.0f;
			g_Pause[ TYPE_QUIT ].Color.a = MIN_ALPHA;

		}	//	end of if

	}	//	end of if

	else if( GetKeyboardTrigger( DIK_S ) )
	{

		if( g_TypePause == TYPE_CONTINUE )
		{

			//	タイプの切り替え
			g_TypePause = TYPE_QUIT;


			//	α値の変更
			g_Pause[ TYPE_CONTINUE ].Color.a = MIN_ALPHA;
			g_Pause[ TYPE_QUIT ].Color.a = 1.0f;

		}	//	end of if

	}	//	end of else if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void PauseTransition( void )
 引数:		
 戻り値:	
 説明:		遷移
-----------------------------------------------------------------------------*/
void PauseTransition( void )
{

	if( GetKeyboardTrigger( DIK_SPACE ) )
	{

		if( g_TypePause == TYPE_CONTINUE )
		{

			g_PauseFlag = false;

		}	//	end of if

		else if( g_TypePause == TYPE_QUIT )
		{

			//	フェード状態設定
			SetFade( FADE_OUT , MODE_TITLE );

		}	//	end of else if

	}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice )
 引数:		LPDIRECT3DDEVICE9 pDevice	デバイス
 戻り値:	良い場合	return S_OK;
			ダメな場合	return E_FAIL;
 説明:		頂点の作成
-----------------------------------------------------------------------------*/
HRESULT MakeVertexPause( LPDIRECT3DDEVICE9 pDevice )
{

	//	仮想アドレスを取得するポインタ変数
	VERTEX_2D* pVtx;


	// FAILEDマクロでエラーチェック
	if ( FAILED ( pDevice -> CreateVertexBuffer ( sizeof ( VERTEX_2D ) * NUM_VERTEX * MAX_TEXTURE , D3DUSAGE_WRITEONLY , FVF_VERTEX_2D , D3DPOOL_MANAGED , &g_pVtxBufferPause , NULL ) ) )
	{

		return E_FAIL;

	}	//	end of if


	// バッファをロックし、仮想アドレスを取得
	g_pVtxBufferPause -> Lock ( 0 , 0 , ( void** )&pVtx , 0 );


	for( int Cnt = 0 ; Cnt < MAX_TEXTURE ; Cnt++ )
	{

		//	頂点座標の設定
		pVtx[ 0 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x                         , g_Pause[ Cnt ].Pos.y , 0.0f );
		pVtx[ 1 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x + g_Pause[ Cnt ].Size.x , g_Pause[ Cnt ].Pos.y , 0.0f );
		pVtx[ 2 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x                         , g_Pause[ Cnt ].Pos.y + g_Pause[ Cnt ].Size.y , 0.0f );
		pVtx[ 3 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x + g_Pause[ Cnt ].Size.x , g_Pause[ Cnt ].Pos.y + g_Pause[ Cnt ].Size.y , 0.0f );

		//	座標変換済み頂点フラグの設定
		pVtx[ 0 ].rhw = 1.0f;
		pVtx[ 1 ].rhw = 1.0f;
		pVtx[ 2 ].rhw = 1.0f;
		pVtx[ 3 ].rhw = 1.0f;

		//	頂点色の設定
		pVtx[ 0 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
		pVtx[ 1 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
		pVtx[ 2 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );
		pVtx[ 3 ].color = D3DCOLOR_RGBA( 255 , 255 , 255 , 100 );

		//	テクスチャ座標の設定
		pVtx[ 0 ].tex = D3DXVECTOR2( 0 , 0 );
		pVtx[ 1 ].tex = D3DXVECTOR2( 1 , 0 );
		pVtx[ 2 ].tex = D3DXVECTOR2( 0 , 1 );
		pVtx[ 3 ].tex = D3DXVECTOR2( 1 , 1 );

		pVtx += 4;

	}	//	end of for


	g_pVtxBufferPause -> Unlock(); //これ以降触れてはいけない

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexPause( VERTEX_2D* pVtx , int Cnt )
 引数:		VERTEX_2D* pVtx		仮想アドレスを取得するポインタ変数
			int Cnt				ポーズ構造体のカウンタ
 戻り値:	
 説明:		頂点の変更
-----------------------------------------------------------------------------*/
void VerTexPause( VERTEX_2D* pVtx , int Cnt )
{

	pVtx += Cnt * NUM_VERTEX;

	//	頂点座標の設定
	pVtx[ 0 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x                         , g_Pause[ Cnt ].Pos.y , 0.0f );
	pVtx[ 1 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x + g_Pause[ Cnt ].Size.x , g_Pause[ Cnt ].Pos.y , 0.0f );
	pVtx[ 2 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x                         , g_Pause[ Cnt ].Pos.y + g_Pause[ Cnt ].Size.y , 0.0f );
	pVtx[ 3 ].pos = D3DXVECTOR3( g_Pause[ Cnt ].Pos.x + g_Pause[ Cnt ].Size.x , g_Pause[ Cnt ].Pos.y + g_Pause[ Cnt ].Size.y , 0.0f );


	//	頂点色の設定
	pVtx[ 0 ].color = D3DXCOLOR( g_Pause[ Cnt ].Color.r , g_Pause[ Cnt ].Color.g , g_Pause[ Cnt ].Color.b , g_Pause[ Cnt ].Color.a );
	pVtx[ 1 ].color = D3DXCOLOR( g_Pause[ Cnt ].Color.r , g_Pause[ Cnt ].Color.g , g_Pause[ Cnt ].Color.b , g_Pause[ Cnt ].Color.a );
	pVtx[ 2 ].color = D3DXCOLOR( g_Pause[ Cnt ].Color.r , g_Pause[ Cnt ].Color.g , g_Pause[ Cnt ].Color.b , g_Pause[ Cnt ].Color.a );
	pVtx[ 3 ].color = D3DXCOLOR( g_Pause[ Cnt ].Color.r , g_Pause[ Cnt ].Color.g , g_Pause[ Cnt ].Color.b , g_Pause[ Cnt ].Color.a );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void VerTexUVPause( VERTEX_2D* pVtx , int Cnt )
 引数:		VERTEX_2D* pVtx		仮想アドレスを取得するポインタ変数
			int CntPause		ポーズ構造体のカウンタ
 戻り値:	
 説明:		UV値の変更
-----------------------------------------------------------------------------*/
void VerTexUVPause( VERTEX_2D* pVtx , int Cnt )
{

	pVtx += Cnt * NUM_VERTEX;

	//	テクスチャ座標の設定
	pVtx[ 0 ].tex = D3DXVECTOR2( 0 + g_UV_Scroll.x , 0 + g_UV_Scroll.y );
	pVtx[ 1 ].tex = D3DXVECTOR2( 1 + g_UV_Scroll.x , 0 + g_UV_Scroll.y );
	pVtx[ 2 ].tex = D3DXVECTOR2( 0 + g_UV_Scroll.x , 1 + g_UV_Scroll.y );
	pVtx[ 3 ].tex = D3DXVECTOR2( 1 + g_UV_Scroll.x , 1 + g_UV_Scroll.y );
														 
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	bool *GetPauseFlag( void )
 引数:		
 戻り値:	return &g_PauseFlag;
			ポーズ中	true
			ポーズ終了	false
 説明:		ポーズフラグ情報の取得
-----------------------------------------------------------------------------*/
bool *GetPauseFlag( void )
{

	return &g_PauseFlag;

}	//	end of func