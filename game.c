/*=============================================================================

		ゲーム[ game.cpp ]

-------------------------------------------------------------------------------
	■　製作者
		大野拓也

	■　作成日
		2016/12/15
-------------------------------------------------------------------------------
	■　Update
=============================================================================*/
/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "camera.h"
#include "MeshField.h"
#include "MeshWall.h"
#include "skydome.h"
#include "Shadow.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyFly.h"
#include "Building.h"
#include "bullet.h"
#include "Effect.h"
#include "explosion.h"
#include "Score.h"
#include "Time.h"
#include "CountDown.h"
#include "Pause.h"
#include "Reticle.h"

/*-----------------------------------------------------------------------------
	定数定義
-----------------------------------------------------------------------------*/

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

/*-----------------------------------------------------------------------------
 関数名:	void InitGame( void )
 引数:		なし
 戻り値:	なし
 説明:		初期化
-----------------------------------------------------------------------------*/
void InitGame( void )
{

	InitCamera();	//	カメラの初期化

	InitMeshField();		//	メッシュフィールドの初期化

	InitMeshWall();		//	メッシュウォールの初期化

	InitSkydome();		//	スカイドームの初期化

	InitShadow();	//	影の初期化

	InitPlayer();		//	プレイヤーの初期化

	InitEnemy();		//	敵の初期化

	InitEnemyFly();		//	敵( 浮遊タイプ )の初期化

	InitBuilding();		//	建物の初期化

	InitBullet();	//	弾の初期化

	InitEffect();	//	エフェクトの初期化

	InitExplosin();		//	爆発の初期化

	InitScore();		//	スコアの初期化

	InitTime();		//	タイムの初期化

	InitCountDown();		//	カウントダウンの初期化

	InitPause();		//	ポーズの初期化

	InitReticle();	//	照準の初期化

	//	音楽再生
	PlaySound( SOUND_LABEL_BGM_GAME );

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UninitGame( void )
 引数:		なし
 戻り値:	なし
 説明:		終了
-----------------------------------------------------------------------------*/
void UninitGame( void )
{

	UninitMeshField();		//	メッシュフィールドの終了

	UninitMeshWall();	//	メッシュウォールの終了

	UninitSkydome();		//	スカイドームの終了

	UninitShadow();	//	影の終了

	UninitPlayer();	//	プレイヤーの終了

	UninitEnemy();	//	敵の終了

	UninitEnemyFly();	//	敵( 浮遊タイプ )の終了

	UninitBuilding();	//	建物の終了

	UninitBullet();	//	弾の終了

	UninitEffect();	//	エフェクトの終了

	UninitExplosin();	//	爆発の終了

	UninitScore();	//	スコアの終了

	UninitTime();	//	タイムの終了

	UninitCountDown();	//	カウントダウンの終了

	UninitPause();	//	ポーズの終了

	UninitReticle();	//	照準の終了

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void UpdateGame( void )
 引数:		なし
 戻り値:	なし
 説明:		更新
-----------------------------------------------------------------------------*/
void UpdateGame( void )
{

	//	スタートしたかの情報取得
	bool *Start = GetStart();

	//	タイム情報の取得
	bool *Time = GetTime();

	//	ポーズフラグ情報の取得
	bool *Pause = GetPauseFlag();


	if( *Start == true && *Pause == false )
	{

		UpdateMeshField();		//	メッシュフィールドの更新

		UpdateMeshWall();	//	メッシュウォールの更新

		UpdateSkydome();		//	スカイドームの更新

		UpdateShadow();	//	影の更新

		UpdatePlayer();	//	プレイヤーの更新

		UpdateEnemy();	//	敵の更新

		UpdateEnemyFly();	//	敵( 浮遊タイプ )の更新

		UpdateBuilding();	//	建物の更新

		UpdateBullet();	//	弾の更新

		UpdateEffect();	//	エフェクトの更新

		UpdateExplosin();	//	爆発の更新

		UpdateScore();	//	スコアの更新

		UpdateTime();	//	タイムの更新

		UpdateReticle();	//	照準の更新


	}	//	end of if

	UpdateCountDown();	//	カウントダウンの更新

	UpdatePause();	//	ポーズの更新


	//	画面遷移
	if( *Time == true )
	{
		
		//	フェードのセット
		SetFade( FADE_OUT , MODE_RESULT );

		//	何番目の音を止める
		StopSound( SOUND_LABEL_BGM_GAME );

	}	//	end of if
	//if( GetKeyboardTrigger( DIK_RETURN ) )
	//{
	//	SetFade( FADE_OUT , MODE_RESULT );

	//	//	何番目の音を止める
	//	StopSound( SOUND_LABEL_BGM_GAME );

	//}	//	end of if

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	void DrawGame( void )
 引数:		なし
 戻り値:	なし
 説明:		描画
-----------------------------------------------------------------------------*/
void DrawGame( void )
{

	DrawMeshField();		//	メッシュフィールドの描画

	DrawSkydome();		//	スカイドームの描画

	DrawMeshWall();		//	メッシュウォールの描画

	DrawShadow();	//	影の描画

	DrawPlayer();		//	プレイヤーの描画

	DrawEnemy();		//	敵の描画

	DrawEnemyFly();		//	敵( 浮遊タイプ )の描画

	DrawBuilding();		//	建物の描画

	DrawBullet();	//	弾の描画

	DrawEffect();	//	エフェクトの描画

	DrawExplosin();		//	爆発の描画

	DrawScore();		//	スコアの描画

	DrawTime();		//	タイムの描画

	DrawCountDown();		//	カウントダウンの描画

	DrawPause();		//	ポーズの描画

	DrawReticle();	//	照準の描画

}	//	end of func