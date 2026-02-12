//=========================================================
//
// 敵の待機状態の末端ノード処理 [ enemywaitleaf.h ]
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
// 待機状態に切り替わるノードのクラス定義
//*********************************************************
class CEnemyWaitLeaf : public CLeafNodeBase
{
public:

	CEnemyWaitLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard)  {};
	~CEnemyWaitLeaf() = default;

	void Update() override;

	/// <summary>
	/// 常に成功である値を返す関数
	/// </summary>
	/// <returns></returns>
	NodeInfo::NodeResult get_node_result() const override
	{
		return NodeInfo::NodeResult::Re_SUCCESS;
	};
};

