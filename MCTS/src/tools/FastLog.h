#include <intrin.h>

class FastLog
{
	FastLog();
	~FastLog();
public:
	static inline unsigned long log2(const int& x)
	{
		unsigned long y;
		_BitScanReverse64(&y, x);
		return y;
	}

	static inline float log2_bis(float val)
	{
		int* const    exp_ptr = reinterpret_cast <int *> (&val);
		int            x = *exp_ptr;
		const int      log_2 = ((x >> 23) & 255) - 128;
		x &= ~(255 << 23);
		x += 127 << 23;
		*exp_ptr = x;

		val = ((-1.0f / 3) * val + 2) * val - 2.0f / 3;
		/*
		The line computes 1+log2(m), m ranging from 1 to 2.
		The proposed formula is a 3rd degree polynomial keeping first derivate continuity.
		Higher degree could be used for more accuracy.
		For faster results, one can remove this line, if accuracy is not the matter (it gives some linear interpolation between powers of 2).
		*/

		return (val + log_2);
	}

	static inline double times_log2(const unsigned long &val)
	{
		return (val * 0.69314718f);
	}

	static inline double fast_log(int n) {
		return times_log2(log2(n));
		//return fast_log(log2_bis(n));
	}
};