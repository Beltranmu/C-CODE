#include "matrix_2.h"
#include "matrix_3.h"
#include "matrix_4.h"
#include "UnitTestHelpers.h"

// Code by Diego Llorens and Lucas Gonzalez

namespace UnitTest
{
  void Matrix2x2Test() {

    Test::Start(__FUNCTION__);
    Matrix2x2 test;
    Matrix2x2 another_test;
    bool worked = false;

    //Check Identity
    test = test.Identity();
    another_test = Matrix2x2(Vector2(1.0f, 0.0f), Vector2(0.0f, 1.0f));

    Assert::AreEqual(another_test.m[0], test.m[0], "Identity() Failed 0", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[1], test.m[1], "Identity() Failed 1", __FILE__, __LINE__);

    Assert::AreEqual(another_test.m[2], test.m[2], "Identity() Failed 2", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[3], test.m[3], "Identity() Failed 3", __FILE__, __LINE__);

    //Check Multiply
    test = Matrix2x2(Vector2(3.0f, 3.0f), Vector2(3.0f, 3.0f));
    another_test = another_test.Multiply(test);

    Assert::AreEqual(another_test.m[0], test.m[0], "Multiply() Failed 0", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[1], test.m[1], "Multiply() Failed 1", __FILE__, __LINE__);

    Assert::AreEqual(another_test.m[2], test.m[2], "Multiply() Failed 2", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[3], test.m[3], "Multiply() Failed 3", __FILE__, __LINE__);

    //Check Determinant
    worked = 0.0f == test.Determinant();
    Assert::AreEqual(true, worked, "Determinant() Failed", __FILE__, __LINE__);
    test = Matrix2x2(Vector2(7.0f, 4.0f), Vector2(6.0f, 11.0f));
    worked = 53.0f == test.Determinant();
    Assert::AreEqual(true, worked, "Determinant() Failed", __FILE__, __LINE__);

    //Check Transpose
    float ttemp[4]{ 7.0f, 4.0f,
                    6.0f, 11.0f };

    float atemp[4]{ 7.0f, 6.0f,
                    4.0f, 11.0f };

    test = Matrix2x2(ttemp);
    another_test = Matrix2x2(atemp);

    test = test.Transpose();

    Assert::AreEqual(another_test.m[0], test.m[0], "Transpose() Failed 0", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[1], test.m[1], "Transpose() Failed 1", __FILE__, __LINE__);

    Assert::AreEqual(another_test.m[2], test.m[2], "Transpose() Failed 2", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[3], test.m[3], "Transpose() Failed 3", __FILE__, __LINE__);

    //Check Adjoint
    float temp[4]{ 2.0f, -1.0f,
                    7.0f, 3.0f };

    float ad_temp[4]{ 3.0f, 1.0f,
                      -7.0f, 2.0f };

    another_test = Matrix2x2(ad_temp);

    test = Matrix2x2(temp);

    test = test.Adjoint().Transpose();

    Assert::AreEqual(another_test.m[0], test.m[0], "Adjoint() Failed 0", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[1], test.m[1], "Adjoint() Failed 1", __FILE__, __LINE__);

    Assert::AreEqual(another_test.m[2], test.m[2], "Adjoint() Failed 2", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[3], test.m[3], "Adjoint() Failed 3", __FILE__, __LINE__);

    //Check Inverse

    float b_temp[4]{ 2.0f, 5.0f,
                    1.0f, 3.0f };

    float inv_temp[4]{ 3.0f, -5.0f,
                    -1.0f, 2.0f };

    another_test = Matrix2x2(inv_temp);
    Matrix2x2 bb_temp = Matrix2x2(b_temp);

    test = bb_temp.Inverse();

    Assert::AreEqual(another_test.m[0], test.m[0], 0.00000001f, "Inverse() Failed 0", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[1], test.m[1], 0.00000001f, "Inverse() Failed 1", __FILE__, __LINE__);

    Assert::AreEqual(another_test.m[2], test.m[2], 0.00000001f, "Inverse() Failed 2", __FILE__, __LINE__);
    Assert::AreEqual(another_test.m[3], test.m[3], 0.00000001f, "Inverse() Failed 3", __FILE__, __LINE__);

    //Check GetLine
    test = test.Identity();

    worked = test.GetLine(0) == Vector2(1.0f, 0.0f);
    Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
    worked = test.GetLine(1) == Vector2(0.0f, 1.0f);
    Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);

    //Check GetColum

    worked = test.GetColum(0) == Vector2(1.0f, 0.0f);
    Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);
    worked = test.GetColum(1) == Vector2(0.0f, 1.0f);
    Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);

    //Check Multiply

    float multa_temp[4]{ 2.0f, 5.0f,
                        1.0f, 3.0f };

    float multb_temp[4]{ 9.0f, 25.0f,
                         5.0f, 14.0f };

    Matrix2x2 matrix_a = Matrix2x2(multa_temp);

    Matrix2x2 matrix_b = Matrix2x2(multb_temp);

    matrix_a = matrix_a.Multiply(matrix_a);

    Assert::AreEqual(matrix_b.m[0], matrix_a.m[0], 0.00000001f, "Multiply() Failed 0", __FILE__, __LINE__);
    Assert::AreEqual(matrix_b.m[1], matrix_a.m[1], 0.00000001f, "Multiply() Failed 1", __FILE__, __LINE__);

    Assert::AreEqual(matrix_b.m[2], matrix_a.m[2], 0.00000001f, "Multiply() Failed 2", __FILE__, __LINE__);
    Assert::AreEqual(matrix_b.m[3], matrix_a.m[3], 0.00000001f, "Multiply() Failed 3", __FILE__, __LINE__);

    Test::End();

  }


	void Matrix3x3Test() {
		
		Test::Start(__FUNCTION__);
		Matrix3x3 test;
		Matrix3x3 another_test;
		bool worked = false;

		//Check Identity
		test = test.Identity();
		another_test = Matrix3x3(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f));

		Assert::AreEqual(another_test.m[0], test.m[0], "Identity() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], "Identity() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], "Identity() Failed 2", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[3], test.m[3], "Identity() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], "Identity() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], "Identity() Failed 5", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[6], test.m[6], "Identity() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], "Identity() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], "Identity() Failed 8", __FILE__, __LINE__);



		//Check Multiply
		test = Matrix3x3(Vector3(3.0f, 3.0f, 3.0f), Vector3(3.0f, 3.0f, 3.0f), Vector3(3.0f, 3.0f, 3.0f));
		another_test = another_test.Multiply(test);

		Assert::AreEqual(another_test.m[0], test.m[0], "Multiply() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], "Multiply() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], "Multiply() Failed 2", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[3], test.m[3], "Multiply() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], "Multiply() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], "Multiply() Failed 5", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[6], test.m[6], "Multiply() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], "Multiply() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], "Multiply() Failed 8", __FILE__, __LINE__);

		//Check Determinant
		worked = 0.0f == test.Determinant();
		Assert::AreEqual(true, worked, "Determinant() Failed", __FILE__, __LINE__);
		test = Matrix3x3(Vector3(1.0f, 8.0f, 3.0f), Vector3(6.0f, 5.0f, 4.0f), Vector3(7.0f, 2.0f, 9.0f));
		worked = -240.0f == test.Determinant();
		Assert::AreEqual(true, worked, "Determinant() Failed", __FILE__, __LINE__);

		//Check Transpose
		float ttemp[9]{ 1.0f, 2.0f, 3.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 1.0f };

		float atemp[9]{ 1.0f, 0.0f, 0.0f,
						2.0f, 1.0f, 0.0f,
						3.0f, 0.0f, 1.0f };

		test = Matrix3x3(ttemp);
		another_test = Matrix3x3(atemp);

		test = test.Transpose();

		Assert::AreEqual(another_test.m[0], test.m[0], "Transpose() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], "Transpose() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], "Transpose() Failed 2", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[3], test.m[3], "Transpose() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], "Transpose() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], "Transpose() Failed 5", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[6], test.m[6], "Transpose() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], "Transpose() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], "Transpose() Failed 8", __FILE__, __LINE__);

		//Check Adjoint
		another_test = Matrix3x3(Vector3(37.0f, -66.0f, 17.0f), Vector3(-26.0f, -12.0f, 14.0f), Vector3(-23.0f, 54.0f, -43.0f));

		float temp[9]{ 2.0f, 5.0f, 4.0f,
					   3.0f, 5.0f, 7.0f,
					   0.0f, 2.0f, 1.0f };

		float ad_temp[9]{ -9.0f, 3.0f, 15.0f,
						  -3.0f, 2.0f, -2.0f,
						   6.0f,-4.0f, -5.0f };

		another_test = Matrix3x3(ad_temp);

		test = Matrix3x3(temp);
		test = test.Adjoint().Transpose();

		Assert::AreEqual(another_test.m[0], test.m[0], "Adjoint() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], "Adjoint() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], "Adjoint() Failed 2", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[3], test.m[3], "Adjoint() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], "Adjoint() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], "Adjoint() Failed 5", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[6], test.m[6], "Adjoint() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], "Adjoint() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], "Adjoint() Failed 8", __FILE__, __LINE__);

		//Check Inverse


		float itemp[9]{ 1.0f         ,-0.333333333333f,-1.6666666666666f,
					  0.333333333333f,-0.222222222222f, 0.2222222222222f,
					  -0.66666666666f, 0.444444444444f, 0.5555555555555f };



		another_test = Matrix3x3(itemp);
		test = Matrix3x3(temp);

		test.Inverse();

		Assert::AreEqual(another_test.m[0], test.m[0], 0.00000001f, "Inverse() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00000001f, "Inverse() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00000001f, "Inverse() Failed 2", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[3], test.m[3], 0.00000001f, "Inverse() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00000001f, "Inverse() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00000001f, "Inverse() Failed 5", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[6], test.m[6], 0.00000001f, "Inverse() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00000001f, "Inverse() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00000001f, "Inverse() Failed 8", __FILE__, __LINE__);

		//Check Translate
		another_test = Matrix3x3(Vector3(1.0f, 0.0f, 3.0f), Vector3(0.0f, 1.0f, 3.0f), Vector3(0.0f, 0.0f, 1.0f));
		test = test.Translate(3.0f, 3.0f);
		/*worked = another_test == test.Translate(3.0f, 3.0f);
		Assert::AreEqual(true, worked, "Translate() Failed", __FILE__, __LINE__);*/

		Assert::AreEqual(another_test.m[0], test.m[0], 0.00001f, "Translate() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00001f, "Translate() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00001f, "Translate() Failed 2", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[3], test.m[3], 0.00001f, "Translate() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00001f, "Translate() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00001f, "Translate() Failed 5", __FILE__, __LINE__);

		Assert::AreEqual(another_test.m[6], test.m[6], 0.00001f, "Translate() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00001f, "Translate() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00001f, "Translate() Failed 8", __FILE__, __LINE__);


		//Check GetColum
		test = test.Identity();
		worked = test.GetColum(0) == Vector3(1.0f, 0.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);
		worked = test.GetColum(1) == Vector3(0.0f, 1.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);
		worked = test.GetColum(2) == Vector3(0.0f, 0.0f, 1.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);

		//Check GetLine
		test = test.Identity();
		worked = test.GetLine(0) == Vector3(1.0f, 0.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
		worked = test.GetLine(1) == Vector3(0.0f, 1.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
		worked = test.GetLine(2) == Vector3(0.0f, 0.0f, 1.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
		Test::End();

	}

	void Matrix4x4Test() {

		Test::Start(__FUNCTION__);
		Matrix4x4 test;
		float temp[16]{ 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 0.0f, 1.0f };
		Matrix4x4 another_test = Matrix4x4(temp);
		bool worked = false;

		//Check Identity
		worked = test.Identity() == another_test;
		Assert::AreEqual(true, worked, "Identity() Failed", __FILE__, __LINE__);

		//Check Multiply
    for (int i = 0; i <= 15; ++i) { temp[i] = 3.0f; }
    another_test = Matrix4x4(temp);
    test = Matrix4x4(3.0f);
    test = test.Multiply(test.Identity());
		for (int i = 0; i <= 15; ++i) {
			Assert::AreEqual(test.m[i], another_test.m[i], "Multiply() Failed", __FILE__, __LINE__);
		}

		//Check Determinant
		worked = 0.0f == test.Determinant();
		Assert::AreEqual(true, worked, "Determinant() Failed", __FILE__, __LINE__);
		float det_temp[16]{ 4.0f, 5.0f, 8.0f, 2.0f,
							7.0f, 6.0f, 5.0f, 6.0f,
							8.0f, 7.0f, 8.0f, 7.0f,
							1.0f, 2.0f, 4.0f, 2.0f };
		test = Matrix4x4(det_temp);
		worked = -45.0f == test.Determinant();
		Assert::AreEqual(true, worked, "Determinant() Failed", __FILE__, __LINE__);

		//Check Adjoint
		float adj_temp[16]{ 0.0f, -1.0f, 0.0f, 5.0f,
							3.0f, -2.0f, 7.0f, 0.0f,
							-2.0f, 7.0f, -2.0f, -5.0f,
							9.0f, -8.0f, 3.0f, -2.0f };
		another_test = Matrix4x4(adj_temp);
		test = another_test.Adjoint();


		Assert::AreEqual(340.0f, test.m[0], 0.00001f, "Adjoint() Failed 0", __FILE__, __LINE__);
		Assert::AreEqual(286.0f, test.m[1], 0.00001f, "Adjoint() Failed 1", __FILE__, __LINE__);
		Assert::AreEqual(-64.0f, test.m[2], 0.00001f, "Adjoint() Failed 2", __FILE__, __LINE__);
		Assert::AreEqual(290.0f, test.m[3], 0.00001f, "Adjoint() Failed 3", __FILE__, __LINE__);
		Assert::AreEqual(-6.0f, test.m[4], 0.00001f, "Adjoint() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(60.0f, test.m[5], 0.00001f, "Adjoint() Failed 5", __FILE__, __LINE__);
		Assert::AreEqual(186.0f, test.m[6], 0.00001f, "Adjoint() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(12.0f, test.m[7], 0.00001f, "Adjoint() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(264.0f, test.m[8], 0.00001f, "Adjoint() Failed 8", __FILE__, __LINE__);
		Assert::AreEqual(270.0f, test.m[9], 0.00001f, "Adjoint() Failed 9", __FILE__, __LINE__);
		Assert::AreEqual(-36.0f, test.m[10], 0.00001f, "Adjoint() Failed 10", __FILE__, __LINE__);
		Assert::AreEqual(54.0f, test.m[11], 0.00001f, "Adjoint() Failed 11", __FILE__, __LINE__);
		Assert::AreEqual(190.0f, test.m[12], 0.00001f, "Adjoint() Failed 12", __FILE__, __LINE__);
		Assert::AreEqual(40.0f, test.m[13], 0.00001f, "Adjoint() Failed 13", __FILE__, __LINE__);
		Assert::AreEqual(-70.0f, test.m[14], 0.00001f, "Adjoint() Failed 14", __FILE__, __LINE__);
		Assert::AreEqual(8.0f, test.m[15], 0.00001f, "Adjoint() Failed 15", __FILE__, __LINE__);

		//Check Transpose
		float trans_temp[16]{ 0.0f, -3.0f,  0.0f, 0.0f,
							 -0.0f,  0.0f,  0.0f, 0.0f,
							  1.0f,  0.0f, -0.0f, 0.0f,
							  15.0f, 0.0f, -0.0f, 0.0f };

		float t_temp[16]{ 0.0f, -0.0f,  1.0f, 15.0f,
							  -3.0f, 0.0f,  0.0f, 0.0f,
							  0.0f,  0.0f, -0.0f, 0.0f,
							  0.0f, 0.0f, -0.0f, 0.0f };
		another_test = Matrix4x4(trans_temp);
		another_test = another_test.Transpose();
		test = Matrix4x4(t_temp);
		for (int i = 0; i < 16; ++i) {
			Assert::AreEqual(another_test.m[i], test.m[i], "Transpose() Failed", __FILE__, __LINE__);
		}


		//Check Inverse

		float inv_emp[16]{ 2.0f,  0.0f,  2.0f, 4.0f,
						   6.0f,  0.0f,  0.0f, 1.0f,
						   0.0f,  0.0f,  2.0f, 4.0f,
						   0.0f,  3.0f,  0.0f, 0.0f };

		float inv_tmp[16]{ 0.5f, -0.0f,  -0.5f, 0.0f,
						   0.0f,   0.0f,  0.0f, 0.333333f,
						   6.0f,  -2.0f, -5.5f, 0.0f,
						   -3.0f,  1.0f,  3.0f, 0.0f };

		another_test = Matrix4x4(inv_tmp);
		test = Matrix4x4(inv_emp);
		Matrix4x4 more = test.Adjoint().Transpose();
		Assert::AreEqual(-6, more.m[0], 0.00001f, "Adjoint2 Failed 0 ", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[1], 0.00001f, "Adjoint2  Failed 1 ", __FILE__, __LINE__);
		Assert::AreEqual(6, more.m[2], 0.00001f, "Adjoint2  Failed 2 ", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[3], 0.00001f, "Adjoint2  Failed 3 ", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[4], 0.00001f, "Adjoint2  Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[5], 0.00001f, "Adjoint2  Failed 5", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[6], 0.00001f, "Adjoint2  Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(-4, more.m[7], 0.00001f, "Adjoint2  Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(-72, more.m[8], 0.00001f, "Adjoint2  Failed 8", __FILE__, __LINE__);
		Assert::AreEqual(24, more.m[9], 0.00001f, "Adjoint2  Failed 9", __FILE__, __LINE__);
		Assert::AreEqual(66, more.m[10], 0.00001f, "Adjoint2  Failed 10", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[11], 0.00001f, "Adjoint2  Failed 11", __FILE__, __LINE__);
		Assert::AreEqual(36, more.m[12], 0.00001f, "Adjoint2  Failed 12", __FILE__, __LINE__);
		Assert::AreEqual(-12, more.m[13], 0.00001f, "Adjoint2  Failed 13", __FILE__, __LINE__);
		Assert::AreEqual(-36, more.m[14], 0.00001f, "Adjoint2  Failed 14", __FILE__, __LINE__);
		Assert::AreEqual(0, more.m[15], 0.00001f, "Adjoint2  Failed 15", __FILE__, __LINE__);


		test.Inverse();


		Assert::AreEqual(another_test.m[0], test.m[0], 0.00001f, "Inverse Failed 0 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00001f, "Inverse Failed 1 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00001f, "Inverse Failed 2 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[3], test.m[3], 0.00001f, "Inverse Failed 3 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00001f, "Inverse Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00001f, "Inverse Failed 5", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[6], test.m[6], 0.00001f, "Inverse Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00001f, "Inverse Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00001f, "Inverse Failed 8", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[9], test.m[9], 0.00001f, "Inverse Failed 9", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[10], test.m[10], 0.00001f, "Inverse Failed 10", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[11], test.m[11], 0.00001f, "Inverse Failed 11", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[12], test.m[12], 0.00001f, "Inverse Failed 12", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[13], test.m[13], 0.00001f, "Inverse Failed 13", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[14], test.m[14], 0.00001f, "Inverse Failed 14", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[15], test.m[15], 0.00001f, "Inverse Failed 15", __FILE__, __LINE__);


		//Check Translate
		float translate_temp[16]{ 1.0f, 0.0f, 0.0f, 3.0f,
								  0.0f, 1.0f, 0.0f, 3.0f,
								  0.0f, 0.0f, 1.0f, 3.0f,
								  0.0f, 0.0f, 0.0f, 1.0f };
		another_test = Matrix4x4(translate_temp);
		test = test.Translate(3.0f, 3.0f, 3.0f);
		for (int i = 0; i < 16; i++)
		{
			Assert::AreEqual(another_test.m[i], test.m[i], "Translate() Failed", __FILE__, __LINE__);
		}

		//Check Scale
		float scale_temp[16]{ 3.0f, 0.0f, 0.0f, 0.0f,
							  0.0f, 3.0f, 0.0f, 0.0f,
							  0.0f, 0.0f, 3.0f, 0.0f,
							  0.0f, 0.0f, 0.0f, 1.0f };
		another_test = Matrix4x4(scale_temp);
		worked = another_test == another_test.Scale(3.0f, 3.0f, 3.0f);
		Assert::AreEqual(true, worked, "Scale() Failed", __FILE__, __LINE__);

		//Check RotateInX
		float rotate_x_temp[16]{ 1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, -1.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f };
		another_test = Matrix4x4(rotate_x_temp);

		test = test.RotateX(1.57079632679f);
		Assert::AreEqual(another_test.m[0], test.m[0], 0.00001f, "RotateInX() Failed 0 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00001f, "RotateInX() Failed 1 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00001f, "RotateInX() Failed 2 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[3], test.m[3], 0.00001f, "RotateInX() Failed 3 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00001f, "RotateInX() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00001f, "RotateInX() Failed3 5", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[6], test.m[6], 0.00001f, "RotateInX() Failed3 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00001f, "RotateInX() Failed3 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00001f, "RotateInX() Failed3 8", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[9], test.m[9], 0.00001f, "RotateInX() Failed3 9", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[10], test.m[10], 0.00001f, "RotateInX() Failed3 10", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[11], test.m[11], 0.00001f, "RotateInX() Failed3 11", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[12], test.m[12], 0.00001f, "RotateInX() Failed3 12", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[13], test.m[13], 0.00001f, "RotateInX() Failed3 13", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[14], test.m[14], 0.00001f, "RotateInX() Failed3 14", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[15], test.m[15], 0.00001f, "RotateInX() Failed3 15", __FILE__, __LINE__);


		//Check RotateInY
		float rotate_y_temp[16]{ 0.0f, 0.0f, 1.0f, 0.0f,
								             0.0f, 1.0f, 0.0f, 0.0f,
								            -1.0f, 0.0f, 0.0f, 0.0f,
								             0.0f, 0.0f, 0.0f, 1.0f };
		another_test = Matrix4x4(rotate_y_temp);
		test = test.RotateY(1.57079632679f);

		Assert::AreEqual(another_test.m[0], test.m[0], 0.00001f, "RotateInY() Failed 0 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00001f, "RotateInY() Failed 1 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00001f, "RotateInY() Failed 2 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[3], test.m[3], 0.00001f, "RotateInY() Failed 3 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00001f, "RotateInY() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00001f, "RotateInY() Failed3 5", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[6], test.m[6], 0.00001f, "RotateInY() Failed3 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00001f, "RotateInY() Failed3 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00001f, "RotateInY() Failed3 8", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[9], test.m[9], 0.00001f, "RotateInY() Failed3 9", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[10], test.m[10], 0.00001f, "RotateInY() Failed3 10", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[11], test.m[11], 0.00001f, "RotateInY() Failed3 11", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[12], test.m[12], 0.00001f, "RotateInY() Failed3 12", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[13], test.m[13], 0.00001f, "RotateInY() Failed3 13", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[14], test.m[14], 0.00001f, "RotateInY() Failed3 14", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[15], test.m[15], 0.00001f, "RotateInY() Failed3 15", __FILE__, __LINE__);



		//Check RotateInZ
		float rotate_z_temp[16]{ 0.0f, -1.0f, 0.0f, 0.0f,
								             1.0f, 0.0f,  0.0f, 0.0f,
								             0.0f, 0.0f,  1.0f, 0.0f,
								             0.0f, 0.0f,  0.0f, 1.0f };
		another_test = Matrix4x4(rotate_z_temp);
		test = test.RotateZ(1.570796f);
		Assert::AreEqual(another_test.m[0], test.m[0], 0.00001f, "RotateInY() Failed 0 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00001f, "RotateInZ() Failed 1 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00001f, "RotateInZ() Failed 2 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[3], test.m[3], 0.00001f, "RotateInZ() Failed 3 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00001f, "RotateInZ() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00001f, "RotateInZ() Failed3 5", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[6], test.m[6], 0.00001f, "RotateInZ() Failed3 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00001f, "RotateInZ() Failed3 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00001f, "RotateInZ() Failed3 8", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[9], test.m[9], 0.00001f, "RotateInZ() Failed3 9", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[10], test.m[10], 0.00001f, "RotateInZ() Failed3 10", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[11], test.m[11], 0.00001f, "RotateInZ() Failed3 11", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[12], test.m[12], 0.00001f, "RotateInZ() Failed3 12", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[13], test.m[13], 0.00001f, "RotateInZ() Failed3 13", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[14], test.m[14], 0.00001f, "RotateInZ() Failed3 14", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[15], test.m[15], 0.00001f, "RotateInZ() Failed3 15", __FILE__, __LINE__);


		//Check GetTransform

    float transformation_temp[16]{ 0.0f,  0.0f, 3.0f,  9.0f,
                                   0.0f, -3.0f, 0.0f, -9.0f,
                                   3.0f,  0.0f, 0.0f,  9.0f,
                                   0.0f,  0.0f, 0.0f,  1.0f };
		another_test = Matrix4x4(transformation_temp);


		test = test.GetTransform(3.0f, 3.0f, 3.0f,
			3.0f, 3.0f, 3.0f,
			1.570796f, 1.570796f, 1.570796f);


		Assert::AreEqual(another_test.m[0], test.m[0], 0.00001f, "GetTransform() Failed 0 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[1], test.m[1], 0.00001f, "GetTransform() Failed 1 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[2], test.m[2], 0.00001f, "GetTransform() Failed 2 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[3], test.m[3], 0.00001f, "GetTransform() Failed 3 ", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[4], test.m[4], 0.00001f, "GetTransform() Failed 4", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[5], test.m[5], 0.00001f, "GetTransform() Failed 5", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[6], test.m[6], 0.00001f, "GetTransform() Failed 6", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[7], test.m[7], 0.00001f, "GetTransform() Failed 7", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[8], test.m[8], 0.00001f, "GetTransform() Failed 8", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[9], test.m[9], 0.00001f, "GetTransform() Failed 9", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[11], test.m[11], 0.00001f, "GetTransform() Failed 11", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[12], test.m[12], 0.00001f, "GetTransform() Failed 12", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[13], test.m[13], 0.00001f, "GetTransform() Failed 13", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[14], test.m[14], 0.00001f, "GetTransform() Failed 14", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[10], test.m[10], 0.00001f, "GetTransform() Failed 10", __FILE__, __LINE__);
		Assert::AreEqual(another_test.m[15], test.m[15], 0.00001f, "GetTransform() Failed 15", __FILE__, __LINE__);

		//Check PerspectiveMatrix

		//Check OrthoMatrix

		//Check GetColum
		test = test.Identity();
		worked = test.GetColum(0) == Vector4(1.0f, 0.0f, 0.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);
		worked = test.GetColum(1) == Vector4(0.0f, 1.0f, 0.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);
		worked = test.GetColum(2) == Vector4(0.0f, 0.0f, 1.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);
		worked = test.GetColum(3) == Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		Assert::AreEqual(true, worked, "GetColum() Failed", __FILE__, __LINE__);

		//Check GetLine
		test = test.Identity();
		worked = test.GetLine(0) == Vector4(1.0f, 0.0f, 0.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
		worked = test.GetLine(1) == Vector4(0.0f, 1.0f, 0.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
		worked = test.GetLine(2) == Vector4(0.0f, 0.0f, 1.0f, 0.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);
		worked = test.GetLine(3) == Vector4(0.0f, 0.0f, 0.0f, 1.0f);
		Assert::AreEqual(true, worked, "GetLine() Failed", __FILE__, __LINE__);

    float a[16]{ 1.0f,  0.0f, 3.0f,  2.0f,
                 3.0f, -3.0f, -1.0f, -9.0f,
                 3.0f, -7.0f, 2.0f,  8.0f,
                 5.0f,  9.0f, 0.0f,  1.0f };

    float b[16]{ 6.0f,  0.0f, 0.0f,  2.0f,
                 8.0f,  0.0f, -8.0f, -5.0f,
                 7.0f, -8.0f, 3.0f,  6.0f,
                 1.0f, -9.0f, 0.0f,  -4.0f };

    Matrix4x4 am, bm;
    am = Matrix4x4(a);
    bm = Matrix4x4(b);
    am.printM4();
    bm.printM4();


    Matrix4x4 ab = am.Multiply(bm);
    Matrix4x4 ba = bm.Multiply(am);

    ab.printM4();
    ba.printM4();


		Test::End();
	}
};

