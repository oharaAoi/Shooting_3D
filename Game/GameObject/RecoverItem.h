#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
#include <cassert>

#ifdef _DEBUG
#include "ImGuiManager.h"
#endif
#include "Collision/Collider.h"
class RecoverItem :
    public Collider {
public:

    RecoverItem(Model* model, const Vector3& pos);
    ~RecoverItem();

    void Init(Model* model, const Vector3& pos);

    void Update();

    void Draw(const ViewProjection& viewpro);

    Vector3 GetWorldPosition() const override;

    void OnCollision([[maybe_unused]] Collider* other) override;

    bool GetIsDead() const { return isDead_; }

private:

    Model* model_;
    WorldTransform worldTransform_;

    bool isDead_;

};

