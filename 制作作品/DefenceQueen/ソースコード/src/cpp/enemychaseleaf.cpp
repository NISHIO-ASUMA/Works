//=========================================================
//
// 敵が対象物を追従する処理 [ enemychaseleaf.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemychaseleaf.h"
#include "enemy.h"
#include "template.h"

//=========================================================
// ノード更新処理
//=========================================================
void CEnemyChaseLeaf::Update()
{
#if 0
	// ローカル変数
	ChaseInfo info = {};

	// 追従する自分を取得する
	auto* DestObject = m_pBlackBoard->GetValue<CEnemy*>("Enemy");

	// 追従するためのベクトルを引く
	auto VecToDestObj = DestPos - DestObject->GetPos();
	float dist = D3DXVec3Length(&VecToDestObj);

	// 停止距離外なら
	if (dist > info.DISTANCE)
	{
		// 正規化
		D3DXVec3Normalize(&VecToDestObj, &VecToDestObj);

		// 速度を乗算する
		VecToDestObj *= info.SPEED;

		// 計算角度
		float angleY = atan2(-VecToDestObj.x, -VecToDestObj.z);
		D3DXVECTOR3 rotDest = DestObject->GetRotDest();
		rotDest.y = angleY;

		// 正規化する
		rotDest.y = NormalAngle(rotDest.y);

		// 目的角を更新
		DestObject->SetRotDest(rotDest);

		// 移動量をセット
		DestObject->SetMove(VecToDestObj);

		// 移動モーションに変更
		DestObject->GetMotion()->SetMotion(CEnemy::MOTION_MOVE);
	}
	else
	{
		// 一定の距離なら動かない
		DestObject->SetMove(VECTOR3_NULL);
	}
#endif
}