#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <cmath>

template <typename T>
inline T clip(T value, T min, T max)
{
	if (value > max) {
		return max;
	}
	else if (value < min) {
		return min;
	}

	return value;
}

template <typename T>
inline T scale(T value, T min, T max)
{
	return (value * (max - min)) + min;
}

inline double pitchToFrequency(int note)
{
	// http://franciscotufro.com/2008/05/translanting-midi-notes-to-frequencies-in-the-diatonic-scale-using-the-central-a-440hz-as-reference/
	return 440 * pow(2.0, (note - 69.0) / 12.0);
}

inline int frequencyToPitch(double frequency)
{
	// http://audiores.uint8.com.ar/blog/2007/08/26/fundamental-in-hz-to-a-midi-note/
	return int( 69. + log(frequency / 440.) * 17.31234 );
}

inline float linearInterpolate(float *buffer, int bufferSize, float phase)
{
	float indexBase = floor(phase);
	float indexFract = phase - indexBase;
	int leftIndex = (int)indexBase >= bufferSize ? 0 : (int)indexBase;
	int rightIndex = (leftIndex + 1) >= bufferSize ? 0 : (leftIndex + 1);
	float leftValue = buffer[leftIndex];
	float rightValue = buffer[rightIndex];
	float value = leftValue + ((rightValue - leftValue) * indexFract);

	return value;
}

#endif //__UTILITIES_H__