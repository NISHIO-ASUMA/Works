//=========================================================
//
// 出現する餌一個に関する処理 [ feed.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "feed.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "parameter.h"
#include "gamemanager.h"
#include "gamesceneobject.h"
#include "manager.h"
#include "xfilemanager.h"
#include "feedmanager.h"
#include "worker.h"
#include "boxcollider.h"
#include "collisionbox.h"
#include "easing.h"
#include "particle.h"
#include "particlepiler.h"
#include "sound.h"
#include "eventareamanager.h"

//*********************************************************
// 定数宣言
//*********************************************************
namespace FEEDINFO
{
	constexpr int LIFE = 25;			// 基底体力値
	constexpr int COL_COUNT_MAX = 30;	// 点滅させる総フレーム
	constexpr float BLINK_SPEED = 1.2f; // sinの速度
};

//=========================================================
// コンストラクタ
//=========================================================
CFeed::CFeed(int nPriority) : CObjectX(nPriority),
m_pSphere(nullptr),
m_pBoxCollider(nullptr),
m_pOwnerArea(nullptr),
m_pParam(nullptr),
m_fRadius(NULL),
m_ColorFrameCnt(NULL),
m_nLife(NULL),
m_isAssing(false),
m_ColType(COLTYPE_NONE)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CFeed::~CFeed()
{

}
//=========================================================
// 生成処理
//=========================================================
CFeed* CFeed::Create
(
	const D3DXVECTOR3& pos, 
	const D3DXVECTOR3& rot,
	const D3DXVECTOR3& scale, 
	const char* pModelName, 
	const float fRadius, 
	const int nLife
)
{
	// インスタンス生成
	CFeed* pFeed = new CFeed;
	if (pFeed == nullptr) return nullptr;

	// オブジェクト設定
	pFeed->SetPos(pos);
	pFeed->SetRot(rot);
	pFeed->SetScale(scale);
	pFeed->SetFilePass(pModelName);
	pFeed->SetRadius(fRadius);
	pFeed->SetLife(nLife);

	// 初期化失敗時
	if (FAILED(pFeed->Init())) return nullptr;

	return pFeed;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CFeed::Init(void)
{
	// 親クラスの初期化
	CObjectX::Init();

	// Xファイルオブジェクト取得
	CXfileManager* pXManager = CManager::GetInstance()->GetXManager();
	if (pXManager == nullptr) return E_FAIL;

	// インデックス番号を取得
	int nModelIdx = GetModelIdx();
	D3DXVECTOR3 Size = pXManager->GetInfo(nModelIdx).Size;

	// 球形コライダー生成
	m_pSphere = std::make_unique<CSphereCollider>();
	m_pSphere->SetPos(GetPos());
	m_pSphere->SetRadius(m_fRadius);

	// 矩形コライダー生成
	m_pBoxCollider = CBoxCollider::Create(GetPos(), GetPos(), Size * 0.88f);

	// パラメーター設定
	m_pParam = std::make_unique<CParameter>();
	m_pParam->SetHp(m_nLife);

	// 影をoffにする
	SetShadow(false);

	// アウトラインをonにする
	SetIsOutLine(true);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CFeed::Uninit(void)
{
	// 球形コライダーの破棄
	m_pSphere.reset();

	// 矩形コライダーの破棄
	m_pBoxCollider.reset();

	// パラメーターポインタの破棄
	m_pParam.reset();

	// 親クラスの終了
	CObjectX::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CFeed::Update(void)
{
	// 座標を取得
	D3DXVECTOR3 pos = GetPos();

	// コライダー更新
	m_pSphere->SetPos(pos);

	// コライダー更新
	m_pBoxCollider->SetPos(pos);

	// カラーチェック関数
	ColorCheck();

	// 親クラスの更新
	CObjectX::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CFeed::Draw(void)
{
	// 親クラスの描画
	CObjectX::Draw();
}
//=========================================================
// パラメーター減算処理
//=========================================================
void CFeed::DecLife(const int& nDecValue)
{
	// 引数の分減少
	int nHp = m_pParam->GetHp();
	nHp -= nDecValue;

	if (nHp <= NULL)
	{
		// 体力を0にする
		m_pParam->SetHp(NULL);

		// ポインタ取得
		auto area = m_pOwnerArea;

		// 動的配列内の餌の要素を削除する
		CEventAreaManager::GetInstance()->EraseFeed(this);

		// 動的配列内の要素を削除する
		CEventAreaManager::GetInstance()->Erase(area);

		// 自身の破棄
		Uninit();

		return;
	}
	else
	{
		// 現在の体力にセット
		m_pParam->SetHp(nHp);

#ifdef NDEBUG
		//パーティクル生成
		CParticlePiler::Create(D3DXVECTOR3(GetPos().x,120.0f,GetPos().z), COLOR_PURPLE,15,150,350,5,0.0f);
#endif // NDEBUG

		// サウンド取得
		const auto& Sound = CManager::GetInstance()->GetSound();
		if (Sound != nullptr)
		{
			// ヒット音SE再生
			Sound->Play(CSound::SOUND_LABEL_DAMAGE);
		}

		// カラー状態変更
		m_ColType = COLTYPE_CHANGE;

		// フレームカウンタ初期化
		m_ColorFrameCnt = 0;

		return;
	}
}
//=========================================================
// チュートリアル用ライフ減少
//=========================================================
void CFeed::DecLifeTuto(const int& nDecValue)
{
	// 引数の分減少
	int nHp = m_pParam->GetHp();
	nHp -= nDecValue;

	if (nHp <= NULL)
	{
		// 体力を0にする
		m_pParam->SetHp(NULL);

		// 自身の破棄
		Uninit();
		return;
	}
	else
	{
		// 現在の体力にセット
		m_pParam->SetHp(nHp);

		// サウンド取得
		const auto& Sound = CManager::GetInstance()->GetSound();
		if (Sound != nullptr)
		{
			// ヒット音SE再生
			Sound->Play(CSound::SOUND_LABEL_DAMAGE);
		}

		// カラー状態変更
		m_ColType = COLTYPE_CHANGE;

		// フレームカウンタ初期化
		m_ColorFrameCnt = NULL;

		return;
	}
}
//=========================================================
// カラー状態監視関数
//=========================================================
void CFeed::ColorCheck(void)
{
	switch (m_ColType)
	{
	case CFeed::COLTYPE_NONE:

		// カラー設定
		SetCol(V_COLOR_WHITE);
		break;

	case CFeed::COLTYPE_CHANGE:
	{
		// フレーム加算
		m_ColorFrameCnt++;

		// 角度を設定
		float fAngle = m_ColorFrameCnt * FEEDINFO::BLINK_SPEED;

		// 振動係数
		float fBlink = (sinf(fAngle) + 1.0f) * 0.5f;

		// カラーを設定
		D3DCOLORVALUE col = LerpColor(V_COLOR_WHITE, V_COLOR_RED, fBlink);
		SetCol(col);

		// 一定フレームで終了
		if (m_ColorFrameCnt >= FEEDINFO::COL_COUNT_MAX)
		{
			m_ColType = COLTYPE_NONE;
			m_ColorFrameCnt = NULL;
			SetCol(V_COLOR_WHITE);
		}
		break;
	}
	default:

		// カラー設定
		SetCol(V_COLOR_WHITE);
		break;
	}
}
//=========================================================
// カラー計算関数
//=========================================================
D3DCOLORVALUE CFeed::LerpColor(const D3DCOLORVALUE& a, const D3DCOLORVALUE& b, float t)
{
	// 算出するカラー変数
	D3DCOLORVALUE outcolor;

	// 各情報に対して計算
	outcolor.r = a.r + (b.r - a.r) * t;
	outcolor.g = a.g + (b.g - a.g) * t;
	outcolor.b = a.b + (b.b - a.b) * t;
	outcolor.a = a.a + (b.a - a.a) * t;

	// 最終出力
	return outcolor;
}
//=========================================================
// 球の当たり判定処理
//=========================================================
bool CFeed::Collision(CSphereCollider* other)
{
	// nullなら
	if (m_pSphere == nullptr) return false;

	return CCollisionSphere::Collision(m_pSphere.get(),other);
}
//=========================================================
// 矩形の当たり判定処理
//=========================================================
bool CFeed::CollisionBox(CBoxCollider* pOther, D3DXVECTOR3* OutPos)
{
	// nullなら
	if (m_pBoxCollider == nullptr) return false;

	return CCollisionBox::Collision(m_pBoxCollider.get(),pOther,OutPos);
}