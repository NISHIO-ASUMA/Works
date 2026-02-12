//=========================================================
//
// イベントエリア表示処理 [ eventarea.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "eventarea.h"
#include "boxcollider.h"
#include "boxtospherecollision.h"

//=========================================================
// コンストラクタ
//=========================================================
CEventArea::CEventArea(int nPriority) : CObject3D(nPriority),
m_isDraw(false),
m_pBoxCollider(nullptr)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEventArea::~CEventArea()
{

}
//=========================================================
// 生成関数
//=========================================================
CEventArea* CEventArea::Create(const D3DXVECTOR3& pos)
{
	// インスタンス生成
	CEventArea* pArea = new CEventArea;
	if (pArea == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pArea->Init())) return nullptr;

	// オブジェクト設定
	pArea->SetPos(pos);
	pArea->SetCol(COLOR_RED);
	pArea->SetSize(Config::WIDTH, Config::HEIGHT);
	pArea->SetTexture(Config::TEXNAME);

	// ポインタを返す
	return pArea;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEventArea::Init(void)
{
	// 親クラスの初期化処理
	CObject3D::Init();

	// 矩形コライダー生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetPos(), D3DXVECTOR3(Config::SIZE, Config::SIZE, Config::SIZE));

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEventArea::Uninit(void)
{
	// コライダーポインタの破棄
	m_pBoxCollider.reset();

	// 親クラスの終了処理
	CObject3D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEventArea::Update(void)
{
	// 座標を取得
	auto pos = GetPos();

	// コライダー更新
	m_pBoxCollider->SetPos(pos);

	// 親クラスの更新処理
	CObject3D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CEventArea::Draw(void)
{
	// 親クラスの描画処理
	CObject3D::Draw();
}
//=========================================================
// 球コライダーと矩形コライダーの当たり判定
//=========================================================
bool CEventArea::Collision(CSphereCollider* pOther)
{
	// nullチェック
	if (m_pBoxCollider == nullptr) return false;

	return CBoxToSphereCollision::Collision(m_pBoxCollider.get(),pOther);
}