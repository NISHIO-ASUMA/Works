//=========================================================
//
// アリで出すUI処理 [ workerui.h ]
// Author: Asuma Nishio
//
//=========================================================

//*********************************************************
// インクルードガード
//*********************************************************
#pragma once

//*********************************************************
// インクルードファイル
//*********************************************************
#include "object2D.h"

//=========================================================
// 働きアリの状態表示UIクラスを定義
//=========================================================
class CWorkerUi : public CObject2D
{
public:

	CWorkerUi(int nPriority = static_cast<int>(CObject::PRIORITY::UI));
	~CWorkerUi();

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	static CWorkerUi* Create(const D3DXVECTOR3 pos, const float fWidth, const float fHeight);

private:

};