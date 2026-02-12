//=========================================================
//
// 指示を出す働きアリの処理 [ worker.h ]
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
#include "movecharactor.h"
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CMotion;
class CStateMachine;
class CSphereCollider;
class CSelectPoint;
class CWorkerStateBase;
class CFeed;
class CSignalUi;

//*********************************************************
// 働きアリクラスの定義
//*********************************************************
class CWorker : public CMoveCharactor
{
public:

	//***********************************
	// モーション列挙型
	//***********************************
	enum MOTION
	{
		MOTION_NEUTRAL,	// ニュートラル
		MOTION_MOVE,	// 移動
		MOTION_MAX
	};

	//***********************************
	// 出す数字列挙型
	//***********************************
	enum NUMARRAY
	{
		NUMARRAY_LITTLE, // 最小値
		NUMARRAY_MIDDLE, // 中央値
		NUMARRAY_LARGE,  // 最大値
		NUMARRAY_MAX
	};

	//***********************************
	// 行動状態列挙型
	//***********************************
	enum MOVE
	{
		NONE,		// 動いてない
		MOVE_FEED,	// 餌に移動
		MOVE_BASE,	// 基地に移動
		MOVE_MAX
	};

	CWorker(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CWorker();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;
	void MoveToPoint(void);
	void MoveToReturnBase(void);
	void AssignFeed(CFeed* pFeed);
	int RequiredNumber(void);

	void ChangeState(CWorkerStateBase* pState);
	bool Collision(CSphereCollider* other);

	inline void SetDestPos(const D3DXVECTOR3 pos) { m_DestPos = pos; }
	inline void SetNeedNumber(const int nNumber) { m_nIdxNumber = nNumber; }
	inline void SetIsWork(const bool iswork) { m_isWork = iswork; }
	inline MOVE SetMoveState(MOVE Move) { m_MoveState = Move; }
	inline CFeed* SetPoint(CFeed* pfeed) { m_pTargetFeed = pfeed; }

	inline bool GetIsWork(void) const { return m_isWork; }
	inline bool GetIsCreate(void) const { return m_isCreate; }
	inline bool GetMoveState(void) { return m_MoveState == NONE; }

	static CWorker* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

private:

	//****************************
	// 定数格納構造体
	//****************************
	struct Config
	{
		static constexpr float Distance = 80.0f; // 判定距離
		static constexpr float Move = 3.5f;		 // 移動速度
		static constexpr float StopDistance = 2.0f; // 停止最小値
		static constexpr float Add_PosY = 160.0f;	// 高さの加算値

		static constexpr int LITTLE = 10;		 // 最小値
		static constexpr int MIDDLE = 15;		 // 中央値
		static constexpr int LARGE = 20;		 // 最大値
		static constexpr int MAX_COUNT = 90;	// 生成するのに必要な最大カウント
		static constexpr int IMPACT_LIFE = 60;	// 体力値
	};

	std::unique_ptr<CStateMachine>m_pStateMachine;	// ステート基底クラスのポインタ

	CMotion* m_pMotion;					// モーションポインタ
	CSphereCollider* m_pSphereCollider;	// 球形のコライダー
	CFeed* m_pTargetFeed;				// 自身が目的地とするターゲットの餌
	CSignalUi* m_pSignal;				// シグナル表示用

	D3DXVECTOR3 m_DestPos;				// 目的の座標
	D3DXVECTOR3 m_SavePos;				// 保存用座標

	int m_nScaleNum;					// 必要な仲間数を返す
	int m_nIdxNumber;					// インデックス番号
	int m_nImpactCounter;				// 生成のカウント
	bool m_isMove;						// 移動中かフラグ
	bool m_isWork;						// 動作中かフラグ
	bool m_isCreate;					// 生成したかどうか
	bool m_isSetNum;					// セットしたかどうか
	MOVE m_MoveState;					// 移動状態
};