//=========================================================
//
// モーション処理 [ motion.h ]
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
#include "model.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CMotionManager;

//*********************************************************
// モーションクラスを定義
//*********************************************************
class CMotion
{
public:

	CMotion();
	~CMotion();

	void Update(std::vector<CModel*> pModel);
	void UpdateCurrentMotion(CMotionManager * pMption,CModel** ppModel, int nModelCount);
	void UpdateBlend(CMotionManager* pMption,CModel** ppModel, int nModelCount);
	void Debug(void);
	bool CheckFrame(int nStartMotion, int nEndMotion, int nMotionType);

	void SetMotion(int motiontype);
	void SetMotion(int nMotionType, bool isBlend, int nBlendFrame);
	inline void SetResetFrame(int nFrame) { m_nCounterMotion = nFrame;}

	inline int GetMotionType(void) { return m_motiontype; }
	inline bool GetFinishMotion(void) { return m_isFinishMotion; }

	/// <summary>
	/// インデックス登録処理
	/// </summary>
	/// <param name="pMotionName">モーションスクリプト名</param>
	/// <param name="pModel">モデルクラスの動的配列</param>
	/// <param name="nDestMotions">モーション数</param>
	/// <param name="isShadow">マトリックスシャドウを使うかどうか</param>
	void RegisterPath(const char* pMotionName,std::vector<CModel*>& pModel, int nDestMotions, const bool isShadow);

	/// <summary>
	/// モーション読み込み関数
	/// </summary>
	/// <param name="pFilename">モーションスクリプト名</param>
	/// <param name="pModel">モデルクラスの動的配列</param>
	/// <param name="nDestMotions">モーション数</param>
	/// <param name="isShadow">マトリックスシャドウを使うかどうか</param>
	/// <returns></returns>
	static std::unique_ptr<CMotion>Load(const char* pFilename, std::vector<CModel*>& pModel, int nDestMotions, const bool isShadow);

private:

	int m_nNumModels;		// モデル総数
	int m_nNumKey;			// キーの総数
	int m_nKey;				// 現在のキーNo
	int m_nCounterMotion;	// モーションのカウンター
	int m_nNextKey;			// 次のキー
	int m_motiontype;		// モーションタイプ
	int m_type;
	int m_nMotionIdx;		// モーションファイル番号

	int m_motiontypeBlend;	// ブレンドの種類
	int m_nFrameBlend;		// ブレンドのフレーム数
	int m_nCounterBlend;	// ブレンドカウンター
	int m_nKeyBlend;		// ブレンドモーションの現在のキー
	int m_nNextKeyBlend;	// ブレンドモーションの次のキー
	int m_nNumAllFrame;		// キーごとのフレーム数
	int m_nAllFrameCount;	// 全体フレーム数

	bool m_isBlendMotion;	// ブレンドがあるかどうか
	bool m_isFinishMotion;	// モーションが終わったかどうか
	bool m_isFirstMotion;	// モーションが始まったフラグ
	bool m_isLoopMotion;	// ループするかどうか

	static constexpr int NEUTRAL = 0; // ニュートラル番号

};