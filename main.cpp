#include <iostream>
#include <string>
#include <unordered_map>
#include "fstream"
using namespace std;
class BaseCache {
public:
    virtual bool has(const string &key) = 0;
    virtual string read(const string &key) = 0;
    virtual void write(const string &key, const string &value) = 0;
    virtual void Delete(const string &key) = 0;
    virtual void print() = 0;
};
class MemoryCache: public BaseCache{
    unordered_map<string,string> Cache;
public:
    void print()override{
        for(auto found : Cache){
            cout << found.first << "\t" << found.second << endl;
        }
    }
    string read(const string &key)override{
        for(auto found : Cache){
            if(found.first == key)
                return found.second;
        }
        return "Key not found!!!";
    }
    void write(const string &key, const string &value)override{
        Cache[key] = value;
    }
    void Delete(const string &key)override{
        Cache.erase(key);
    }
    bool has(const string &key)override{
        for(auto found : Cache){
            if(found.first == key)
                return true;
        }
        return false;
    }
};
class FileCache: public BaseCache{
    string filename;
public:
    virtual bool has(const string &key)override{
        ifstream is;
        is.open(filename, ifstream::in);
        if(!is.is_open())
            throw exception();
        string str = "";
        string str2 = "";
        while(is.eof()){
            is >> str;
            is >> str2;
            if (str == key){
                return true;
            }else{
                str.clear();
                str2.clear();
            }
        }
        is.close();
        return false;
    }
    void print()override{
        ifstream is;
        is.open(filename);
        if(!is.is_open())
            throw exception();
        string str = "";
        while(!is.eof()) {
            is >> str;
            cout << str << "\t";
            is >> str;
            cout << str << endl;
            str.clear();
        }
    }
    virtual string read(const string &key)override{
        ifstream is;
        is.open(filename, ifstream::in);
        if(!is.is_open())
            throw exception();
        string str = "";
        string str2 = "";
        while(!is.eof()){
            is >> str;
            is >> str2;
            if (str == key){
               return str2;
            }else{
                str.clear();
                str2.clear();
            }
        }
        is.close();
        return "Key not found!!!";
    }
    virtual void write(const string &key, const string &value)override{
        ofstream os;
        os.open(filename, ofstream::out | ofstream::app);
        if(!os.is_open())
            throw exception();
        os << key << "\t" << value << endl;
        os.close();
    }
    virtual void Delete(const string &key){
        ifstream is;
        unordered_map<string, string> cache;
        is.open(filename);
        if(!is.is_open())
            throw exception();
        string str = "";
        string str2 = "";
        while(!is.eof()){
            is >> str;
            is >> str2;
            if (str != key){
                cache[str] = str2;
                str.clear();
                str2.clear();
            }else{
                str.clear();
                str2.clear();
            }
        }
        is.close();
        ofstream os;
        os.open(filename, ofstream::out | ofstream::trunc);
        for(auto temp : cache){
            os << temp.first << "\t" << temp.second << endl;
        }
    }
    FileCache(string filename = "cash.txt"): filename(filename){
        ofstream os;
        os.open(filename);
        if(!os.is_open())
            throw exception();
        os.close();
    }
};
class NullCache: public BaseCache{
public:
    virtual bool has(const string &key)override{
        return false;
    }
    virtual string read(const string &key)override{
        return "This is a NULL cache! and it hold NOTHING!!!";
    }
    virtual void write(const string &key, const string &value)override{
        cout << "This is a NULL cash and it store nothing!!!" << endl;
        return;
    }
    virtual void Delete(const string &key)override{
        cout << "It is nothing to delete here!!!" << endl;
    }
    void print()override{
        cout << "It is a NULL Cache!!!" << endl;
    }
};
class PoorManMemoryCache: public BaseCache{
    int size;
    string Key[10];
    string Value[10];
public:
    void print()override{
        for(int i = 0; i < size; i++){
            cout << Key[i] << "\t" << Value[i] << endl;
        }
    }
    string read(const string &key)override{
        for(int i = 0; i < 10; i++){
            if(Key[i] == key)
                return Value[i];
        }
        return "Key not found!!!";
    }
    void write(const string &key, const string &value)override{
        if(size == 10){
            for(int i = 1; i < 10; i++){
                Key[i-1] = Key[i];
                Value[i-1] = Value[i];
            }
            Key[9] = key;
            Value[9] = value;
        }else{
            Key[size] = key;
            Value[size] = value;
            size++;
        }
    }
    void Delete(const string &key)override{
        for (int i = 0; i < 9; ++i) {
            if(Key[i] == key){
                for(int j = i; j < 10 && Key[j] != ""; j++ ){
                    Key[j] = Key[j+1];
                    Value[j] = Value[j+1];
                }
                size--;
            }
        }
    }
    bool has(const string &key)override{
        for (int i = 0; i < 10; i++){
            if (Key[i] == key)
                return true;
        }
        return false;
    }
    PoorManMemoryCache(): size(0){
        for(int i = 0; i < 10; i++){
            Key[i] = "";
            Value[i] = "";
        }
    }
};
class CacheApplier {
    BaseCache *strategy;
public:
    void set_strategy(BaseCache* strategy){
        delete this->strategy;
        this->strategy = strategy;
    }
    string read_from_cache(const string &key){
        return strategy->read(key);
    }
    void write_from_cache(const string &key, const string &value){
        strategy->write(key,value);
    }
    void delete_from_cache(const string &key){
        strategy->Delete(key);
    }
    void print(){
        strategy->print();
    }
    CacheApplier(BaseCache *strategy = nullptr): strategy(strategy){}
    ~CacheApplier(){
        if(strategy != nullptr)
            delete strategy;
    }
};
int main() {
    setlocale(LC_ALL, "");
    CacheApplier* cache = new CacheApplier(new FileCache);
    cache->write_from_cache("1","12345");
    cache->write_from_cache("2","12345");
    cache->write_from_cache("3","12345");
    cache->write_from_cache("4","12345");
    cache->print();
    cout << "_______________________________________" << endl;
    cout << cache->read_from_cache("4") << endl;
    cout << "_______________________________________" << endl;
    cache->delete_from_cache("3");
    cache->print();
    return 0;
}