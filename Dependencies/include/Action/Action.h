#pragma once
#include <vector>
#include<functional>
namespace EventSystem
{
	template<class ... T> class Action;
	template<>
	class Action<>
	{
	public:
		std::vector<std::function<void()>> pointers;

		void Call()
		{
			for (int i = 0; i < pointers.size(); i++)
				pointers[i]();
		}

		void operator +=(std::function<void()> func)
		{
			pointers.push_back(func);
		}

		void operator()()
		{
			Call();
		}

		template <typename TFunc, typename TInstance>
		void SubscribeMethod(TFunc func, TInstance& instance)
		{
			pointers.push_back([func, &instance]() {(instance.*func)(); });
		}
	};

	template<class T0>
	class Action<T0>
	{
	public:
		std::vector<std::function<void(T0)>> pointers;
		void Call(T0 p0)
		{
			for (int i = 0; i < pointers.size(); i++)
				pointers[i](p0);
		}

		void operator()(T0 p0)
		{
			Call(p0);
		}

		void operator +=(std::function<void(T0)> func)
		{
			pointers.push_back(func);
		}

		template <typename TFunc, typename TInstance>
		void SubscribeMethod(TFunc func, TInstance& instance)
		{
			pointers.push_back([func, &instance](T0 p0) {(instance.*func)(p0); });
		}
	};

	template<class T0, class T1>
	class Action<T0, T1>
	{
	public:
		std::vector<std::function<void(T0, T1)>> pointers;
		void Call(T0 p0, T1 p1)
		{
			for (int i = 0; i < pointers.size(); i++)
				pointers[i](p0, p1);
		}

		void operator()(T0 p0, T1 p1)
		{
			Call(p0, p1);
		}

		void operator +=(std::function<void(T0, T1)> func)
		{
			pointers.push_back(func);
		}

		template <typename TFunc, typename TInstance>
		void SubscribeMethod(TFunc func, TInstance& instance)
		{
			pointers.push_back([func, &instance](T0 p0, T1 p1) {(instance.*func)(p0, p1); });
		}
	};

	template<class T0, class T1, class T2>
	class Action<T0, T1, T2>

	{
	public:
		std::vector<std::function<void(T0, T1, T2)>> pointers;
		void Call(T0 p0, T1 p1, T2 p2)
		{
			for (int i = 0; i < pointers.size(); i++)
				pointers[i](p0, p1, p2);
		}

		void operator()(T0 p0, T1 p1, T2 p2)
		{
			Call(p0, p1, p2);
		}

		void operator +=(std::function<void(T0, T1, T2)> func)
		{
			pointers.push_back(func);
		}

		template <typename TFunc, typename TInstance>
		void SubscribeMethod(TFunc func, TInstance& instance)
		{
			pointers.push_back([func, &instance](T0 p0, T1 p1, T2 p2) {(instance.*func)(p0, p1, p2); });
		}
	};

};