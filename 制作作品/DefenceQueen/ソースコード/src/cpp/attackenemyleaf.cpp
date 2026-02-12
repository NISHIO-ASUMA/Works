//===================================================================
//
// 敵を攻撃する末端ノード処理 [ attackenemyleaf.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "attackenemyleaf.h"
#include "array.h"

//===================================================================
// 更新処理　     ここの攻撃展開を考える(時間とか)
//===================================================================
void CAttackEnemyLeaf::Update(void)
{
	// アリを取得
	const auto& Array = m_pBlackBoard->GetValue<CArray*>("Array");

	// フラグ格納変数
	bool IsAttack = false;

	// キー情報が見つかったら
	if (m_pBlackBoard->HasKeyData("AttackMode"))
	{
		// 判別フラグを取得
		IsAttack = m_pBlackBoard->GetValue<bool>("AttackMode");
	}

	// コリジョン関数実行
	if (IsAttack) Array->CollisionEnemy();
}