//=========================================================
//
// 敵の待機状態の末端ノード処理 [ arraywaitleaf.h ]
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
// 前方宣言
//*********************************************************
class CBlackBoard;

//*********************************************************
// 待機状態のノードクラスを定義
//*********************************************************
class CArrayWaitLeaf : public CLeafNodeBase
{
public:

	CArrayWaitLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) {};
	~CArrayWaitLeaf() = default;

	void Update() override;

	/// <summary>
	/// 常に成功である値を返す関数
	/// </summary>
	/// <returns></returns>
	NodeInfo::NodeResult get_node_result() const override
	{
		return NodeInfo::NodeResult::Re_SUCCESS;
	};

private:

};