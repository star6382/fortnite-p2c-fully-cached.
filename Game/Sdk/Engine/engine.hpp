#include <Windows.h>
#include <cstdint>

//typedef unsigned long long uintptr_t; // 8 bytes
//typedef unsigned long long uint64_t;// 8 bytes
//typedef unsigned int uint32_t; // 4 bytes
//typedef int int32_t;// 4 bytes

uint64_t game_base;
uintptr_t findobject_address = 0x17A803C;
uintptr_t process_event_address = 0x4D;

bool is_bad_write_ptr(LPVOID ptr, UINT_PTR size)
{
	return ptr ? false : true;
}

bool valid_pointer(uintptr_t address)
{
	if (!is_bad_write_ptr((LPVOID)address, (UINT_PTR)8)) return TRUE;
	else return FALSE;
}

template<typename ReadT>
ReadT read(DWORD_PTR address, const ReadT& def = ReadT())
{
	if (valid_pointer(address)) {
		return *(ReadT*)(address);
	}
}

template <class type>
struct tarray {
public:
	tarray() { this->data = nullptr; this->count = this->max = 0; };

	type* data;
	int32_t count, max;

	type& operator[](int i) {
		return this->data[i];
	};

	int size() {
		return this->count;
	}

	bool valid(int i) {
		return bool(i < this->count);
	}
};

struct fstring : private tarray<wchar_t> {
	fstring() { };
	fstring(const wchar_t* other) {
		this->max = this->count = *other ? static_cast<int>(wcslen(other)) + 1 : 0;

		if (this->count)
			this->data = const_cast<wchar_t*>(other);
	};

	wchar_t* c_str() const {
		return this->data;
	}

	bool valid() const {
		return this->data != nullptr;
	}
};

struct fmatrix {
	double m[4][4];
};

struct fvector {
	fvector() : x(), y(), z() { }
	fvector(double _x, double _y, double _z) : x(_x), y(_y), z(_z) { }

	double x, y, z;

	__forceinline bool null_check() const {
		return (!x && !y && !z);
	}

	operator bool() { return bool(this->x && this->y && this->z); }
	bool operator==(fvector in) { return bool(this->x == in.x && this->y == in.y && this->z == in.z); }
	fvector operator+(fvector in) { return fvector(this->x + in.x, this->y + in.y, this->z + in.z); }
	fvector operator-(fvector in) { return fvector(this->x - in.x, this->y - in.y, this->z - in.z); }
};

struct fvector2d {
	fvector2d() : x(), y() { }
	fvector2d(double _x, double _y) : x(_x), y(_y) { }

	double x, y;

	operator bool() { return bool(this->x && this->y); }
	bool operator==(fvector2d in) { return bool(this->x == in.x && this->y == in.y); }
	fvector2d operator+(fvector2d in) { return fvector2d(this->x + in.x, this->y + in.y); }
	fvector2d operator-(fvector2d in) { return fvector2d(this->x - in.x, this->y - in.y); }
};

struct frotator {
	frotator() : pitch(), yaw(), roll() { }
	frotator(double _pitch, double _yaw, double _roll) : pitch(_pitch), yaw(_yaw), roll(_roll) { }

	double pitch, yaw, roll;

	operator bool() { return bool(this->pitch && this->yaw); }
	bool operator==(frotator in) { return bool(this->pitch == in.pitch && this->yaw == in.yaw && this->roll == in.roll); }
	frotator operator+(frotator in) { return frotator(this->pitch + in.pitch, this->yaw + in.yaw, this->roll + in.roll); }
	frotator operator-(frotator in) { return frotator(this->pitch - in.pitch, this->yaw - in.yaw, this->roll - in.roll); }
};

struct flinearcolor {
	flinearcolor() : r(), g(), b(), a() { }
	flinearcolor(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) { }

	float r, g, b, a;

	operator bool() { return bool(this->r || this->g || this->b || this->a); }
	bool operator==(flinearcolor in) { return bool(this->r == in.r && this->g == in.g && this->b == in.b && this->a == in.a); }
};

struct fminimalviewinfo {
	fvector location;
	frotator rotation;
	float fov;
};

struct fname {
	fname() : index() { }
	fname(uint32_t _index) : index(_index) { }

	uint32_t index;
};

struct fkey {
	fname name;
	std::uint8_t details[20];
};

class uobject {
public:

	static uobject* find_object(const wchar_t* name, uobject* outer = nullptr) {
		return reinterpret_cast<uobject*>(uobject::static_find_objecttwo(nullptr, outer, name));
	}

	static uobject* static_find_objecttwo(uobject* klass, uobject* outer, const wchar_t* name) {
		return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*)>(game_base + findobject_address)(klass, outer, name);
	}

	static uobject* static_find_object(uobject* klass, uobject* outer, const wchar_t* name, bool exact) {

		return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*, bool)>(game_base + findobject_address)(klass, outer, name, exact);
	}

	//static uobject* static_find_object(uobject* klass, uobject* outer, const wchar_t* name, bool exact) {
	//	return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*, bool)>(game_base + findobject_address)(klass, outer, name, exact);
	//}

	void process_event(uobject* function, void* args) {
		auto vtable = *reinterpret_cast<void***>(this);
		reinterpret_cast<void(*)(void*, void*, void*)>(vtable[process_event_address])(this, function, args);
	}

};


void(*processevent_original)(uobject*, uobject*, void*);
void process_event(uobject* ClassObject, uobject* UFunction, void* Params) {





	process_event(ClassObject, UFunction, Params);
}