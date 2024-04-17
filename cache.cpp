#include "cache.h"
#include <sstream>
using namespace std;

void Cache::addCache(Node *newNode) {
    // 첫 캐시 추가는 따로 처리
    if(currentSize == 0) {
        head = newNode;
        tail = newNode;
        currentSize++;
        return;
    }
    // 캐시가 가득 찼다면, 가장 오래전에 사용된 캐시(head) 제거
    if(currentSize == CACHE_SIZE) {
        Node *temp = head;
        head = head->prev;
        head->next = NULL;
        delete temp;
    }
    tail->prev = newNode;
    newNode->next = tail;
    tail = newNode;

    if(currentSize < CACHE_SIZE)
        currentSize++;
}

void Cache::usedCache(Node *node) {
    if(node->prev == NULL) return;  // 이미 tail이면 아무 것도 할 필요가 없음
    else {
        // 노드 연결 제거
        if(node->next == NULL) {  // head 라면 따로 처리
            head = head->prev;
            head->next = NULL;
        } 
        else {  // tail도 head도 아닌 그 중간 이라면
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        // tail에 다시 추가(연결)
        tail->prev = node;
        node->next = tail;
        node->prev = NULL;
        tail = node;
    }
}

Cache::Cache() {
    tail = NULL;
    head = NULL;
    cacheList = new Node[CACHE_SIZE];
    currentSize = 0;
}

Cache::~Cache() {
    delete[] cacheList;
}

void Cache::add(string key, int value) {
    // palindrome 데이터 캐시 노드 생성
    Node *newNode = new Node;
    newNode->type = "palindrome";
    newNode->filename = key;
    newNode->intValue = value;
    newNode->next = NULL;
    newNode->prev = NULL;

    addCache(newNode);
}

void Cache::add(string key, double value) {
    // multiply 데이터 캐시 노드 생성
    Node *newNode = new Node;
    newNode->type = "multiply";
    newNode->filename = key;
    newNode->doubleValue = value;
    newNode->next = NULL;
    newNode->prev = NULL;

    addCache(newNode);
}

bool Cache::get(string key, int &value) {
    Node *nextNode = tail;
    // 캐시 탐색
    while(nextNode != NULL) {
        if(nextNode->filename == key) {
            value = nextNode->intValue;
            usedCache(nextNode);  // 캐시를 사용했으므로 최근 사용(tail)로 이동
            return true;
        }
        nextNode = nextNode->next;
    }
    return false;
}

bool Cache::get(string key, double &value) {
    Node *nextNode = tail;
    // 캐시 탐색
    while(nextNode != NULL) {
        if(nextNode->filename == key) {
            value = nextNode->doubleValue;
            usedCache(nextNode);  // 캐시를 사용했으므로 최근 사용(tail)로 이동
            return true;
        }
        nextNode = nextNode->next;
    }
    return false;
}

string Cache::toString() {
    string tostring = "";
    Node *nextNode = tail;
    while(true) {
        tostring += "[";
        tostring += nextNode->type;
        tostring += "(";
        tostring += nextNode->filename;
        tostring += ")";
        tostring += ": ";
        if(nextNode->type == "palindrome") {  // palindrome 데이터 캐시
            tostring += to_string(nextNode->intValue);
        } 
        else {                                // multiply 데이터 캐시
            ostringstream ss;
            ss << nextNode->doubleValue;
            tostring += ss.str();
        }
        tostring += "]";
        if(nextNode->next != NULL) {
            tostring += " -> ";
            nextNode = nextNode->next;
        }
        else
            break;
    }
    tostring += "\n";
    return tostring;
}