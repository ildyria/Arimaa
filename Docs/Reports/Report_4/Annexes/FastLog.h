static inline float log2(float val)
{
	int* const	exp_ptr = reinterpret_cast <int *> (&val);
	int 				x = *exp_ptr;
	const int 	log_2 = ((x >> 23) & 255) - 128;
	x &= ~(255 << 23);
	x += 127 << 23;
	*exp_ptr = x;
	val = ((-1.0f / 3) * val + 2) * val - 2.0f / 3;
	/*
	The line computes 1+log2(m), m ranging from 1 to 2.
	The proposed formula is a 3rd degree polynomial keeping first derivate continuity.
	Higher degree could be used for more accuracy.
	*/
	return (val + log_2);
}
