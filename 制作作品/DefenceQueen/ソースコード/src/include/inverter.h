//=========================================================
//
// ノード結果を反転させて返す処理 [ inverter.h ]
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
#include "decorator.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBlackBoard;

//*********************************************************
// 反転する結果を返すクラスを定義
//*********************************************************
class CInverter : public CDecorator
{
public:

	CInverter(CBlackBoard* blackboard, CNode* pChildnode) : CDecorator(blackboard)
	{
		// 加工するノードをセット
		SetNode(pChildnode);
	}

	~CInverter() { delete m_ChildNode; m_ChildNode = nullptr; }

	void Update() override
	{
		// 子ノードの更新処理
		m_ChildNode->Update();

		// 子ノードの結果を取得する
		auto result = m_ChildNode->get_node_result();

		// 取得した結果を反転させてセットする
		if (result == NodeInfo::NodeResult::Re_SUCCESS)
		{// 成功なら失敗に
			m_NodeResult = NodeInfo::NodeResult::Re_FAIL;
			return;
		}
		else if (result == NodeInfo::NodeResult::Re_FAIL)
		{// 失敗なら成功にして返す
			m_NodeResult = NodeInfo::NodeResult::Re_SUCCESS;
			return;
		}

		// 実行中ならそのまま実行中にする
		m_NodeResult = NodeInfo::NodeResult::Re_RUNING;
	}
};