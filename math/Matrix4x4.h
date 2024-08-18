#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];

	//*======================================================================
	//	加算
	//*======================================================================
	Matrix4x4 operator+(const Matrix4x4& obj) const {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				result.m[row][col] = this->m[row][col] + obj.m[row][col];
			}
		}

		return result;
	}

	//*======================================================================
	//	減算
	//*======================================================================
	Matrix4x4 operator-(const Matrix4x4& obj) const {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				result.m[row][col] = this->m[row][col] - obj.m[row][col];
			}
		}

		return result;
	}

	//*======================================================================
	//	掛算
	//*======================================================================
	Matrix4x4 operator*(const Matrix4x4& obj) const {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				for (int oi = 0; oi < 4; oi++) {
					result.m[row][col] += this->m[row][oi] * obj.m[oi][col];
				}
			}
		}

		return result;
	}
};
