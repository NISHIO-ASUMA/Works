//=========================================================
// 
// ポーズ処理 [ pause.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "pause.h"
#include "manager.h"
#include "texture.h"
#include "pausemanager.h"

//=========================================================
// コンストラクタ
//=========================================================
CPause::CPause(int nPriority) : CObject2D(nPriority),
m_nIdxTexture(-1),
m_nPauseType(NULL)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CPause::~CPause()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CPause* CPause::Create(const D3DXVECTOR3& pos, float fWidth, float fHeight, const D3DXCOLOR& col,int nType)
{
	// インスタンス生成
	CPause* pPause = new CPause;
	if (pPause == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pPause->Init())) return nullptr;

	// 2Dオブジェクト設定
	pPause->SetPos(pos);
	pPause->SetSize(fWidth, fHeight);
	pPause->SetCol(col);
	pPause->SetType(nType);
	pPause->SetTexture();

	return pPause;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CPause::Init(void)
{
	// オブジェクトの種類を設定
	SetObjType(TYPE_PAUSE);

	// オブジェクトの初期化
	CObject2D::Init();

	// 初期化結果を返す
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CPause::Uninit(void)
{
	// オブジェクトの破棄
	CObject2D::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CPause::Update(void)
{
	// ポーズが有効なら
	if (!CPauseManager::GetPause()) return;

	// オブジェクトの更新処理
	CObject2D::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CPause::Draw(void)
{
	// ポーズが有効なら
	if (!CPauseManager::GetPause()) return;

	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// テクスチャ取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	if (pTexture == nullptr) return;

	// テクスチャセット
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	// オブジェクトの描画処理
	CObject2D::Draw();
}
//=========================================================
// テクスチャ処理
//=========================================================
void CPause::SetTexture(void)
{
	// テクスチャポインタをマネージャーから取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	// nullだったらここで処理終了
	if (pTexture == nullptr) return;

	// 種類によってテクスチャ割り当てを切り替える
	switch (m_nPauseType)
	{
	case MENU_BACK: // ただの背景
		m_nIdxTexture = pTexture->Register("data/TEXTURE/PauseBack.png"); 		// テクスチャ割り当て
		break;

	case MENU_RETRY: // リトライ選択時
		m_nIdxTexture = pTexture->Register("data/TEXTURE/pause_retry.png"); 		// テクスチャ割り当て
		break;

	case MENU_CONTINUE: // コンテニュー選択時
		m_nIdxTexture = pTexture->Register("data/TEXTURE/pause_continue.png"); 	// テクスチャ割り当て
		break;

	case MENU_QUIT: // クイット選択時
		m_nIdxTexture = pTexture->Register("data/TEXTURE/pause_quit.png"); 		// テクスチャ割り当て
		break;

	default:
		m_nIdxTexture = -1;
		break;
	}
}