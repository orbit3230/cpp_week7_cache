#include "cached_runner.h"
#include "task_runner.h"
using namespace std;

CachedRunner::CachedRunner(Cache &cache) {
    this->cache = cache;
    hitsCount = 0;
    missesCount = 0;
}

double CachedRunner::multiply(string filename) {
    double result;
    // get 함수가 true를 반환했다면, result에 캐시에서 찾은 값이 들어가 있을 것
    if(cache.get(filename, result)) {
        hitsCount++;
        return result;
    }
    // otherwise, 새로 계산하고 캐시에 추가
    else {
        missesCount++;
        result = TaskRunner::multiply(filename);
        cache.add(filename, result);
        return result;
    }
}

int CachedRunner::palindrome(string filename) {
    int result;
    // get 함수가 true를 반환했다면, result에 캐시에서 찾은 값이 들어가 있을 것
    if(cache.get(filename, result)) {
        hitsCount++;
        return result;
    }
    // otherwise, 새로 계산하고 캐시에 추가
    else {
        missesCount++;
        result = TaskRunner::palindrome(filename);
        cache.add(filename, result);
        return result;
    }
}

int CachedRunner::hits() {
    return hitsCount;
}

int CachedRunner::misses() {
    return missesCount;
}