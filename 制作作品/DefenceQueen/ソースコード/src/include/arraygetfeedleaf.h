//===================================================================
//
// 餌を巣に持って帰る末端ノードクラスの処理 [ arraygetfeedleaf.h ]
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
// 餌を持ち帰る行動ノードの処理を行うクラスを定義
//*******************************************************************
class CArrayGetFeedLeaf : public CLeafNodeBase
{
public:

	CArrayGetFeedLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) {};
	~CArrayGetFeedLeaf() = default;

	void Update() override;

	NodeInfo::NodeResult get_node_result() const override
	{
		// 成功を返す
		return NodeInfo::NodeResult::Re_SUCCESS;
	};

private:

};

