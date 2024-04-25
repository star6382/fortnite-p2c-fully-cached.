#include <Windows.h>
#include <cstdint>
#include <corecrt_math.h>

uint64_t game_address;
uintptr_t staticfindobject_address = 0x14557B0;
uintptr_t vft_processevent_address = 0x4D;

//
//template <class type>
//struct tarray {
//public:
//	tarray() { this->data = nullptr; this->count = this->max = 0; };
//
//	type* data;
//	int32_t count, max;
//
//	type& operator[](int i) {
//		return this->data[i];
//	};
//
//	int size() {
//		return this->count;
//	}
//
//	bool valid(int i) {
//		return bool(i < this->count);
//	}
//};

template<class T>
class tarray
{
protected:
	T* Data;
	int32_t NumElements;
	int32_t MaxElements;

public:

	inline tarray()
		:NumElements(0), MaxElements(0), Data(nullptr)
	{
	}

	inline tarray(int32_t Size) : NumElements(0), MaxElements(Size), Data(reinterpret_cast<T*>(allocation.malloc(sizeof(T)* Size)))
	{

	}

	inline T& operator[](uint32_t Index)
	{
		return Data[Index];
	}
	inline const T& operator[](uint32_t Index) const
	{
		return Data[Index];
	}

	inline int32_t Num()
	{
		return NumElements;
	}

	inline int32_t Max()
	{
		return MaxElements;
	}

	inline int32_t GetSlack()
	{
		return MaxElements - NumElements;
	}

	inline bool IsValid()
	{
		return Data != nullptr;
	}

	inline bool IsValidIndex(int32_t Index)
	{
		return Index >= 0 && Index < NumElements;
	}

	inline void ResetNum()
	{
		NumElements = 0;
	}



	// CUSTOM FUNCTIONS

	inline T GetByIndex(int32_t Index)
	{
		if (IsValidIndex(Index))
		{
			return Data[Index];
		}

		return T();
	}

	inline void Add(const T& Element)
	{
		if (NumElements < MaxElements)
		{
			Data[NumElements++] = Element;
		}
	}

	inline void Add(T&& Element)
	{
		if (NumElements < MaxElements)
		{
			Data[NumElements++] = std::move(Element);
		}
	}

	inline void Add(T* Element)
	{
		if (NumElements < MaxElements)
		{
			Data[NumElements++] = *Element;
		}
	}
};

int _wcslen(const wchar_t* string) {
	int cnt = 0;
	if (string) {
		for (; *string != 0; ++string) ++cnt;
	}

	return cnt;
}

struct fstring : private tarray<wchar_t> {
    fstring() = default;
    fstring(const wchar_t* other) {
		NumElements = MaxElements = *other ? static_cast<int>(_wcslen(other)) + 1 : 0;
        if (NumElements) Data = const_cast<wchar_t*>(other);
    }

    wchar_t* c_str() const { return Data; }
    bool valid() const { return Data != nullptr; }
};

struct FVector
{
	FVector() : X(), Y(), Z() { }
	FVector(double X, double Y, double Z) : X(X), Y(Y), Z(Z) { }

	FVector operator + (const FVector& other) const { return { this->X + other.X, this->Y + other.Y, this->Z + other.Z }; }
	FVector operator - (const FVector& other) const { return { this->X - other.X, this->Y - other.Y, this->Z - other.Z }; }
	FVector operator * (double offset) const { return { this->X * offset, this->Y * offset, this->Z * offset }; }
	FVector operator / (double offset) const { return { this->X / offset, this->Y / offset, this->Z / offset }; }

	FVector& operator *= (const double other) { this->X *= other; this->Y *= other; this->Z *= other; return *this; }
	FVector& operator /= (const double other) { this->X /= other; this->Y /= other; this->Z /= other; return *this; }

	FVector& operator = (const FVector& other) { this->X = other.X; this->Y = other.Y; this->Z = other.Z; return *this; }
	FVector& operator += (const FVector& other) { this->X += other.X; this->Y += other.Y; this->Z += other.Z; return *this; }
	FVector& operator -= (const FVector& other) { this->X -= other.X; this->Y -= other.Y; this->Z -= other.Z; return *this; }
	FVector& operator *= (const FVector& other) { this->X *= other.X; this->Y *= other.Y; this->Z *= other.Z; return *this; }
	FVector& operator /= (const FVector& other) { this->X /= other.X; this->Y /= other.Y; this->Z /= other.Z; return *this; }

	operator bool() { return bool(this->X || this->Y || this->Z); }
	friend bool operator == (const FVector& a, const FVector& b) { return a.X == b.X && a.Y == b.Y && a.Z == b.Z; }
	friend bool operator != (const FVector& a, const FVector& b) { return !(a == b); }

	double Dot(const FVector& V) { return X * V.X + Y * V.Y + Z * V.Z; }
	double SizeSquared() { return X * X + Y * Y + Z * Z; }

	inline float Distance(FVector v) {
		return float(sqrt(pow(v.X - X, 2.0) + pow(v.Y - Y, 2.0) + pow(v.Z - Z, 2.0)));
	}

	double X, Y, Z;
};

struct FVector2D
{
	FVector2D() : X(), Y() { }
	FVector2D(double X, double Y) : X(X), Y(Y) { }

	FVector2D operator + (const FVector2D& other) const { return { this->X + other.X, this->Y + other.Y }; }
	FVector2D operator - (const FVector2D& other) const { return { this->X - other.X, this->Y - other.Y }; }
	FVector2D operator * (double offset) const { return { this->X * offset, this->Y * offset }; }
	FVector2D operator / (double offset) const { return { this->X / offset, this->Y / offset }; }

	FVector2D& operator *= (const double other) { this->X *= other; this->Y *= other; return *this; }
	FVector2D& operator /= (const double other) { this->X /= other; this->Y /= other; return *this; }

	FVector2D& operator = (const FVector2D& other) { this->X = other.X; this->Y = other.Y; return *this; }
	FVector2D& operator += (const FVector2D& other) { this->X += other.X; this->Y += other.Y; return *this; }
	FVector2D& operator -= (const FVector2D& other) { this->X -= other.X; this->Y -= other.Y; return *this; }
	FVector2D& operator *= (const FVector2D& other) { this->X *= other.X; this->Y *= other.Y; return *this; }
	FVector2D& operator /= (const FVector2D& other) { this->X /= other.X; this->Y /= other.Y; return *this; }

	operator bool() { return bool(this->X || this->Y); }
	friend bool operator == (const FVector2D& A, const FVector2D& B) { return A.X == B.X && A.Y == A.Y; }
	friend bool operator != (const FVector2D& A, const FVector2D& B) { return !(A == B); }

	double X, Y;
};

struct FRotator
{
	FRotator() : Pitch(), Yaw(), Roll() { }
	FRotator(double Pitch, double Yaw, double Roll) : Pitch(Pitch), Yaw(Yaw), Roll(Roll) { }

	FRotator operator + (const FRotator& other) const { return { this->Pitch + other.Pitch, this->Yaw + other.Yaw, this->Roll + other.Roll }; }
	FRotator operator - (const FRotator& other) const { return { this->Pitch - other.Pitch, this->Yaw - other.Yaw, this->Roll - other.Roll }; }
	FRotator operator * (double offset) const { return { this->Pitch * offset, this->Yaw * offset, this->Roll * offset }; }
	FRotator operator / (double offset) const { return { this->Pitch / offset, this->Yaw / offset, this->Roll / offset }; }

	FRotator& operator *= (const double other) { this->Pitch *= other; this->Yaw *= other; this->Roll *= other; return *this; }
	FRotator& operator /= (const double other) { this->Pitch /= other; this->Yaw /= other; this->Roll /= other; return *this; }

	FRotator& operator = (const FRotator& other) { this->Pitch = other.Pitch; this->Yaw = other.Yaw; this->Roll = other.Roll; return *this; }
	FRotator& operator += (const FRotator& other) { this->Pitch += other.Pitch; this->Yaw += other.Yaw; this->Roll += other.Roll; return *this; }
	FRotator& operator -= (const FRotator& other) { this->Pitch -= other.Pitch; this->Yaw -= other.Yaw; this->Roll -= other.Roll; return *this; }
	FRotator& operator *= (const FRotator& other) { this->Pitch *= other.Pitch; this->Yaw *= other.Yaw; this->Roll *= other.Roll; return *this; }
	FRotator& operator /= (const FRotator& other) { this->Pitch /= other.Pitch; this->Yaw /= other.Yaw; this->Roll /= other.Roll; return *this; }

	operator bool() { return bool(this->Pitch || this->Yaw || this->Roll); }
	friend bool operator == (const FRotator& a, const FRotator& b) { return a.Pitch == b.Pitch && a.Yaw == b.Yaw && a.Roll == b.Roll; }
	friend bool operator != (const FRotator& a, const FRotator& b) { return !(a == b); }

	FVector Euler() const
	{
		return FVector(Pitch, Yaw, Roll);
	}

	void NormalizeAngles()
	{
		Pitch = NormalizeAngle(Pitch);
		Yaw = NormalizeAngle(Yaw);
		Roll = NormalizeAngle(Roll);
	}

	double NormalizeAngle(double Angle)
	{
		while (Angle > 180)
			Angle -= 360;
		while (Angle < -180)
			Angle += 360;
		return Angle;
	}

	double Pitch, Yaw, Roll;
};

struct flinearcolor {
    float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

    flinearcolor() = default;
    flinearcolor(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}

    explicit operator bool() const { return r || g || b || a; }
    bool operator==(const flinearcolor& in) const { return r == in.r && g == in.g && b == in.b && a == in.a; }
};

struct fminimalviewinfo {
	FVector location;
	FRotator rotation;
	float fov;
};
class fname
{
public:
	fname()
		: ComparisonIndex(0)
#if SEASON_20_PLUS == false
		, Number(0)
#endif
	{
	}

	fname(const wchar_t* Name)
		: ComparisonIndex(0)
#if SEASON_20_PLUS == false
		, Number(0)
#endif
	{
		/*if (this == nullptr || SDK::Cached::Functions::FNameConstructor == 0x0) return;

		static void(*FNameConstructor)(FName*, const wchar_t*, bool) = nullptr;

		if (!FNameConstructor)
			FNameConstructor = reinterpret_cast<void(*)(FName*, const wchar_t*, bool)>(SDK::GetBaseAddress() + SDK::Cached::Functions::FNameConstructor);

		FNameConstructor(this, Name, true);*/
	}
public:
	// Members of FName - depending on configuration [WITH_CASE_PRESERVING_NAME | FNAME_OUTLINE_NUMBER]
	int32_t ComparisonIndex;
#if SEASON_20_PLUS == false
	int32_t Number;
#endif


	// GetDisplayIndex - returns the Id of the string depending on the configuration [default: ComparisonIndex, WITH_CASE_PRESERVING_NAME: DisplayIndex]
	inline int32_t GetDisplayIndex() const
	{
		return ComparisonIndex;
	}

	//// GetRawString - returns an unedited string as the engine uses it
	//inline std::string GetRawString() const
	//{
	//	if (this == nullptr || SDK::Cached::Functions::AppendString == 0x0) return skCrypt("Failed!").decrypt();

	//	static void(*AppendString)(const FName*, FString*) = nullptr;

	//	if (!AppendString)
	//		AppendString = reinterpret_cast<void(*)(const FName*, FString*)>(SDK::GetBaseAddress() + SDK::Cached::Functions::AppendString);

	//	FString TempString;
	//	AppendString(const_cast<SDK::FName*>(this), &TempString);

	//	return TempString.ToString();
	//}

	//// ToString - returns an edited string as it's used by most SDKs ["/Script/CoreUObject" -> "CoreUObject"]
	//inline std::string ToString() const
	//{
	//	std::string OutputString = GetRawString();

	//	size_t pos = OutputString.rfind('/');

	//	if (pos == std::string::npos)
	//		return OutputString;

	//	return OutputString.substr(pos + 1);
	//}

	inline bool operator==(const fname& Other) const
	{
#if SEASON_20_PLUS
		return ComparisonIndex == Other.ComparisonIndex;
#else
		return ComparisonIndex == Other.ComparisonIndex && Number == Other.Number;
#endif
	}

	inline bool operator!=(const fname& Other) const
	{
#if SEASON_20_PLUS
		return ComparisonIndex != Other.ComparisonIndex;
#else
		return ComparisonIndex != Other.ComparisonIndex || Number != Other.Number;
#endif
	}
};

//struct fname {
//	fname() : index() { }
//	fname(uint32_t _index) : index(_index) { }
//
//	uint32_t index;
//};

struct fkey {
	fname name;
	std::uint8_t details[20];
};

class uobject {
public:
	// Simplified find_object function using default parameters
	static uobject* find_object(const wchar_t* name, uobject* outer = nullptr) {
		return static_find_object(nullptr, outer, name);
	}

	using FindObjectFunc = uobject * (*)(uobject*, uobject*, const wchar_t*);

	// Simplified static_find_object function with clearer function pointer usage
	static uobject* static_find_object(uobject* klass, uobject* outer, const wchar_t* name) {
		auto function = reinterpret_cast<FindObjectFunc>(game_address + staticfindobject_address);
		return function(klass, outer, name);
	}

	// Simplified process_event function with clearer vtable usage
	void process_event(uobject* function, void* args) {
		auto vtable = *reinterpret_cast<void***>(this);
		auto eventFunc = reinterpret_cast<void(*)(void*, void*, void*)>(vtable[vft_processevent_address]);
		eventFunc(this, function, args);
	}

	bool IsA(class UClass* Clss) const
	{
		if (Clss) {
			return true;
		}

		else if (!Clss) {
			return false;
		}

		return false;
	}
};

//class uobject {
//public:
//
//	static uobject* find_object(const wchar_t* name, uobject* outer = nullptr) {
//		return reinterpret_cast<uobject*>(uobject::static_find_object(nullptr, outer, name));
//	}
//
//	static uobject* static_find_object(uobject* klass, uobject* outer, const wchar_t* name) {
//		return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*)>(game_address + staticfindobject_address)(klass, outer, name);
//	}
//
//	//static uobject* static_find_object(uobject* klass, uobject* outer, const wchar_t* name, bool exact) {
//	//	return reinterpret_cast<uobject * (*)(uobject*, uobject*, const wchar_t*, bool)>(game_address + staticfindobject_address)(klass, outer, name, exact);
//	//}
//
//	void process_event(uobject* function, void* args) {
//		auto vtable = *reinterpret_cast<void***>(this);
//		reinterpret_cast<void(*)(void*, void*, void*)>(vtable[vft_processevent_address])(this, function, args);
//	}
//
//};