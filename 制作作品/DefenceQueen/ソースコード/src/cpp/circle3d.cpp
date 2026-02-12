//=========================================================
//
// 円形3Dオブジェクト処理 [ circle3d.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "circle3d.h"
#include "spherecollider.h"
#include "collisionsphere.h"

//=========================================================
// コンストラクタ
//=========================================================
CCircle3D::CCircle3D(int nPriority) : CObject3D(nPriority),
m_pSphere(nullptr),
m_fHitRange(NULL)
{

}
//=========================================================
// デストラクタ
//=========================================================
CCircle3D::~CCircle3D()
{

}
//=========================================================
// 生成処理
//=========================================================
CCircle3D* CCircle3D::Create
(
	const D3DXVECTOR3& pos,
	const D3DXVECTOR3& rot,
	const float fWidth,
	const float fHeight,
	const float fRadius
)
{
	// インスタンス生成
	CCircle3D* pCircle = new CCircle3D;
	if (pCircle == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pCircle->Init())) return nullptr;

	// オブジェクト設定
	pCircle->SetPos(pos);
	pCircle->SetRot(rot);
	pCircle->SetCol(COLOR_GREEN);
	pCircle->SetSize(fWidth, fHeight);
	pCircle->SetTexture(Config::TEXNAME);
	pCircle->SetHitRange(fRadius);

	return pCircle;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CCircle3D::Init(void)
{
	// 親クラスの初期化処理
	CObject3D::Init();

	// 球形コライダー生成
	m_pSphere = CSphereCollider::Create(GetPos(), Config::HIT_RANGE);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CCircle3D::Uninit(void)
{
	// 球形コライダーの破棄
	m_pSphere.reset();

	// 親クラスの終了処理
	CObject3D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CCircle3D::Update(void)
{
	// 座標取得
	auto pos = GetPos();

	// 球形コライダーの座標更新
	m_pSphere->SetPos(pos);

	// 親クラスの更新処理
	CObject3D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CCircle3D::Draw(void)
{
	// 親クラスの描画処理
	CObject3D::Draw();
}
//=========================================================
// 球形同士の当たり判定処理
//=========================================================
bool CCircle3D::Collision(CSphereCollider* other)
{
	//nullなら
	if (m_pSphere == nullptr) return false;

	return CCollisionSphere::Collision(m_pSphere.get(),other);
}