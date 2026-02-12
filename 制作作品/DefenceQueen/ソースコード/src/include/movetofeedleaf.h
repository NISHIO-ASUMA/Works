//===================================================================
//
// 餌に向かって進み続ける末端ノード処理 [ movetofeedleaf.h ]
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
// 餌に向かって進む末端ノード処理クラスを定義
//*******************************************************************
class CMoveToFeedLeaf : public CLeafNodeBase
{
public:

	CMoveToFeedLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard), m_Result(NodeInfo::NodeResult::Re_RUNING) {};
	~CMoveToFeedLeaf() = default;

	void Update(void);

	NodeInfo::NodeResult get_node_result() const override
	{
		return m_Result;
	};

private:

	NodeInfo::NodeResult m_Result; // 判別フラグ
};