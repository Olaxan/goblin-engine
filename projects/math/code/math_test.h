#pragma once
#include <cstdio>

#include "matrix4.h"
#include "vector4.h"
#include "glm/glm.hpp"
#include "glm/ext.hpp"

using namespace efiilj;

static void MathTest() {
	bool test_vectors = true;
	bool test_Matrices = true;
	bool test_Inverse = true;
	bool test_Clip = true;
	bool test_View = true;

	glm::mat4 mat1_glm; glm::mat4 mat2_glm; glm::mat4 mat3_glm;  glm::mat4 mat4_glm;

	matrix4 mat1; matrix4 mat2; matrix4 mat3; matrix4 mat4;

	glm::vec4 vec1_glm; glm::vec4 vec2_glm;
	vector4 vec1; vector4 vec2; vector4 vec3;
	int failures = 0;
	int tests = 4;

	for (int i = 0; i < tests; i++) {
		printf("\n    Test %d:", i + 1);
		printf("\n----------------------------------------------------------------------------\n");
		if (test_vectors) {
			for (int i = 0; i < 4; i++) { float r = fmodf((float)rand(), 15.0f); vec1[i] = r; vec1_glm[i] = r; }
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f); mat1[i] = r; mat1_glm[i / 4][i % 4] = r; }
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[0], mat1[1], mat1[2], mat1[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[4], mat1[5], mat1[6], mat1[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[8], mat1[9], mat1[10], mat1[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[12], mat1[13], mat1[14], mat1[15]);
			printf("                       X\n");
			printf("you: (%.2f, % 6.2f, % 6.2f, %6.2f)\n", vec1[0], vec1[1], vec1[2], vec1[3]);
			vec2 = mat1 * vec1;
			printf("                       =\n");
			printf("you: (%.2f, % 6.2f, % 6.2f, %6.2f)\n", vec2[0], vec2[1], vec2[2], vec2[3]);
			printf("----------------------------------------------------------------------------\n\n");

			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[0][0], mat1_glm[0][1], mat1_glm[0][2], mat1_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[1][0], mat1_glm[1][1], mat1_glm[1][2], mat1_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[2][0], mat1_glm[2][1], mat1_glm[2][2], mat1_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[3][0], mat1_glm[3][1], mat1_glm[3][2], mat1_glm[3][3]);
			printf("                       X\n");
			printf("glm: (%.2f, % 6.2f, % 6.2f, %6.2f)\n", vec1_glm[0], vec1_glm[1], vec1_glm[2], vec1_glm[3]);
			vec2_glm = mat1_glm * vec1_glm;
			printf("                       =\n");
			printf("glm: (%.2f, % 6.2f, % 6.2f, %6.2f)\n", vec2_glm[0], vec2_glm[1], vec2_glm[2], vec2_glm[3]);
			printf("----------------------------------------------------------------------------\n");
			for (int i = 0; i < 4; i++) { if (std::fabs(vec2[i] - vec2_glm[i]) > 0.001f) { failures++; printf("\n   FAILED!\n\n"); break; } }
		}
		if (test_Matrices) {
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f); mat1[i] = r; mat1_glm[i / 4][i % 4] = r; }
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f); mat2[i] = r; mat2_glm[i / 4][i % 4] = r; }
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f); mat3[i] = r; mat3_glm[i / 4][i % 4] = r; }

			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[0], mat1[1], mat1[2], mat1[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[4], mat1[5], mat1[6], mat1[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[8], mat1[9], mat1[10], mat1[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[12], mat1[13], mat1[14], mat1[15]);
			printf("                       X\n");
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2[0], mat2[1], mat2[2], mat2[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2[4], mat2[5], mat2[6], mat2[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2[8], mat2[9], mat2[10], mat2[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2[12], mat2[13], mat2[14], mat2[15]);
			printf("                       =\n");
			mat3 = mat2 * mat1;
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[0], mat3[1], mat3[2], mat3[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[4], mat3[5], mat3[6], mat3[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[8], mat3[9], mat3[10], mat3[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[12], mat3[13], mat3[14], mat3[15]);
			printf("                       = (reverse multiplication order)\n");
			mat4 = mat1 * mat2;
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4[0], mat4[1], mat4[2], mat4[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4[4], mat4[5], mat4[6], mat4[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4[8], mat4[9], mat4[10], mat4[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4[12], mat4[13], mat4[14], mat4[15]);

			printf("----------------------------------------------------------------------------\n\n");

			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[0][0], mat1_glm[0][1], mat1_glm[0][2], mat1_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[1][0], mat1_glm[1][1], mat1_glm[1][2], mat1_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[2][0], mat1_glm[2][1], mat1_glm[2][2], mat1_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[3][0], mat1_glm[3][1], mat1_glm[3][2], mat1_glm[3][3]);
			printf("                       X\n");
			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2_glm[0][0], mat2_glm[0][1], mat2_glm[0][2], mat2_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2_glm[1][0], mat2_glm[1][1], mat2_glm[1][2], mat2_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2_glm[2][0], mat2_glm[2][1], mat2_glm[2][2], mat2_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat2_glm[3][0], mat2_glm[3][1], mat2_glm[3][2], mat2_glm[3][3]);
			printf("                       =\n");
			mat3_glm = mat2_glm * mat1_glm;
			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[0][0], mat3_glm[0][1], mat3_glm[0][2], mat3_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[1][0], mat3_glm[1][1], mat3_glm[1][2], mat3_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[2][0], mat3_glm[2][1], mat3_glm[2][2], mat3_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[3][0], mat3_glm[3][1], mat3_glm[3][2], mat3_glm[3][3]);
			printf("                       = (reverse multiplication order)\n");
			mat4_glm = mat1_glm * mat2_glm;
			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4_glm[0][0], mat4_glm[0][1], mat4_glm[0][2], mat4_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4_glm[1][0], mat4_glm[1][1], mat4_glm[1][2], mat4_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4_glm[2][0], mat4_glm[2][1], mat4_glm[2][2], mat4_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat4_glm[3][0], mat4_glm[3][1], mat4_glm[3][2], mat4_glm[3][3]);
			printf("----------------------------------------------------------------------------\n");
			for (int i = 0; i < 16; i++) { if (std::fabs(mat3[i] - mat3_glm[i / 4][i % 4]) > 0.001f) { failures++; printf("\n   FAILED!\n"); break; } }
			for (int i = 0; i < 16; i++) { if (std::fabs(mat4[i] - mat4_glm[i / 4][i % 4]) > 0.001f) { failures++; printf("\n   FAILED, reverse order!\n\n"); break; } }
		}
		if (test_Inverse) {
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f) + 1.0f; mat1[i] = r; mat1_glm[i / 4][i % 4] = r; }

			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[0], mat1[1], mat1[2], mat1[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[4], mat1[5], mat1[6], mat1[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[8], mat1[9], mat1[10], mat1[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1[12], mat1[13], mat1[14], mat1[15]);
			printf("                       I=\n");
			mat3 = mat1.inverse();
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[0], mat3[1], mat3[2], mat3[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[4], mat3[5], mat3[6], mat3[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[8], mat3[9], mat3[10], mat3[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[12], mat3[13], mat3[14], mat3[15]);
			printf("----------------------------------------------------------------------------\n\n");

			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[0][0], mat1_glm[0][1], mat1_glm[0][2], mat1_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[1][0], mat1_glm[1][1], mat1_glm[1][2], mat1_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[2][0], mat1_glm[2][1], mat1_glm[2][2], mat1_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat1_glm[3][0], mat1_glm[3][1], mat1_glm[3][2], mat1_glm[3][3]);
			printf("                       I=\n");
			mat3_glm = glm::inverse(mat1_glm);
			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[0][0], mat3_glm[0][1], mat3_glm[0][2], mat3_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[1][0], mat3_glm[1][1], mat3_glm[1][2], mat3_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[2][0], mat3_glm[2][1], mat3_glm[2][2], mat3_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[3][0], mat3_glm[3][1], mat3_glm[3][2], mat3_glm[3][3]);
			printf("----------------------------------------------------------------------------\n");
			for (int i = 0; i < 16; i++) { if (std::fabs(mat3[i] - mat3_glm[i / 4][i % 4]) > 0.001f) { failures++; printf("\n   FAILED!\n"); break; } }
		}
		if (test_Clip) {
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f) + 1.0f; mat1[i] = r; mat1_glm[i / 4][i % 4] = r; }
			printf("\nClip space projection\n\n");
			mat3 = mat3.get_perspective(1.6f, 4.0f/3.0f, 0.1f, 30.0f);
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[0], mat3[1], mat3[2], mat3[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[4], mat3[5], mat3[6], mat3[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[8], mat3[9], mat3[10], mat3[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[12], mat3[13], mat3[14], mat3[15]);
			printf("----------------------------------------------------------------------------\n\n");

			mat3_glm = glm::perspectiveFov(1.6f, 4.0f, 3.0f, 0.1f, 30.0f);
			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[0][0], mat3_glm[0][1], mat3_glm[0][2], mat3_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[1][0], mat3_glm[1][1], mat3_glm[1][2], mat3_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[2][0], mat3_glm[2][1], mat3_glm[2][2], mat3_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[3][0], mat3_glm[3][1], mat3_glm[3][2], mat3_glm[3][3]);
			printf("----------------------------------------------------------------------------\n");
			for (int i = 0; i < 16; i++) { if (std::fabs(mat3[i] - mat3_glm[i / 4][i % 4]) > 0.001f) { failures++; printf("\n   FAILED!\n"); break; } }
		}
		if (test_View) {
			for (int i = 0; i < 16; i++) { float r = fmodf((float)rand(), 15.0f) + 1.0f; mat1[i] = r; mat1_glm[i / 4][i % 4] = r; }
			printf("\nView transform\n\n");
			mat3 = mat3.get_lookat(vector3(4.0f, 2.0f, 1.0f), vector3(-5.0f, -2.0f, 4.0f), vector3(0.0f, 1.0f, 0.0f));
			printf("you: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[0], mat3[1], mat3[2], mat3[3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[4], mat3[5], mat3[6], mat3[7]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[8], mat3[9], mat3[10], mat3[11]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3[12], mat3[13], mat3[14], mat3[15]);
			printf("----------------------------------------------------------------------------\n\n");

			mat3_glm = glm::lookAt(glm::vec3(4.0f, 2.0f, 1.0f), glm::vec3(-5.0f, -2.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			printf("glm: | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[0][0], mat3_glm[0][1], mat3_glm[0][2], mat3_glm[0][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[1][0], mat3_glm[1][1], mat3_glm[1][2], mat3_glm[1][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[2][0], mat3_glm[2][1], mat3_glm[2][2], mat3_glm[2][3]);
			printf("     | % 6.2f, % 6.2f, % 6.2f, % 6.2f  |\n", mat3_glm[3][0], mat3_glm[3][1], mat3_glm[3][2], mat3_glm[3][3]);
			printf("----------------------------------------------------------------------------\n");
			for (int i = 0; i < 16; i++) { if (std::fabs(mat3[i] - mat3_glm[i / 4][i % 4]) > 0.001f) { failures++; printf("\n   FAILED!\n"); break; } }
		}

	}
	int n = ((int)test_vectors + (int)test_Matrices * 2 + (int)test_Inverse) * tests;
	if (failures > 0) {
		printf("----------------------------------------------------------------------------\n\n");
		printf("\n TEST FAILED! With %d out of %d tests passed!\n", n - failures, n);
	}
	else {
		printf("----------------------------------------------------------------------------\n\n");
		printf("\n TEST SUCCEEDED! With %d out of %d tests passed!\n", n - failures, n);
	}

}
