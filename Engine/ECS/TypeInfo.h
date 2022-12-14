#pragma once
#include <cstddef>
#include <string_view>


#define DECLARE_TYPE_INFO(T) 						   \
public:												   \
	static constexpr std::string_view getTypeName()	   \
	{												   \
		return #T;									   \
	}												   \
	static constexpr std::size_t GetTypeHash()		   \
	{												   \
		auto fnvOffsetBasis = 14695981039346656037ULL; \
		constexpr auto cFnvPrime = 1099511628211ULL;   \
													   \
		for (auto idx : #T)							   \
		{											   \
			fnvOffsetBasis ^= static_cast<size_t>(idx);\
			fnvOffsetBasis *= cFnvPrime;			   \
		}											   \
		return fnvOffsetBasis;						   \
	}												   \
	void _dumyFunction() = delete


namespace type
{
	template<typename T>
	struct CallGetTypeHash
	{
	public:
		using Type = decltype(Test<T>(0));
	private:
		template<typename U>
		static auto Test(int)
			-> decltype(U::GetTypeHash(), std::true_type());
		template<typename U> 
		static auto Test(...)
			-> decltype(std::false_type());
	};

	template<typename T>
	constexpr bool kCallGetTypeName = CallGetTypeHash<T>::Type::value;
}


class TypeInfo
{
public:
	constexpr TypeInfo() : 
		type_hash_(-1),
		size_(0)
	{
	}

	constexpr bool operator== (const TypeInfo &rhs) const 
	{
		return type_hash_ == rhs.type_hash_;
	}
	constexpr bool operator!=(const TypeInfo &rhs) const
	{
		return !(*this == rhs);
	}

	[[nodiscad]] constexpr std::size_t GetHash() const
	{
		return type_hash_;
	}
	[[nodiscad]] constexpr std::size_t GeSize() const
	{
		return size_;
	}

	static constexpr TypeInfo Create(const std::size_t hash, const std::size_t size)
	{
		TypeInfo(hash, size);
	}

private:
	constexpr explicit TypeInfo(const std::size_t type_hash, const std::size_t size) :
		type_hash_(type_hash),
		size_(size)
	{
	}

	std::size_t type_hash_;
	std::size_t size_;
};