//===================================================================
//
// 命令を受けているか判別する末端ノード処理 [ hastoporderleaf.h ]
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
// 命令を受けているか判別する末端ノードクラスを定義
//*******************************************************************
class CHasTopOrderLeaf : public CLeafNodeBase
{
public:

	CHasTopOrderLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard),m_Result(NodeInfo::NodeResult::Re_FAIL) {};
	~CHasTopOrderLeaf() = default;

	void Update() override;

	NodeInfo::NodeResult get_node_result() const override
	{
		// 判別結果を返す
		return m_Result;
	};

private:

	NodeInfo::NodeResult m_Result; // 判別フラグによって変更されるノード結果の変数
};