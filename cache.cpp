#include "cache.h"
#include <sstream>
#include <iostream>
using namespace std;

void Cache::addCache(Node *newNode) {
    // 첫 캐시 추가는 따로 처리
    if(currentSize == 0) {
        cacheList.head = newNode;
        cacheList.tail = newNode;
        currentSize++;
        return;
    }
    // 캐시가 가득 찼다면, 가장 오래전에 사용된 캐시(head) 제거
    if(currentSize == CACHE_SIZE) {
        Node *temp = cacheList.head;
        cacheList.head = cacheList.head->prev;
        cacheList.head->next = NULL;
        removeHashTable(getHash(temp->key), temp->key);
        delete temp;
    }
    cacheList.tail->prev = newNode;
    newNode->next = cacheList.tail;
    cacheList.tail = newNode;

    if(currentSize < CACHE_SIZE) 
        currentSize++;
}

void Cache::usedCache(Node *node) {
    if(node->prev == NULL) return;  // 이미 tail이면 아무 것도 할 필요가 없음
    else {
        // 노드 연결 제거
        if(node->next == NULL) {  // head 라면 따로 처리
            cacheList.head = cacheList.head->prev;
            cacheList.head->next = NULL;
        } 
        else {  // tail도 head도 아닌 그 중간 이라면
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
        // tail에 다시 추가(연결)
        cacheList.tail->prev = node;
        node->next = cacheList.tail;
        node->prev = NULL;
        cacheList.tail = node;
    }
}

int Cache::getHash(string key) {
    int hash = 0;
    for(int i = 0; i < key.length(); i++)
        // 분배법칙에 따라 매번 모듈러 연산을 하여도 결과는 같음.
        // 이렇게 하면 오버플로우 방지 가능
        hash = (hash * 31 + key[i]) % HASH_SIZE;
    return hash;
}

void Cache::addHashTable(int hash, Node *newNode) {
    // 해당 해시 인덱스에 아무것도 가리키는 노드가 없다면
    if(hashTable[hash].head == NULL) {
        hashTable[hash].head = newNode;
        hashTable[hash].tail = newNode;
    }
    // 그렇지 않고 하나라도 있다면
    else {
        hashTable[hash].tail->prev = newNode;
        newNode->next = hashTable[hash].tail;
        hashTable[hash].tail = newNode;
    }
}

void Cache::removeHashTable(int hash, string key) {
    Node *nextNode = hashTable[hash].tail;
    while(nextNode != NULL) {
        if(nextNode->key == key) {
            if(nextNode->prev == NULL) {  // tail 이라면
                hashTable[hash].tail = hashTable[hash].tail->next;
                hashTable[hash].tail->prev = NULL;
            }
            else if(nextNode->next == NULL) {  // head 라면
                hashTable[hash].head = hashTable[hash].head->next;
                hashTable[hash].head->next = NULL;
            }
            else {  // head도 tail도 아닌 중간 노드라면
                nextNode->prev->next = nextNode->next;
                nextNode->next->prev = nextNode->prev;
            }
            delete nextNode;
            break;
        }
        nextNode = nextNode->next;
    }
}

Cache::Node* Cache::searchHashTable(int hash, string key, int &value) {
    Node *nextNode = hashTable[hash].tail;
    if(nextNode == NULL) return nullptr;  // 해당 해시 인덱스에 아무것도 없으면, 등록이 안 되어 있는 것.
    while(nextNode != NULL) {    // 해시 충돌이 없었더라도 확인은 해야함
        if(nextNode->key == key) {
            value = nextNode->intValue;
            return nextNode;
        }
        nextNode = nextNode->next;
    }
    return nullptr;  // 해시 값은 같았으나, key가 다른 경우
}
Cache::Node* Cache::searchHashTable(int hash, string key, double &value) {
    Node *nextNode = hashTable[hash].tail;
    if(nextNode == NULL) return nullptr;  // 해당 해시 인덱스에 아무것도 없으면, 등록이 안 되어 있는 것.
    while(nextNode != NULL) {    // 해시 충돌이 없었더라도 확인은 해야함
        if(nextNode->key == key) {
            value = nextNode->doubleValue;
            return nextNode;
        }
        nextNode = nextNode->next;
    }
    return nullptr;  // 해시 값은 같았으나, key가 다른 경우
}

Cache::Cache() {
    cacheList.tail = NULL;
    cacheList.head = NULL;
    currentSize = 0;

    hashTable = new ListNode[HASH_SIZE];
    for(int i = 0; i < HASH_SIZE; i++) {
        hashTable[i].head = NULL;
        hashTable[i].tail = NULL;
    }
}

Cache::~Cache() {
    delete[] hashTable;
}

void Cache::add(string key, int value) {
    // palindrome 데이터 캐시 노드 생성
    Node *newNode = new Node;
    newNode->key = key;
    newNode->intValue = value;
    newNode->next = NULL;
    newNode->prev = NULL;

    addCache(newNode);
    int hash = getHash(key);
    addHashTable(hash, newNode);
}

void Cache::add(string key, double value) {
    // multiply 데이터 캐시 노드 생성
    Node *newNode = new Node;
    newNode->key = key;
    newNode->doubleValue = value;
    newNode->next = NULL;
    newNode->prev = NULL;

    addCache(newNode);
    int hash = getHash(key);
    addHashTable(hash, newNode);
}

bool Cache::get(string key, int &value) {
    int hash = getHash(key);
    Node* finding = searchHashTable(hash, key, value);
    if(finding != nullptr) {
        usedCache(finding);  // 캐시를 사용했으므로 최근 사용(tail)로 이동
        return true;
    }
    return false;
}

bool Cache::get(string key, double &value) {
    int hash = getHash(key);
    Node* finding = searchHashTable(hash, key, value);
    if(finding != nullptr) {
        usedCache(finding);  // 캐시를 사용했으므로 최근 사용(tail)로 이동
        return true;
    }
    return false;
}

string Cache::toString() {
    string tostring = "";
    Node *nextNode = cacheList.tail;
    while(true) {
        tostring += "[";
        tostring += nextNode->key;
        tostring += ": ";
        if(nextNode->key[0] == 'p') {  // palindrome 데이터 캐시
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