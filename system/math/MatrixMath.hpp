//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
// Copyright (c) 2022 sijh (s1Jh.199[at]gmail.com)
//                                      =*=
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//                                      =*=
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//                                      =*=
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.                            =*=
//=*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*==*=
#pragma once

#include "define.hpp"

#include "Matrix.hpp"
#include "Trigonometry.hpp"

namespace PROJECT_NAMESPACE::math
{

	/**********************
	** MATRIX OPERATIONS **
	**********************/

	template <typename T>
	Mat4<T> CreateProjection(float fov, float aspect_ratio, float nearz,
							 float farz)
	{
		float scale = Tan(fov * 0.5f) * nearz;
		float r = aspect_ratio * scale;
		float l = -r;
		float t = scale;
		float b = -t;

		Mat4<T> mat;
		mat.row_view(0)[0] = 2 * nearz / (r - l);
		mat.row_view(0)[1] = 0;
		mat.row_view(0)[2] = 0;
		mat.row_view(0)[3] = 0;

		mat.row_view(1)[0] = 0;
		mat.row_view(1)[1] = 2 * nearz / (t - b);
		mat.row_view(1)[2] = 0;
		mat.row_view(1)[3] = 0;

		mat.row_view(2)[0] = (r + l) / (r - l);
		mat.row_view(2)[1] = (t + b) / (t - b);
		mat.row_view(2)[2] = -(farz + nearz) / (farz - nearz);
		mat.row_view(2)[3] = -1;

		mat.row_view(3)[0] = 0;
		mat.row_view(3)[1] = 0;
		mat.row_view(3)[2] = -2 * farz * nearz / (farz - nearz);
		mat.row_view(3)[3] = 0;

		return mat;
	}

	template <typename T1, typename T2>
	Mat4<T1> LookAt(const vec3d<T2> &eye, const vec3d<T2> &target,
					const vec3d<T2> &up)
	{
		vec3d<T2> forward = Normalize(target - eye);
		vec3d<T2> left = Normalize(Cross(forward, up));
		vec3d<T2> upp = Cross(left, forward);

		Mat4<T1> mat;

		mat.column_view(0)[0] = left[0];
		mat.column_view(0)[1] = left[1];
		mat.column_view(0)[2] = left[2];
		mat.column_view(0)[3] = -Dot(left, eye);

		mat.column_view(1)[0] = upp[0];
		mat.column_view(1)[1] = upp[1];
		mat.column_view(1)[2] = upp[2];
		mat.column_view(1)[3] = -Dot(upp, eye);

		mat.column_view(2)[0] = forward[0];
		mat.column_view(2)[1] = forward[1];
		mat.column_view(2)[2] = forward[2];
		mat.column_view(2)[3] = -Dot(forward, eye);

		mat.column_view(3)[0] = 0;
		mat.column_view(3)[1] = 0;
		mat.column_view(3)[2] = 0;
		mat.column_view(3)[3] = 1;

		return mat;
	}

	template <typename T>
	T Det(const Mat2<T> &mat)
	{
		return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
	}

	template <typename T>
	T Det(const Mat3<T> &mat)
	{
		return mat[0][0] * mat[1][1] * mat[2][2] + mat[1][0] * mat[2][1] * mat[0][2] +
			   mat[2][0] * mat[0][1] * mat[1][2] - mat[0][0] * mat[2][1] * mat[1][2] -
			   mat[1][0] * mat[0][1] * mat[2][2] - mat[2][0] * mat[1][1] * mat[0][2];
	}

	template <typename T>
	T Det(const Mat4<T> &mat)
	{
		T s0 = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
		T s1 = mat[0][0] * mat[2][1] - mat[0][1] * mat[2][0];
		T s2 = mat[0][0] * mat[3][1] - mat[0][1] * mat[3][0];
		T s3 = mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0];
		T s4 = mat[1][0] * mat[3][1] - mat[1][1] * mat[3][0];
		T s5 = mat[2][0] * mat[3][1] - mat[2][1] * mat[3][0];

		T c5 = mat[2][2] * mat[3][3] - mat[2][3] * mat[3][2];
		T c4 = mat[1][2] * mat[3][3] - mat[1][3] * mat[3][2];
		T c3 = mat[1][2] * mat[2][3] - mat[1][3] * mat[2][2];
		T c2 = mat[0][2] * mat[3][3] - mat[0][3] * mat[3][2];
		T c1 = mat[0][2] * mat[2][3] - mat[0][3] * mat[2][2];
		T c0 = mat[0][2] * mat[1][3] - mat[0][3] * mat[1][2];

		return (s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0);
	}

	template <typename T>
	Mat2<T> Inverse(const Mat2<T> &mat)
	{
		Mat2<T> inv;

		T invdet = 1.0 / Det<T>(mat);

		inv[0][0] = mat[1][1] * invdet;
		inv[0][1] = mat[0][1] * -invdet;
		inv[1][0] = mat[1][0] * -invdet;
		inv[1][1] = mat[0][0] * invdet;

		return inv;
	}

	template <typename T>
	Mat3<T> Inverse(const Mat3<T> &mat)
	{
		Mat3<T> inv;

		T invdet = 1.0 / Det<T>(mat);

		inv[0][0] = (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1]) * invdet;
		inv[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * invdet;
		inv[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invdet;
		inv[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * invdet;
		inv[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invdet;
		inv[1][2] = (mat[0][2] * mat[1][0] - mat[0][0] * mat[1][2]) * invdet;
		inv[2][0] = (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]) * invdet;
		inv[2][1] = (mat[0][1] * mat[2][0] - mat[0][0] * mat[2][1]) * invdet;
		inv[2][2] = (mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0]) * invdet;

		return inv;
	}

	template <typename T>
	Mat4<T> Inverse(const Mat4<T> &mat)
	{
		Mat4<T> inv;

		T invdet = 1.0 / Det<T>(mat);

		inv[0][0] =
			(mat[1][2] * mat[2][3] * mat[3][1] - mat[1][3] * mat[2][2] * mat[3][1] +
			 mat[1][3] * mat[2][1] * mat[3][2] - mat[1][1] * mat[2][3] * mat[3][2] -
			 mat[1][2] * mat[2][1] * mat[3][3] + mat[1][1] * mat[2][2] * mat[3][3]) *
			invdet;
		inv[0][1] =
			(mat[0][3] * mat[2][2] * mat[3][1] - mat[0][2] * mat[2][3] * mat[3][1] -
			 mat[0][3] * mat[2][1] * mat[3][2] + mat[0][1] * mat[2][3] * mat[3][2] +
			 mat[0][2] * mat[2][1] * mat[3][3] - mat[0][1] * mat[2][2] * mat[3][3]) *
			invdet;
		inv[0][2] =
			(mat[0][2] * mat[1][3] * mat[3][1] - mat[0][3] * mat[1][2] * mat[3][1] +
			 mat[0][3] * mat[1][1] * mat[3][2] - mat[0][1] * mat[1][3] * mat[3][2] -
			 mat[0][2] * mat[1][1] * mat[3][3] + mat[0][1] * mat[1][2] * mat[3][3]) *
			invdet;
		inv[0][3] =
			(mat[0][3] * mat[1][2] * mat[2][1] - mat[0][2] * mat[1][3] * mat[2][1] -
			 mat[0][3] * mat[1][1] * mat[2][2] + mat[0][1] * mat[1][3] * mat[2][2] +
			 mat[0][2] * mat[1][1] * mat[2][3] - mat[0][1] * mat[1][2] * mat[2][3]) *
			invdet;
		inv[1][0] =
			(mat[1][3] * mat[2][2] * mat[3][0] - mat[1][2] * mat[2][3] * mat[3][0] -
			 mat[1][3] * mat[2][0] * mat[3][2] + mat[1][0] * mat[2][3] * mat[3][2] +
			 mat[1][2] * mat[2][0] * mat[3][3] - mat[1][0] * mat[2][2] * mat[3][3]) *
			invdet;
		inv[1][1] =
			(mat[0][2] * mat[2][3] * mat[3][0] - mat[0][3] * mat[2][2] * mat[3][0] +
			 mat[0][3] * mat[2][0] * mat[3][2] - mat[0][0] * mat[2][3] * mat[3][2] -
			 mat[0][2] * mat[2][0] * mat[3][3] + mat[0][0] * mat[2][2] * mat[3][3]) *
			invdet;
		inv[1][2] =
			(mat[0][3] * mat[1][2] * mat[3][0] - mat[0][2] * mat[1][3] * mat[3][0] -
			 mat[0][3] * mat[1][0] * mat[3][2] + mat[0][0] * mat[1][3] * mat[3][2] +
			 mat[0][2] * mat[1][0] * mat[3][3] - mat[0][0] * mat[1][2] * mat[3][3]) *
			invdet;
		inv[1][3] =
			(mat[0][2] * mat[1][3] * mat[2][0] - mat[0][3] * mat[1][2] * mat[2][0] +
			 mat[0][3] * mat[1][0] * mat[2][2] - mat[0][0] * mat[1][3] * mat[2][2] -
			 mat[0][2] * mat[1][0] * mat[2][3] + mat[0][0] * mat[1][2] * mat[2][3]) *
			invdet;
		inv[2][0] =
			(mat[1][1] * mat[2][3] * mat[3][0] - mat[1][3] * mat[2][1] * mat[3][0] +
			 mat[1][3] * mat[2][0] * mat[3][1] - mat[1][0] * mat[2][3] * mat[3][1] -
			 mat[1][1] * mat[2][0] * mat[3][3] + mat[1][0] * mat[2][1] * mat[3][3]) *
			invdet;
		inv[2][1] =
			(mat[0][3] * mat[2][1] * mat[3][0] - mat[0][1] * mat[2][3] * mat[3][0] -
			 mat[0][3] * mat[2][0] * mat[3][1] + mat[0][0] * mat[2][3] * mat[3][1] +
			 mat[0][1] * mat[2][0] * mat[3][3] - mat[0][0] * mat[2][1] * mat[3][3]) *
			invdet;
		inv[2][2] =
			(mat[0][1] * mat[1][3] * mat[3][0] - mat[0][3] * mat[1][1] * mat[3][0] +
			 mat[0][3] * mat[1][0] * mat[3][1] - mat[0][0] * mat[1][3] * mat[3][1] -
			 mat[0][1] * mat[1][0] * mat[3][3] + mat[0][0] * mat[1][1] * mat[3][3]) *
			invdet;
		inv[2][3] =
			(mat[0][3] * mat[1][1] * mat[2][0] - mat[0][1] * mat[1][3] * mat[2][0] -
			 mat[0][3] * mat[1][0] * mat[2][1] + mat[0][0] * mat[1][3] * mat[2][1] +
			 mat[0][1] * mat[1][0] * mat[2][3] - mat[0][0] * mat[1][1] * mat[2][3]) *
			invdet;
		inv[3][0] =
			(mat[1][2] * mat[2][1] * mat[3][0] - mat[1][1] * mat[2][2] * mat[3][0] -
			 mat[1][2] * mat[2][0] * mat[3][1] + mat[1][0] * mat[2][2] * mat[3][1] +
			 mat[1][1] * mat[2][0] * mat[3][2] - mat[1][0] * mat[2][1] * mat[3][2]) *
			invdet;
		inv[3][1] =
			(mat[0][1] * mat[2][2] * mat[3][0] - mat[0][2] * mat[2][1] * mat[3][0] +
			 mat[0][2] * mat[2][0] * mat[3][1] - mat[0][0] * mat[2][2] * mat[3][1] -
			 mat[0][1] * mat[2][0] * mat[3][2] + mat[0][0] * mat[2][1] * mat[3][2]) *
			invdet;
		inv[3][2] =
			(mat[0][2] * mat[1][1] * mat[3][0] - mat[0][1] * mat[1][2] * mat[3][0] -
			 mat[0][2] * mat[1][0] * mat[3][1] + mat[0][0] * mat[1][2] * mat[3][1] +
			 mat[0][1] * mat[1][0] * mat[3][2] - mat[0][0] * mat[1][1] * mat[3][2]) *
			invdet;
		inv[3][3] =
			(mat[0][1] * mat[1][2] * mat[2][0] - mat[0][2] * mat[1][1] * mat[2][0] +
			 mat[0][2] * mat[1][0] * mat[2][1] - mat[0][0] * mat[1][2] * mat[2][1] -
			 mat[0][1] * mat[1][0] * mat[2][2] + mat[0][0] * mat[1][1] * mat[2][2]) *
			invdet;

		return inv;
	}

	template <typename T>
	inline Mat3<T> MakeTranslationMatrix(const vec2d<T> &vec)
	{
		return Mat3<T>({1.f, 0.f, vec[0], 0.f, 1.f, vec[1], 0.f, 0.f, 1.f});
	}

	template <typename T>
	inline Mat3<T> MakeShearMatrix(const vec2d<T> &shear)
	{
		return Mat3<T>({1.f, std::tan(shear[0]), 0.f, std::tan(shear[1]), 1.f, 0.f, 0.f,
						0.f, 1.f});
	}

	template <typename T>
	inline Mat3<T> MakeRotationMatrix(float rotation,
									  fvec2d center = {0.0f, 0.0f})
	{
		auto rotMatrix =
			Mat3<T>({std::cos(rotation), -std::sin(rotation), 0.f, std::sin(rotation),
					 std::cos(rotation), 0.f, 0.f, 0.f, 1.f});
		return MakeTranslationMatrix(center * -1) * rotMatrix *
			   MakeTranslationMatrix(center);
	}

	template <typename T>
	inline Mat3<T> MakeScaleMatrix(const vec2d<T> &scale,
								   fvec2d center = {0.0f, 0.0f})
	{
		auto scaleMat = Mat3<T>({scale[0], 0.f, 0.f, 0.f, scale[1], 0.f, 0.f, 0.f, 1.f});

		return MakeTranslationMatrix(center * -1) * scaleMat *
			   MakeTranslationMatrix(center);
	}

	template <typename T, size_t I>
	T LaplaceExpansion(const SquareMatrix<T, I> &in)
	{
		T sum = 0;
		for (int i = I; i > 0; i--)
			for (int j = I; j > 0; j--)
			{
				SquareMatrix<T, I - 1> lower_order;

				sum += std::pow(-1, i + j) * in[i][j] * LaplaceExpansion(lower_order);
			}
	}

}
