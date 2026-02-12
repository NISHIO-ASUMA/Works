//=========================================================
//
// 動かないキャラクタークラス処理 [ nomovecharactor.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "nomovecharactor.h"
#include "motion.h"
#include "shadowstencil.h"
#include "manager.h"
#include "blackboard.h"
#include "outline.h"

//=========================================================
// コンストラクタ
//=========================================================
CNoMoveCharactor::CNoMoveCharactor(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_posOld(VECTOR3_NULL),
m_rot(VECTOR3_NULL),
m_rotDest(VECTOR3_NULL),
m_scale(INITSCALE),
m_pModel{},
m_pMotion{},
m_pShadowS{},
m_isStencilUse(false),
m_isOutLine(false)
{
	// 値のクリア
	D3DXMatrixIdentity(&m_mtxworld);
}
//=========================================================
// デストラクタ
//=========================================================
CNoMoveCharactor::~CNoMoveCharactor()
{

}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CNoMoveCharactor::Init(void)
{
	// 変数の初期化
	m_pModel.clear();

	// 使用可能時
	if (m_isStencilUse)
		m_pShadowS = CShadowStencil::Create(m_pos, m_rot);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CNoMoveCharactor::Uninit(void)
{
	// 動的確保分のモデルの破棄
	for (auto iter = m_pModel.begin(); iter != m_pModel.end(); iter++)
	{
		// nullチェック
		if ((*iter) != nullptr)
		{
			// 終了処理
			(*iter)->Uninit();

			// ポインタの破棄
			delete (*iter);

			// null初期化
			(*iter) = nullptr;
		}
	}

	// 配列のクリア
	m_pModel.clear();

	// ポインタ破棄
	m_pMotion.reset();
	
	// 自身の破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CNoMoveCharactor::Update(void)
{
	// ステンシルシャドウ更新
	if (m_pShadowS)
	{
		// オブジェクト設定
		m_pShadowS->SetPos(m_pos);
		m_pShadowS->SetRot(m_rot);
	}

	// 計算用のマトリックスを宣言
	D3DXMATRIX mtxRot, mtxTrans, mtxScal;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxworld);

	// 大きさを反映
	D3DXMatrixScaling(&mtxScal, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxScal);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxworld, &m_mtxworld, &mtxTrans);

#ifdef NDEBUG
	// モーションの更新処理
	m_pMotion->Update(m_pModel);
#endif // _NDEBUG
}
//=========================================================
// 描画処理
//=========================================================
void CNoMoveCharactor::Draw(void)
{
	// デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// モデルの描画
	for (auto& model : m_pModel)
	{
		model->Draw();
	}

	// falseなら
	if (!m_isOutLine) return;

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxworld);

	// カリング切る
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	// 開始
	COutLine::GetInstance()->Begin();
	COutLine::GetInstance()->BeginPass();

	for (auto& model : m_pModel)
	{
		model->DrawOutLine(D3DXVECTOR4(1.0f, 0.65f, 0.2f, 1.0f));
	}

	// 終了
	COutLine::GetInstance()->EndPass();
	COutLine::GetInstance()->End();

	// カリングを戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=========================================================
// モーション読み込み
//=========================================================
void CNoMoveCharactor::MotionLoad(const char* pScriptName, int nDestMotions,const bool isShadow)
{
	// ポインタを取得する
	m_pMotion = CMotion::Load(pScriptName, m_pModel, nDestMotions, isShadow);
}