//=========================================================
//
// 仲間アリの分割可能UI表示処理 [ extractantsignal.h ]
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
// 分割可能UIを表示するクラスを定義
//*********************************************************
class CExtractAntSignal : public CBillboard
{
public:

	CExtractAntSignal(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CExtractAntSignal();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline void SetisDraw(const bool& isDrawFlag) { m_isDraw = isDrawFlag; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <returns></returns>
	static CExtractAntSignal* Create(const D3DXVECTOR3& pos);

private:

	//*****************************
	// 定数構造体宣言
	//*****************************
	struct Config
	{
		static constexpr float WIDTH = 60.0f;	// 横幅
		static constexpr float HEIGHT = 40.0f;	// 高さ
		static constexpr const char* TEXNAME = "Signal.png"; // テクスチャ名
	};

	bool m_isDraw;	// 描画するかどうか
};