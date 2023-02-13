#include "vector_2.h"
#include "vector_3.h"
#include "vector_4.h"
#include "UnitTestHelpers.h"

// Code by Diego Llorens and Lucas Gonzalez

namespace UnitTest{
  
  void Vector2Test(){

	  Test::Start(__FUNCTION__);

    Vector2 test;
    Vector2 another_test = Vector2(0.5f, 0.5f);
    Vector2 b;
    b = another_test;
    bool worked = false;

    //Check Magnitude function.
    Assert::AreEqual(0.707107f, another_test.Magnitude(), 0.000001f, "Magnitude Failed", __FILE__, __LINE__);

    //Check Normalize function.
    another_test = Vector2(0.5f, 0.5f);
    another_test.Normalize();
    worked = Vector2(0.7071f, 0.7071f) == another_test;
    Assert::AreEqual(0.7071f, another_test.x, 0.00001f, "Normalize X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.7071f, another_test.y, 0.00001f, "Normalize Y Failed", __FILE__, __LINE__);

    //Check Normalized function.
    another_test = Vector2(0.5f, 0.5f);
    worked = Vector2(0.7071f, 0.7071f) == another_test.Normalized();
    another_test = another_test.Normalized();
    Assert::AreEqual(0.7071f, another_test.x, 0.00001f, "Normalized X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.7071f, another_test.y, 0.00001f, "Normalized Y Failed", __FILE__, __LINE__);

    //Check Scale function.
    another_test = Vector2(1.0f, 1.0f);
    another_test.Scale(Vector2(2.0f, 2.0f));
    worked = Vector2(2.0f, 2.0f) == another_test;
    Assert::AreEqual(true, worked, "Scale Failed", __FILE__, __LINE__);

    //Check SqrMagnitude function.
    another_test = Vector2(1.5f, 0.5f);
    Assert::AreEqual(2.5f, another_test.SqrMagnitude(), "SqrMagnitude Failed", __FILE__, __LINE__);

  
    //Check Distance function.
    another_test = Vector2(0.5f, 0.5f);
    test = Vector2(1.5f, 1.5f);
    Assert::AreEqual(1.414214f, Vector2::Distance(test, another_test), 0.0001f, "Distance failed", __FILE__, __LINE__);

    //Check Lerp function.
    another_test = Vector2(0.5f, 0.5f);
    test = Vector2(1.5f, 1.5f);
    another_test = Vector2::Lerp(another_test, test, 0.7f);
    Assert::AreEqual(1.2f, another_test.x, "X value on Lerp failed", __FILE__, __LINE__);
    Assert::AreEqual(1.2f, another_test.y, "Y value on Lerp failed", __FILE__, __LINE__);
    //Not needed to check LerpUnclamped due to is used on Lerp function.

    //Check Dot Product function.
    test = Vector2(0.5f, 0.5f);
    another_test = Vector2(1.0f, 1.0f);
    Assert::AreEqual(1.0f, Vector2::DotProduct(test, another_test), "Dot Product Failed", __FILE__, __LINE__);


	Test::End();
  }

  void Vector3Test(){

	Test::Start(__FUNCTION__);
    Vector3 test;
    Vector3 another_test = Vector3(0.5f);
    //Check Magnitude function.
    Assert::AreEqual(0.86603f, another_test.Magnitude(), 0.00001f, "Magnitude Failed", __FILE__, __LINE__);
    //Check Normalize function.
    another_test = Vector3(0.5f);
    another_test.Normalize();
    Assert::AreEqual(0.57735f, another_test.x, 0.00001f, "Normalize X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.57735f, another_test.y, 0.00001f, "Normalize Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.57735f, another_test.Normalized().z, 0.00001f, "Normalize Z Failed", __FILE__, __LINE__);

    //Check Normalized function.
    another_test = Vector3(0.5f);
    Assert::AreEqual(0.57735f, another_test.Normalized().x, 0.00001f, "Normalized X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.57735f, another_test.Normalized().y, 0.00001f, "Normalized Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.57735f, another_test.Normalized().z, 0.00001f, "Normalized Z Failed", __FILE__, __LINE__);

    //Check Lerp function.
    another_test = Vector3::Lerp(Vector3(0.0f), Vector3(1), 0.5);
    Assert::AreEqual(0.5f, another_test.x, 0.1f, "Lerp X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, another_test.y, 0.1f, "Lerp Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, another_test.z, 0.1f, "Lerp Z Failed", __FILE__, __LINE__);
    another_test = Vector3::Lerp(Vector3(0.0f), Vector3(1), 1.5);
    Assert::AreEqual(1.0f, another_test.x, 0.1f, "Lerp X Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, another_test.y, 0.1f, "Lerp Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, another_test.z, 0.1f, "Lerp Z Failed", __FILE__, __LINE__);
    another_test = Vector3::Lerp(Vector3(0.0f), Vector3(1), -1);
    Assert::AreEqual(0.0f, another_test.x, 0.1f, "Lerp X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, another_test.y, 0.1f, "Lerp Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, another_test.z, 0.1f, "Lerp Z Failed", __FILE__, __LINE__);

    //Check LerpUnclamped function.
    another_test = Vector3::LerpUnclamped(Vector3(0.0f), Vector3(1), 0.5);
    Assert::AreEqual(0.5f, another_test.x, 0.1f, "LerpUnclamped X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, another_test.y, 0.1f, "LerpUnclamped Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, another_test.z, 0.1f, "LerpUnclamped Z Failed", __FILE__, __LINE__);
    another_test = Vector3::LerpUnclamped(Vector3(0.0f), Vector3(1), 1.5);
    Assert::AreEqual(1.5f, another_test.x, 0.1f, "LerpUnclamped X Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.5f, another_test.y, 0.1f, "LerpUnclamped Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.5f, another_test.z, 0.1f, "LerpUnclamped Z Failed", __FILE__, __LINE__);
    another_test = Vector3::LerpUnclamped(Vector3(0.0f), Vector3(1), -1);
    Assert::AreEqual(-1.0f, another_test.x, 0.1f, "LerpUnclamped X Failed", __FILE__, __LINE__);
    Assert::AreEqual(-1.0f, another_test.y, 0.1f, "LerpUnclamped Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(-1.0f, another_test.z, 0.1f, "LerpUnclamped Z Failed", __FILE__, __LINE__);

    //Check Dot Product function.
    test = Vector3(0.5f);
    another_test = Vector3(1.0f);
    Assert::AreEqual(1.5f, Vector3::DotProduct(test, another_test), "Dot Product Failed", __FILE__, __LINE__);

    //Check Angle function
    Assert::AreEqual(2.6714f, Vector3::Angle(Vector3(2, -4, -1), Vector3(0, 5, 2)), 0.0001f, "Angle Failed", __FILE__, __LINE__);

    //Check Cross Product function
    another_test = Vector3::CrossProduct(Vector3(1, 2, 3), Vector3(1, 5, 7));
    Assert::AreEqual(-1.0f, another_test.x, 0.1f, "CrossProduct X Failed", __FILE__, __LINE__);
    Assert::AreEqual(-4.0f, another_test.y, 0.1f, "CrossProduct Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(3.0f, another_test.z, 0.1f, "CrossProduct Z Failed", __FILE__, __LINE__);

    //Check Distance function
    Assert::AreEqual(10.246951f, Vector3::Distance(Vector3(7, 4, 3), Vector3(17, 6, 2)), 0.000001f, "Distance Failed", __FILE__, __LINE__);

    //Check Reflect function
    another_test = Vector3::Reflect(Vector3(-0.5f, 0.5f, 0), Vector3(-1, 0, 0));
    Assert::AreEqual(0.5f, another_test.x, 0.1f, "Reflect X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, another_test.y, 0.1f, "Reflect Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, another_test.z, 0.1f, "Reflect Z Failed", __FILE__, __LINE__);

    //Check SqrMagnitude function
    another_test = Vector3(1, 2, 3);
    Assert::AreEqual(14.0f, another_test.SqrMagnitude(), 0.1f, "SqrMagnitude Failed", __FILE__, __LINE__);

    //Check Scale function.
    another_test = Vector3(1.0f);
    another_test.Scale(Vector3(2.0f));
    Assert::AreEqual(2.0f, another_test.x, "X value on scale failed", __FILE__, __LINE__);
    Assert::AreEqual(2.0f, another_test.y, "Y value on scale failed", __FILE__, __LINE__);
	Test::End();

  }

  void Vector4Test(){

	Test::Start(__FUNCTION__);
    Vector4 test;
    Vector4 a = Vector4(5.0f, 3.0f, 2.0f, 1.0f);
    Vector4 b = Vector4(3.0f, 2.0f, 1.0f, 0.0f);

    /////////////////////////
    // ---- OPERATORS ----///
    /////////////////////////

    //-----------------//
    // -- Operator+ -- //
    //----------------//
    test = a + b;
    Assert::AreEqual(8.0f, test.x, 0.00001f, "Operator+(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(5.0f, test.y, 0.00001f, "Operator+(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(3.0f, test.z, 0.00001f, "Operator+(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.w, 0.00001f, "Operator+(Vector4) Failed", __FILE__, __LINE__);

    test = a + 2.0f;
    Assert::AreEqual(7.0f, test.x, 0.00001f, "Operator+(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(5.0f, test.y, 0.00001f, "Operator+(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(4.0f, test.z, 0.00001f, "Operator+(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(3.0f, test.w, 0.00001f, "Operator+(float) Failed", __FILE__, __LINE__);

    //-----------------//
    // -- Operator+= --//
    //----------------//
    test = a;
    test += b;
    Assert::AreEqual(8.0f, test.x, 0.00001f, "Operator+=(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(5.0f, test.y, 0.00001f, "Operator+=(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(3.0f, test.z, 0.00001f, "Operator+=(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.w, 0.00001f, "Operator+=(Vector4) Failed", __FILE__, __LINE__);

    test = a;
    test += 2.0f;
    Assert::AreEqual(7.0f, test.x, 0.00001f, "Operator+=(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(5.0f, test.y, 0.00001f, "Operator+=(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(4.0f, test.z, 0.00001f, "Operator+=(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(3.0f, test.w, 0.00001f, "Operator+=(float) Failed", __FILE__, __LINE__);

    //-----------------//
    // -- Operator- -- //
    //----------------//
    test = a - b;
    Assert::AreEqual(2.0f, test.x, 0.00001f, "Operator-(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.y, 0.00001f, "Operator-(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.z, 0.00001f, "Operator-(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.w, 0.00001f, "Operator-(Vector4) Failed", __FILE__, __LINE__);

    test = a - 2.0f;
    Assert::AreEqual(3.0f, test.x, 0.00001f, "Operator-(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.y, 0.00001f, "Operator-(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, test.z, 0.00001f, "Operator-(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(-1.0f, test.w, 0.00001f, "Operator-(float) Failed", __FILE__, __LINE__);

    //-------------------//
    // -- Operator-= -- //
    //------------------//
    test = a;
    test -= b;
    Assert::AreEqual(2.0f, test.x, 0.00001f, "Operator-=(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.y, 0.00001f, "Operator-=(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.z, 0.00001f, "Operator-=(Vector4) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.w, 0.00001f, "Operator-=(Vector4) Failed", __FILE__, __LINE__);

    test = a;
    test -= 2.0f;
    Assert::AreEqual(3.0f, test.x, 0.00001f, "Operator-=(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.y, 0.00001f, "Operator-=(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, test.z, 0.00001f, "Operator-=(float) Failed", __FILE__, __LINE__);
    Assert::AreEqual(-1.0f, test.w, 0.00001f, "Operator-=(float) Failed", __FILE__, __LINE__);

    //-----------------//
    // -- Operator* -- //
    //----------------//
    test = a * 2.0f;
    Assert::AreEqual(10.0f, test.x, 0.00001f, "Operator* Failed", __FILE__, __LINE__);
    Assert::AreEqual(6.0f, test.y, 0.00001f, "Operator* Failed", __FILE__, __LINE__);
    Assert::AreEqual(4.0f, test.z, 0.00001f, "Operator* Failed", __FILE__, __LINE__);
    Assert::AreEqual(2.0f, test.w, 0.00001f, "Operator* Failed", __FILE__, __LINE__);

    //------------------//
    // -- Operator*= -- //
    //-----------------//
    test = a;
    test *= 2.0f;
    Assert::AreEqual(10.0f, test.x, 0.00001f, "Operator*= Failed", __FILE__, __LINE__);
    Assert::AreEqual(6.0f, test.y, 0.00001f, "Operator*= Failed", __FILE__, __LINE__);
    Assert::AreEqual(4.0f, test.z, 0.00001f, "Operator*= Failed", __FILE__, __LINE__);
    Assert::AreEqual(2.0f, test.w, 0.00001f, "Operator*= Failed", __FILE__, __LINE__);

    //-----------------//
    // -- Operator/ -- //
    //----------------//
    test = a / 2.0f;
    Assert::AreEqual(2.5f, test.x, 0.00001f, "Operator/ Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.5f, test.y, 0.00001f, "Operator/ Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.z, 0.00001f, "Operator/ Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, test.w, 0.00001f, "Operator/ Failed", __FILE__, __LINE__);

    //------------------//
    // -- Operator/= -- //
    //-----------------//
    test = a;
    test /= 2.0f;
    Assert::AreEqual(2.5f, test.x, 0.00001f, "Operator/= Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.5f, test.y, 0.00001f, "Operator/= Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.z, 0.00001f, "Operator/= Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, test.w, 0.00001f, "Operator/= Failed", __FILE__, __LINE__);

    //-----------------//
    // -- Operator= -- //
    //----------------//
    test = a;
    Assert::AreEqual(5.0f, test.x, 0.00001f, "Operator= Failed", __FILE__, __LINE__);
    Assert::AreEqual(3.0f, test.y, 0.00001f, "Operator= Failed", __FILE__, __LINE__);
    Assert::AreEqual(2.0f, test.z, 0.00001f, "Operator= Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.w, 0.00001f, "Operator= Failed", __FILE__, __LINE__);



    //Check Magnitude function.
    Assert::AreEqual(6.244998f, a.Magnitude(), 0.00001f, "Magnitude Failed", __FILE__, __LINE__);

    //Check Normalized function.
    Assert::AreEqual(0.800641f, a.Normalized().x, 0.00001f, "Normalized X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.480384f, a.Normalized().y, 0.00001f, "Normalized Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.320256f, a.Normalized().z, 0.00001f, "Normalized Z Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.160128f, a.Normalized().w, 0.00001f, "Normalized W Failed", __FILE__, __LINE__);

    //Check Normalize function.
    test = a;
    test.Normalize();
    Assert::AreEqual(0.800641f, test.x, 0.00001f, "Normalize X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.480384f, test.y, 0.00001f, "Normalize Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.320256f, test.z, 0.00001f, "Normalize Z Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.160128f, test.w, 0.00001f, "Normalize W Failed", __FILE__, __LINE__);

    //Check Scale function.
    test = a;
    test.Scale(b);
    Assert::AreEqual(15.0f, test.x, 0.00001f, "Scale X Failed", __FILE__, __LINE__);
    Assert::AreEqual(6.0f, test.y, 0.00001f, "Scale Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(2.0f, test.z, 0.00001f, "Scale Z Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, test.w, 0.00001f, "Scale W Failed", __FILE__, __LINE__);

    //Check Distance function
    Assert::AreEqual(2.645751f, Vector4::Distance(a, b), 0.000001f, "Distance Failed", __FILE__, __LINE__);

    // Check Dot Product function.
    Assert::AreEqual(23.0f, Vector4::DotProduct(a, b), "Dot Product Failed", __FILE__, __LINE__);

    //Check Lerp function.
    test = Vector4::Lerp(Vector4(0.0f), Vector4(1), 0.5);
    Assert::AreEqual(0.5f, test.x, 0.1f, "Lerp X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, test.y, 0.1f, "Lerp Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, test.z, 0.1f, "Lerp Z Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.5f, test.w, 0.1f, "Lerp W Failed", __FILE__, __LINE__);
    test = Vector4::Lerp(Vector4(0.0f), Vector4(1), 1.5);
    Assert::AreEqual(1.0f, test.x, 0.1f, "Lerp X Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.y, 0.1f, "Lerp Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.z, 0.1f, "Lerp Z Failed", __FILE__, __LINE__);
    Assert::AreEqual(1.0f, test.w, 0.1f, "Lerp W Failed", __FILE__, __LINE__);
    test = Vector4::Lerp(Vector4(0.0f), Vector4(1), -1);
    Assert::AreEqual(0.0f, test.x, 0.1f, "Lerp X Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, test.y, 0.1f, "Lerp Y Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, test.z, 0.1f, "Lerp Z Failed", __FILE__, __LINE__);
    Assert::AreEqual(0.0f, test.w, 0.1f, "Lerp W Failed", __FILE__, __LINE__);
	Test::End();
  }
  };