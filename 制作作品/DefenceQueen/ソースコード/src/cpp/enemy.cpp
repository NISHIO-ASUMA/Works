//=========================================================
//
// 敵の処理 [ enemy.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemy.h"
#include "parameter.h"
#include "motion.h"
#include "shadowstencil.h"
#include "manager.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "blackboard.h"
#include "node.h"
#include "behaviortree.h"
#include "gamesceneobject.h"
#include "enemybehaviortree.h"
#include "queen.h"
#include "template.h"
#include "input.h"
#include "feedmanager.h"
#include "feed.h"
#include "enemymanager.h"
#include "gamewallmanager.h"
#include "boxtospherecollision.h"
#include "gamewallmodel.h"
#include "motioninstancing.h"
#include "eventareamanager.h"

//*********************************************************
// 定数宣言空間
//*********************************************************
namespace EnemyInfo
{
	constexpr float HitRange = 60.0f; // 球形範囲
	constexpr float StopRange = 10.0f;	// 停止範囲
	constexpr float MoveSpeed = 1.0f;	// 移動速度
	const char* MOTION_NAME = "data/MOTION/Enemy/Enemy_Motion.txt"; // モーションパス
};

//=========================================================
// コンストラクタ
//=========================================================
CEnemy::CEnemy(int nPriority) : CInstancingCharactor(nPriority),
m_pMotion(nullptr),
m_pParameter(nullptr),
m_pSphereCollider(nullptr),
m_pTargetFeed(nullptr),
m_isActive(false),
m_isDestQueen(false),
m_isDestFeed(false)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CEnemy::~CEnemy()
{
	// 無し
}
//=========================================================
// 生成処理
//=========================================================
CEnemy* CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nLife)
{
	// インスタンス生成
	CEnemy* pEnemy = new CEnemy;
	if (pEnemy == nullptr) return nullptr;

	// オブジェクト設定
	pEnemy->SetPos(pos);
	pEnemy->SetRot(rot);
	pEnemy->SetUseStencil(false);

	// ポインタ生成
	pEnemy->m_pParameter = std::make_unique<CParameter>();

	// nullチェック
	if (pEnemy->m_pParameter != nullptr)
	{
		pEnemy->m_pParameter->SetMaxHp(nLife);
		pEnemy->m_pParameter->SetHp(nLife);
	}

	// 初期化失敗時
	if (FAILED(pEnemy->Init())) return nullptr;

	return pEnemy;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemy::Init(void)
{
	// キャラクタ―の初期化
	CInstancingCharactor::Init();

	// オブジェクトの種類をセット
	SetObjType(CObject::TYPE_ENEMY);

	// モーションセット
	MotionLoad(EnemyInfo::MOTION_NAME, MOTION_MAX,true);

	// コライダー生成
	m_pSphereCollider = std::make_unique<CSphereCollider>();
	m_pSphereCollider->SetPos(GetPos());
	m_pSphereCollider->SetRadius(EnemyInfo::HitRange);

	// ランダム数
	int nRand = rand() % 100;

	if (nRand < 80)
	{
		// 80%
		m_isDestFeed = true;   // 1 に相当
	}
	else
	{
		// 20%
		m_isDestQueen = true; // 0 に相当
	}

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemy::Uninit(void)
{
	// パラメータ終了処理
	m_pParameter.reset();

	// コライダー破棄
	m_pSphereCollider.reset();

	// キャラクターの破棄
	CInstancingCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CEnemy::Update(void)
{
	// 未使用なら
	if (!m_isActive) return;

	// 座標取得
	D3DXVECTOR3 pos = GetPos();

	// 女王移動
	if (m_isDestQueen) MoveToQueen();

	// 餌移動
	if (m_isDestFeed) MoveToFeed();

	// 座標のみの更新
	CInstancingCharactor::UpdatePosition();

	// 更新後の座標
	D3DXVECTOR3 UpdatePos = GetPos();

	if (m_isDestFeed)
	{
		// 餌との当たり判定関数
		CollisionFeed();
	}

	if (m_isDestQueen)
	{
		// 女王との当たり判定関数
		CollisionQueen();
	}

	// 世界の壁に当たる
	CollisionWall();

	// 球コライダー更新
	if (m_pSphereCollider) m_pSphereCollider->SetPos(UpdatePos);

	// キャラクターの更新処理
	CInstancingCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CEnemy::Draw(void)
{
	// 未使用なら
	if (!m_isActive) return;

	// キャラクターの描画処理
	CInstancingCharactor::Draw();
}
//=========================================================
// 餌に向かう処理
//=========================================================
void CEnemy::MoveToFeed(void)
{
	// ターゲットが無い
	if (!m_pTargetFeed)
	{
		// 餌を見つける
		m_pTargetFeed = FindFeed();

		if (m_pTargetFeed == nullptr) 
		{
			// 女王のに向かうようにターゲットフラグ変更
			m_isDestQueen = true;
			m_isDestFeed = false;

			MoveToQueen();

			return;
		}
	}

	// 目的地座標をセット
	D3DXVECTOR3 destPos = m_pTargetFeed->GetPos();
	destPos.y = 0.0f;

	// ベクトルを生成する
	D3DXVECTOR3 vec = destPos - GetPos();
	float dist = D3DXVec3Length(&vec);

	// 一定距離以内なら止まる
	if (dist < EnemyInfo::StopRange)
	{
		SetMove(VECTOR3_NULL);
		GetMotion()->SetMotion(MOTION_NEUTRAL);
		return;
	}

	// ベクトルを正規化する
	D3DXVec3Normalize(&vec, &vec);
	vec *= EnemyInfo::MoveSpeed; // 移動速度

	// 向き
	float angleY = atan2(-vec.x, -vec.z);
	D3DXVECTOR3 rot = GetRotDest();

	rot.y = NormalAngle(angleY);
	SetRotDest(rot);

	SetMove(vec);

	// 移動モーションに設定
	GetMotion()->SetMotion(MOTION_MOVE);
}
//=========================================================
// 女王アリに向かう処理
//=========================================================
void CEnemy::MoveToQueen(void)
{
	// 対象を取得
	CQueen * Queen = CGameSceneObject::GetInstance()->GetQueen();
	if (Queen == nullptr) return;
	if (!Queen->GetIsUse()) return;

	// 対象座標を取得
	auto DestinationPos = D3DXVECTOR3(Queen->GetPos().x,NULL,Queen->GetPos().z);

	// ベクトル生成
	D3DXVECTOR3 VecToQueen = DestinationPos - GetPos();
	float fDiff = D3DXVec3Length(&VecToQueen);

	if (fDiff > 10.0f)
	{
		// ベクトルを正規化
		D3DXVec3Normalize(&VecToQueen, &VecToQueen);
		VecToQueen *= EnemyInfo::MoveSpeed;

		// 角度設定
		float angleY = atan2(-VecToQueen.x, -VecToQueen.z);
		D3DXVECTOR3 rotDest = GetRotDest();
		rotDest.y = NormalAngle(angleY);

		// 目的角に設定
		SetRotDest(rotDest);

		// 移動量を設定
		SetMove(VecToQueen);

		// 移動モーションに変更
		GetMotion()->SetMotion(MOTION_MOVE);
	}
	else
	{
		SetMove(VECTOR3_NULL);
		GetMotion()->SetMotion(MOTION_NEUTRAL, true, 10);
	}
}
//=========================================================
// 餌を見つける処理
//=========================================================
CFeed* CEnemy::FindFeed(void)
{
	// 餌取得
	auto Event = CEventAreaManager::GetInstance();
	auto EventFeedSize = Event->GetFeedSize();
	if (EventFeedSize <= NULL) return nullptr;

	// 最近点を生成
	CFeed* pNearest = nullptr;
	float minDist = FLT_MAX;

	// 座標取得
	D3DXVECTOR3 myPos = GetPos();

	// ランダムな餌に飛ぶように設定する
	int nRandomfeed = rand() % EventFeedSize;

	// 餌設定
	CFeed* feed = Event->GetFeedIdx(nRandomfeed);
	if (!feed) return nullptr;

	// 差分セット
	D3DXVECTOR3 diff = feed->GetPos() - myPos;
	float dist = D3DXVec3Length(&diff);

	// 一定値以下なら
	if (dist < minDist)
	{
		minDist = dist;
		pNearest = feed;
	}

	// ポインタを取得先でセットする
	return pNearest;
}
//==========================================================
// 餌との当たり判定
//==========================================================
void CEnemy::CollisionFeed(void)
{
	// アリと餌の当たり判定
	auto Event = CEventAreaManager::GetInstance();
	auto EventFeedSize = Event->GetFeedSize();
	if (EventFeedSize <= NULL) return;

	// 配列取得
	for (int nCnt = 0; nCnt < EventFeedSize; nCnt++)
	{
		// 変数格納
		auto feed = Event->GetFeedIdx(nCnt);
		auto Collider = feed->GetCollider();

		// 当たっていたら
		if (Collision(Collider))
		{
			// 当たった対象物の体力値を減らす
			feed->DecLife(1);

			// 管理クラスの配列の要素を消す
			CGameSceneObject::GetInstance()->GetEnemyManager()->Erase(this);

			// 自身の破棄
			Uninit();

			break;
		}
	}
}
//==========================================================
// 女王との当たり判定
//==========================================================
void CEnemy::CollisionQueen(void)
{
	// 女王の取得
	CQueen * Queen = CGameSceneObject::GetInstance()->GetQueen();
	if (Queen == nullptr) return;

	// コライダー取得
	CSphereCollider * Collider = Queen->GetCollider();
	if (Collider == nullptr) return;

	// 有効時
	if (Collision(Collider))
	{
		// 対象オブジェクトの体力値を減算する
		Queen->Hit(1);

		// 管理クラスの配列の要素を消す
		CGameSceneObject::GetInstance()->GetEnemyManager()->Erase(this);

		// 自身の破棄
		Uninit();

		return;
	}
}
//==========================================================
// 世界の壁との当たり判定処理
//==========================================================
void CEnemy::CollisionWall(void)
{
	// 世界の壁の取得
	CGameWallManager* WallManager = CGameSceneObject::GetInstance()->GetGameWall();
	if (WallManager == nullptr) return;

	// サイズで判断する
	for (int nCnt = 0; nCnt < WallManager->GetSize(); nCnt++)
	{
		// 各配列から取得
		auto Wall = WallManager->GetGameWall(nCnt);
		if (Wall == nullptr) return;

		// 当たったら
		if (CollisionBoxToSphere(Wall->GetCollider()))
		{
			// 管理クラスの配列の要素を消す
			CGameSceneObject::GetInstance()->GetEnemyManager()->Erase(this);

			// 自身の破棄
			Uninit();

			return;
		}
	}
}
//==========================================================
// 当たり判定処理
//==========================================================
bool CEnemy::Collision(CSphereCollider* pOther)
{
	// nulなら処理しない
	if (pOther == nullptr) return false;

	// 球形同士の当たり判定
	return CCollisionSphere::Collision(m_pSphereCollider.get(),pOther);
}
//==========================================================
// 当たり判定処理
//==========================================================
bool CEnemy::CollisionBoxToSphere(CBoxCollider* pOther)
{
	return CBoxToSphereCollision::Collision(pOther,m_pSphereCollider.get());
}
//=========================================================
// ノード作成処理
//=========================================================
void CEnemy::NodeSetting(void)
{
	// 名前空間取得
	using namespace EnemyTree;

	// ブラックボード生成
	m_pBlackBoard = new CBlackBoard;

	// 自身の情報
	auto pos = GetPos();
	m_pBlackBoard->SetValue<CEnemy*>("Enemy", this);
	m_pBlackBoard->SetValue<D3DXVECTOR3>("EnemyPos", pos);

	// 防衛対象の情報
	auto Queen = CGameSceneObject::GetInstance()->GetQueen();
	m_pBlackBoard->SetValue<CQueen*>("Queen", Queen);
	m_pBlackBoard->SetValue<D3DXVECTOR3>("QueenPos", Queen->GetPos());

	// 敵に使用するツリーノードにセットする
	m_pBehaviorTree = CEnemyBehaviorTree::SetEnemyTreeNode(m_pBlackBoard);
	m_pBehaviorTree->Init();
}