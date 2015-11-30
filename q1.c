#include<stdio.h>

#include"deps/hash/khash.h"

// set of int64
KHASH_MAP_INIT_INT64(ptr, int);
// hash type
typedef khash_t(ptr) hash_t;
// allocate a new hash
#define hash_new() kh_init(ptr)
// destroy the hash
#define hash_free(self) kh_destroy(ptr, self)
// hash size
#define hash_size kh_size

void
hash_set(hash_t *self, int64_t key, int val);

int
hash_has(hash_t *self, int64_t key);

int
hash_get(hash_t *self, int64_t key);

inline void
hash_set(hash_t *self, int64_t key, int val) {
  int ret;
  khiter_t k = kh_put(ptr, self, key, &ret);
  kh_value(self, k) = val;
}

inline int
hash_has(hash_t *self, int64_t key) {
  khiter_t k = kh_get(ptr, self, key);
  return k != kh_end(self);
}

inline int
hash_get(hash_t *self, int64_t key) {
  khiter_t k = kh_get(ptr, self, key);
  return k == kh_end(self) ? NULL : kh_value(self, k);
}

#define hash_each(self, block) { \
   int64_t key; \
    for (khiter_t k = kh_begin(self); k < kh_end(self); ++k) { \
      if (!kh_exist(self, k)) continue; \
      key = kh_key(self, k); \
      block; \
    } \
  }

int main()
{
    int64_t array[1000000+100];

    hash_t *hash = hash_new();
    FILE* file = fopen("2sum.txt", "r");
    int size = 0;
    while (!feof(file)) {
        int64_t n;
        int c;
        if(fscanf(file, "%lld", &n) == 1) {
            if(c = hash_get(hash, n)) {
                hash_set(hash, n, c+1);
            } else {
                hash_set(hash, n, 1);
            }
            array[size++] = n;
        }
    }

    int result = 0;
    for(int64_t n = -10000; n <= 10000; n++) {
        if(n % 100 == 0) printf("%d %d\n", n, result);

        for(int i = 0; i < size; i++) {
            int64_t a = n - array[i];
            if(a == array[i]) {
                if(hash_get(hash, a) > 1) {
                    result++;
                    break;
                }
            } else {
                if(hash_has(hash, a)) {
                    result++;
                    break;
                }
            }
        }
    }
    printf("result: %d\n", result);

//    printf("%lld: %d (1)\n", -10002475097, hash_get(hash, -10002475097));
//    printf("%lld: %d (2)\n", -1345392551, hash_get(hash, -1345392551));
//    printf("%lld: %d (2)\n", -14267154278, hash_get(hash, -14267154278));
//    printf("%lld: %d (1)\n", 99998714216, hash_get(hash, 99998714216));

//    printf("%d\n", hash_size(hash));
}
