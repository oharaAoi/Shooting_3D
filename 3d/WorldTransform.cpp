#include "WorldTransform.h"

void WorldTransform::UpdateMatrix() {
	// 行列の生成
	matWorld_ = MakeAffineMatrix(scale_, rotation_, translation_);

	// 親があれば親のワールド行列を掛ける
	if (parent_) {
		matWorld_ = Multiply(matWorld_, parent_->matWorld_);
	}

	// 定数バッファを転送
	TransferMatrix();
}