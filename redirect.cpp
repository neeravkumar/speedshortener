#include <cstdio>
#include <rdestl/hash_map.h>
#include<cstring>
#include<stdint.h>
#include <mongoose/mongoose.h>
#if !defined(LISTENING_PORT)
#define LISTENING_PORT "54321"
#endif
/* 32-bit cross-platform rotl */
#ifdef _MSC_VER /* use built-in method in MSVC */
#	define rotl(v, s) (uint32_t)_rotl(v, s)
#else /* use bitops in GCC; with o2 this gets optimized to a rotl instruction */
#	define rotl(v, s) (uint32_t)(((uint32_t)(v) << (s)) | ((uint32_t)(v) >> (32 - (s))))
#endif
using namespace std;
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

namespace rde
{
template<>
struct less<char*>
{
    bool operator()(const char *lhs, const char *rhs) const
    {
        return (lhs == rhs) ? false : strcmp(lhs, rhs) < 0;
    }
};

template<>
struct greater<char*>
{
    bool operator()(const char *lhs, const char *rhs) const
    {
        return (lhs == rhs) ? false : strcmp(lhs, rhs) > 0;
    }
};

template<>
struct equal_to<char*>
{
    bool operator()(const char *lhs, const char *rhs) const
    {
        return (lhs == rhs) ? true : strcmp(lhs, rhs) == 0;
    }
};
};

rde::hash_map<const char *, const char *, stringhasher_murmur, 6, rde::equal_to<char*> > urls;

static void *callback(enum mg_event event,
                      struct mg_connection *conn,
                      const struct mg_request_info *request_info) {
    char *query= request_info->uri+1;
    if (event == MG_NEW_REQUEST) {
        mg_printf(conn, "HTTP/1.1 302 Found\r\n"
                  "Location: http://%s\r\n\r\n",
                  urls[query]
                 );
        return const_cast<char *>("");
    } else {
        return NULL;
    }
}

int main()
{
    //add
    urls["0"] = "nero.im/hello-world";
    //printf("%s\n",urls[argv[1]]);
    struct mg_context *ctx;
    const char *options[] = {"listening_ports", LISTENING_PORT, NULL};

    ctx = mg_start(&callback, NULL, options);
    getchar();  // Wait until user hits "enter"
    mg_stop(ctx);

    return 0;
}
