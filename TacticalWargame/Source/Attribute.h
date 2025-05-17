#pragma once
namespace aie {
	class Attribute
	{
	private:
		const float max;
		float current;
		void Clamp() {
			if (current > max)
				current = max;
			if (current < 0)
				current = 0;
		}
	public:
		Attribute(float v) :max{ v }, current{ v } {}
		
		template<typename T>
		Attribute operator+(T rhs) {
			Attribute na = *this;
			na += rhs;
			return na;
		}

		template<typename T>
		Attribute operator-(T rhs) {
			Attribute na = *this;
			na -= rhs;
			return na;
		}

		template<typename T>
		Attribute& operator+=(T rhs) {
			current += rhs;
			Clamp();
			return *this;
		}

		template<typename T>
		Attribute& operator-=(T rhs) {
			current -= rhs;
			Clamp();
			return *this;
		}

		template<typename T>
		Attribute& operator=(T rhs) {
			current = rhs;
			Clamp();
			return *this;
		}

		template<typename T>
		bool operator<(T rhs) { return current < rhs; }
		
		template<typename T>
		bool operator>(T rhs) { return current > rhs; }
		
		template<typename T>
		bool operator==(T rhs) { return current == rhs; }
		
		Attribute& Restore() { current = max; }
		float Fraction() const { return current / max; }
		operator float() const { return current; }
	};
}