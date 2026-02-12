//===================================================================
//
// ビヘイビアツリー基底の処理( サンプル )[ behaviortree.cpp ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "behaviortree.h"
#include "inverter.h"
#include "chasedestination.h"
#include "sepuence.h"
#include "checkneardestination.h"
#include "alwayssuccess.h"
#include "waitleaf.h"

//===================================================================
// コンストラクタ
//===================================================================
CBehaviorTree::CBehaviorTree()
{

}
//===================================================================
// デストラクタ
//===================================================================
CBehaviorTree::~CBehaviorTree()
{

}
//===================================================================
// 更新関数
//===================================================================
CNode* CBehaviorTree::GetAttackTree(CBlackBoard* blackboard)
{
	// Inverterノードを作成する
	auto chase_inverter = new CInverter(blackboard, new CChaseDestinationLeaf(blackboard));

	// Sequenceノードを作成する
	auto Sequence = new CSequence(blackboard);

	// 近いかどうかのノード作成
	Sequence->AddNode(new CCheckNearDestination(blackboard, new CAlwaysSuccessLeaf(blackboard), chase_inverter, 100.0f));

	// 追従ノードを作成
	Sequence->AddNode(new CChaseDestinationLeaf(blackboard));

	// 待機時のノード作成
	Sequence->AddNode(new CWaitLeaf(blackboard));

	// 作成されたSequenceを返す
	return Sequence;
}