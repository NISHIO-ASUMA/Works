//===================================================================
//
// 選択されているモデルを示すオブジェクト [ pointobj.h ]
// Author: Asuma Nishio
//
//===================================================================

//*******************************************************************
// インクルードガード
//*******************************************************************
#pragma once

//*******************************************************************
// インクルードファイル
//*******************************************************************
#include "objectX.h"

//*******************************************************************
// 選択オブジェクトクラスを定義
//*******************************************************************
class CPointObj : public CObjectX
{
public:

	CPointObj(int nPriority = static_cast<int>(CObject::PRIORITY::MODELOBJECT));
	~CPointObj();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	/// <summary>
	/// ポインタ生成処理
	/// </summary>
	/// <param name="pos">生成座標</param>
	/// <param name="rot">角度</param>
	/// <returns></returns>
	static CPointObj* Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

private:
	static constexpr float ROT_VALUE = 0.06f; // 回転量
	static constexpr const char* MODELNAME = "STAGEOBJ/yajirusi.x"; // モデルパス
};