#include "MyMath.h"
#include "PrimitiveDrawer.h"

// ノルムを求める
float Length(const Vector3& vec3) {
	return std::sqrt(vec3.x * vec3.x + vec3.y * vec3.y + vec3.z * vec3.z);
}

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

// 減算
Vector3  Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

// スカラー倍
Vector3 Multiply(float scalar, const Vector3& v) {
	Vector3 result{};

	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;

	return result;
}

/// <summary>
/// ベクトルの距離を測る
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Distance(const Vector3& v1, const Vector3& v2) {
	float result;
	Vector3 lengthVector;
	lengthVector = Subtract(v1, v2);
	result = Length(lengthVector);

	return result;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	float result{};

	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return result;
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	Vector3 result{};

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

Vector3 Normalize(const Vector3& vec3) {
	Vector3 result = vec3;

	float len = Length(vec3);
	if (len != 0) {
		result.x /= len;
		result.y /= len;
		result.z /= len;
	}

	return result;
}

Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
	Vector3 result{
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
	};

	return result;
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result{};
	result.x = std::lerp(v1.x, v2.x, t);
	result.y = std::lerp(v1.y, v2.y, t);
	result.z = std::lerp(v1.z, v2.z, t);

	//v1 + (v2 - v1) * t;

	return result;
}

Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t) {
	Vector3 result{};
	// なす角を求める
	float theta = Angle(v1, v2);

	// 引数のベクトルのlerpを求める
	float lerp = (1 - t) * Length(v1) + (t * Length(v2)); // s

	// 引数のベクトルの単位ベクトルを求める
	Vector3 unitV1 = Normalize(v1);    // ea
	Vector3 unitV2 = Normalize(v2);    // eb

	// ベクトルの角度を求める
	float midTheta1;
	float midTheta2;

	// なす角が0の時はそのままの角度を返すようにする
	if (theta == 0) {
		midTheta1 = 1;
		midTheta2 = 1;
	} else {
		midTheta1 = std::sin((1 - t) * theta) / std::sin(theta);
		midTheta2 = std::sin(t * theta) / std::sin(theta);
	}

	// 角度からベクトルを求める
	Vector3 midV1 = Multiply(midTheta1, unitV1);
	Vector3 midV2 = Multiply(midTheta2, unitV2);

	Vector3 mid = midV1 + midV2;

	// 正規の補完分と求めたベクトルを掛ける
	result = Multiply(lerp, mid);

	return result;
}

float LerpShortAngle(const float& originalAngle, const float& targetAngle, const float& t) {
	float diff = targetAngle - originalAngle;

	// 角度を-2π~2πに補完
	diff = std::fmodf(diff, std::numbers::pi_v<float> * 2.0f);
	// 角度を-π~πに補完
	if (diff > std::numbers::pi_v<float>) {
		diff = diff - (std::numbers::pi_v<float> *2.0f);

	} else if(diff < -std::numbers::pi_v<float>) {
		diff = diff + (std::numbers::pi_v<float> *2.0f);
	}
	

	return originalAngle + (diff * t);
}

float Angle(const Vector3& v1, const Vector3& v2) {
	float result;

	// ベクトル長さを求める
	float normV1 = Length(v1);
	float normV2 = Length(v2);

	if (normV1 == 0 || normV2 == 0) {
		return 0;
	}
	// 内積
	float dot = Dot(v1, v2);

	// cosを求める
	float cosTheta = dot / (normV1 * normV2);

	result = std::acos(cosTheta);

	return result;
}

/// <summary>///
/// CatmullRom補完
/// </summary>///
/// <param name="p0">点0の座標</param>///
/// <param name="p1">点1の座標</param>///
/// <param name="p2">点2の座標</param>///
/// <param name="p3">点3の座標</param>///
/// <param name="t">点1と点2の間で指定された座標</param>///
/// <returns>点1と点2の間で指定された座標</returns>
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
	const float s = 0.5f; // 数式に出てくる 1/2のこと

	float t2 = t * t; // tの2乗
	float t3 = t2 * t; // tの3乗

	// ベクトル値関数の式
	Vector3 e3 = (p0 * -1) + (p1 * 3) - (p2 * 3) + p3;
	Vector3 e2 = (p0 * 2) - (p1 * 5) + (p2 * 4) - p3;
	Vector3 e1 = (p0 * -1) + p2;
	Vector3 e0 = p1 * 2;

	return ((e3 * t3) + (e2 * t2) + (e1 * t) + e0) * s;
}

/// <summary>///
/// CatmullRomスプライン曲線上の座標を得る
/// </summary>///
/// <param name="points">制御点の集合</param>///
/// <param name="t">スプライン全区間の中で割合指定</param>///
/// <returns>座標</returns>
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t) {
	assert(points.size() >= 4 && "制御点は4以上必要です");
	// 区間数は制御点の数の-1
	size_t division = points.size() - 1;
	// 1区間の長さ(全体を1.0とした割合)
	float areaWidth = 1.0f / division;

	// 区間の始点を0.0f, 終点を1.0fとしたときの現在の位置
	float t2 = std::fmod(t, areaWidth) * division;
	// 下限(0.0f)と上限(1.0f)の範囲に収める
	t2 = std::clamp(t2, 0.0f, 1.0f);

	// 区間番号
	size_t index = static_cast<size_t>(t / areaWidth);
	// 区間番号が上限を超えないように収める
	index = std::clamp(static_cast<int>(index), 0, static_cast<int>(division) - 1);

	// 4点分のインデックス
	size_t index0 = index - 1;
	size_t index1 = index;
	size_t index2 = index + 1;
	size_t index3 = index + 2;

	// 始点の場合はp0とp1は同じ
	if (index == 0) {
		index0 = index1;
	}
	// 最後の区間はp3とp2は同じ
	if (index3 >= points.size()) {
		index3 = index2;
	}

	// 4点の座標
	const Vector3& p0 = points[index0];
	const Vector3& p1 = points[index1];
	const Vector3& p2 = points[index2];
	const Vector3& p3 = points[index3];

	return CatmullRomInterpolation(p0, p1, p2, p3, t2);
}

/// <summary>
/// ベジエ曲線を書く
/// </summary>
/// <param name="controlPoint">制御点がまとまった配列</param>
/// <returns></returns>
Vector3 Bezier(const std::vector<Vector3>& controlPoint, const float& t) {
	if (controlPoint.size() == 1) {
		return controlPoint[0];
	}

	std::vector<Vector3> lerpVec;
	for (size_t i = 0; i < controlPoint.size() - 1; ++i) {
		lerpVec.push_back(Lerp(controlPoint[i], controlPoint[i + 1], t));
	}

	return Bezier(lerpVec, t);
}

void DrawOBB(const OBB& obb, const Vector4& color) {
	// 回転行列を作成する
	Matrix4x4 rotateMatrix = obb.matRotate;
	// 平行移動分を作成
	Matrix4x4 matTranslate = MakeTranslateMatrix(obb.center);
	// 拡縮分
	Matrix4x4 matScale = MakeScaleMatrix({ 1.0f,1.0f, 1.0f });

	// ワールド行列を求める
	Matrix4x4 worldMat = matScale * rotateMatrix * matTranslate;

	// ローカルの頂点を求める
	Vector3 localVertex[8];
	Vector3 min = obb.size * -1;
	Vector3 max = obb.size;
	// 手前の面
	localVertex[0] = min;
	localVertex[1] = Vector3{ min.x, max.y, min.z };
	localVertex[2] = Vector3{ max.x, max.y , min.z };
	localVertex[3] = Vector3{ max.x, min.y , min.z };
	// 奥の面
	localVertex[4] = Vector3{ min.x, min.y, max.z };
	localVertex[5] = Vector3{ min.x, max.y, max.z };
	localVertex[6] = Vector3{ max.x, min.y, max.z };
	localVertex[7] = max;

	// スクリーンの頂点を求める
	Vector3 worldVertex[8];
	for (size_t index = 0; index < 8; index++) {
		worldVertex[index] = Transform(localVertex[index], worldMat);
	}

	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[0], worldVertex[1], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[0], worldVertex[3], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[0], worldVertex[4], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[1], worldVertex[2], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[1], worldVertex[5], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[2], worldVertex[3], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[2], worldVertex[7], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[3], worldVertex[6], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[4], worldVertex[5], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[4], worldVertex[6], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[5], worldVertex[7], color);
	PrimitiveDrawer::GetInstance()->DrawLine3d(worldVertex[6], worldVertex[7], color);
}

void OBB::MakeOBBAxis(const Vector3& rotate) {
	Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

	// 回転行列から軸を抽出
	orientations[0].x = rotateMatrix.m[0][0];
	orientations[0].y = rotateMatrix.m[0][1];
	orientations[0].z = rotateMatrix.m[0][2];

	orientations[1].x = rotateMatrix.m[1][0];
	orientations[1].y = rotateMatrix.m[1][1];
	orientations[1].z = rotateMatrix.m[1][2];

	orientations[2].x = rotateMatrix.m[2][0];
	orientations[2].y = rotateMatrix.m[2][1];
	orientations[2].z = rotateMatrix.m[2][2];

	matRotate = rotateMatrix;
}

std::vector<Vector3> OBB::MakeIndex() const {
	std::vector<Vector3> vertices;
	for (uint8_t x = 0; x < 2; ++x) {
		for (uint8_t y = 0; y < 2; ++y) {
			for (uint8_t z = 0; z < 2; ++z) {
				Vector3 localVertex = {
					(x ? size.x : -size.x),
					(y ? size.y : -size.y),
					(z ? size.z : -size.z),
				};

				Vector3 worldVertex = Transform(localVertex, matRotate);
				vertices.push_back(worldVertex + center);
			}
		}
	}

	return vertices;
}
