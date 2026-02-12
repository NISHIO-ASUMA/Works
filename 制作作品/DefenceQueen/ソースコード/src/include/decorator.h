//=========================================================
//
// ノード結果を加工して返すクラス処理 [ decorator.h ]
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
#include "nodebase.h"

//*********************************************************
// 前方宣言
//*********************************************************
class CBlackBoard;

//*********************************************************
// ノード加工クラスを定義
//*********************************************************
class CDecorator : public CNodeBase
{
public:

	CDecorator(CBlackBoard* blackboard) : CNodeBase(blackboard) {};

	virtual ~CDecorator()
	{
		delete m_ChildNode;
		m_ChildNode = nullptr;
	};

	virtual void Init() override
	{
		// 親クラスの初期化
		CNodeBase::Init();

		// メンバ変数の初期化
		m_ChildNode->Init();
	}

	virtual void Exit() override
	{
		// 親クラスの終了処理
		CNodeBase::Exit();

		// メンバ変数の終了処理
		m_ChildNode->Exit();
	}

	/// <summary>
	/// 加工ノードをセットする
	/// </summary>
	/// <param name="pNode">CNodeクラスのポインタ</param>
	void SetNode(CNode* pNode) { m_ChildNode = pNode; }

protected:

	CNode* m_ChildNode = nullptr; 	// 子ノードの変数
};