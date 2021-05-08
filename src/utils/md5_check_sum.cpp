#include "md5_check_sum.h"


#include <Windows.h>

#define Q_binarytohex	v_binarytohex
#define MD5_DIGEST_LENGTH 16  
#define MD5_BIT_LENGTH ( MD5_DIGEST_LENGTH * sizeof(unsigned char) )
#define COPY_ALL_CHARACTERS -1
#define ARRAYSIZE(A)    RTL_NUMBER_OF_V2(A)
#define _ARRAYSIZE(A)   RTL_NUMBER_OF_V1(A)

#define Q_ARRAYSIZE(p)		ARRAYSIZE(p)

#include <cstdio>
#include <cstring>


void v_binarytohex(const char* in, int inputbytes, char* out, int outsize)
{
	char doublet[10];
	int i;

	out[0] = 0;

	for (i = 0; i < inputbytes; i++)
	{
		unsigned char c = in[i];
		snprintf(doublet, sizeof(doublet), "%02x", c);
		strncat(out, doublet, outsize);
	}
}
struct md5_value_t
{
	unsigned char bits[MD5_DIGEST_LENGTH];

	void zero();
	bool is_zero() const;

	bool operator==(const md5_value_t& src) const;
	bool operator!=(const md5_value_t& src) const;

};
typedef struct
{
	unsigned int	buf[4];
	unsigned int	bits[2];
	unsigned char	in[64];
} md5_context_t;
// The four core functions - F1 is optimized somewhat
// #define F1(x, y, z) (x & y | ~x & z)
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

// This is the central step in the MD5 algorithm.
#define MD5STEP(f, w, x, y, z, data, s) \
        ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

//-----------------------------------------------------------------------------
// Purpose: The core of the MD5 algorithm, this alters an existing MD5 hash to
//  reflect the addition of 16 longwords of new data.  MD5Update blocks
//  the data and converts bytes into longwords for this routine.
// Input  : buf[4] - 
//			in[16] - 
// Output : static void
//-----------------------------------------------------------------------------
static void md5_transform(unsigned int buf[4], unsigned int const in[16])
{
	register unsigned int a, b, c, d;

	a = buf[0];
	b = buf[1];
	c = buf[2];
	d = buf[3];

	MD5STEP(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
	MD5STEP(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
	MD5STEP(F1, c, d, a, b, in[2] + 0x242070db, 17);
	MD5STEP(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
	MD5STEP(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
	MD5STEP(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
	MD5STEP(F1, c, d, a, b, in[6] + 0xa8304613, 17);
	MD5STEP(F1, b, c, d, a, in[7] + 0xfd469501, 22);
	MD5STEP(F1, a, b, c, d, in[8] + 0x698098d8, 7);
	MD5STEP(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
	MD5STEP(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
	MD5STEP(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
	MD5STEP(F1, a, b, c, d, in[12] + 0x6b901122, 7);
	MD5STEP(F1, d, a, b, c, in[13] + 0xfd987193, 12);
	MD5STEP(F1, c, d, a, b, in[14] + 0xa679438e, 17);
	MD5STEP(F1, b, c, d, a, in[15] + 0x49b40821, 22);

	MD5STEP(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
	MD5STEP(F2, d, a, b, c, in[6] + 0xc040b340, 9);
	MD5STEP(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
	MD5STEP(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
	MD5STEP(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
	MD5STEP(F2, d, a, b, c, in[10] + 0x02441453, 9);
	MD5STEP(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
	MD5STEP(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
	MD5STEP(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
	MD5STEP(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
	MD5STEP(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
	MD5STEP(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
	MD5STEP(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
	MD5STEP(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
	MD5STEP(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
	MD5STEP(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

	MD5STEP(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
	MD5STEP(F3, d, a, b, c, in[8] + 0x8771f681, 11);
	MD5STEP(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
	MD5STEP(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
	MD5STEP(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
	MD5STEP(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
	MD5STEP(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
	MD5STEP(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
	MD5STEP(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
	MD5STEP(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
	MD5STEP(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
	MD5STEP(F3, b, c, d, a, in[6] + 0x04881d05, 23);
	MD5STEP(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
	MD5STEP(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
	MD5STEP(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
	MD5STEP(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

	MD5STEP(F4, a, b, c, d, in[0] + 0xf4292244, 6);
	MD5STEP(F4, d, a, b, c, in[7] + 0x432aff97, 10);
	MD5STEP(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
	MD5STEP(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
	MD5STEP(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
	MD5STEP(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
	MD5STEP(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
	MD5STEP(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
	MD5STEP(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
	MD5STEP(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
	MD5STEP(F4, c, d, a, b, in[6] + 0xa3014314, 15);
	MD5STEP(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
	MD5STEP(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
	MD5STEP(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
	MD5STEP(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
	MD5STEP(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

//-----------------------------------------------------------------------------
// Purpose: Start MD5 accumulation.  Set bit count to 0 and buffer to mysterious initialization constants.

// Input  : *ctx - 
//-----------------------------------------------------------------------------
inline void md5_init(md5_context_t* ctx)
{
	ctx->buf[0] = 0x67452301;
	ctx->buf[1] = 0xefcdab89;
	ctx->buf[2] = 0x98badcfe;
	ctx->buf[3] = 0x10325476;

	ctx->bits[0] = 0;
	ctx->bits[1] = 0;
}

//-----------------------------------------------------------------------------
// Purpose: Update context to reflect the concatenation of another buffer full of bytes.
// Input  : *ctx - 
//			*buf - 
//			len - 
//-----------------------------------------------------------------------------
inline void md5_update(md5_context_t* ctx, unsigned char const* buf, unsigned int len)
{
	unsigned int t;

	/* Update bitcount */

	t = ctx->bits[0];
	if ((ctx->bits[0] = t + ((unsigned int)len << 3)) < t)
		ctx->bits[1]++;         /* Carry from low to high */
	ctx->bits[1] += len >> 29;

	t = (t >> 3) & 0x3f;        /* Bytes already in shsInfo->data */

	/* Handle any leading odd-sized chunks */

	if (t)
	{
		unsigned char* p = (unsigned char*)ctx->in + t;

		t = 64 - t;
		if (len < t)
		{
			memcpy(p, buf, len);
			return;
		}
		memcpy(p, buf, t);
		//byteReverse(ctx->in, 16);
		md5_transform(ctx->buf, (unsigned int*)ctx->in);
		buf += t;
		len -= t;
	}
	/* Process data in 64-byte chunks */

	while (len >= 64)
	{
		memcpy(ctx->in, buf, 64);
		//byteReverse(ctx->in, 16);
		md5_transform(ctx->buf, (unsigned int*)ctx->in);
		buf += 64;
		len -= 64;
	}

	/* Handle any remaining bytes of data. */
	memcpy(ctx->in, buf, len);
}

//-----------------------------------------------------------------------------
// Purpose: Final wrapup - pad to 64-byte boundary with the bit pattern 
// 1 0* (64-bit count of bits processed, MSB-first)
// Input  : digest[MD5_DIGEST_LENGTH] - 
//			*ctx - 
//-----------------------------------------------------------------------------
void md5_final(unsigned char digest[MD5_DIGEST_LENGTH], md5_context_t* ctx)
{
	unsigned count;
	unsigned char* p;

	/* Compute number of bytes mod 64 */
	count = (ctx->bits[0] >> 3) & 0x3F;

	/* Set the first char of padding to 0x80.  This is safe since there is
	   always at least one byte free */
	p = ctx->in + count;
	*p++ = 0x80;

	/* Bytes of padding needed to make 64 bytes */
	count = 64 - 1 - count;

	/* Pad out to 56 mod 64 */
	if (count < 8)
	{
		/* Two lots of padding:  Pad the first block to 64 bytes */
		memset(p, 0, count);
		//byteReverse(ctx->in, 16);
		md5_transform(ctx->buf, (unsigned int*)ctx->in);

		/* Now fill the next block with 56 bytes */
		memset(ctx->in, 0, 56);
	}
	else
	{
		/* Pad block to 56 bytes */
		memset(p, 0, count - 8);
	}
	//byteReverse(ctx->in, 14);

	/* Append length in bits and transform */
	((unsigned int*)ctx->in)[14] = ctx->bits[0];
	((unsigned int*)ctx->in)[15] = ctx->bits[1];

	md5_transform(ctx->buf, (unsigned int*)ctx->in);
	//byteReverse((unsigned char *) ctx->buf, 4);
	memcpy(digest, ctx->buf, MD5_DIGEST_LENGTH);
	memset(ctx, 0, sizeof(*ctx));        /* In case it's sensitive */
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *hash - 
//			hashlen - 
// Output : char
//-----------------------------------------------------------------------------
char* md5_print(const char* hash, int hashlen)
{
	static char szReturn[64];


	Q_binarytohex(hash, hashlen, szReturn, sizeof(szReturn));
	return szReturn;
}

//-----------------------------------------------------------------------------
// Purpose: generate pseudo random number from a seed number
// Input  : seed number
// Output : pseudo random number
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
inline bool md5_compare(const md5_value_t& data, const md5_value_t& compare)
{
	return memcmp(data.bits, compare.bits, MD5_DIGEST_LENGTH) == 0;
}

//-----------------------------------------------------------------------------
void md5_value_t::zero()
{
	memset(bits, 0, sizeof(bits));
}

//-----------------------------------------------------------------------------
bool md5_value_t::is_zero() const
{
	for (int i = 0; i < ARRAYSIZE(bits); ++i)
	{
		if (bits[i] != 0)
			return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
inline void md5_process_single_buffer(const void* p, int len, md5_value_t& md5Result)
{
	md5_context_t ctx;
	md5_init(&ctx);
	md5_update(&ctx, (unsigned char const*)p, len);
	md5_final(md5Result.bits, &ctx);
}

unsigned md5::md5_pseudo_random(unsigned nSeed)
{
	md5_context_t ctx;
	unsigned char digest[MD5_DIGEST_LENGTH]; // The MD5 Hash

	memset(&ctx, 0, sizeof(ctx));

	md5_init(&ctx);
	md5_update(&ctx, (unsigned char*)&nSeed, sizeof(nSeed));
	md5_final(digest, &ctx);

	return *(unsigned int*)(digest + 6);	// use 4 middle bytes for random value
}
