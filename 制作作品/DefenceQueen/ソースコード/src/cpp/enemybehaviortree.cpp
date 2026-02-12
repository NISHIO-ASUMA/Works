//=========================================================
//
// 敵のAIツリーを管理するクラス [ enemybehaviortree.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemybehaviortree.h"
#include "node.h"
#include "inverter.h"
#include "sepuence.h"
#include "alwayssuccess.h"
#include "enemychaseleaf.h"
#include "enemychecknear.h"
#include "enemywaitleaf.h"
#include "enemyattackleaf.h"
#include "selector.h"
#include "enemygetfeedleaf.h"
#include "enemyreturnleaf.h"

//=========================================================
// 実際のツリーを生成する
//=========================================================
CNode* EnemyTree::CEnemyBehaviorTree::SetEnemyTreeNode(CBlackBoard* blackboard)
{
	// インバーターを作成する
	auto chaseinverter = new CInverter(blackboard, new CEnemyChaseLeaf(blackboard));

	// シーケンスノードを作成する
	auto Sequence = new CSequence(blackboard);

	// 近いかどうかチェックするノード作成
	Sequence->AddNode(new CEnemyCheckNear(blackboard, new CAlwaysSuccessLeaf(blackboard), chaseinverter, 100.0f));

	// 追従をするノードをシーケンスノードに追加
	Sequence->AddNode(new CEnemyChaseLeaf(blackboard));

	// 攻撃継続ノードを生成
	auto attackLoop = new CEnemyAttackLeaf(blackboard);

	// 餌を取得するノードを生成
	auto getFood = new CEnemyGetFeedLeaf(blackboard);
	auto returnToNest = new CEnemyReturnLeaf(blackboard);

	// 食料取得後は巣へ戻るためのSequenceノードを作成
	auto foodSequence = new CSequence(blackboard);
	foodSequence->AddNode(getFood);
	foodSequence->AddNode(returnToNest);

	// 攻撃するか餌を持ち帰るかのセレクター用のノードを生成する
	auto Selector = new CSelector(blackboard);

	Selector->AddNode(attackLoop);		// 成功したらずっと攻撃し続ける
	Selector->AddNode(foodSequence);	//  失敗したら餌を持って帰る

	// セレクターノードを根幹のシーケンスに追加する
	Sequence->AddNode(Selector);

	// 待機時用のノードをシーケンスノードに追加
	Sequence->AddNode(new CEnemyWaitLeaf(blackboard));
	
	// 作成されたシーケンスノードを返す
	return Sequence;
}