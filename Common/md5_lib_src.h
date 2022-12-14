#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

#define TEST_BLOCK_LEN 1000
#define TEST_BLOCK_COUNT 1000
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

#define FF(a,b,c,d,x,s,ac) {(a)+=F((b),(c),(d))+(x)+(unsigned long int)(ac); \
(a)=ROTATE_LEFT((a),(s)); (a)+=(b);}
#define GG(a,b,c,d,x,s,ac) {(a)+=G((b),(c),(d))+(x)+(unsigned long int)(ac); \
(a)=ROTATE_LEFT((a),(s)); (a)+=(b);}
#define HH(a,b,c,d,x,s,ac) {(a)+=H((b),(c),(d))+(x)+(unsigned long int)(ac); \
(a)=ROTATE_LEFT((a),(s)); (a)+=(b);}
#define II(a,b,c,d,x,s,ac) {(a)+=I((b),(c),(d))+(x)+(unsigned long int)(ac); \
(a)=ROTATE_LEFT((a),(s)); (a)+=(b);}

class MD5
{
private:
	unsigned long int state[4];
	unsigned long int count[2];
	unsigned char buffer[64];
	unsigned char PADDING[64];
 
protected:
	void MD5Update(unsigned char *, unsigned int);
	void MD5Final(unsigned char [16]);
	void MD5Transform(unsigned long int [4], unsigned char [64]);
	void Encode(unsigned char *, unsigned long int *, unsigned int);
	void Decode(unsigned long int *, unsigned char *, unsigned int);
	void MD5_memcpy(unsigned char *, unsigned char *, unsigned int);
	void MD5_memset(unsigned char *, int, unsigned int);

public:
	MD5();
	virtual ~MD5();
	void MD5Encode(char *,unsigned char *);
	static void MD5Binary2String(unsigned char [16], char *EncodedString);
	static void MD5String2Binary(char *EncodedString, unsigned char [16]);
};
