//=========================================================
//
// タイトルで出現するオブジェクト管理処理 [ titleobject.h ]
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
class CTitleWallManager;
class CTitleAntManager;

//*********************************************************
// タイトルオブジェクト管理クラスを定義
//*********************************************************
class CTitleObject
{
public:

	~CTitleObject();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	
	static CTitleObject* GetInstance(void);

	inline CTitleWallManager* GetWallManager(void) const { return m_pWallManager.get(); }
	
private:

	CTitleObject();					  // コンストラクタ

	static CTitleObject* m_pInstance; // シングルトン変数

	std::unique_ptr<CTitleWallManager>m_pWallManager; // 壁管理クラスポインタ
	std::unique_ptr<CTitleAntManager>m_pTitleAntManager; // タイトルのアリを管理する

};