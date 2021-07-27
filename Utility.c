/*=============================================================================

		[ Utility.cpp ]

-------------------------------------------------------------------------------

	■　作成日
		2016/01/05
-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
	ヘッダファイル
-----------------------------------------------------------------------------*/
#include "main.h"
#include "Utility.h"

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
 関数名:	bool RectangleCollision( D3DXVECTOR2 Pos_A , D3DXVECTOR2 Size_A , D3DXVECTOR2 Pos_B , D3DXVECTOR2 Size_B )
 引数:		D3DXVECTOR2 Pos_A	物体Aの座標
			D3DXVECTOR2 Size_A	物体Aの大きさ
			D3DXVECTOR2 Pos_B	物体Bの座標
			D3DXVECTOR2 Size_B	物体Bの大きさ

 戻り値:	当たっている場合	return true;
			当たっていない場合	return false;

 説明:		矩形と矩形の当たり判定
-----------------------------------------------------------------------------*/
bool RectangleCollision( D3DXVECTOR2 Pos_A , D3DXVECTOR2 Size_A , D3DXVECTOR2 Pos_B , D3DXVECTOR2 Size_B )
{

	float x[] = { Pos_A.x , Pos_A.x +  Size_A.x , Pos_B.x , Pos_B.x +  Size_B.x };
	float y[] = { Pos_A.y , Pos_A.y +  Size_A.y , Pos_B.y , Pos_B.y +  Size_B.y };

	//	矩形と矩形の当たり判定
	if( x[ 0 ] < x[ 3 ] && x[ 2 ] < x[ 1 ] && y[ 0 ] < y[ 3 ] && y[ 2 ] < y[ 1 ] )
	{

		return true;

	}	//	end of if

	return false;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	bool CircleCollision( D3DXVECTOR2 Pos_A , D3DXVECTOR2 Pos_B , float Radius_A , float Radius_B )
 引数:		D3DXVECTOR2 Pos_A	物体Aの座標
			D3DXVECTOR2 Pos_B	物体Bの座標
			float Radius_A		物体Aの半径
			float Radius_B		物体Bの半径

 戻り値:	当たっている場合	return true;
			当たっていない場合	return false;

 説明:		円と円の当たり判定
-----------------------------------------------------------------------------*/
bool CircleCollision( D3DXVECTOR2 Pos_A , D3DXVECTOR2 Pos_B , float Radius_A , float Radius_B )
{

	//	円と円の当たり判定
	if( ( Pos_B.x - Pos_A.x ) * ( Pos_B.x - Pos_A.x ) + ( Pos_B.y - Pos_A.y ) * ( Pos_B.y - Pos_A.y ) <= ( Radius_A + Radius_B ) * ( Radius_A + Radius_B ) )
	{

		return true;

	}	//	end of if

	return false;

}	//	end of func

/*-----------------------------------------------------------------------------
 関数名:	bool SphereCollision( D3DXVECTOR3 Pos_A , D3DXVECTOR3 Pos_B , float Radius_A , float Radius_B )
 引数:		D3DXVECTOR3 Pos_A	物体Aの座標
			D3DXVECTOR3 Pos_B	物体Bの座標
			float Radius_A		物体Aの半径
			float Radius_B		物体Bの半径

 戻り値:	当たっている場合	return true;
			当たっていない場合	return false;

 説明:		球と球の当たり判定
-----------------------------------------------------------------------------*/
bool SphereCollision( D3DXVECTOR3 Pos_A , D3DXVECTOR3 Pos_B , float Radius_A , float Radius_B )
{

	//	球と球の当たり判定
	if( ( Pos_B.x - Pos_A.x ) * ( Pos_B.x - Pos_A.x ) + ( Pos_B.y - Pos_A.y ) * ( Pos_B.y - Pos_A.y ) + ( Pos_B.z - Pos_A.z ) * ( Pos_B.z - Pos_A.z ) <= ( Radius_A + Radius_B ) * ( Radius_A + Radius_B ) )
	{

		return true;

	}	//	end of if

	return false;

}	//	end of func
