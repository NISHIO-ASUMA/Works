//===================================================================
//
// 待機状態時で外部からの命令を待つ末端ノードの処理 [ waitorderleaf.h ]
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
// 待機状態時で外部からの命令を待つ末端ノードクラスを定義
//*******************************************************************
class CWaitOrderLeaf : public CLeafNodeBase
{
public:

	CWaitOrderLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard), m_Result(NodeInfo::NodeResult::Re_RUNING){};
	~CWaitOrderLeaf() = default;

	void Update(void) override;

	NodeInfo::NodeResult get_node_result() const override
	{
		return m_Result;
	};

private:
	NodeInfo::NodeResult m_Result; // ノードの判別結果
};