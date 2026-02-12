//=========================================================
//
// 切り離し可能を合図するUI処理 [ sepalationsign.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "sepalationsign.h"

//=========================================================
// コンストラクタ
//=========================================================
CSepalationSign::CSepalationSign(int nPriority) : CBillboard(nPriority),
m_isDraw(false)
{

}
//=========================================================
// デストラクタ
//=========================================================
CSepalationSign::~CSepalationSign()
{

}
//=========================================================
// 生成処理
//=========================================================
CSepalationSign* CSepalationSign::Create(const D3DXVECTOR3& pos,const char * pTexName)
{
	// インスタンス生成
	CSepalationSign* pSign = new CSepalationSign;
	if (pSign == nullptr) return nullptr;

	// オブジェクト設定
	pSign->SetPos(pos);
	pSign->SetRot(VECTOR3_NULL);
	pSign->SetSize(Config::SIZE, Config::SIZE);
	pSign->SetTexture(pTexName);

	// 初期化失敗時
	if (FAILED(pSign->Init())) return nullptr;

	return pSign;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CSepalationSign::Init(void)
{
	// 親クラスの初期化
	CBillboard::Init();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CSepalationSign::Uninit(void)
{
	// 親クラスの終了
	CBillboard::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CSepalationSign::Update(void)
{
	// 描画フラグがoffなら
	if (!m_isDraw) return;

	// 親クラスの更新
	CBillboard::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CSepalationSign::Draw(void)
{
	// 描画フラグがoffなら
	if (!m_isDraw) return;

	// 親クラスの描画
	CBillboard::Draw();
}