#ifndef CACHE_H
#define CACHE_H

#include <string>

#define CACHE_SIZE 10

class Cache {
private:
  // 캐시 노드
  struct Node {
    std::string type;
    std::string filename;
    int intValue;
    double doubleValue;
    Node *next; // 다음 노드를 가리키는 포인터
    Node *prev; // 이전 노드를 가리키는 포인터
  };
  // 캐시 테일 노드
  Node *tail;
  // 캐시 헤드 노드
  Node *head;
  // 캐시 리스트
  Node *cacheList;
  // 현재 캐시 사이즈
  int currentSize;

  // 캐시 노드를 추가하는 함수
  void addCache(Node *newNode);
  // 캐시가 사용되었다면 tail로 보내는 함수
  void usedCache(Node *node);

public:
  Cache();
  ~Cache();
  // int를 cache에 추가한다
  void add(std::string key, int value);
  // double을 cache에 추가한다
  void add(std::string key, double value);
  // key에 해당하는 value를 cache에서 가져온다
  // 타입과 key가 동일한 원소가 없다면 false를 반환한다.
  bool get(std::string key, int &value);
  // key에 해당하는 value를 cache에서 가져온다.
  // 타입과 key가 동일한 원소가 없다면 false를 반환한다.
  bool get(std::string key, double &value);

  // 디버그 및 채점 용: 연결 리스트를 문자열로 표현하여 반환한다
  // 다음과 같이 표현된 문자열을 반환한다
  // [key1: value1] -> [key2: value2] -> ... -> [keyN: valueN]
  std::string toString();
};

#endif