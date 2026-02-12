//=========================================================
//
// イベントエリア表示処理 [ eventarea.h ]
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
#include "object3D.h"
#include <memory>

//*********************************************************
// 前方宣言
//*********************************************************
class CBoxCollider;
class CSphereCollider;

//*********************************************************
// 一個のイベントエリアを表示するクラスを定義
//*********************************************************
class CEventArea : public CObject3D
{
public:

	CEventArea(int nPriority = static_cast<int>(CObject::PRIORITY::BASENUMBER));
	~CEventArea();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	bool Collision(CSphereCollider* pOther);

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <returns></returns>
	static CEventArea* Create(const D3DXVECTOR3& pos);

private:

	//****************************
	// 定数構造体
	//****************************
	struct Config
	{
		static constexpr float SIZE = 80.0f;	// コリジョンサイズ
		static constexpr float WIDTH = 100.0f;	// 横幅
		static constexpr float HEIGHT = 3.0f;	// 高さ
		static constexpr const char* TEXNAME = "Circle.png"; // テクスチャ名
	};

	std::unique_ptr<CBoxCollider>m_pBoxCollider;	// 矩形のコライダー
	bool m_isDraw;									// 描画するかどうか
};