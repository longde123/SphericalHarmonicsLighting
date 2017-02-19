#ifndef SPHERICAL_HARMONICS_H_
#define SPHERICAL_HARMONICS_H_

#include <array>

// normalize first
void HarmonicBasis4(float basis[4], float x, float y, float z, float r = 1.f);
void HarmonicBasis9(float basis[9], float x, float y, float z, float r = 1.f);
void HarmonicBasis16(float basis[16], float x, float y, float z, float r = 1.f);

#endif