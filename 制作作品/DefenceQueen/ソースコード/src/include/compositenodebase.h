//===================================================================
//
// 子ノードを複数持ち管理するベースクラス [ compositenodebase.h ]
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
#include "nodebase.h"
#include <vector>

//*******************************************************************
// 前方宣言
//*******************************************************************
class CBlackBoard;
class CNode;

//*******************************************************************
// 子ノードを管理するベースクラスを定義
//*******************************************************************
class CCompositeNodeBase : public CNodeBase
{
public:

	CCompositeNodeBase(CBlackBoard* blackboard) : CNodeBase(blackboard) {};

	virtual ~CCompositeNodeBase()
	{
		// ポインタの破棄
		for (auto node : m_ChildeNode)
		{
			delete node;
		}

		// 配列クリア
		m_ChildeNode.clear();
	}

	virtual void Init() override
	{
		// 親クラスの初期化
		CNodeBase::Init();

		// メンバ変数の初期化
		m_RanningIndex = NULL;

		// 最初のノードを初期化
		if (static_cast<int>(m_ChildeNode.size()) > NULL)
		{
			// 指定番号の初期化をする
			m_ChildeNode[m_RanningIndex]->Init();
		}
		else
		{
			// 失敗リザルトとして初期化
			m_NodeResult = NodeInfo::NodeResult::Re_FAIL;
		}
	}

	virtual void Exit() override
	{
		// 親クラスの破棄
		CNodeBase::Exit();

		// 最初のノードを初期化する
		m_RanningIndex = 0;
		m_ChildeNode[m_RanningIndex]->Init();
	}

	void AddNode(CNode* node) { m_ChildeNode.push_back(node); }

	int get_running_node_id() const override
	{
		// 現在動いているノードのIDを返す
		return m_ChildeNode[m_RanningIndex]->get_running_node_id();
	}

protected:

	// 派生先のクラスで使う
	virtual const int GetNextIdx() const = 0;

	/// <summary>
	/// ノードインデックスの加算処理
	/// </summary>
	void NodeIncrement()
	{
		// 現在のノードの破棄
		m_ChildeNode[m_RanningIndex]->Exit();

		// ノードインデックスを加算
		m_RanningIndex = GetNextIdx();

		// 例外設定
		if (m_RanningIndex > static_cast<int>(m_ChildeNode.size() - 1))
		{
			m_NodeResult = NodeInfo::NodeResult::Re_FAIL;
			Exit();
			return;
		}

		// 次のノード初期化
		m_ChildeNode[m_RanningIndex]->Init();
	};

	std::vector<CNode*>m_ChildeNode; // 複数の子ノードを管理する動的配列
	int m_RanningIndex = 0;			 // 現在ノードのインデックス
};