//=========================================================
//
// 仲間のスポナー管理処理 [ arrayspawnmanager.h ]
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
#include <array>

//*********************************************************
// 前方宣言
//*********************************************************
class CArraySpawner;
class CArrayManager;

//*********************************************************
// 名前空間
//*********************************************************
namespace SpawnManager
{
	static constexpr int NUM_CREATEARRAY = 50; // 一個のスポナーに渡すアリの数
	static constexpr int NUM_ARRAYSPAWNER = 4; // スポナーの数

	// 生成座標情報
	const D3DXVECTOR3 POS[NUM_ARRAYSPAWNER] =
	{
		{-555.0f,0.0f,-700.0f},	// 左下手前
		{885.0f,0.0f,-450.0f},	// 右下手前
		{665.0f,0.0f,540.0f},	// 右奥
		{-890.0f,0.0f,325.0f}	// 左奥
	};

	// 生成角度情報
	const D3DXVECTOR3 ROTVALUE[NUM_ARRAYSPAWNER] =
	{
		{0.0f,-D3DX_PI * 0.5f,0.0f},// 左下手前
		{0.0f,-D3DX_PI,0.0f},		// 右下手前
		{0.0f,D3DX_PI * 0.5f,0.0f},	// 右奥
		{VECTOR3_NULL}				// 左奥
	};

	// ターゲット座標
	const D3DXVECTOR3 BasePos[NUM_ARRAYSPAWNER] =
	{
		{ 885.0f,0.0f,-680.0f}, // 右に向かう
		{ 885.0f,0.0f, 545.0f}, // 奥側に向かう
		{-885.0f,0.0f, 545.0f}, //左に向かう
		{ -885.0f,0.0f, -700.0f}
	};

};

//*********************************************************
// スポーン配列の管理クラスを定義
//*********************************************************
class CArraySpawnManager
{
public:

	CArraySpawnManager();
	~CArraySpawnManager();

	HRESULT Init(CArrayManager * pManager);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	/// <summary>
	/// 自身で選択したスポナーの取得
	/// </summary>
	/// <param name="nIdx">取得したいスポナー番号</param>
	/// <returns></returns>
	inline CArraySpawner* GetIndexSpawner(const int& nIdx) { return m_pArraySpawner[nIdx]; }

private:

	std::array<CArraySpawner*, SpawnManager::NUM_ARRAYSPAWNER>m_pArraySpawner; // 仲間アリのスポナー配列
};