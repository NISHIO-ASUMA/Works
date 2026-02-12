//===================================================================
//
// 敵を攻撃する末端ノード処理 [ attackenemyleaf.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#pragma once

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "leafnodebase.h"

//*******************************************************************
// 敵を攻撃する末端ノード処理クラスを定義
//*******************************************************************
class CAttackEnemyLeaf : public CLeafNodeBase
{
public:

	CAttackEnemyLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) {};
	~CAttackEnemyLeaf() = default;

	void Update(void);

	NodeInfo::NodeResult get_node_result() const override
	{
		return NodeInfo::NodeResult::Re_SUCCESS;
	};

private:

};