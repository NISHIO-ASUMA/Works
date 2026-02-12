//=========================================================
//
// 切り離し可能を合図するUI処理 [ sepalationsign.h ]
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
#include "billboard.h"

//*********************************************************
// 切り離し可能を合図するUI表示クラスを定義
//*********************************************************
class CSepalationSign : public CBillboard
{
public:

	CSepalationSign(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CSepalationSign();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline void SetIsDraw(const bool& isDraw) { m_isDraw = isDraw; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="pTexName">テクスチャ名</param>
	/// <returns></returns>
	static CSepalationSign* Create(const D3DXVECTOR3& pos, const char* pTexName);

private:

	//******************************
	// 定数構造体
	//******************************
	struct Config
	{
		static constexpr float SIZE = 70.0f; // 描画のサイズ
	};

	bool m_isDraw;		// 描画フラグ
};