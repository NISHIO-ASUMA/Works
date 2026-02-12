//=========================================================
//
// プレイヤー処理 [ player.h ]
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
#include "nomovecharactor.h"
#include <memory>
#include <array>

//*********************************************************
// 前方宣言
//*********************************************************
class CInputKeyboard;
class CJoyPad;
class CShadowS;
class CStateMachine;
class CPlayerStateBase;
class CMotion;
class CMeshCylinder;
class CPointObj;

//*********************************************************
// プレイヤークラスを定義
//*********************************************************
class CPlayer : public CNoMoveCharactor
{
public:

	//***********************************
	// プレイヤーが行うモーション列挙型
	//***********************************
	enum MOTION
	{
		MOTION_NEUTRAL,		// ニュートラル
		MOTION_MOVE,		// 移動
		MOTION_MAX
	};

	CPlayer(int nPriority = static_cast<int>(CObject::PRIORITY::CHARACTOR));
	~CPlayer();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void OrderToArray(const D3DXVECTOR3& destpos);
	void SetSendArrayMoving(int nIdx,int nNum);

	inline int GetSelectIndex() const { return m_nSelectSpawn; }
	inline int GetSendNum(void) const { return m_nSendNum; }

	CStateMachine* GetStateMachine() { return m_pStateMachine.get(); }

	static CPlayer* Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);

private:

	//***********************************
	// 定数構造体
	//***********************************
	struct Config
	{
		static constexpr int NUM_SPAWN = 3; // スポーンポイント数
		static constexpr int VALUE_ANT = 5;	// キー入力の増加数
		static constexpr int MAX_VALUE = 50;// 最大の増加数
		static constexpr float POINT_VALUE = 120.0f; // ポイントオブジェクトの高さ
	};

	std::unique_ptr<CStateMachine>m_pStateMachine;	// ステート基底クラスのポインタ
	CMotion* m_pMotion;				// モーションポインタ
	CPointObj* m_pPointObj;			// ポイントXファイルオブジェクト

	int m_nSendNum;					// 送る数を決める
	int m_nSelectSpawn;				// スポーン選択変数
	int m_nPrevSelectSpawn;			// 前回のスポーン選択変数
	std::array<int, Config::NUM_SPAWN>m_pSpawnData; // スポーンデータ配列
};