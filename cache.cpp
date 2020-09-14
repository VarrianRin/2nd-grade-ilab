#include <iostream>
#include <stdio.h>
#include <list>
#include <unordered_map>
#include <time.h>
#include <stdlib.h>

typedef int Elem_t;

struct Elem {

    Elem_t value = 0;
    int use_num  = 1;
};

typedef std::list<Elem>::iterator ListIt;
typedef std::unordered_map<int, ListIt>::iterator HashIt;

struct cache_t {

    size_t size = 0;
    std::list<Elem> calist;
    std::unordered_map<int, ListIt> hash_;

    cache_t(size_t sz) : size(sz) {}

    bool full() /*const?*/{
        return(calist.size() == size);
    }

    ListIt LeastUsed() {

        ListIt min  = calist.end();
        min--; //calist.end() - 1??????
        ListIt elem = calist.end();

        for(int i = size - 1; i >= 0; i--)
            if((--elem)->use_num < min->use_num)
                min = elem;

        return min;
    }

    bool LookUp(const Elem* elem) {

        if (!elem) {
            printf("Null pointer!");
            return 0;
        }

        HashIt hit = hash_.find(elem->value);

        if (hit == hash_.end()) {

            if (full()) {

                ListIt les_use = LeastUsed();
                calist.erase(les_use);
                hash_.erase(les_use->value);
            }

            calist.push_front(*elem);
            hash_[elem->value] = calist.begin();
            return 0;
        }

        ListIt eltit = hit->second;
        (eltit->use_num)++;
        if (eltit != calist.begin())
            calist.splice(calist.begin(), calist, eltit, std::next(eltit));

        return 1;

    }
};

int CacheTests(int size, int keys);

int main() {

    int hints    = 0,
        size     = 0,
        n        = 0,
        max_size = 0,
        max_keys = 0;
    Elem elem;

    scanf("%d %d", &size, &n);
    cache_t cache{size};

    for (int i = 0; i < n; i++) {

        scanf("%d", &elem.value);
        if (cache.LookUp(&elem))
            hints++;
    }

    printf("hints number = %d\n", hints);

//TEST
    scanf("%d %d", &max_size, &max_keys);
    CacheTests(max_size, max_keys);

    return 0;
}

int CacheTests(int size, int keys) {

    int hints = 0;
    Elem elem;
    srand(time(NULL));

    for (int i = 1; i <= size; i++) {

        cache_t cache{i};

        for (int j = 1; j <= keys; j++) {

            for (int k = 1; k <= j; k++) {

                elem.value = rand() % keys;

                if (cache.LookUp(&elem))
                    hints++;
            }

            //printf("%d[%d, %d], ", hints, i, j);
            hints = 0;
        }
    }
}
