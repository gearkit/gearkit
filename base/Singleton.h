#ifndef __KIT_SINGLETON_H__
#define __KIT_SINGLETON_H__

#include <memory>

/** 自动析构的单例模版 */
template <typename T>
class Singleton
{
public:
    Singleton()
    {
        instance_.reset((T*)this);
    }
	~Singleton()
	{
		instance_.release();
	}

	static T * getInstance()
	{
		if (0 == instance_.get())
		{
			T* p = new T();
			p->init();
		} 
		return instance_.get();
	}
    static void destroyInstance()
    {
        Singleton<T>* p = instance_.get();
        if(p)
        {
			p->destory();
        }
        instance_.reset(0);
    }
protected:
	virtual void init(){};
	virtual void destory(){};
private:
	static std::auto_ptr<T>		instance_;
};

template< class T > std::auto_ptr<T>  Singleton<T>::instance_(0);

#endif
