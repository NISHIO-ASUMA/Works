//=========================================================
//
// リザルトのオブジェクト管理 [ resultobject.h ]
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
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CResultScoreManager;
class CBlockManager;

//*********************************************************
// リザルトで使うオブジェクトを管理するクラスを定義
//*********************************************************
class CResultObject
{
public:

	~CResultObject();

	HRESULT Init(void);
	void Uninit(void);

	static CResultObject* GetInstance(void);

private:

	CResultObject();					// コンストラクタ

	static CResultObject* m_pInstance;	// シングルトン変数

	std::unique_ptr<CResultScoreManager>m_pResultScoreManager; // スコア管理クラス
	std::unique_ptr<CBlockManager>m_pBlock;		// ステージマップ
};