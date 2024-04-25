#pragma once

#define _CRT_SECURE_NO_WARNINGS


#include "library_definitions.hpp"

#include <xmmintrin.h>
#include <immintrin.h>
#include <string>
#include <vector>



u_int c5;
#define	wsize	sizeof(u_int)
#define	wmask	(wsize - 1)

#ifdef BZERO
#define	RETURN	return
#define	VAL	0
#define	WIDEVAL	0
void
bzero(dst0, length)
void* dst0;
register size_t length;
#else
#define	RETURN	return (dst0)
#define	VAL	c0
#define	WIDEVAL	c5
#endif

#define UC(c)	((unsigned char)c)

struct custom {
public : 

	int toupper(int c)
	{
		if (c >= 'a' && c <= 'z') return c - 'a' + 'A';
		return c;
	}

	int tolower(int c)
	{
		if (c >= 'A' && c <= 'Z') return c - 'A' + 'a';
		return c;
	}

	void* memcpy(void* dest, const void* src, unsigned __int64 count)
	{
		char* char_dest = (char*)dest;
		char* char_src = (char*)src;
		if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
		{
			while (count > 0)
			{
				*char_dest = *char_src;
				char_dest++;
				char_src++;
				count--;
			}
		}
		else
		{
			char_dest = (char*)dest + count - 1;
			char_src = (char*)src + count - 1;
			while (count > 0)
			{
				*char_dest = *char_src;
				char_dest--;
				char_src--;
				count--;
			}
		}
		return dest;
	}


	void* memset(void* dst0, register int c0, register size_t length)
	{
		register size_t t;
		register u_char* dst;

		dst = (u_char*)dst0;

		if (length < 3 * wsize) {
			while (length != 0) {
				*dst++ = c0;
				--length;
			}
			RETURN;
		}

		if ((t = (int)dst & wmask) != 0) {
			t = wsize - t;
			length -= t;
			do {
				*dst++ = c0;
			} while (--t != 0);
		}

		t = length / wsize;
		do {
			*(u_int*)dst = WIDEVAL;
			dst += wsize;
		} while (--t != 0);

		t = length & wmask;
		if (t != 0)
			do {
				*dst++ = c0;
			} while (--t != 0);
			RETURN;
	}

	void* memccpy(void* to, const void* from, int c, unsigned __int64 count)
	{
		char t;
		unsigned __int64 i;
		char* dst = (char*)to;
		const char* src = (const char*)from;
		for (i = 0; i < count; i++)
		{
			dst[i] = t = src[i];
			if (t == 0) break;
			if (t == c) return &dst[i + 1];
		}
		return 0;
	}

	void* memchr(const void* s, int c, unsigned __int64 n)
	{
		if (n)
		{
			const char* p = (const char*)s;
			do
			{
				if (*p++ == c) return (void*)(p - 1);
			} while (--n != 0);
		}
		return 0;
	}

	int memcmp(const void* s1, const void* s2, unsigned __int64 n)
	{
		if (n != 0)
		{
			const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
			do
			{
				if (*p1++ != *p2++) return (*--p1 - *--p2);
			} while (--n != 0);
		}
		return 0;
	}

	int memicmp(const void* s1, const void* s2, unsigned __int64 n)
	{
		if (n != 0)
		{
			const unsigned char* p1 = (unsigned char*)s1, * p2 = (unsigned char*)s2;
			do
			{
				if (toupper(*p1) != toupper(*p2)) return (*p1 - *p2);
				p1++;
				p2++;
			} while (--n != 0);
		}
		return 0;
	}

	void* memmove(void* dest, const void* src, unsigned __int64 count)
	{
		char* char_dest = (char*)dest;
		char* char_src = (char*)src;
		if ((char_dest <= char_src) || (char_dest >= (char_src + count)))
		{
			while (count > 0)
			{
				*char_dest = *char_src;
				char_dest++;
				char_src++;
				count--;
			}
		}
		else
		{
			char_dest = (char*)dest + count - 1;
			char_src = (char*)src + count - 1;
			while (count > 0)
			{
				*char_dest = *char_src;
				char_dest--;
				char_src--;
				count--;
			}
		}
		return dest;
	}

	char* _cslwr(char* x)
	{
		char* y = x;
		while (*y)
		{
			*y = tolower(*y);
			y++;
		}
		return x;
	}

	char* _csupr(char* x)
	{
		char* y = x;
		while (*y)
		{
			*y = tolower(*y);
			y++;
		}
		return x;
	}

	int strlen(const char* string)
	{
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}

	int wcslen(const wchar_t* string)
	{
		int cnt = 0;
		if (string)
		{
			for (; *string != 0; ++string) ++cnt;
		}
		return cnt;
	}

	const char* strcpy(char* buffer, const char* string)
	{
		char* ret = buffer;
		while (*string) *buffer++ = *string++;
		*buffer = 0;
		return ret;
	}

	int strcmp(const char* cs, const char* ct)
	{
		if (cs && ct)
		{
			while (*cs == *ct)
			{
				if (*cs == 0 && *ct == 0) return 0;
				if (*cs == 0 || *ct == 0) break;
				cs++;
				ct++;
			}
			return *cs - *ct;
		}
		return -1;
	}

	int stricmp(const char* cs, const char* ct)
	{
		if (cs && ct)
		{
			while (tolower(*cs) == tolower(*ct))
			{
				if (*cs == 0 && *ct == 0) return 0;
				if (*cs == 0 || *ct == 0) break;
				cs++;
				ct++;
			}
			return tolower(*cs) - tolower(*ct);
		}
		return -1;
	}



	float fabsf(float x)
	{
		{
			if (x < 0)
			{
				return -x;
			}
			return x;
		}
	}

	const char* strstr(const char* string, char* substring)
	{
		const char* a, * b;
		b = substring;
		if (*b == 0) {
			return string;
		}
		for (; *string != 0; string += 1) {
			if (*string != *b) {
				continue;
			}
			a = string;
			while (1) {
				if (*b == 0) {
					return string;
				}
				if (*a++ != *b++) {
					break;
				}
			}
			b = substring;
		}
		return NULL;
	}

	static bool is_bad_write_ptr(void* ptr, unsigned long long size)
	{
		return ptr ? false : true;
	}

	int _wcsicmp(const wchar_t* string1, const wchar_t* string2)
	{
		wchar_t c1, c2;
		do
		{
			c1 = *string1++;
			c2 = *string2++;
			if (c2 == L'\0')
				return c1 - c2;
		} while (c1 == c2);
		return c1 < c2 ? -1 : 1;
	}

	size_t strnlen(const char* s, size_t maxlen)
	{
		size_t i;
		for (i = 0; i < maxlen; ++i)
			if (s[i] == '\0')
				break;
		return i;
	}

	char* strncpy(void* s1, const void* s2, size_t n)
	{
		size_t size = strnlen((char*)s2, n);
		if (size != n)
			memset((char*)s1 + size, '\0', n - size);
		return (char*)memcpy(s1, s2, size);
	}

	double floor(double num)
	{
		if (num < 0)
			return (int)num - 1;
		else
			return (int)num;
	}

	int isspace(int c)
	{
		return (c == '\t' || c == '\n' ||
			c == '\v' || c == '\f' || c == '\r' || c == ' ' ? 1 : 0);
	}

	int isdigit(int c)
	{
		return (c >= '0' && c <= '9' ? 1 : 0);
	}

	int isalpha(int c)
	{
		return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0);
	}

	char isupper(unsigned char c)
	{
		if (c >= UC('A') && c <= UC('Z'))
			return 1;
		return 0;
	}

	//unsigned long strtoul(const char* nptr, char** endptr, register int base)
	//{
	//	register const char* s = nptr;
	//	register unsigned long acc;
	//	register int c;
	//	register unsigned long cutoff;
	//	register int neg = 0, any, cutlim;
	//	do {
	//		c = *s++;
	//	} while (isspace(c));
	//	if (c == '-') {
	//		neg = 1;
	//		c = *s++;
	//	}
	//	else if (c == '+')
	//		c = *s++;
	//	if ((base == 0 || base == 16) &&
	//		c == '0' && (*s == 'x' || *s == 'X')) {
	//		c = s[1];
	//		s += 2;
	//		base = 16;
	//	}
	//	if (base == 0)
	//		base = c == '0' ? 8 : 10;
	//	cutoff = (unsigned long)ULONG_MAX / (unsigned long)base;
	//	cutlim = (unsigned long)ULONG_MAX % (unsigned long)base;
	//	for (acc = 0, any = 0;; c = *s++) {
	//		if (isdigit(c))
	//			c -= '0';
	//		else if (isalpha(c))
	//			c -= isupper(c) ? 'A' - 10 : 'a' - 10;
	//		else
	//			break;
	//		if (c >= base)
	//			break;
	//		if (any < 0 || acc > cutoff || acc == cutoff && c > cutlim)
	//			any = -1;
	//		else {
	//			any = 1;
	//			acc *= base;
	//			acc += c;
	//		}
	//	}
	//	if (any < 0) {
	//		acc = ULONG_MAX;
	//		errno = ERANGE;
	//	}
	//	else if (neg)
	//		acc = -acc;
	//	if (endptr != 0)
	//		*endptr = (char*)(any ? s - 1 : nptr);
	//	return (acc);
	//}

	inline int wcompare(const wchar_t* X, const wchar_t* Y)
	{
		while (*X && *Y) {
			if (*X != *Y) {
				return 0;
			}
			X++;
			Y++;
		}

		return (*Y == L'\0');
	}

	inline const wchar_t* wcsstr(const wchar_t* X, const wchar_t* Y)
	{
		while (*X != L'\0') {
			if ((*X == *Y) && wcompare(X, Y)) {
				return X;
			}
			X++;
		}
		return NULL;
	}


	inline float sqrtf(float number)
	{
		float temp, mysqrt;
		mysqrt = number / 2;
		temp = 0;
		while (mysqrt != temp) {
			temp = mysqrt;
			mysqrt = (number / temp + temp) / 2;
		}
		return mysqrt;
	}


	std::string to_string(int n)
	{
		int m = n;
		char s[100]{}; //max
		char ss[100]{}; //max
		int i = 0, j = 0;
		if (n < 0)
		{
			m = 0 - m;
			j = 1;
			ss[0] = '-';
		}
		while (m > 0)
		{
			s[i++] = m % 10 + '0';
			m /= 10;
		}
		s[i] = '\0';
		i = i - 1;
		while (i >= 0)
		{
			ss[j++] = s[i--];
		}
		ss[j] = '\0';
		return ss;
	}

	float powf(float _X, float _Y) { return _mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))); }
} windows;

class BaseSpinLock {
private:
	volatile long _lock;

public:
	BaseSpinLock() : _lock(0) {}

	void lock() {
		while (_InterlockedCompareExchange(&_lock, 1, 0) != 0) {
			_mm_pause();
		}
	}

	void unlock() {
		_InterlockedExchange(&_lock, 0);
	}
};

class RLock {
private:
	BaseSpinLock _lock;
	volatile const char* _ownerThreadId;
	volatile long _depth;

public:
	RLock() : _ownerThreadId(0), _depth(0) {}

	void lock() {
		const char* currentThreadId = reinterpret_cast<const char*>(__readgsqword(0x30)); // 0x30 -> TEB

		if (_ownerThreadId == currentThreadId) {
			_depth++;
		}
		else {
			_lock.lock();
			_ownerThreadId = currentThreadId;
			_depth = 1;
		}
	}

	void unlock() {
		const char* currentThreadId = reinterpret_cast<const char*>(__readgsqword(0x30)); // 0x30 -> TEB

		if (_ownerThreadId == currentThreadId) {
			if (--_depth == 0) {
				_ownerThreadId = 0;
				_lock.unlock();
			}
		}
	}
};

template <typename T, size_t N>
class NoArray {
private:
	T _data[N];
	static T _errorValue;

public:
	class Iterator {
	private:
		T* ptr;

	public:
		Iterator(T* p) : ptr(p) {}

		Iterator& operator++() {
			ptr++;
			return *this;
		}

		Iterator operator++(int) {
			Iterator temp = *this;
			(*this)++;
			return temp;
		}

		T& operator*() const {
			return *ptr;
		}

		T* operator->() {
			return ptr;
		}

		bool operator==(const Iterator& other) const {
			return ptr == other.ptr;
		}

		bool operator!=(const Iterator& other) const {
			return ptr != other.ptr;
		}
	};

	NoArray() : _data{} {}

	Iterator begin() {
		return Iterator(_data);
	}

	Iterator end() {
		return Iterator(_data + N);
	}

	T& at(size_t index) {
		if (index >= N)
			return _errorValue;
		return _data[index];
	}

	const T& at(size_t index) const {
		if (index >= N)
			return _errorValue;
		return _data[index];
	}

	constexpr bool empty() const { return N == 0; }

	constexpr size_t size() const { return N; }

	T& operator[](size_t index) {
		return _data[index];
	}

	const T& operator[](size_t index) const {
		return _data[index];
	}

	T& front() { return _data[0]; }

	const T& front() const { return _data[0]; }

	T& back() { return _data[N - 1]; }

	const T& back() const { return _data[N - 1]; }

	void fill(const T& value) {
		for (size_t i = 0; i < N; i++) {
			_data[i] = value;
		}
	}

	T* data() { return _data; }

	const T* data() const { return _data; }
};

template<typename T, size_t N>
T NoArray<T, N>::_errorValue = T{};

template <size_t HeapSize = 1024 * 1024>
class NoAlloc {
private:
	struct BlockHeader {
		size_t          size;
		bool            allocated;
		BlockHeader* next;
	};

	static uint8_t      _heap[HeapSize];

	BlockHeader* _firstBlock;
	RLock               _lock;
	size_t              _totalHeapSize;
	uint8_t* _heapBase;  // start
	uint8_t* _heapLimit; // end + 1

	void memcpy(void* dest, const void* src, size_t n) {
		char* d = reinterpret_cast<char*>(dest);
		const char* s = reinterpret_cast<const char*>(src);
		while (n--) {
			*d++ = *s++;
		}
	}

	void memset(void* ptr, int val, size_t n) {
		unsigned char* p = reinterpret_cast<unsigned char*>(ptr);
		while (n-- > 0) {
			*p++ = static_cast<unsigned char>(val);
		}
	}

	void memmove(void* dest, const void* src, size_t n) {
		char* d = reinterpret_cast<char*>(dest);
		const char* s = reinterpret_cast<const char*>(src);

		if (d < s) {
			while (n--) {
				*d++ = *s++;
			}
		}
		else {
			d += n;
			s += n;
			while (n--) {
				*--d = *--s;
			}
		}
	}

	int memcmp(const void* s1, const void* s2, size_t n) {
		const unsigned char* p1 = reinterpret_cast<const unsigned char*>(s1);
		const unsigned char* p2 = reinterpret_cast<const unsigned char*>(s2);
		while (n-- > 0) {
			if (*p1 != *p2) {
				return *p1 - *p2;
			}
			p1++;
			p2++;
		}
		return 0;
	}


	void calcStats(size_t& totalAllocated, size_t& totalFree, size_t& largestFreeBlock, size_t& freeBlocksCount) const {
		totalAllocated = 0;
		totalFree = 0;
		largestFreeBlock = 0;
		freeBlocksCount = 0;

		BlockHeader* current = _firstBlock;
		while (current != nullptr) {
			if (current->allocated) {
				totalAllocated += current->size;
			}
			else {
				totalFree += current->size;
				++freeBlocksCount;
				if (current->size > largestFreeBlock) {
					largestFreeBlock = current->size;
				}
			}
			current = current->next;
		}
	}

	bool isValidHeapPtr(void* ptr) const {
		return ptr >= _heapBase && ptr < _heapLimit;
	}

public:
	NoAlloc() : _totalHeapSize(HeapSize), _heapBase(_heap), _heapLimit(_heap + HeapSize) {
		_firstBlock = reinterpret_cast<BlockHeader*>(_heapBase);
		_firstBlock->size = HeapSize - sizeof(BlockHeader);
		_firstBlock->allocated = false;
		_firstBlock->next = nullptr;
	}


	void* malloc(size_t size) {
		_lock.lock();
		BlockHeader* current = _firstBlock;
		while (current != nullptr) {
			if (!current->allocated && current->size >= size + sizeof(BlockHeader)) {
				size_t remainingSize = current->size - size - sizeof(BlockHeader);

				current->allocated = true;
				current->size = size;

				if (remainingSize > sizeof(BlockHeader)) {
					BlockHeader* newBlock = reinterpret_cast<BlockHeader*>(reinterpret_cast<char*>(current + 1) + size);
					newBlock->size = remainingSize;
					newBlock->allocated = false;
					newBlock->next = current->next;
					current->next = newBlock;
				}
				_lock.unlock();

				merge();
				return reinterpret_cast<void*>(current + 1);
			}
			current = current->next;
		}
		_lock.unlock();
		return nullptr;
	}

	void free(void* ptr) {
		if (ptr == nullptr) {
			return;
		}

		_lock.lock();

		// free ptr out of bounds
		if (!isValidHeapPtr(ptr)) {
			_lock.unlock();
			return;
		}

		// free already freed ptr
		BlockHeader* header = reinterpret_cast<BlockHeader*>(ptr) - 1;
		if (!header->allocated) {
			_lock.unlock();
			return;
		}

		header->allocated = false;

		merge();

		_lock.unlock();
	}

	void* realloc(void* ptr, size_t newSize) {
		if (ptr == nullptr) {
			return malloc(newSize);
		}

		BlockHeader* header = reinterpret_cast<BlockHeader*>(ptr) - 1;
		if (header->size >= newSize) {
			return ptr;
		}

		void* newPtr = malloc(newSize);
		if (newPtr != nullptr) {
			memcpy(newPtr, ptr, header->size);
			free(ptr);
		}
		return newPtr;
	}

	void* calloc(size_t numElements, size_t elementSize) {
		_lock.lock();
		size_t totalSize = numElements * elementSize;
		void* ptr = malloc(totalSize);
		if (ptr != nullptr) {
			memset(ptr, 0, totalSize);
		}
		_lock.unlock();
		return ptr;
	}

	void merge() {
		BlockHeader* current = _firstBlock;
		while (current != nullptr && current->next != nullptr) {
			if (!current->allocated && !current->next->allocated) {
				current->size += current->next->size + sizeof(BlockHeader);
				current->next = current->next->next;
			}
			else {
				current = current->next;
			}
		}
	}

};

NoAlloc allocation;


template <size_t HeapSize>
uint8_t NoAlloc<HeapSize>::_heap[HeapSize];

template<typename T> struct remove_reference;

template<typename T> struct remove_reference<T&> {
	typedef typename remove_reference<T>::type type;
};

template<typename T> struct remove_reference {
	typedef T type;
};

template<typename T> struct remove_const;

template<typename T> struct remove_const<const T> {
	typedef typename remove_const<T>::type type;
};

template<typename T> struct remove_const {
	typedef T type;
};

template<typename T> struct remove_volatile;

template<typename T> struct remove_volatile<volatile T> {
	typedef typename remove_volatile<T>::type type;
};

template<typename T> struct remove_volatile {
	typedef T type;
};

template<typename T>
struct decay {
private:
	typedef typename remove_reference<T>::type no_ref;
	typedef typename remove_const<no_ref>::type no_const;
	typedef typename remove_volatile<no_const>::type type;

public:
	typedef type type;
};

// std::true_type and std::false_type impl

struct true_type {
	static constexpr bool value = true;
	typedef bool value_type;
	typedef true_type type;
	constexpr operator bool() const noexcept { return value; }
};

struct false_type {
	static constexpr bool value = false;
	typedef bool value_type;
	typedef false_type type;
	constexpr operator bool() const noexcept { return value; }
};

// base template

template<typename T> struct is_integral_base : false_type {};

// specializations -> 15 total integral types

// Boolean:

template<> struct is_integral_base<bool> : true_type {};

// Characters:

template<> struct is_integral_base<char> : true_type {};
template<> struct is_integral_base<signed char> : true_type {};
template<> struct is_integral_base<unsigned char> : true_type {};
template<> struct is_integral_base<wchar_t> : true_type {};
template<> struct is_integral_base<char16_t> : true_type {};
template<> struct is_integral_base<char32_t> : true_type {};

// Integers:

template<> struct is_integral_base<short> : true_type {};
template<> struct is_integral_base<unsigned short> : true_type {};
template<> struct is_integral_base<int> : true_type {};
template<> struct is_integral_base<unsigned int> : true_type {};
template<> struct is_integral_base<long> : true_type {};
template<> struct is_integral_base<unsigned long> : true_type {};
template<> struct is_integral_base<long long> : true_type {};
template<> struct is_integral_base<unsigned long long> : true_type {};

// Custom one just in case for some other type

template<typename T>
struct is_integral : is_integral_base<typename decay<T>::type> {};

// is_same impl

template<typename T, typename U> struct is_same : false_type {};

template<typename T> struct is_same<T, T> : true_type {};

// is_same_v impl

template<typename T, typename U> constexpr bool is_same_v = is_same<T, U>::value; //constexprs are implicitly inline

// is_array impl

template<typename T> struct is_array : false_type {};

template<typename T, size_t N> struct is_array<NoArray<T, N>> : true_type {};

// is_void impl

template<typename T> struct is_void : is_same<typename decay<T>::type, void> {};

// enable_if impl   

template<bool cond, typename T = void> struct enable_if {};

template<typename T> struct enable_if<true, T> { typedef T type; };

template<bool cond, typename T = void> struct enable_if_t { typedef typename enable_if<cond, T>::type type; };

// std::move and std::forward impl

template<typename T>
constexpr typename remove_reference<T>::type&& move(T&& arg) noexcept {
	return static_cast<typename remove_reference<T>::type&&>(arg);
}

template<typename T>
constexpr T&& forward(typename remove_reference<T>::type& arg) noexcept {
	return static_cast<T&&>(arg);
}

PEB* get_peb()
{
	PEB* peb = (PEB*)__readgsqword(0x60);

	return peb;
}

uintptr_t get_base_address(const wchar_t* mod_name)
{
	LDR_DATA_TABLE_ENTRY* mod_entry = nullptr;

	PEB* peb = get_peb();

	LIST_ENTRY head = peb->Ldr->InMemoryOrderModuleList;

	LIST_ENTRY curr = head;

	for (auto curr = head; curr.Flink != &peb->Ldr->InMemoryOrderModuleList; curr = *curr.Flink)
	{
		LDR_DATA_TABLE_ENTRY* mod = (LDR_DATA_TABLE_ENTRY*)CONTAINING_RECORD(curr.Flink, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

		if (mod->BaseDllName.Buffer)
		{
			if (windows._wcsicmp((const wchar_t*)mod_name, (const wchar_t*)mod->BaseDllName.Buffer) == 0)
			{
				mod_entry = mod;
				break;
			}
		}
	}

	return (uintptr_t)mod_entry->DllBase;
}


__forceinline uintptr_t pattern_scan(std::string pattern, int result = 0)
{              
	uintptr_t moduleAdress = *(uint64_t*)(__readgsqword(0x60) + 0x10);
	static auto patternToByte = [](const char* pattern)
		{
			auto bytes = std::vector<int>{};
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);
			for (auto current = start; current < end; ++current)
			{
				if (*current == '?')
				{
					++current;
					if (*current == '?')
						++current;
					bytes.push_back(-1);
				}
				else
				{
					bytes.push_back(strtoul(current, &current, 16));
				}
			}
			return bytes;
		};

	const auto dosHeader = (PIMAGE_DOS_HEADER)moduleAdress;
	const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)moduleAdress + dosHeader->e_lfanew);
	const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = patternToByte(pattern.c_str());
	const auto scanBytes = reinterpret_cast<std::uint8_t*>(moduleAdress);
	const auto s = patternBytes.size();
	const auto d = patternBytes.data();
	int CurrentRes = 0;

	for (auto i = 0ul; i < sizeOfImage - s; ++i)
	{
		bool found = true;
		for (auto j = 0ul; j < s; ++j)
		{
			if (scanBytes[i + j] != d[j] && d[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
		{
			if (!result)
				return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			if (result)
			{
				if (result > CurrentRes)
				{
					CurrentRes++;
					found = false;
				}
				else
					return reinterpret_cast<uintptr_t>(&scanBytes[i]);
			}
		}
	}
	return NULL;
}

//void erase_pe(void* pbase) {
//	auto base = reinterpret_cast<uintptr_t>(pbase);
//	auto dos_headers = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
//	auto pINH = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dos_headers->e_lfanew);
//	auto p_section_header = (IMAGE_SECTION_HEADER*)(pINH + 1);
//	auto dir = pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
//	auto iat = (IMAGE_IMPORT_DESCRIPTOR*)(base + dir.VirtualAddress);
//
//	// Clear import names
//	for (; iat->Name; ++iat) {
//		auto mod_name = reinterpret_cast<char*>(base + static_cast<uintptr_t>(iat->Name));
//		auto entry = reinterpret_cast<IMAGE_THUNK_DATA64*>(base + iat->OriginalFirstThunk);
//
//		for (uintptr_t index = 0; entry->u1.AddressOfData; index += sizeof(uintptr_t), ++entry) {
//			auto pimport = reinterpret_cast<IMAGE_IMPORT_BY_NAME*>(base + entry->u1.AddressOfData);
//			auto import_name = pimport->Name;
//			auto len = windows.strlen(import_name);
//			windows.memset(import_name, 0, len);
//		}
//
//		auto len = windows.strlen(mod_name);
//		windows.memset(mod_name, 0, len);
//	}
//
//	// ...
//
//	// Clear sections
//	for (int i = 0; i < pINH->FileHeader.NumberOfSections; i++) {
//		auto section = p_section_header[i];
//		auto name = section.Name;
//		auto rva = section.VirtualAddress;
//		auto size = section.SizeOfRawData;
//
//		auto secbase = reinterpret_cast<uintptr_t>(base) + rva;
//		auto secend = secbase + size;
//
//		if (windows.strstr(reinterpret_cast<const char*>(name), _(".rsrc")) ||
//			windows.strstr(reinterpret_cast<const char*>(name), _(".reloc")) ||
//			windows.strstr(reinterpret_cast<const char*>(name), _(".pdata"))) {
//			std::memset(reinterpret_cast<void*>(secbase), 0, size);
//		}
//	}
//
//	// Clear headers
//	windows.memset(reinterpret_cast<void*>(base), 0, pINH->OptionalHeader.SizeOfHeaders);
//}

//#define _ZeroMemory(x, y) (windows.memset(x, 0, y));
//void erase_pe(void* pbase) {
//	auto base = (uintptr_t)pbase;
//	auto dos_headers = (IMAGE_DOS_HEADER*)(base);
//	auto pINH = (IMAGE_NT_HEADERS*)(base + dos_headers->e_lfanew);
//
//	auto p_section_header = (IMAGE_SECTION_HEADER*)(pINH + 1);
//
//	auto sizeOfImage = pINH->OptionalHeader.SizeOfImage;
//
//	DWORD op;
//
//	auto dir = pINH->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
//
//	auto iat = (IMAGE_IMPORT_DESCRIPTOR*)(base + dir.VirtualAddress);
//
//	for (; iat->Name; ++iat) {
//		auto mod_name = (char*)(base + (uintptr_t)iat->Name);
//		auto entry = (IMAGE_THUNK_DATA64*)(base + iat->OriginalFirstThunk);
//
//		for (uintptr_t index = 0; entry->u1.AddressOfData; index += sizeof(uintptr_t), ++entry) {
//			auto pimport = (IMAGE_IMPORT_BY_NAME*)(base + entry->u1.AddressOfData);
//			auto import_name = pimport->Name;
//			auto len = windows.strlen(import_name);
//			_ZeroMemory(import_name, len);
//		}
//
//		auto len = windows.strlen(mod_name);
//		_ZeroMemory(mod_name, len);
//	}
//
//
//	for (int i = 0; i < pINH->FileHeader.NumberOfSections; i++) {
//		auto section = p_section_header[i];
//		auto name = section.Name;
//		auto rva = section.VirtualAddress;
//		auto size = section.SizeOfRawData;
//
//		auto secbase = (uintptr_t)base + rva;
//		auto secend = secbase + size;
//
//		if (windows.strstr((const char*)name, _(".rsrc")) || windows.strstr((const char*)name, _(".reloc")) || windows.strstr((const char*)name, _(".pdata"))) {
//			_ZeroMemory((void*)secbase, size);
//		}
//	}
//
//	_ZeroMemory((void*)base, pINH->OptionalHeader.SizeOfHeaders);
//}

