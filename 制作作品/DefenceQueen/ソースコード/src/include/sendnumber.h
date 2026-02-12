//=========================================================
//
// 送る数の数字処理 [ sendnumber.h ]
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
#include "number.h"
#include "object.h"
#include <array>

//*********************************************************
// 送る数を表示するナンバークラスを定義
//*********************************************************
class CSendNumber : public CObject
{
public:

	CSendNumber(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CSendNumber();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetDestScore(int nDestScore) { m_nScore = nDestScore; }

	inline D3DXVECTOR3 GetPos(void) const { return m_pos; }
	inline int GetScore(void) const { return m_nScore; }

	static CSendNumber* Create(D3DXVECTOR3 pos, float fWidth, float fHeight);

private:

	static constexpr int NUM_SCORE = 2;		// 桁数
	static constexpr int NUM_DIGIT = 10;	// 表示する分割桁数の値

	D3DXCOLOR m_col;		// カラー
	D3DXVECTOR3 m_pos;		// 座標
	D3DXVECTOR3 m_rot;		// 角度

	float m_fWidth;			// 横幅
	float m_fHeight;		// 高さ
	int m_nScore;			// スコア保持用

	std::array<CNumber*, NUM_SCORE>m_apNumber;	// 桁数分のナンバーのポインタ
};