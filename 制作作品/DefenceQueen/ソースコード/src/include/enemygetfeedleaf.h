//===================================================================
//
// 餌を奪う末端ノードのクラス処理 [ enemyreturnleaf.h ]
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

//===================================================================
// 餌を獲得する末端ノードのクラス定義
//===================================================================
class CEnemyGetFeedLeaf : public CLeafNodeBase
{
public:

	CEnemyGetFeedLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) {};
	~CEnemyGetFeedLeaf() = default;

	void Update() override;

	NodeInfo::NodeResult get_node_result() const override
	{
		// 成功を返す
		return NodeInfo::NodeResult::Re_SUCCESS;
	};

private:

};