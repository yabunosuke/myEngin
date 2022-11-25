#include<math.h>
#include"easing.h"

static const double PI = 3.14159265359;

using namespace std;

namespace EasingFunction
{
    //�C�[�W���O�֐�
    double InQuad(double t)
    {
        return t * t;
    }

    double InCubic(double t)
    {
        return t * t * t;
    }

    double InQuart(double t)
    {
        return t * t * t * t;
    }

    double InQuint(double t)
    {
        return t * t * t * t * t;
    }

    double InSine(double t)
    {
        return 1 - cos(t * PI / 2);
    }

    double InExpo(double t)
    {
        return pow(2, -(1 - t) * 10);
    }

    double InCirc(double t)
    {
        return 1 - sqrt((0 > 1 - t * t) ? 0 : 1 - t * t);
    }

    double InBack(double t)
    {
        return t * t * (2.70158 * t - 1.70158);
    }
    double InSoftBack(double t)
    {
        return t * t * (2 * t - 1);
    }

    double InElastic(double t)
    {
        return 56 * t * t * t * t * t - 105 * t * t * t * t + 60 * t * t * t - 10 * t * t;
    }

    double InBounce(double t)
    {
        double pow2;
        int bounce = 4;
        while (t < ((pow2 = pow(2, -bounce)) - 1) / 11) {}
        return 1 / pow(4, 3 - bounce) - 7.5625 * pow((pow2 * 3 - 2) / 22 - t, 2);
    }

    double InLinear(double t)
    {
        return t;
    }
}

using namespace EasingFunction;
static double (*FuncPtr[12])(double t)
= {
    InQuad,InCubic,InQuart,InQuint,InSine,InExpo,InCirc,InBack,InSoftBack,InElastic,InBounce,InLinear
};


double Ease(type InOutType, ease EaseType, double t, double start, double end)
{
    if (InOutType == In)
    {
        return (end - start) * FuncPtr[EaseType](t) + start;
    }
    else if (InOutType == Out)
    {
        return (end - start) * (1 - FuncPtr[EaseType](1 - t)) + start;
    }
    else if (InOutType == InOut)
    {
        if (t < 0.5)
            return (end - start) * (FuncPtr[EaseType](t * 2) / 2) + start;

        return (end - start) * (1 - FuncPtr[EaseType](2 - 2 * t) / 2) + start;
    }

    return -1.0f;
}

Vector3 Ease(type InOutType, ease EaseType, double t, Vector3 start, Vector3 end)
{
    Vector3 result;
    result.x = static_cast<float>(Ease(InOutType, EaseType, t, start.x, end.x));
    result.y = static_cast<float>(Ease(InOutType, EaseType, t, start.y, end.y));
    result.z = static_cast<float>(Ease(InOutType, EaseType, t, start.z, end.z));

    return result;
}
