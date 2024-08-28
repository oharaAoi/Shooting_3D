#include "Player.h"
#include "GameScene.h"

Player::Player(std::vector<Model*> models) {
	Collider::SetTypeID(static_cast<uint32_t>(CollisionTypeIdDef::kPlayer));
	Init(models);
}
Player::~Player() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Init(std::vector<Model*> models) {
	BaseCharacter::Init(models);

	bullerModel_ = ModelLoader::GetInstacne()->GetModel("cube");

	// partsの親の設定
	worldTransforms_[PlayerParts::Parts_Body].parent_ = &worldTransform_;
	worldTransforms_[PlayerParts::Parts_Face].parent_ = &worldTransforms_[PlayerParts::Parts_Body];
	worldTransforms_[PlayerParts::Parts_LeftArm].parent_ = &worldTransforms_[PlayerParts::Parts_Body];
	worldTransforms_[PlayerParts::Parts_RightArm].parent_ = &worldTransforms_[PlayerParts::Parts_Body];

	worldTransform_.translation_.z = -20;

	// ---------------------------------------------
	// ↓ 基本となる変数の初期化
	// ---------------------------------------------
	behaviorRequest_ = PlayerBehavior::kRoot;
	ChangeBehavior(std::make_unique<PlayerRootState>(this));

	isLockOnMode_ = false;

	obb_.size = { 1,1,50 };

	// ---------------------------------------------
	// ↓ 調整項目
	// ---------------------------------------------
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	const char* groupName = "Player";
	// グループを追加
	adjustItem->CreateGroup(groupName);
	// 項目の追加
	adjustItem->AddItem(groupName, "face_Translation", worldTransforms_[PlayerParts::Parts_Face].translation_);
	adjustItem->AddItem(groupName, "L_Arm_Translation", worldTransforms_[PlayerParts::Parts_LeftArm].translation_);
	adjustItem->AddItem(groupName, "R_Arm_Translation", worldTransforms_[PlayerParts::Parts_RightArm].translation_);
	adjustItem->AddItem(groupName, "Hp", hp_);

	ApplyAdjustItems();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Update() {

	// ---------------------------------------------
	// ↓ Reticleの更新を行う
	// ---------------------------------------------
	for (auto it = canLockOnList_.begin(); it != canLockOnList_.end(); ) {
		if ((*it)->GetIsDead()) {
			it = canLockOnList_.erase(it);

			if (canLockOnList_.size() == 0) {
				isLockOnMode_ = false;
				continue;
			}

			Vector3 enemyPos = canLockOnList_.front()->GetScreenPosition(*viewProjection_);
			reticle_->SetLockOnScreenPos({ enemyPos.x, enemyPos.y });
		} else {
			++it;
		}
	}

	// ---------------------------------------------
	// ↓ Playerの状態に関する処理を行う
	// ---------------------------------------------

	// 状態のリクエストをチェックする
	CheckBehaviorRequest();

	// 現在の状態を更新する
	behaviorState_->Update();

	// 弾の更新を行う
	BulletsUpdate();

	// ---------------------------------------------
	// ↓ Playerの向きを調整する
	// ---------------------------------------------


	// ---------------------------------------------
	// ↓ 基本となる処理を行う
	// ---------------------------------------------
	if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		isLockOnMode_ = !isLockOnMode_;
	}

	// 計算転送
	BaseCharacter::Update();
	// ImGuiの編集
	EditImGui();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Draw(const ViewProjection& viewProjection) const {
	BaseCharacter::Draw(viewProjection);

	// 弾の描画
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBulletList_) {
		playerBullet->Draw(viewProjection);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　Playerの行動関連
//////////////////////////////////////////////////////////////////////////////////////////////////

void Player::Move() {
	XINPUT_STATE joyState;
	bool isMoving = false;

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {
		const float threshold = 0.7f;
		const float speed = 0.3f;
		float targetAngle = 0;

		velocity_ = {
			static_cast<float>(joyState.Gamepad.sThumbLX) / SHRT_MAX,
			0.0f,
			static_cast<float>(joyState.Gamepad.sThumbLY) / SHRT_MAX,
		};

		// スティックの押し込みが閾値を超えていたら移動可能にする
		if (Length(velocity_) > threshold) {
			isMoving = true;
		}

		// 移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 cameraRotate = MakeRotateXYZMatrix(viewProjection_->rotation_);
		velocity_ = TransformNormal(velocity_, cameraRotate);

		velocity_ = Normalize(velocity_) * speed;

		// 移動処理
		if (isMoving) {
			worldTransform_.translation_ += velocity_;
			// 方向を取得しておく
			direction_ = velocity_;
			// 目標角度の算出
			targetAngle = std::atan2f(velocity_.x, velocity_.z);
		}
		if (reticle_->GetIsLockOn()) {
			Vector3 diff = reticle_->GetTargetWorldPos() - worldTransform_.translation_;
			// y軸周り角度
			worldTransform_.rotation_.y = std::atan2f(diff.x, diff.z);
		}

		// --------------------------------------
		// プレイヤーの向きを移動方向に合わせる
		// --------------------------------------
		// 回転させる
		// y軸まわり(z軸方向とy軸方向のベクトルで求まる)
		if (isMoving) {
			worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, targetAngle, 0.05f);
		} else {
			worldTransform_.rotation_.y = std::atan2f(direction_.x, direction_.z);
		}
	}
}

// ------------------- 弾の更新を行う ------------------- //

void Player::BulletsUpdate() {
	// 弾の更新
	for (const std::unique_ptr<PlayerBullet>& playerBullet : playerBulletList_) {
		playerBullet->Update();
	}

	// 弾の削除
	playerBulletList_.remove_if([](const std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->GetIsDead()) {
			return true;
		}
		return false;
	});
}

// ------------------- 弾をリストに追加する ------------------- //

void Player::AddBulletList(const Vector3& velocity) {
	playerBulletList_.push_back(
		std::make_unique<PlayerBullet>(bullerModel_, worldTransform_.translation_, velocity, worldTransform_.rotation_, worldTransform_.parent_, 5
		));
}

void Player::AddCanLockOnList(BaseEnemy* baseEnemy) {
	canLockOnList_.push_back(baseEnemy);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　状態変化関連
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- Playerの状態を変化させる ------------------- //

void Player::ChangeBehavior(std::unique_ptr<BaseCharacterState> behavior) {
	behaviorState_ = std::move(behavior);
}

// ------------------- Playerの状態を変化させるリクエストがあるかを確認する ------------------- //

void Player::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case PlayerBehavior::kRoot:
			ChangeBehavior(std::make_unique<PlayerRootState>(this));
			break;
		case PlayerBehavior::kAttack:
			ChangeBehavior(std::make_unique<PlayerAttackState>(this));
			break;
		case PlayerBehavior::kDash:
			ChangeBehavior(std::make_unique<PlayerDashState>(this));
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Player::OnCollision(Collider* other) {
	uint32_t typeID = other->GetTypeID();
	if (typeID == static_cast<uint32_t>(CollisionTypeIdDef::kEnemy)) {

	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　値を返す関数群
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- world空間での座標を取得する ------------------- //

Vector3 Player::GetWorldPosition() const {
	// ローカル座標でのオフセット
	const Vector3 offset = { 0.0f, 0.0f, 0.0f };
	// ワールド座標に変換
	Vector3 worldPos = Transform(offset, worldTransform_.matWorld_);

	return worldPos;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　調整項目関連
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- ImGuiの編集 ------------------- //

void Player::EditImGui() {
#ifdef _DEBUG
	ImGui::Begin("Player");
	Vector3 worldPos = GetWorldPosition();
	ImGui::DragFloat3("worldPos", &worldPos.x, 0.1f);
	ImGui::DragFloat3("translate", &worldTransform_.translation_.x, 0.1f);
	ImGui::DragFloat3("rotate", &worldTransform_.rotation_.x, 0.1f);
	ImGui::DragFloat3("head", &worldTransforms_[PlayerParts::Parts_Face].translation_.x, 0.1f);
	ImGui::DragFloat3("leftArm", &worldTransforms_[PlayerParts::Parts_LeftArm].translation_.x, 0.1f);
	ImGui::DragFloat3("rightArm", &worldTransforms_[PlayerParts::Parts_RightArm].translation_.x, 0.1f);

	ImGui::Separator();

	ImGui::DragScalar("hp", ImGuiDataType_U32, &hp_);

	ImGui::End();
#endif // _DEBUG
}

// ------------------- 調整項目の適応 ------------------- //

void Player::ApplyAdjustItems() {
	AdjustmentItem* adjustItem = AdjustmentItem::GetInstance();
	const char* groupName = "Player";

	worldTransforms_[PlayerParts::Parts_Face].translation_ = adjustItem->GetValue<Vector3>(groupName, "face_Translation");
	worldTransforms_[PlayerParts::Parts_LeftArm].translation_ = adjustItem->GetValue<Vector3>(groupName, "L_Arm_Translation");
	worldTransforms_[PlayerParts::Parts_RightArm].translation_ = adjustItem->GetValue<Vector3>(groupName, "R_Arm_Translation");
	hp_ = adjustItem->GetValue<uint32_t>(groupName, "Hp");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

Vector3 Player::GetForward() {
	return TransformNormal({0,0,1.0f}, worldTransform_.matWorld_);
}

void Player::SetParent(const WorldTransform* parent) {
	// 親子関係を結ぶ
	worldTransform_.parent_ = parent;
}

void Player::SetWorldTransform(const WorldTransform& worldTransform) {
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
}

void Player::SetReticleParent(const WorldTransform* parent) {
	reticle_->SetParent(parent);
}
