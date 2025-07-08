#ifndef UTILS_SINGLETON_H
#define UTILS_SINGLETON_H

#include <QObject>

//单例模式
template <typename T>
class Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

protected:
    Singleton() {}
    ~Singleton() {}

public:
    // 删除拷贝构造和赋值操作，确保单例的唯一性
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;
};

#endif // UTILS_SINGLETON_H