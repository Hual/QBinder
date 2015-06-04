#pragma once

typedef unsigned short bool_16;

class Bitwise
{
public:
	template<class T = bool_16>
	static inline bool Get(const T& variable, const unsigned char index)
	{
		return variable & (T)(1 << index);
	}

	template<class T = bool_16>
	static inline void True(T& variable, const unsigned char index)
	{
		variable |= 1 << index;
	}

	template<class T = bool_16>
	static inline void False(T& variable, const unsigned char index)
	{
		variable &= ~(1 << index);
	}

	template<class T = bool_16>
	static inline void Fill(T& variable)
	{
		variable = (T)~0;
	}

	template<class T = bool_16>
	static inline void Clear(T& variable)
	{
		variable = 0;
	}

};