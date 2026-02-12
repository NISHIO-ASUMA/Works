//=========================================================
//
// 負けリザルト出現するオブジェクト管理処理 [ loseresultobject.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// 負けリザルトオブジェクト管理クラスを定義
//*********************************************************
class CLoseResultObject
{
public:

	~CLoseResultObject();

	HRESULT Init(void);
	void Uninit(void);

	static CLoseResultObject* GetInstance(void);

private:

	CLoseResultObject();					// コンストラクタ

	static CLoseResultObject* m_pInstance;	// シングルトン変数
};