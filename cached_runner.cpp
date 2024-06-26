#include "cached_runner.h"
#include "task_runner.h"
using namespace std;

// 참조 변수 Cache& cache는 initializer list로 초기화
// 만들어질 때 초기화, 즉 선언과 동시에 초기화하기 위함
CachedRunner::CachedRunner(Cache &cache) : cache(cache) {
    hitsCount = 0;
    missesCount = 0;
}

double CachedRunner::multiply(string filename) {
    string key = "multiply("+filename+")";
    double result;
    // get 함수가 true를 반환했다면, result에 캐시에서 찾은 값이 들어가 있을 것
    if(cache.get(key, result)) {
        hitsCount++;
        return result;
    }
    // otherwise, 새로 계산하고 캐시에 추가
    else {
        missesCount++;
        result = TaskRunner::multiply(filename);
        cache.add(key, result);
        return result;
    }
}

int CachedRunner::palindrome(string filename) {
    string key = "palindrome("+filename+")";
    int result;
    // get 함수가 true를 반환했다면, result에 캐시에서 찾은 값이 들어가 있을 것
    if(cache.get(key, result)) {
        hitsCount++;
        return result;
    }
    // otherwise, 새로 계산하고 캐시에 추가
    else {
        missesCount++;
        result = TaskRunner::palindrome(filename);
        cache.add(key, result);
        return result;
    }
}

int CachedRunner::hits() {
    return hitsCount;
}

int CachedRunner::misses() {
    return missesCount;
}