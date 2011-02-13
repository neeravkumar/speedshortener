#include <cstdio>
#include <google/dense_hash_map>
#include<cstring>
#include<stdlib.h>
#include<signal.h>
void not_found(int signum)
{
    exit(22);
}
/* 32-bit cross-platform rotl */
#ifdef _MSC_VER /* use built-in method in MSVC */
#	define rotl(v, s) (uint32_t)_rotl(v, s)
#else /* use bitops in GCC; with o2 this gets optimized to a rotl instruction */
#	define rotl(v, s) (uint32_t)(((uint32_t)(v) << (s)) | ((uint32_t)(v) >> (32 - (s))))
#endif
using namespace std;
using google::dense_hash_map;
uint32_t MurmurHash3(const void *key, int len, uint32_t seed)
{

#define MURMUR_BLOCK() {\
    k1 *= c1; \
    k1  = rotl(k1,11);\
    k1 *= c2;\
    h1 ^= k1;\
    h1 = h1*3 + 0x52dce729;\
    c1 = c1*5 + 0x7b7d159c;\
    c2 = c2*5 + 0x6bce6396;\
}

    const uint8_t *data = (const uint8_t*)key;
    const int nblocks = len / 4;

    const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);
    const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

    uint32_t h1 = 0x971e137b ^ seed;
    uint32_t k1;

    uint32_t c1 = 0x95543787;
    uint32_t c2 = 0x2ad7eb25;

    int i;

    for (i = -nblocks; i; i++) {
        k1 = blocks[i];
        MURMUR_BLOCK();
    }

    k1 = 0;

    switch(len & 3) {
    case 3:
        k1 ^= tail[2] << 16;
    case 2:
        k1 ^= tail[1] << 8;
    case 1:
        k1 ^= tail[0];
        MURMUR_BLOCK();
    }

    h1 ^= len;
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
}

class stringhasher_murmur //: public hash_compare <const char*>
{
public:
    size_t operator() (const char* s) const
    {
        return MurmurHash3(s, strlen(s), 0);
    }
    bool operator() (const char* s1, const char* s2) const
    {
        return (s1 == s2) ? false : strcmp(s1, s2) > 0;
    }
};


struct eqstr
{
    bool operator()(const char* s1, const char* s2) const
    {
        return (s1 == s2) || (s1 && s2 && strcmp(s1, s2) == 0);
    }
};

int main(int argc, char *argv[])
{
    dense_hash_map<const char*, const char *, stringhasher_murmur, eqstr> urls;
    urls.set_empty_key(NULL);
    //add
    urls["0"] = "nero.im/hello-world";
    signal(SIGSEGV, not_found);
    printf("%s\n",urls[argv[1]]);
}
