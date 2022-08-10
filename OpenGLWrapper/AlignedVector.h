#pragma once
#include <memory>
template<typename T,int alignment = alignof(T)>
class AlignedVector
{
private:
	int count = 0, al_count = 0;
	char* begin;
	int roundedSize;
public:
	const T*& Begin() { return (T*)begin; }
	int SizeInBytes() { return count * roundedSize; }
	int Count() { return count; }
	AlignedVector()
	{
		roundedSize = 0;
		while (roundedSize < sizeof(T))roundedSize += alignment;
		begin = (char*)malloc(roundedSize);
		al_count = 1;
	}
	AlignedVector(AlignedVector&& o)
	{
		begin = o.begin;
		count = o.count;
		al_count = o.al_count;
		roundedSize = o.roundedSize;
		
		o.count = 0;
		o.al_count = 0;
		o.begin = 0;
	}
	void operator=(AlignedVector&& o)
	{
		begin = o.begin;
		count = o.count;
		al_count = o.al_count;
		roundedSize = o.roundedSize;

		o.count = 0;
		o.al_count = 0;
		o.begin = 0;
	}

	void push_back(T&& t)
	{
		if (count + 1 > al_count)
		{
			char* old = begin;
			begin = (char*)malloc(roundedSize * al_count * 2);
			al_count *= 2;
			memcpy(begin, old, count * roundedSize);
			delete[] old;
		}
		(*this)[count] = std::move(t);
		count++;
	}
	void push_back(const T& t)
	{
		if (count + 1 > al_count)
		{
			char* old = begin;
			begin = (char*)malloc(roundedSize * al_count * 2);
			al_count *= 2;
			memcpy(begin, old, count * roundedSize);
			delete[] old;
		}
		(*this)[count] = t;
		count++;
	}
	void Remove(int idx)
	{
		for (int i = idx; i < count - 1; i++)
		{
			(*this)[i] = (*this)[i + 1];
		}
		count--;
	}
	T& operator[](int idx)
	{
		char* pt = begin + idx * roundedSize;
		return *((T*)pt);
	}
	~AlignedVector() {
		delete[] begin;
	}
};
