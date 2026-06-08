#include "class.h"

// TargetFollowCamera::aproximateCooefficientsForAproximationOfDampingFunktion(
//     float t, double* outB, double* outA, double* outC, double* outD, double* outE)
// (+ a 7th hidden double* spilled on the stack)
// Evaluates five degree-8 polynomials in t (the precomputed damping-curve fit) and stores the
// coefficients into the five/six output doubles. The constant terms live in the C arrays below;
// for coverage we model them as extern coefficient tables (9 doubles each, c[0]=x^8 .. c[8]=1).

extern "C" {
extern const double g_TFC_dampA[9];
extern const double g_TFC_dampB[9];
extern const double g_TFC_dampC[9];
extern const double g_TFC_dampD[9];
extern const double g_TFC_dampE[9];   // E has no constant term (c[8] == 0)
}

static inline double poly8(const double *c, double x2, double x3, double x4,
                           double x5, double x6, double x7, double x8, double x)
{
    return x8 * c[0] + x7 * c[1] + x6 * c[2] + x5 * c[3] + x4 * c[4] +
           x3 * c[5] + x2 * c[6] + x * c[7] + c[8];
}

extern "C" void TFC_aproximateCooefficientsForAproximationOfDampingFunktion(
    TargetFollowCamera *self, float t, double *outB, double *outA, double *outC,
    double *outD, double *outE, double *outF)
{
    (void)self;
    double x = (double)t;
    double x2 = x * x;
    double x3 = x2 * x;
    double x4 = x3 * x;
    double x5 = x4 * x;
    double x6 = x5 * x;
    double x7 = x6 * x;
    double x8 = x7 * x;

    double a = poly8(g_TFC_dampA, x2, x3, x4, x5, x6, x7, x8, x);
    double b = poly8(g_TFC_dampB, x2, x3, x4, x5, x6, x7, x8, x);
    double c = poly8(g_TFC_dampC, x2, x3, x4, x5, x6, x7, x8, x);
    double d = poly8(g_TFC_dampD, x2, x3, x4, x5, x6, x7, x8, x);
    double e = poly8(g_TFC_dampE, x2, x3, x4, x5, x6, x7, x8, x);

    *outB = e;
    *outA = -a;
    *outC = c;
    *outD = d;
    *outF = -b;
}
