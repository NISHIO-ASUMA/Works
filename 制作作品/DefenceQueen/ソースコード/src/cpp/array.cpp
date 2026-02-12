//=========================================================
//
// プレイヤーの仲間アリ1匹に対する処理 [ array.cpp ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "array.h"
#include "parameter.h"
#include "manager.h"
#include "spherecollider.h"
#include "collisionsphere.h"
#include "blackboard.h"
#include "boxcollider.h"
#include "collisionbox.h"
#include "arraymanager.h"
#include "gamesceneobject.h"
#include "template.h"
#include "arrayspawnmanager.h"
#include "arrayspawner.h"
#include "topant.h"
#include "player.h"
#include "separationant.h"
#include "feedmanager.h"
#include "feed.h"
#include "arraybehaviortree.h"
#include "node.h"
#include "blackboard.h"
#include "score.h"
#include "enemymanager.h"
#include "enemy.h"
#include "motioninstancing.h"
#include "basemapfeed.h"
#include "eventareamanager.h"
#include "particlepiler.h"
#include "extractantsignal.h"

//=========================================================
// コンストラクタ
//=========================================================
CArray::CArray(int nPriority) : CInstancingCharactor(nPriority),
m_pSphereCollider(nullptr),
m_pMotion(nullptr),
m_pParameter(nullptr),
m_pBoxCollider(nullptr),
m_pFollowTarget(nullptr),
m_pTopAnt(nullptr),
m_pAntSignal(nullptr),
m_isActive(false),
m_isMove(true),
m_isTopAntFollow(false),
m_isHit(false),
m_isReturn(false),
m_isAtBase(true),
m_isStop(false),
m_isCheckNearFeed(false),
m_isAttackMode(false),
m_isGettingTopOrder(false),
m_isSetPoint(false),
m_MoveDestPos(VECTOR3_NULL),
m_ActivePos(VECTOR3_NULL),
m_nListGroupId(NULL)
{
	// 値のクリア
}
//=========================================================
// デストラクタ
//=========================================================
CArray::~CArray()
{
	
}
//=========================================================
// 生成処理
//=========================================================
CArray* CArray::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,const int nLife)
{
	// インスタンス生成
	CArray* pArray = new CArray;
	if (pArray == nullptr) return nullptr;

	// オブジェクト設定
	pArray->SetPos(pos);
	pArray->SetRot(rot);
	pArray->SetUseStencil(false);

	// パラメータ設定
	pArray->m_pParameter = std::make_unique<CParameter>();
	pArray->m_ActivePos = pos;

	// nullチェック
	if (pArray->m_pParameter)
	{
		// 体力パラメーターを設定
		pArray->m_pParameter->SetHp(nLife);
		pArray->m_pParameter->SetMaxHp(nLife);
	}

	// 初期化処理
	if (FAILED(pArray->Init())) return nullptr;

	return pArray;
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CArray::Init(void)
{
	// 親クラスの初期化
	CInstancingCharactor::Init();

	// オブジェクトの種類をセット
	SetObjType(CObject::TYPE_ARRAY);

	// モーションセット
	MotionLoad(Arrayinfo::SCRIPT, MOTION_MAX,true);

	// スフィアコライダー生成
	m_pSphereCollider = CSphereCollider::Create(GetPos(), Arrayinfo::SphereRange);

	// モーション取得
	m_pMotion = CInstancingCharactor::GetMotion();

	m_pAntSignal = CExtractAntSignal::Create(D3DXVECTOR3(GetPos().x, GetPos().y + 120.0f, GetPos().z));

	// フラグ初期化
	m_isActive = false;
	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CArray::Uninit(void)
{
	// パラメーター終了処理
	m_pParameter.reset();

	// コライダー破棄
	m_pSphereCollider.reset();

	// ビヘイビアツリーの破棄
	if (m_pBehaviorTree)
	{
		m_pBehaviorTree->Exit();
		delete m_pBehaviorTree;
		m_pBehaviorTree = nullptr;
	}

	// ブラックボードの破棄
	if (m_pBlackBoard)
	{
		m_pBlackBoard->Clear();
		delete m_pBlackBoard;
		m_pBlackBoard = nullptr;
	}

	// キャラクターの破棄
	CInstancingCharactor::Uninit();
}
//=========================================================
// 更新処理
//=========================================================
void CArray::Update(void)
{
	// falseなら通さない
	if (!m_isActive) return;
	
	// ブラックボードの毎フレーム更新
	m_pBlackBoard->SetValue("TopPos", m_pTopAnt->GetPos());

	// 基地に帰る
	if (m_isReturn)
	{
		// 判定フラグ初期化
		m_isHit = false;

		// モーション変更
		m_pMotion->SetMotion(MOTION_MOVE);

		// 基地に帰る
		SpawnReturn();
	}

	// ツリーノードの更新処理
	m_pBehaviorTree->Update();

	// 座標のみの更新
	CInstancingCharactor::UpdatePosition();

	// 更新座標の取得
	auto UpdatePos = GetPos();

	// コライダー座標の更新
	m_pSphereCollider->SetPos(UpdatePos);

	// 全判定関数
	CollsionAll();

	// キャラクターの更新
	CInstancingCharactor::Update();
}
//=========================================================
// 描画処理
//=========================================================
void CArray::Draw(void)
{
	// キャラクターの描画
	CInstancingCharactor::Draw();
}
//=========================================================
// 切り離される処理
//=========================================================
void CArray::OnSeparation(void)
{
	// 移動フラグを無効化
	SetIsMove(false);

	// 帰還モードにする
	SetReturnSpawn(true);
}

//=========================================================
// ある一定の目標地点に向かう処理
//=========================================================
void CArray::FollowDestination(const D3DXVECTOR3& DestPos)
{
	if (!m_isMove)
	{
		// 待機モーションに設定
		m_pMotion->SetMotion(MOTION_NEUTRAL);
		return;
	}
	
	// 命令フラグが有効なら
	if (m_isGettingTopOrder)
	{
		// 追従切り替え
		FollowTop(m_pBlackBoard->GetValue<D3DXVECTOR3>("TopPos"));
		return;
	}

	// 目的地までの距離を算出
	D3DXVECTOR3 moveVec = DestPos - GetPos();
	float distToDest = D3DXVec3Length(&moveVec);

	// 目的地が近いなら隊列を切って直接目的地へ
	if (distToDest < Arrayinfo::ARRAY_DISTANCE)
	{
		// 基地判定がtrueなら
		if (m_isAtBase)
		{
			m_isAtBase = false;
		}

		// 到着したら止まる
		if (distToDest < Arrayinfo::STOP_DISTANCE)
		{
			// 待機モーションに設定
			m_pMotion->SetMotion(CArray::MOTION_NEUTRAL);

			// ブラックボードを更新する
			if (!m_isCheckNearFeed)
			{
				m_isCheckNearFeed = true;
				m_pBlackBoard->SetValue("CheckNearFeed", m_isCheckNearFeed);
			}

			return;
		}

		// 目的地へ直接移動
		D3DXVec3Normalize(&moveVec, &moveVec);
		moveVec *= Arrayinfo::MoveSpeed;

		float fAngle = atan2(-moveVec.x, -moveVec.z);
		D3DXVECTOR3 Rotdest = GetRotDest();

		// 正規化
		Rotdest.y = NormalAngle(fAngle);

		// 値をセット
		SetRotDest(Rotdest);

		// 移動量を加算
		SetMove(moveVec);

		// 移動モーションにセット
		m_pMotion->SetMotion(CArray::MOTION_MOVE);

		return;
	}

	// 単独で目的に向かう
	D3DXVec3Normalize(&moveVec, &moveVec);
	moveVec *= Arrayinfo::MoveSpeed;

	float angle = atan2(-moveVec.x, -moveVec.z);
	D3DXVECTOR3 rotdest = GetRotDest();
	rotdest.y = NormalAngle(angle);
	SetRotDest(rotdest);

	// キャラクターの移動量に設定
	SetMove(moveVec);

	// 移動モーションに設定
	m_pMotion->SetMotion(CArray::MOTION_MOVE);
}

//=========================================================
// トップのアリを追従する関数
//=========================================================
void CArray::FollowTop(D3DXVECTOR3 vpos)
{
	// 目的地までの距離を算出
	D3DXVECTOR3 moveVec = vpos - GetPos();
	float distToDest = D3DXVec3Length(&moveVec);

	// 到着したら止まる
	if (distToDest < Arrayinfo::TOP_DISTANCE)
	{
		// 移動量を0にする
		SetMove(VECTOR3_NULL);

		// 待機モーションに設定
		m_pMotion->SetMotion(CArray::MOTION_NEUTRAL);
	}
	else
	{
		// 目的地へ直接移動
		D3DXVec3Normalize(&moveVec, &moveVec);
		moveVec *= Arrayinfo::MoveSpeed;

		// 目的角を計算
		float fAngle = atan2(-moveVec.x, -moveVec.z);
		D3DXVECTOR3 Rotdest = GetRotDest();

		// 正規化関数
		Rotdest.y = NormalAngle(fAngle);

		// 目的角の値をセット
		SetRotDest(Rotdest);

		// 移動量を加算
		SetMove(moveVec);

		// 移動モーションにセット
		m_pMotion->SetMotion(CArray::MOTION_MOVE);
	}
}
//=========================================================
// 仲間を追従する
//=========================================================
void CArray::ArrayFollow(void)
{
	// 無かったら
	if (!m_isReturn && !m_pFollowTarget) return;

	// ターゲット座標を取得
	D3DXVECTOR3 targetPos = m_pFollowTarget->GetPos();
	D3DXVECTOR3 followVec = targetPos - GetPos();
	float fDistance = D3DXVec3Length(&followVec);

	if (fDistance > Arrayinfo::ARRAY_DISTANCE)
	{
		// ベクトルを正規化
		D3DXVec3Normalize(&followVec, &followVec);
		followVec *= Arrayinfo::MoveSpeed;

		// 角度設定
		float angleY = atan2(-followVec.x, -followVec.z);
		D3DXVECTOR3 rotDest = GetRotDest();
		rotDest.y = NormalAngle(angleY);

		// 目的角に設定
		SetRotDest(rotDest);

		// 移動量を設定
		SetMove(followVec);

		// 移動モーションに変更
		m_pMotion->SetMotion(CArray::MOTION_MOVE);
	}
	else
	{
		// 停止
		SetMove(VECTOR3_NULL);

		// ニュートラルモーションに変更
		m_pMotion->SetMotion(CArray::MOTION_NEUTRAL);
	}
}
//=========================================================
// 巣にもどる命令を受けたときの移動
//=========================================================
void CArray::SpawnReturn(void)
{
	// ベクトルの成分計算
	D3DXVECTOR3 followVec = m_ActivePos - GetPos();
	float fDistance = D3DXVec3Length(&followVec);

	// 距離が一定値以上の場合
	if (fDistance > 3.0f)
	{
		// ベクトルを正規化
		D3DXVec3Normalize(&followVec, &followVec);
		followVec *= Arrayinfo::MoveSpeed;

		// 角度設定
		float angleY = atan2(-followVec.x, -followVec.z);
		D3DXVECTOR3 rotDest = GetRotDest();
		rotDest.y = NormalAngle(angleY);

		// 目的角に設定
		SetRotDest(rotDest);

		// 移動量を設定
		SetMove(followVec);

		// 移動モーションに変更
		m_pMotion->SetMotion(CArray::MOTION_MOVE);
	}
	else
	{
		// 停止する
		SetMove(VECTOR3_NULL);

		// 移動モーションに変更
		m_pMotion->SetMotion(CArray::MOTION_NEUTRAL);

		// 基地についたので帰還モード解除
		SetReturnSpawn(false);

		// 基地到着
		SetAtBase(true);
		SetIsTopOrder(false);

		// 行動フラグ完全リセット
		m_isMove = true;
		m_isGettingTopOrder = false;
		m_isSetPoint = false;
		m_isAttackMode = false;
		m_isCheckNearFeed = false;
		m_isHit = false;

		// Blackboardもリセット
		m_pBlackBoard->SetValue("GetTopOrder", false);
		m_pBlackBoard->SetValue("SetPoint", false);
		m_pBlackBoard->SetValue("AttackMode", false);
		m_pBlackBoard->SetValue("CheckNearFeed", false);

		// ターゲットに向かう関数を設定
		FollowDestination(m_MoveDestPos);
	}
}
//=========================================================
// ノードを設定する
//=========================================================
void CArray::NodeSetting(void)
{
	// 名前空間取得
	using namespace ArrayTree;

	// ブラックボード生成
	m_pBlackBoard = new CBlackBoard;

	// ブラックボードに情報をセットする
	m_pBlackBoard->SetValue<CArray*>("Array", this);					 // 自身のポインタ
	m_pBlackBoard->SetValue<CTopAnt*>("TopAnt",m_pTopAnt);				 // トップアリのポインタ
	m_pBlackBoard->SetValue<D3DXVECTOR3>("ArrayDestPos", m_MoveDestPos); // 目的座標
	m_pBlackBoard->SetValue<D3DXVECTOR3>("TopPos", m_pTopAnt->GetPos()); // 目的座標

	m_pBlackBoard->SetValue<bool>("ReturnSpawn", m_isReturn);			 // 基地に帰るフラグ
	m_pBlackBoard->SetValue<bool>("GetTopOrder", m_isGettingTopOrder);	 // トップからの命令取得
	m_pBlackBoard->SetValue<bool>("AttackMode", m_isAttackMode);		 // 攻撃状態フラグ
	m_pBlackBoard->SetValue<bool>("CheckNearFeed", m_isCheckNearFeed);	 // 餌近くの判別フラグ
	m_pBlackBoard->SetValue<bool>("SetPoint", m_isSetPoint);			 // セットポイントフラグ

	// 仲間に使用するツリーノードにセットする
	m_pBehaviorTree = CArrayBehaviorTree::SetArrayTreeNode(m_pBlackBoard);
	m_pBehaviorTree->Init();
}
//=========================================================
// 球の当たり判定処理 TODO : ここ修正してイベントモードの時の当たり判定に設定する
//=========================================================
bool CArray::Colision(CSphereCollider* other)
{
	// nullなら
	if (m_pSphereCollider == nullptr) return false;

	// 球同士の当たり判定の関数を返す
	return CCollisionSphere::Collision(m_pSphereCollider.get(),other);
}
//=========================================================
// 当たり判定格納関数
//=========================================================
void CArray::CollsionAll(void)
{
	if (m_isGettingTopOrder)
		CollisionEnemy();

	CollisionEventFeed();
}
//=========================================================
// 敵との当たり判定関数
//=========================================================
void CArray::CollisionEnemy(void)
{
	// マップ内の敵の当たり判定
	CEnemyManager* pEnemyManager = CGameSceneObject::GetInstance()->GetEnemyManager();
	if (pEnemyManager == nullptr) return;

	// 最大の物と判定
	for (int nCnt = 0; nCnt < pEnemyManager->GetSize(); nCnt++)
	{
		// 単体取得
		auto Enemy = pEnemyManager->GetEnemyIdx(nCnt);
		if (!Enemy->GetIsActive()) continue;

		// 球形同士の当たり判定を実行
		if (Colision(Enemy->GetCollider()))
		{
			// 管理クラスの配列の要素を消す
			CGameSceneObject::GetInstance()->GetEnemyManager()->Erase(Enemy);

			// パーティクル生成
			//CParticlePiler::Create(D3DXVECTOR3(GetPos().x, 120.0f, GetPos().z), COLOR_RED, 15, 150, 350, 15, 0.0f);

			// 敵の終了
			Enemy->Uninit();

			// 自身は有効化を終了する
			this->m_isActive = false;

			// 自身の出てきたスポナーに戻る
			SetPos(m_ActivePos);

			// フラグ初期化
			SetIsTopOrder(false);

			return;
		}
	}
}
//=========================================================
// イベントの餌との当たり判定
//=========================================================
void CArray::CollisionEventFeed(void)
{
	// 配列を取得する
	int nSize = CEventAreaManager::GetInstance()->GetFeedSize();
	if (nSize <= NULL) return;

	for (int nCnt = 0; nCnt < nSize; nCnt++)
	{
		// 各餌の要素を取得する
		auto Feed = CEventAreaManager::GetInstance()->GetFeedIdx(nCnt);

		// 有効なら
		if (Colision(Feed->GetCollider()) && !m_isHit)
		{
			// 体力を減らす
			Feed->DecLife(Arrayinfo::Damage);

			// スコアを加算
			CGameSceneObject::GetInstance()->GetScore()->AddScore(Arrayinfo::SCORE_UP);

			// 通常行動は停止
			m_isGettingTopOrder = false;

			// 帰還モード突入
			m_isReturn = true;
			
			// 移動は許可
			m_isMove = true;

			// ヒット判定
			m_isHit = true;

			// モーション変更
			m_pMotion->SetMotion(MOTION_MOVE);

			break;
		}
	}
}
//=========================================================
// ベースの餌との当たり判定
//=========================================================
void CArray::CollisionBase(void)
{
	// 配列を取得する
	CBaseMapFeed* BaseFeed = CGameSceneObject::GetInstance()->GetBaseMapFeed();
	if (BaseFeed == nullptr) return;

	for (int nCnt = 0; nCnt < BaseFeed->GetFeedArraySize(); nCnt++)
	{
		// 各餌の要素を取得する
		auto Feed = BaseFeed->GetFeedIdx(nCnt);

		// 有効なら
		if (Colision(Feed->GetCollider()))
		{
			// 基地に帰るフラグを有効化し、ブラックボードを更新する
			if (!m_isReturn)
			{
				m_isReturn = true;
				m_pBlackBoard->SetValue("ReturnSpawn", m_isReturn);
			}
			break;
		}
	}
}
//=========================================================
// 目的座標設定関数
//=========================================================
void CArray::SetDestPos(const D3DXVECTOR3& pos)
{
	// 値を変更する
	m_MoveDestPos = pos;

	// ノードの値に再設定
	m_pBlackBoard->SetValue("ArrayDestPos", m_MoveDestPos);
}
//=========================================================
// 命令設定関数
//=========================================================
void CArray::SetIsTopOrder(const bool& isToporder)
{
	// フラグ変更
	m_isGettingTopOrder = isToporder;

	// ノードの値に再設定
	m_pBlackBoard->SetValue("GetTopOrder", m_isGettingTopOrder);
}
//=========================================================
// セットポイントフラグ関数
//=========================================================
void CArray::SetIsPointFlag(const bool& isSetpoint)
{
	// ニュートラルモーションに変更
	m_pMotion->SetMotion(MOTION_NEUTRAL, true, 10);

	// フラグ変更
	m_isSetPoint = isSetpoint;

	// 値を再設定
	m_pBlackBoard->SetValue("SetPoint", m_isSetPoint);
}
//=========================================================
// アタックモード設定関数
//=========================================================
void CArray::SetIsAtackMode(const bool& isMode)
{
	// フラグ変更
	m_isAttackMode = isMode;

	// 値を再設定
	m_pBlackBoard->SetValue("AttackMode", m_isAttackMode);
}