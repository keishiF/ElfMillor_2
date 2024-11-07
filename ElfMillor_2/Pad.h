#pragma once

namespace Pad
{
	// パッドの入力状態の更新処理
	void Update();

	// Press判定の取得
	bool IsPress(int button);

	// 押された瞬間の取得
	bool IsTrigger(int button);
}