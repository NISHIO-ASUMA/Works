//=========================================================
//
// 指示を出す働きアリの処理 [ worker.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "worker.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "motion.h"
#include "statemachine.h"
#include "gamesceneobject.h"
#include "workerstateneutral.h"
#include "template.h"
#include "feed.h"
#include "feedmanager.h"
#include "parameter.h"
#include "signalui.h"
#include "meshimpact.h"

//=========================================================
// コンストラクタ
//=========================================================
CWorker::CWorker(int nPriority) : CMoveCharactor(nPriority),
m_pMotion(nullptr),
m_pSphereCollider(nullptr),
m_pStateMachine(nullptr),
m_pTargetFeed(nullptr),
m_pSignal(nullptr),
m_isMove(false),
m_isWork(false),
m_isCreate(false),
m_isSetNum(false),
m_DestPos(VECTOR3_NULL),
m_SavePos(VECTOR3_NULL),
m_nIdxNumber(NULL),
m_nScaleNum(NULL),
m_nImpactCounter(NULL),
m_MoveState(NONE)
{

}
//=========================================================
// デストラクタ
//=========================================================
CWorker::~CWorker()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CWorker* CWorker::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// インスタンス生成
	CWorker* pWorker = new CWorker;
	if (pWorker == nullptr) return nullptr;

	// オブジェクト設定
	pWorker->SetPos(pos);
	pWorker->SetRot(rot);
	pWorker->SetUseStencil(false);

	// 保存変数
	pWorker->m_SavePos = pos;

	// 初期化失敗時
	if (FAILED(pWorker->Init())) return nullptr;

	return pWorker;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CWorker::Init(void)
{
	// 親クラスの初期化
	CMoveCharactor::Init();

	// オブジェクトの種類をセット
	SetObjType(CObject::TYPE_WORKER);

	// モーションセット
	MotionLoad("data/MOTION/Work/Worker_Motion.txt", MOTION_MAX,true);

	// コライダー生成
	//m_pSphereCollider = CSphereCollider::Create(GetPos(), 50.0f);

	// シグナルui生成
	m_pSignal = CSignalUi::Create(D3DXVECTOR3(GetPos().x, GetPos().y + Config::Add_PosY, GetPos().z), VECTOR3_NULL, 60.0f, 40.0f);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CWorker::Uninit(void)
{
	// ステートポインタ破棄
	m_pStateMachine.reset();

	// ポインタ破棄
	if (m_pSphereCollider)
	{
		delete m_pSphereCollider;
		m_pSphereCollider = nullptr;
	}

	// 親クラスの終了
	CMoveCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CWorker::Update(void)
{
	// 有効なら
	if (m_isWork)
	{
		// 目的地に移動する関数
		MoveToPoint();
	}
	else if (!m_isWork)
	{
		// 基地に帰る関数
		MoveToReturnBase();
	}

	// キャラクターの座標更新
	CMoveCharactor::UpdatePosition();

	// 更新後の座標を取得
	D3DXVECTOR3 UpdatePos = GetPos();

	// UIの座標を設定する
	m_pSignal->SetPos(D3DXVECTOR3(UpdatePos.x, UpdatePos.y + Config::Add_PosY, UpdatePos.z));

	// コライダー座標の更新
	if (m_pSphereCollider) m_pSphereCollider->SetPos(UpdatePos);

	// 当たり判定
	auto pFeed = CGameSceneObject::GetInstance()->GetFeedManager();

	for (int nCnt = 0; nCnt < pFeed->GetSize(); nCnt++)
	{
		// 餌を取得
		auto ChildFeed = pFeed->GetFeed(nCnt);
		if (ChildFeed == nullptr) continue;

		// コリジョンしていた
		if (Collision(ChildFeed->GetCollider()))
		{
			// ナンバーパネルのパラメーターを渡す
			if (!m_isSetNum)
			{ 
				// ランダムパネル
				int nData = RequiredNumber();

				// 体力値を更新
				ChildFeed->GetParam()->SetHp(nData);

				// フラグを有効化
				m_isSetNum = true;
			}

			// イベント登録
			ChildFeed->RegisterEvent([&](void) { this->MoveToReturnBase(); });

			// コライダー更新
			m_pSphereCollider->SetPos(UpdatePos);
		}
	}

	// 加算
	m_nImpactCounter++;

	// 生成条件に達したら
	if (m_isSetNum && m_nImpactCounter >= Config::MAX_COUNT)
	{
		// インパクト生成
		CMeshImpact::Create(GetPos(), Config::IMPACT_LIFE, 30.0f, 10.0f, 1.5f);

		// 初期化する
		m_nImpactCounter = NULL;
	}

	// キャラクター全体の更新処理
 	CMoveCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CWorker::Draw(void)
{
	// キャラクター描画
	CMoveCharactor::Draw();
}
//=========================================================
// 対象の座標に移動する関数
//=========================================================
void CWorker::MoveToPoint(void)
{
	// 描画させる
	m_pSignal->SetIsDraw(true);

	// 現在の座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 目的から現在の座標までのベクトル
	D3DXVECTOR3 VecMove = m_DestPos - pos;
	float dist = D3DXVec3Length(&VecMove);

	// 距離が一定範囲外だったら
	if (dist > Config::Distance)
	{
		// 正規化
		D3DXVec3Normalize(&VecMove, &VecMove);

		// 速度を乗算する
		VecMove *= Config::Move;

		// 計算角度
		float angleY = atan2(-VecMove.x, -VecMove.z);
		D3DXVECTOR3 rotDest = GetRotDest();
		rotDest.y = angleY;

		// 正規化する
		rotDest.y = NormalAngle(rotDest.y);

		// 目的角を更新
		SetRotDest(rotDest);

		// 移動量をセット
		SetMove(VecMove);

		// 移動モーションに変更
		GetMotion()->SetMotion(CWorker::MOTION_MOVE);
	}
	else
	{
		// 描画しない
		m_pSignal->SetIsDraw(false);

		// 待機モーションに変更
		GetMotion()->SetMotion(CWorker::MOTION_NEUTRAL);
	}
}
//=========================================================
// 元の場所にもどってくる処理
//=========================================================
void CWorker::MoveToReturnBase(void)
{
	// フラグ初期化
	m_isSetNum = false;
	m_isWork = false;
	m_isCreate = false;
	m_pSignal->SetIsDraw(false);

	// もどる座標を設定
	SetDestPos(m_SavePos);

	// 現在の座標を取得
	D3DXVECTOR3 pos = GetPos();

	// 目的から現在の座標までのベクトル
	D3DXVECTOR3 VecMove = m_SavePos - pos;
	float dist = D3DXVec3Length(&VecMove);

	if (dist > Config::StopDistance)
	{
		// 正規化
		D3DXVec3Normalize(&VecMove, &VecMove);

		// 速度を乗算する
		VecMove *= Config::Move;

		// 計算角度
		float angleY = atan2(-VecMove.x, -VecMove.z);
		D3DXVECTOR3 rotDest = GetRotDest();
		rotDest.y = angleY;

		// 正規化する
		rotDest.y = NormalAngle(rotDest.y);

		// 目的角を更新
		SetRotDest(rotDest);

		// 移動量をセット
		SetMove(VecMove);

		// 移動モーションに変更
		GetMotion()->SetMotion(CWorker::MOTION_MOVE);
	}
	else
	{
		// 移動しない
		SetMove(VECTOR3_NULL);

		// 移動モーションに変更
		GetMotion()->SetMotion(CWorker::MOTION_NEUTRAL);

		// 状態変更
		m_MoveState = NONE;

		// フラグ初期化
		m_isSetNum = false;
		m_isCreate = false;
	}
}
//=========================================================
// 餌に働きアリを割り当てる処理
//=========================================================
void CWorker::AssignFeed(CFeed* pFeed)
{
	// ターゲット設定
	m_pTargetFeed = pFeed;

	// 状態切り替え
	m_MoveState = MOVE::MOVE_FEED;

	// 目的の座標にセット
	SetDestPos(D3DXVECTOR3(pFeed->GetPos().x,0.0f,pFeed->GetPos().z));

	// 働いている状態にする
	m_isWork = true;
}

//=========================================================
// 球の当たり判定処理
//=========================================================
bool CWorker::Collision(CSphereCollider* other)
{
	return CCollisionSphere::Collision(m_pSphereCollider,other);
}
//=========================================================
// 状態切り替え関数
//=========================================================
void CWorker::ChangeState(CWorkerStateBase* pState)
{
	// 自分自身を代入
	pState->SetOwner(this);

	// ステート変更
	m_pStateMachine->ChangeState(pState);
}
//=========================================================
// プレイヤー側に通知する数を返す関数
//=========================================================
int CWorker::RequiredNumber(void)
{
	// ランダム値
	int nrand = rand() % NUMARRAY_MAX;

	if (!m_isCreate)
	{
		// セットする
		SetNeedNumber(nrand);

		// インデックスに応じて変更
		switch (m_nIdxNumber)
		{
		case NUMARRAY_LITTLE: // 最小値
			m_nScaleNum = Config::LITTLE;
			break;

		case NUMARRAY_MIDDLE: // 中間値
			m_nScaleNum = Config::MIDDLE;
			break;

		case NUMARRAY_LARGE: // 最大値
			m_nScaleNum = Config::LARGE;
			break;

		default:
			// 例外対策値
			m_nScaleNum = 1;
			break;
		}

		// フラグを有効化
		m_isCreate = true;
	}

	// 取得する値を返す
	return m_nScaleNum;
}