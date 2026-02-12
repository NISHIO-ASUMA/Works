//===================================================================
//
// 仲間が敵に攻撃するノードの管理クラス [ arrayattackleaf.h ]
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
// 攻撃操作を行う末端ノードのクラスを定義
//*******************************************************************
class CArrayAttackLeaf : public CLeafNodeBase
{
public:

	CArrayAttackLeaf(CBlackBoard* blackboard) : CLeafNodeBase(blackboard) {};
	~CArrayAttackLeaf() = default;

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