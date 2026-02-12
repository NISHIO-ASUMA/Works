//=========================================================
//
// パーティクルウィンドウ処理 [ particlepiler.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "particlepiler.h"
#include "manager.h"
#include "effect.h"

//=========================================================
// コンストラクタ
//=========================================================
CParticlePiler::CParticlePiler(int nPriority) : CObject(nPriority),
m_pos(VECTOR3_NULL),
m_nLife(NULL),
m_col(COLOR_WHITE),
m_nLength(NULL),
m_nMaxParticle(NULL),
m_nRadius(NULL),
m_fAngle(NULL),
m_pEffect{}
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CParticlePiler::~CParticlePiler()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CParticlePiler* CParticlePiler::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nMaxParticle, int nRadius, int nLength, int nLife,float fAngle)
{
	// パーティクルのポインタを宣言
	CParticlePiler* pParticle = new CParticlePiler;
	if (pParticle == nullptr) return nullptr;

	// 初期化失敗時
	if (FAILED(pParticle->Init())) return nullptr;

	// オブジェクト設定
	pParticle->m_pos = pos;
	pParticle->m_col = col;
	pParticle->m_nMaxParticle = nMaxParticle;
	pParticle->m_nLength = nLength;
	pParticle->m_nLife = nLife;
	pParticle->m_nRadius = nRadius;
	pParticle->m_fAngle = fAngle;

	// ポインタを返す
	return pParticle;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CParticlePiler::Init(void)
{
	// 配列クリア
	m_pEffect.clear();

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CParticlePiler::Uninit(void)
{
	// 配列クリア
	m_pEffect.clear();

	// 親クラスの破棄
	CObject::Release();
}
//=========================================================
// 更新処理
//=========================================================
void CParticlePiler::Update(void)
{
	// 変数格納
	D3DXCOLOR col = m_col;
	D3DXVECTOR3 pos = m_pos;

	// 扇形の角度
	float fHalfSpread = D3DXToRadian(30.0f);

	for (int nCnt = 0; nCnt < m_nMaxParticle; nCnt++)
	{
		// 水平角
		float fYaw = m_fAngle + ((float)rand() / RAND_MAX * 2.0f - 1.0f) * fHalfSpread;

		// 縦角
		float fPitch = ((float)rand() / RAND_MAX) * fHalfSpread;

		// 速度
		float fSpeed = ((float)(rand() % m_nRadius) / 10.0f + 0.5f);

		// 移動方向
		D3DXVECTOR3 Move = VECTOR3_NULL;
		Move.x = cosf(fPitch) * sinf(fYaw) * fSpeed;
		Move.z = cosf(fPitch) * cosf(fYaw) * fSpeed;
		Move.y = sinf(fPitch) * fSpeed;

		// 寿命
		int nLife = (rand() % m_nLife) + 10;

		// エフェクト生成
		CEffect* pEffect = CEffect::Create(pos,col,Move,nLife,fSpeed);

		if (pEffect)
		{
			m_pEffect.push_back(pEffect);
		}
		else
		{
			break;
		}
	}

	// 破棄
	Uninit();
}