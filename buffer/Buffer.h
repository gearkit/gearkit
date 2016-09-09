#ifndef __KIT_BUFFER_H__
#define __KIT_BUFFER_H__

#include <stddef.h>
#include <stdint.h>
#include <string.h>
// not safe for thread

namespace kit {

class Buffer
{
public:
    Buffer() {};
    ~Buffer() {};

    void init(size_t size);
	void destroy();

    size_t getSize() const;
    size_t getWrittenSize() const;
    size_t getWritableSize() const;
    size_t getReadableSize() const;

    bool writeBuffer(const void* buf, size_t size);
	bool readBuffer(void* buf, size_t size);

	template<typename T>
	Buffer& operator<<(T v);

	template<typename T>
	Buffer& operator>>(T& v);

public:
    char* head_;
    char* tail_;
    char* write_cur_;
    char* read_cur_;
};

inline size_t Buffer::getSize() const
{
    return (size_t)(tail_ - head_);
}

inline size_t Buffer::getWrittenSize() const
{
    return (size_t)(write_cur_ - tail_);
}

inline size_t Buffer::getWritableSize() const
{
    return (size_t)(tail_ - write_cur_);
}

inline size_t Buffer::getReadableSize() const
{
    return (size_t)(write_cur_ - read_cur_);
}

template<typename T>
inline Buffer& Buffer::operator<<(T v)
{
	bool res = writeBuffer(&v, sizeof(T));
	//to do sothing
	return *this;
}

template<>
inline Buffer& Buffer::operator<< <const char*>(const char* v)
{
	uint16_t size = (uint16_t)strlen(v);
	(*this) << size;
	writeBuffer(v, (size_t)size);
	return *this;
}

template<typename T>
inline Buffer& Buffer::operator>>(T& v)
{
	char buf[sizeof(T)];
	if(readBuffer(buf, sizeof(T)))
		v = *((T*)buf);
	return *this;
}

template<>
inline Buffer& Buffer::operator>> <char*>(char*& v)
{
	char* buf = v;
	uint16_t size;
	(*this) >> size;
	readBuffer(buf, size);
	return *this;
}

} // namespace kit

#endif
