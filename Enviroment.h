#pragma once
#include "Vector3.h"

/// <summary>
/// 1フレームレート当たりの時間
/// </summary>
static float kDeltaTime_ = (1.0f / 60.0f);

/// <summary>
/// World空間上の広さ
/// </summary>
static Vector3 kWorldSize = Vector3(160.0f, 120.0f, 160.0f);