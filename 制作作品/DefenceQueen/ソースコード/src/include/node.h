//=========================================================
//
// AIノードインターフェースクラス処理 [ node.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// ノードが返す基本結果列挙型
//*********************************************************
namespace NodeInfo
{
	enum class NodeResult
	{
		Re_IDLE,	// 待機中
		Re_RUNING,	// 実行中
		Re_SUCCESS,	// 成功結果
		Re_FAIL,	// 失敗結果
		Re_MAX
	};
};

//*********************************************************
// ノード基底クラスを定義
//*********************************************************
class CNode
{
public:

	// 仮想デストラクタ
	CNode() {};
	virtual ~CNode() = default;

	// 純粋仮想関数として持つ
	virtual void Init() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	// 取得関数
	virtual NodeInfo::NodeResult get_node_result() const = 0;

	// ノードのIDを保存
	virtual void set_node_id(const int id) = 0;

	// 現在動作中のノードのIDを取得
	virtual int get_running_node_id() const = 0;
};