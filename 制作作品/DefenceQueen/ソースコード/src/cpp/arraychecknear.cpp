//=========================================================
//
// 範囲内にいるかチェックする処理 [ arraychecknear.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "arraychecknear.h"
#include "array.h"

//=========================================================
// コンストラクタ
//=========================================================
CArrayCheckNear::CArrayCheckNear(CBlackBoard* blackboard, CNode* TrueNode, CNode* FalseNode, const float fDistance)
    : CBranchNodeBase(blackboard, TrueNode, FalseNode), m_fDistance(fDistance)
{

}
//=========================================================
// 判別関数 
//=========================================================
const bool CArrayCheckNear::IsCondition()
{
#if 0
	// 対象オブジェクトの位置を取得
	auto DestPos = m_pBlackBoard->GetValue<D3DXVECTOR3>("SelectorPos");

	// 追従する自分を取得する
	auto Agent = m_pBlackBoard->GetValue<CEnemy*>("Enemy");
	auto AgentPos = Agent->GetPos();

	// 追従するためのベクトルを引く
	auto VecToDestObj = DestPos - AgentPos;

	// 長さに変換
	float fDistance = D3DXVec3Length(&VecToDestObj);

	// 距離が近ければtrueを返す
	return fDistance < m_fDistance;
#endif
	return false;
}
