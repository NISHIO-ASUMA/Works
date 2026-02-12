//=========================================================
//
// 餌獲得のシグナルを表すUI表示クラス処理 [ feedsignal.h ]
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
// 餌獲得可能シグナルを表すUIのクラスを定義
//*********************************************************
class CFeedSignal : public CBillboard
{
public:

	CFeedSignal(int nPriority = static_cast<int>(CObject::PRIORITY::BILLBOARD));
	~CFeedSignal();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	inline void SetIsDraw(const bool isDraw) { m_isDraw = isDraw; }
	inline bool GetIsDraw(void) const { return m_isDraw; }

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="color">カラー</param>
	/// <returns></returns>
	static CFeedSignal* Create
	(
		const D3DXVECTOR3& pos, 
		const D3DXVECTOR3& rot,
		const float fWidth, 
		const float fHeight, 
		const D3DXCOLOR& color = COLOR_WHITE
	);

private:

	//**********************************
	// 定数構造体宣言
	//**********************************
	struct Config
	{
		static constexpr const char* TEX_NAME = "FeedSignal.png"; // テクスチャ名
	};

	bool m_isDraw;		// 描画するかどうか
};