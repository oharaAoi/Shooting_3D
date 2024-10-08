#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "MyMatrix4x4.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cassert>
#include <algorithm>
#include <vector>
#include <numbers>

/// <summary>
/// OBB(有向境界箱)
/// </summary>
struct OBB {
	Vector3 center; // 中心点
	Vector3 orientations[3]; // 座標軸、正規化、直交必須
	Vector3 size; // 座標軸方向の長さの半分

	// 回転軸
	Matrix4x4 matRotate;

	void MakeOBBAxis(const Vector3& rotate);
	std::vector<Vector3> MakeIndex() const;
};

/// <summary>
/// 加算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Add(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 減算
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3  Subtract(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 掛け算
/// </summary>
/// <param name="scalar"></param>
/// <param name="v"></param>
/// <returns></returns>
Vector3 Multiply(float scalar, const Vector3& v);

/// <summary>
/// ベクトルの距離を測る
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Distance(const Vector3& v1, const Vector3& v2);

/// <summary>
/// 内積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
float Dot(const Vector3& v1, const Vector3& v2);

/// <summary>
/// クロス積
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <returns></returns>
Vector3 Cross(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& vec3);
float Length(const Vector2& vec2);

Vector3 Normalize(const Vector3& vec3);
Vector2 Normalize(const Vector2& vec2);

/// <summary>
/// ベクトル変換
/// </summary>
/// <param name="v"></param>
/// <param name="m"></param>
/// <returns></returns>
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

////////////////////////////////////////////////////////////////////////////////////
// 線形補完
////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 線形補完
/// </summary>
/// <param name="v1"></param>
/// <param name="v2"></param>
/// <param name="t"></param>
/// <returns></returns>
Vector3 Slerp(const Vector3& v1, const Vector3& v2, float t);

/// <summary>
/// 最短角度を求める
/// </summary>
/// <param name="a"></param>
/// <param name="b"></param>
/// <param name="t"></param>
/// <returns></returns>
float LerpShortAngle(const float& originalAngle, const float& targetAngle, const float& t);

/// <summary>/// 
/// 角度を求める
/// </summary>///
///  <param name="v1"></param>/// 
/// <param name="v2"></param>///
///  <returns></returns>
float Angle(const Vector3& v1, const Vector3& v2);

/// <summary>/// 
/// CatmullRom補完
/// </summary>/// 
/// <param name="p0">点0の座標</param>/// 
/// <param name="p1">点1の座標</param>/// 
/// <param name="p2">点2の座標</param>/// 
/// <param name="p3">点3の座標</param>/// 
/// <param name="t">点1と点2の間で指定された座標</param>/// 
/// <returns>点1と点2の間で指定された座標</returns>
Vector3 CatmullRomInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t);

/// <summary>/// 
/// CatmullRomスプライン曲線上の座標を得る
/// </summary>///
/// <param name="points">制御点の集合</param>/// 
/// <param name="t">スプライン全区間の中で割合指定</param>/// 
/// <returns>座標</returns>
Vector3 CatmullRomPosition(const std::vector<Vector3>& points, float t);

/// <summary>
/// ベジエ曲線を書く
/// </summary>
///  <param name="controlPoint">制御点がまとまった配列</param>
///  <returns></returns>
Vector3 Bezier(const std::vector<Vector3>& controlPoint, const float& t);

////////////////////////////////////////////////////////////////////////////////////
// 描画
////////////////////////////////////////////////////////////////////////////////////

/// <summary>
/// 
/// </summary>
/// <param name="obb"></param>
/// <param name="viewProjection"></param>
/// <param name="viewportMatrix"></param>
/// <param name="color"></param>
void DrawOBB(const OBB& obb, const Vector4& color);