//=========================================================
//
// 敵の攻撃ノードの管理クラス [ enemyattackleaf.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "leafnodebase.h"

//*********************************************************
// 攻撃操作を行う末端ノードのクラスを定義
//*********************************************************
class CEnemyAttackLeaf : public CLeafNodeBase
{
public:

	CEnemyAttackLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) {};
	~CEnemyAttackLeaf() = default;

	void Update() override;

	NodeInfo::NodeResult get_node_result() const override
	{
		// 成功を返す
		return NodeInfo::NodeResult::Re_SUCCESS;
	};

private:

	//***************************
	// 定数構造体
	//***************************
	struct Config
	{
		static constexpr int DAMAGE = 1; // ダメージ量
	};
};

