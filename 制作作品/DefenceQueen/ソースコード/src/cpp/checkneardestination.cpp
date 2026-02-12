//=============================================================================
//
// 追跡対象が近くにいるかを判別するノード処理 [ checkneardestination.h ]
// Author: Asuma Nishio
//
//=============================================================================

//*****************************************************************************
// インクルードファイル
//*****************************************************************************
#include "checkneardestination.h"
#include "enemy.h"

//=============================================================================
// コンストラクタ
//=============================================================================
CCheckNearDestination::CCheckNearDestination(CBlackBoard* blackboard, CNode* TrueNode, CNode* FalseNode, float fDistance)
	: CBranchNodeBase(blackboard,TrueNode,FalseNode),m_fDistance(fDistance)
{

}
//=============================================================================
// 判別してフラグを返す関数
//=============================================================================
const bool CCheckNearDestination::IsCondition()
{
	// 対象オブジェクトの位置を取得 ( 今回は選択ポインタ )
	auto DestPos = m_pBlackBoard->GetValue<D3DXVECTOR3>("SelectorPos");

	// 追従する自分を取得する ( 今回は敵 )
	auto AgentPos = m_pBlackBoard->GetValue<D3DXVECTOR3>("EnemyPos");

	// 追従するためのベクトルを引く
	auto VecToDestObj = DestPos - AgentPos;

	// 長さに変換
	float fDistance = D3DXVec3Length(&VecToDestObj);

	// 距離が近ければtrueを返す
	return fDistance < m_fDistance;
}