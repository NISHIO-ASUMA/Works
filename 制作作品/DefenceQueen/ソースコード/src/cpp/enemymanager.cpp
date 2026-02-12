//=========================================================
//
// 敵の複数管理処理 [ enemymanager.cpp ]
// Author: Asuma Nishio
// 
//=========================================================

//*********************************************************
// インクルードファイル
//*********************************************************
#include "enemymanager.h"
#include "enemy.h"
#include "gamesceneobject.h"
#include "gametime.h"
#include "enemyspawnmanager.h"

//=========================================================
// コンストラクタ
//=========================================================
CEnemyManager::CEnemyManager() : m_pEnemys{},
m_nCreateLastTime(-1)
{

}
//=========================================================
// デストラクタ
//=========================================================
CEnemyManager::~CEnemyManager()
{
	Uninit();
}
//=========================================================
// 初期化処理
//=========================================================
HRESULT CEnemyManager::Init(void)
{
	// 配列クリア
	m_pEnemys.clear();

	// 配列のサイズを設定
	m_pEnemys.reserve(Config::NUM_ENEMY);

	return S_OK;
}
//=========================================================
// 終了処理
//=========================================================
void CEnemyManager::Uninit(void)
{
	// 配列クリア
	m_pEnemys.clear();
}
//=========================================================
// 更新処理
//=========================================================
void CEnemyManager::Update(void)
{
	// ゲームシーンのオブジェクト取得
	auto GameSceneObject = CGameSceneObject::GetInstance();
	int time = GameSceneObject->GetTime()->GetToAll();

	// 使用名前空間
	using namespace Enemy_Spawn;

	// サイズオーバーチェック
	if (m_pEnemys.size() >= Config::NUM_ENEMY) return;

	if (time > 0 &&
		time % Config::SPAWM_INTERBAL == NULL &&
		time != m_nCreateLastTime)
	{
		// 今回の湧き数
		int spawnCount = SPAWN_MIN + rand() % (SPAWN_MAX + 1);

		for (int nSpawn = 0; nSpawn < spawnCount; nSpawn++)
		{
			// スポーン位置をランダム
			int index = rand() % CREATE_NUM;
			D3DXVECTOR3 pos = SpawnPos[index];

			// オフセットとして値を作成
			D3DXVECTOR3 offset
			(
				(rand() % Config::RANDOM_VALUE_MAX - Config::RANDOM_VALUE_MIN) * Config::SPEED,
				0.0f,
				(rand() % Config::RANDOM_VALUE_MAX - Config::RANDOM_VALUE_MIN) * Config::SPEED
			);

			// 座標と合わせて少しずらす
			pos += offset;

			// 敵を生成
			CEnemy* pEnemy = CEnemy::Create(pos, VECTOR3_NULL, Config::LIFE);

			// nullチェック
			if (pEnemy)
			{
				// 有効状態に設定
				pEnemy->SetIsActive(true);
				m_pEnemys.push_back(pEnemy);
			}
		}

		// 生成時間を変更
		m_nCreateLastTime = time;
	}
}
//=========================================================
// 配列の要素を消す関数
//=========================================================
void CEnemyManager::Erase(CEnemy* pEnemy)
{
	// 削除処理
	auto DeleteDestObj = std::find(m_pEnemys.begin(), m_pEnemys.end(), pEnemy);

	// もし要素の最後尾なら
	if (DeleteDestObj == m_pEnemys.end()) return;

	// 削除
	(*DeleteDestObj)->Uninit();

	// 先をnullにする
	pEnemy = nullptr;

	// 配列の要素を消す
	DeleteDestObj = m_pEnemys.erase(DeleteDestObj);
}