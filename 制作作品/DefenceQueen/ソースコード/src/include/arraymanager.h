//=========================================================
//
// 仲間クラス全体の管理処理 [ arraymanager.h ]
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
#include <vector>

//*********************************************************
// 前方宣言
//*********************************************************
class CArray;
class CTopAnt;

//*********************************************************
// 仲間全体を管理するクラスを定義
//*********************************************************
class CArrayManager
{
public:

	CArrayManager();
	~CArrayManager();

	HRESULT Init(CTopAnt * pTop);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetActiveAll(void);
	
	/// <summary>
	/// アクティブ数を返す
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetArraysActive(void) const { return m_nActiveAll; }

	/// <summary>
	/// 配列の要素数を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	inline int GetAll(void) const { return static_cast<int>(m_pArrays.size()); }

	/// <summary>
	/// 配列の要素番号から取得
	/// </summary>
	/// <param name="nIdx">取得したい番号</param>
	/// <returns></returns>
	inline CArray* GetArrays(const int nIdx) { return m_pArrays[nIdx]; }

	/// <summary>
	/// 動的配列全体を取得する
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	const std::vector<CArray*>& GetArray(void) const { return m_pArrays; }

	/// <summary>
	/// スポナー用に確保する関数
	/// </summary>
	/// <param name="nStock">未使用数</param>
	/// <returns></returns>
	std::vector<CArray*> Allocate(const int& nStock, const int& nSatrtIdx);

	/// <summary>
	/// 切り離し可能状態を通知する
	/// </summary>
	/// <param name="center"></param>
	/// <param name="radius"></param>
	void MessageSepalation(const D3DXVECTOR3& CenterPos, float fRadius);

	/// <summary>
	/// 仲間のアリの切り離しを実行する
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="radius">半径</param>
	void ApplySeparation(const D3DXVECTOR3& CenterPos, float fRadius);

	/// <summary>
	/// エリア内に配置する関数
	/// </summary>
	/// <param name="putpos">配置先の座標</param>
	void PuttingArea(const D3DXVECTOR3& putpos);

	/// <summary>
	/// アクティブな物をカウントする
	/// </summary>
	/// <param name="nCountArrays">カウントする数</param>
	void CountActiveArrays(const int& nCountArrays);

	int GetIsFollowtopArrays(void);

	inline int GetAllUseArrays(void) const { return ArrayConfig::ALLARRAYS; }

private:

	//**********************
	// 定数構造体
	//**********************
	struct ArrayConfig
	{
		static constexpr int LIFE = 1; // 体力値
		static constexpr int ALLARRAYS = 200; // 最大使用数
	};

	std::vector<CArray*>m_pArrays;		// 動的配列
	int m_nActiveAll;					// 現在の出現数
};