//=========================================================
//
// チュートリアルUI処理 [ tutorialui.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once 

//*********************************************************
// インクルードファイル宣言
//*********************************************************
#include "object2D.h"

//*********************************************************
// チュートリアルuiクラスを定義
//*********************************************************
class CTutorialUi : public CObject2D
{
public:

	CTutorialUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CTutorialUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="fWidth">横幅</param>
	/// <param name="fHeight">高さ</param>
	/// <param name="pFileName">テクスチャファイル名</param>
	/// <returns></returns>
	static CTutorialUi* Create(const D3DXVECTOR3& pos, const float& fWidth, const float& fHeight,const char * pFileName);

private:
};