//=========================================================
//
// 開始シーンの管理処理 [ startscenemanager.h ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// 最初のゲームシーン管理処理
//*********************************************************
class CStartSceneManager
{
public:

	~CStartSceneManager();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	static CStartSceneManager* GetInstance(void);

private:

	//******************************
	// 定数格納構造体
	//******************************
	struct Config
	{
		static constexpr int MAX_CHANGECOUNT = 170; // 画面を切り替えるカウント
	};

	CStartSceneManager();	// 格納コンストラクタ
	
	int m_nChangeCount;		// 切り替わるまでのカウント変数
};