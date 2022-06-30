//#include "Matrix4.h"
//#include "Vector3.h"
//#include <cmath>
//
//Matrix4 identity()
//{
//    static const Matrix4 result =
//    {
//        1.0f,0.0f,0.0f,0.0f,
//        0.0f,1.0f,0.0f,0.0f,
//        0.0f,0.0f,1.0f,0.0f,
//        0.0f,0.0f,0.0f,1.0f
//    };
//    return result;
//}
//
//Matrix4 scale(const Vector3 &s)
//{
//    Matrix4 result
//    {
//        s.x,0.0f,0.0f,0.0f,
//        0.0f,s.y,0.0f,0.0f,
//        0.0f,0.0f,s.z,0.0f,
//        0.0f,0.0f,0.0f,1.0f
//    };
//    return result;
//}
//
//Matrix4 rotateX(float angle)
//{
//    float sin = std::sin(angle);
//    float cos = std::cos(angle);
//    Matrix4 result
//    {
//        1.0f,0.0f,0.0f,0.0f,
//        0.0f,cos,sin,0.0f,
//        0.0f,-sin,cos,0.0f,
//        0.0f,0.0f,0.0f,1.0f
//    };
//    return result;
//}
//
//Matrix4 rotateY(float angle)
//{
//    float sin = std::sin(angle);
//    float cos = std::cos(angle);
//    Matrix4 result
//    {
//        cos,0.0f,-sin,0.0f,
//        0.0f,1.0f,0.0f,0.0f,
//        sin,0.0f,cos,0.0f,
//        0.0f,0.0f,0.0f,1.0f
//    };
//    return result;
//}
//
//Matrix4 rotateZ(float angle)
//{
//    float sin = std::sin(angle);
//    float cos = std::cos(angle);
//    Matrix4 result
//    {
//        cos,sin,0.0f,0.0f,
//        -sin,cos,0.0f,0.0f,
//        0.0f,0.0f,1.0f,0.0f,
//        0.0f,0.0f,0.0f,1.0f
//    };
//    return result;
//}
//
//Matrix4 rotate(float x, float y, float z)
//{
//    return Matrix4();
//}
//
//Matrix4 translate(const Vector3 & t)
//{
//    Matrix4 result
//    {
//        1.0f,0.0f,0.0f,0.0f,
//        0.0f,1.0f,0.0f,0.0f,
//        0.0f,0.0f,1.0f,0.0f,
//        t.x, t.y, t.z, 1.0f
//    };
//    return result;
//}
//
//Vector3 transform(const Vector3 & v, const Matrix4 & m)
//{
//    float w = v.x * m.m[0][3]+ v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];
//
//    Vector3 result
//    {
//        (v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
//        (v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
//        (v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
//    };
//
//    return result;
//}
//
//
//Vector3 & operator*(const Vector3 & v, const Matrix4 & m)
//{
//    Vector3 result= transform(v, m);
//    return result;
//}
