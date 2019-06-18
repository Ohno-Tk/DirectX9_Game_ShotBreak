/*=============================================================================

		[ main.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/12/07
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/

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
#include "fade.h"
#include "camera.h"
#include "light.h"

#include "title.h"
#include "Tutorial.h"
#include "game.h"
#include "result.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/
#define CLASS_NAME     "3Dゲーム"			//	ウインドウクラスの名前
#define WINDOW_NAME    "SHOT BREAK"	//	ウインドウの名前

/*-----------------------------------------------------------------------------
	列挙
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	構造体
-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	プロトタイプ宣言
-----------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam );

HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow );	//	初期化関数
void Uninit( void );	//	終了処理
void Update( void );	//	更新処理
void Draw( void );	//	描画処理

#ifdef _DEBUG

void DrawFPS( void );

#endif	//	_DEBUG

//	ワイヤーフレーム
void Wireframe( void );

/*-----------------------------------------------------------------------------
	グローバル変数
-----------------------------------------------------------------------------*/
//	Direct3Dインターフェース取得
LPDIRECT3D9 g_pD3D = NULL;

LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	//	Direct3Dデバイスインターフェース（ポインタ）

MODE g_mode = MODE_TITLE;	//	今のモード

#ifdef _DEBUG

	LPD3DXFONT g_pFont = NULL;

#endif	//	_DEBUG

int g_nCountFPS = 0;	//	FPSカウンタ

bool g_WireframeFlag = false;	//	ワイヤーフレームフラグ


/*-----------------------------------------------------------------------------
	メイン関数
-----------------------------------------------------------------------------*/
int APIENTRY WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR IpCmdLine , int nCmdShow )
{
	//  waringの警告を消せる
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( IpCmdLine );

	//	ウインドウのパーツ登録
	WNDCLASSEX wcex =
	{
		sizeof( WNDCLASSEX ) ,					//	WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC ,							//	表示するウインドウのスタイルを設定
		WndProc ,								//	ウィンドウプロシージャのアドレス( 関数名 )を指定　プロトタイプ宣言した関数を書く（重要！！）
		0 ,										//	通常は使用しないので" 0 "を指定
		0 ,										//	通常は使用しないので" 0 "を指定
		hInstance ,								//	WinMainのパラメータのインスタンスハンドルを設定
		NULL ,									//	使用するアイコンを指定( Windowsが持っているアイコンを使うなら )
		LoadCursor( NULL , IDC_ARROW ),			//	マウスカーソルを指定	（授業ではやらない）
		( HBRUSH )( COLOR_WINDOW + 1 ) ,		//	ウインドウのクライアント領域の背景色を設定	（授業ではやらない）
		NULL ,									//	Windowsにつけるメニューを設定
		CLASS_NAME ,							//	ウインドウクラスの名前	（重要！！）
		NULL									//	小さいアイコンが設定された場合の情報を記述	（授業ではやらない）
	};

	//	ウインドウクラスの登録
	RegisterClassEx( &wcex );	//	オリジナルのウインドウパーツの名前をつける( カスタムウインドウ用の設定 )

	HWND hWnd;	//	ウインドウハンドル（　メインハンドル　）
	MSG msg;

	const int Windowstyle = WS_OVERLAPPEDWINDOW ^  WS_MINIMIZEBOX ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME;

	//	描画領域のサイズからウインドウの矩形を取得
	RECT cr = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };
	AdjustWindowRect( &cr , Windowstyle , FALSE );

	RECT dr;
	GetWindowRect( GetDesktopWindow() , &dr );

	int dw = dr.right - dr.left;
	int dh = dr.bottom - dr.top;

	int ww = cr.right - cr.left;
	int wh = cr.bottom - cr.top;

	int wx = ww > dw ? 0 : ( dw - ww ) / 2 ;
	int wy = wh > dh ? 0 : ( dh - wh ) / 2 ;

	//	ウインドウの作成
	hWnd/*←ウインドハンドル*/ = CreateWindowEx( 0 ,		//	拡張ウインドウスタイル（　きちんと設定する　）
		CLASS_NAME ,										//	ウインドウクラスの名前
		WINDOW_NAME ,										//	ウインドウの名前
		Windowstyle ,								//	ウインドウスタイル
		wx ,										//	ウインドウの左上Ｘ座標
		wy ,										//	ウインドウの左上Ｙ座標
		ww ,										//	ウインドウの幅（　フレームも合わせた幅　）
		wh ,										//	ウインドウの高さ（　フレームも合わせた高さ　）
		NULL ,												//	親ウインドウのハンドル（　ポインタ　）
		NULL ,												//	メニューハンドルまたは子ウインドウID
		hInstance ,											//	インスタンスハンドル
		NULL );												//	ウインドウ作成データ

	//	ウインドウの表示
	ShowWindow( hWnd , nCmdShow );		//	指定されたウインドウの表示
	UpdateWindow( hWnd );				//	ウインドウの状態を直ちに反映

	Init( hInstance , hWnd , TRUE );	//	初期化関数

	GetDevice();

	//	分解能の設定（タイマーの精度を良くする）
	timeBeginPeriod( 1 );

	//	各カウンターの初期化
	DWORD dwFrameCount = 0;
	DWORD dwCurrentTime = 0;	//	現在の時間
	DWORD dwExecLastTime = timeGetTime();	//	現在の時刻をミリ単位で取得
	DWORD dwFPSLastTime = dwExecLastTime;

	//	メッセージループ
	//	第１引数：メッセージ情報
	//	第２引数：ウインドウのハンドル
	//	第３引数：最初のメッセージ
	//	第４引数：最後のメッセージ
	for( ;; )
	{
		if( PeekMessage( &msg , NULL , 0 , 0 , PM_REMOVE ) != 0 )
		{
			//	Windowsの処理
			if( msg.message == WM_QUIT )
			{
				break;
			}
			else
			{
				//	呼び出し側スレッドのメッセージキューから
				//	メッセージの翻訳と送出
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();	//	現在の時刻をミリ単位で取得
			if( (dwCurrentTime - dwFPSLastTime) >= 500 )
			{
				g_nCountFPS = ( dwFrameCount * 1000 ) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if( (dwCurrentTime - dwExecLastTime) >= (1000/60) )	//	現在のフレームからどれ位たったのか( 差分計算 )	デバッグで計測したいときはifをコメントアウト
			{
				dwExecLastTime = dwCurrentTime;	//	処理した時間を保存

				//	DirectXの処理（ゲーム処理）
				Update();	//	更新処理

				Draw();	//	描画処理

				dwFrameCount++;

			}
		}
	}

	//	ウインドウクラスの登録を解除
	//	第一引数：メクラス名
	//	第二引数：アプリケーションインスタンス
	UnregisterClass( CLASS_NAME , wcex.hInstance );

	Uninit();	//	終了処理

	return ( int )msg.wParam;
}	//	end of func

/*-----------------------------------------------------------------------------
				ウインドウプロシージャ
		( CALLBACK:Win32API関数を呼び出すときの規約 )
		hWnd :   ウインドウのハンドル
		uMsg :   メッセージの識別子
		wParam : メッセージの最初のパラメータ
		lParam : メッセージの２番目のパラメータ
-------------------------------------------------------------------------------*/
LRESULT CALLBACK WndProc( HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam )
{
	UINT nSelect;	//	Escキーの入力判定

	switch( uMsg )
	{

	case WM_DESTROY:										//	ウインドウ破棄のメッセージ
		//MessageBox( NULL , "WM_DESTROYが実行された" , "" , MB_OK );
		PostQuitMessage( 0 );								//	"WM_QUIT"メッセージを返す
		break;

	case WM_KEYDOWN:	//	キー入力
		switch( wParam )
		{
			case VK_ESCAPE:	//	ESCAPEキー
			nSelect = MessageBox( NULL , "終了しますか？" , "終了メッセージ" , MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 );
			if( nSelect == IDYES )
			{
				DestroyWindow( hWnd );
			}
			break;
		}
	case WM_CLOSE:
		//MessageBox( NULL , "WM_CLOSEが実行された" , "" , MB_OK );
		break;

	default:
		break;
	}

	return DefWindowProc( hWnd , uMsg , wParam , lParam );	//	既定の処理を提供
}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow )
 引数:		HINSTANCE hInstance
			HWND hWnd
			BOOL bWindow
 戻り値:	S_OK
 説明:		初期化関数
-----------------------------------------------------------------------------*/
HRESULT Init( HINSTANCE hInstance , HWND hWnd , BOOL bWindow )
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	//	D3D9インターフェースの取得
	g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	if( g_pD3D == NULL )
	{
		return E_FAIL;
	}	//	end of if
	//	現在のディスプレイモードの取得
	if( FAILED( g_pD3D -> GetAdapterDisplayMode( D3DADAPTER_DEFAULT , &d3ddm ) ) )
	{
		return E_FAIL;
	}	//	end of if

	//	デバイスのプレゼンテーションパラメータの設定(デバイスの設定のための設定)
	ZeroMemory( &d3dpp , sizeof( d3dpp ) );
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//	Direct3Dデバイスの作成
	//	どちらもハードウェア処理
	if( FAILED( g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , hWnd , D3DCREATE_HARDWARE_VERTEXPROCESSING , &d3dpp , &g_pD3DDevice ) ) )
	{
		//	描画処理はハードウェア処理　頂点処理はソフトウェア処理
		if( FAILED( g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT , D3DDEVTYPE_HAL , hWnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &g_pD3DDevice ) ) )
		{
			//	どちらもソフトウェア処理
			if( FAILED( g_pD3D -> CreateDevice( D3DADAPTER_DEFAULT , D3DDEVTYPE_REF , hWnd , D3DCREATE_SOFTWARE_VERTEXPROCESSING , &d3dpp , &g_pD3DDevice ) ) )
			{
				MessageBox( NULL , "デバイスが作れませんでした" , "警告" , MB_OK | MB_ICONHAND );
				return E_FAIL;
			}	//	end of if
		}	//	end of if
	}	//	end of if

#if 0
	//	ビューポート変数
	D3DVIEWPORT9 vp;


	vp.X = 0.0f;
	vp.Y = SCREEN_HEIGHT / 2;
	vp.MinZ = 0.0f;
	vp.MaxZ = 1.0f;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT / 2;

	//	ビューポートをいじる
	g_pD3DDevice -> SetViewport( &vp );

#endif

	// 逆時計回りを裏面とする
	g_pD3DDevice -> SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );
#if 0
	// 時計回りを裏面とする
	_pD3DDevice -> SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );

	// 両面にする
	g_pD3DDevice -> SetRenderState( D3DRS_CULLMODE , D3DCULL_NONE );
#endif

	
	//	aブレンド
	//	加算合成
	g_pD3DDevice -> SetRenderState( D3DRS_ALPHABLENDENABLE , TRUE );
	g_pD3DDevice -> SetRenderState( D3DRS_SRCBLEND , D3DBLEND_SRCALPHA );
	g_pD3DDevice -> SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );

	//	アルファブレンドの設定
	g_pD3DDevice -> SetTextureStageState( 0 , D3DTSS_ALPHAOP , D3DTOP_MODULATE );
	g_pD3DDevice -> SetTextureStageState( 0 , D3DTSS_ALPHAARG1 , D3DTA_TEXTURE );
	g_pD3DDevice -> SetTextureStageState( 0 , D3DTSS_ALPHAARG2 , D3DTA_DIFFUSE );


	// 現在画面に描かれている状態に対して今から描画する内容

	// サンプラーステートの設定
	// 縮小時
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR );

	// 拡大時
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR );

#if 0
	// 左右ミラー
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSU , D3DTADDRESS_MIRROR );

	// 上下ミラー
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSV , D3DTADDRESS_MIRROR );

	// CLAMP
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSU , D3DTADDRESS_CLAMP );
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSV , D3DTADDRESS_CLAMP );
#endif

	// CLAMP
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSU , D3DTADDRESS_WRAP );
	g_pD3DDevice -> SetSamplerState( 0 , D3DSAMP_ADDRESSV , D3DTADDRESS_WRAP );

#ifdef _DEBUG
	D3DXCreateFont( g_pD3DDevice ,
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
					&g_pFont);
#endif	//	_DEBUG

	//	初期化
	g_WireframeFlag = false;	//	ワイヤーフレームフラグ

	InitKeyboard( hInstance, hWnd);	//	キーボード初期化

	InitSound( hWnd );	//	音楽の初期化

	InitFade();	//	フェードの初期化

	SetMode( g_mode );


//	InitCamera();	//	カメラの初期化

	InitLight();		//	ライトの初期化

	return S_OK;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void Uninit( void )
 引数:		
 戻り値:	
 説明:		終了関数
-----------------------------------------------------------------------------*/
void Uninit( void )
{
	ModeFunc pUninitFunc[]={
	UninitTitle,
	UninitTutorial,
	UninitGame,
	UninitResult,
	};

	if( g_pD3DDevice != NULL )
	{
		g_pD3DDevice -> Release();	//	Direct3DDevice開放
		g_pD3DDevice = NULL;	
	}
	if( g_pD3D != NULL )
	{
		g_pD3D -> Release();	//	Direct3Dインターフェース開放
		g_pD3D = NULL;
	}

#ifdef _DEBUG
	if( g_pFont != NULL )
	{
		g_pFont -> Release();	//	フォントのインターフェース開放
		g_pFont = NULL;
	}
#endif	//	_DEBUG

	UninitKeyboard();	//	キーボードの終了

	UninitSound();	//	音楽の終了

	UninitFade();	//	フェードの終了

	pUninitFunc[g_mode]();

	UninitCamera();	//	カメラの終了

	UninitLight();	//	ライトの終了

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void Update( void )
 引数:		
 戻り値:	
 説明:		更新関数
-----------------------------------------------------------------------------*/
void Update( void )
{

	ModeFunc pUpdateFunc[]={
	UpdateTitle,
	UpdateTutorial,
	UpdateGame,
	UpdateResult,
	};

	
	//	ワイヤーフレームのON・OFF
	if( GetKeyboardTrigger( DIK_F2 ) )
	{
		if( g_WireframeFlag == false )
		{
			g_WireframeFlag = true;

		}

		else if( g_WireframeFlag == true )
		{
			g_WireframeFlag = false;

		}

	}

	Wireframe();	//	ワイヤーフレーム

	UpdateKeyboard();	//	キーボードの更新

	UpdateFade();	//	フェードの更新

	pUpdateFunc[g_mode]();

	UpdateCamera();	//	カメラの更新

	UpdateLight();	//	ライトの更新

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void Draw( void )
 引数:		
 戻り値:	
 説明:		描画関数
-----------------------------------------------------------------------------*/
void Draw( void )
{
	ModeFunc pDrawFunc[]={
	DrawTitle,
	DrawTutorial,
	DrawGame,
	DrawResult,
	};


	//	バックバッファとＺバッファのクリア
	g_pD3DDevice ->Clear( 0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER , D3DCOLOR_RGBA( 0 , 177 , 184 , 255 ) , 1.0f , 0 );

	//	描画の開始
	if( SUCCEEDED( g_pD3DDevice -> BeginScene() ) )
	{

		DrawCamera();	//	カメラの描画

		DrawLight();	//	ライトの描画

		pDrawFunc[g_mode]();

		DrawFade();	//	フェードの描画
		
#ifdef _DEBUG
		DrawFPS();	//	FPS表示

#endif

		//	描画終了
		g_pD3DDevice -> EndScene();

	}	//	end of if

	g_pD3DDevice -> Present( NULL , NULL , NULL , NULL );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetMode(MODE mode)
 引数:		MODE mode
 戻り値:	
 説明:		モード設置
-----------------------------------------------------------------------------*/
void SetMode( MODE mode )
{
	ModeFunc pInitFunc[]={
	InitTitle,
	InitTutorial,
	InitGame,
	InitResult,
	};

	ModeFunc pUninitFunc[]={
	UninitTitle,
	UninitTutorial,
	UninitGame,
	UninitResult,
	};

	pUninitFunc[ g_mode ]();

	pInitFunc[ mode ]();

	g_mode = mode;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	LPDIRECT3DDEVICE9 GetDevice( void )
 引数:		
 戻り値:	
 説明:		D3DDeviceの取得
-----------------------------------------------------------------------------*/
LPDIRECT3DDEVICE9 GetDevice( void )
{

	return g_pD3DDevice;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	MODE *GetMode( void )
 引数:		
 戻り値:	return &g_mode;
 説明:		モードの取得
-----------------------------------------------------------------------------*/
MODE *GetMode( void )
{

	return &g_mode;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetWorldInv( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転
			D3DXVECTOR3 Scl		拡大率
 戻り値:	
 説明:		逆行列ありのワールド座標変換
-----------------------------------------------------------------------------*/
void SetWorldInv( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl )
{
	//	ビュー行列の逆行列の作成
	D3DXMATRIX mtxViewInverse;

	//	ビュー行列の情報取得
	D3DXMATRIX mtxView = GetView();

	//	転置行列
	D3DXMatrixTranspose( &mtxViewInverse , &mtxView );

	//	平行移動成分をカット
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	D3DXMATRIX mtxWorld , mtxScl , mtxRot , mtxPos;

	D3DXMatrixIdentity( &mtxWorld );	//	行列を単位行列にする

		//	拡大行列を作る
	D3DXMatrixScaling( &mtxScl ,	//拡大行列が作られる
		Scl.x ,	//	X軸拡大
		Scl.y ,	//	Y軸拡大
		Scl.z );	//	Z軸拡大

	//	回転行列を作る
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,	//	回転行列が作られる
		D3DXToRadian( Rot.y ) , //	Y軸回転
		D3DXToRadian( Rot.x ) , //	X軸回転
		D3DXToRadian( Rot.z ) ); //	Z軸回転

	//	平行移動行列を作る
	D3DXMatrixTranslation( &mtxPos ,
		Pos.x ,	//	X軸移動
		Pos.y ,	//	Y軸移動
		Pos.z );	//	Z軸移動


	//	掛けてあげる
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxViewInverse );

	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScl );

	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxPos );

	//	デバイスにワールド変換行列を設定
	g_pD3DDevice -> SetTransform( D3DTS_WORLD , &mtxWorld );


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void SetWorld( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl )
 引数:		D3DXVECTOR3 Pos		座標
			D3DXVECTOR3 Rot		回転
			D3DXVECTOR3 Scl		拡大率
 戻り値:	
 説明:		逆行列なしのワールド座標変換
-----------------------------------------------------------------------------*/
void SetWorld( D3DXVECTOR3 Pos , D3DXVECTOR3 Rot , D3DXVECTOR3 Scl )
{

	D3DXMATRIX mtxWorld , mtxScl , mtxRot , mtxPos;

	D3DXMatrixIdentity( &mtxWorld );	//	行列を単位行列にする

		//	拡大行列を作る
	D3DXMatrixScaling( &mtxScl ,	//拡大行列が作られる
		Scl.x ,	//	X軸拡大
		Scl.y ,	//	Y軸拡大
		Scl.z );	//	Z軸拡大

	//	回転行列を作る
	D3DXMatrixRotationYawPitchRoll( &mtxRot ,	//	回転行列が作られる
		D3DXToRadian( Rot.y ) , //	Y軸回転
		D3DXToRadian( Rot.x ) , //	X軸回転
		D3DXToRadian( Rot.z ) ); //	Z軸回転

	//	平行移動行列を作る
	D3DXMatrixTranslation( &mtxPos ,
		Pos.x ,	//	X軸移動
		Pos.y ,	//	Y軸移動
		Pos.z );	//	Z軸移動


	//	掛けてあげる
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScl );

	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxPos );

	//	デバイスにワールド変換行列を設定
	g_pD3DDevice -> SetTransform( D3DTS_WORLD , &mtxWorld );


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void Wireframe( void )
 引数:		
 戻り値:	
 説明:		ワイヤーフレーム
-----------------------------------------------------------------------------*/
void Wireframe( void )
{

	if( g_WireframeFlag == false )
	{

		// ワイヤーフレーム表示にする
		g_pD3DDevice -> SetRenderState( D3DRS_FILLMODE , D3DFILL_FORCE_DWORD );

	}	//	end of if

	else if( g_WireframeFlag == true )
	{

		// ワイヤーフレーム表示にする
		g_pD3DDevice -> SetRenderState( D3DRS_FILLMODE , D3DFILL_WIREFRAME );

	}	//	end of else if


}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawFPS( void )
 引数:		
 戻り値:	
 説明:		FPS表示
-----------------------------------------------------------------------------*/
#ifdef _DEBUG
void DrawFPS( void )
{
	RECT rect = { 0 , 0 , SCREEN_WIDTH , SCREEN_HEIGHT };

	char aStr[ 256 ];

	sprintf( &aStr[ 0 ] , "FPS:%d" , g_nCountFPS );

	g_pFont -> DrawText( NULL ,
						&aStr[ 0 ] ,
						-1 ,
						&rect ,
						DT_LEFT ,
						D3DCOLOR_RGBA( 255 , 255 , 255 , 255));
}
#endif	//	_DEBUG
