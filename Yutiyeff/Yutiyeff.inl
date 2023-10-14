//////////////////////////////////////////////////////////////////////////////
//
// Yutiyeff (https://github.com/Hapaxia/Yutiyeff)
//
// Copyright(c) 2023 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Yutiyeff.hpp"

#include <iostream>

namespace yutiyeff
{

template <class T, class charT>
inline std::basic_string<char> String<T, charT>::priv_utf8FromUtf32(const std::basic_string<char32_t>& utf32String)
{
	std::basic_string<char> result{};
	const std::size_t sequenceLength{ utf32String.size() };
	result.reserve(sequenceLength); // UTF-8 will almost definitely be at least at long as the UTF-32 (unless invalid values are discarded)
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		const char32_t current{ utf32String[i] };

		if (current <= 0x7F)
			result.push_back(current & 0x7F);
		else if (current <= 0x7FF)
		{
			result.push_back(static_cast<char>(0xc0 | ((current >> 6) & 0x1F)));
			result.push_back(static_cast<char>(0x80 | (current & 0x3F)));
		}
		else if (current <= 0xFFFF)
		{
			result.push_back(static_cast<char>(0xe0 | ((current >> 12) & 0x0F)));
			result.push_back(static_cast<char>(0x80 | ((current >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (current & 0x3F)));
		}
		else if (current <= 0x10FFF)
		{
			result.push_back(static_cast<char>(0xF0 | ((current >> 18) & 0x07)));
			result.push_back(static_cast<char>(0x80 | ((current >> 12) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | ((current >> 6) & 0x3F)));
			result.push_back(static_cast<char>(0x80 | (current & 0x3F)));
		}
	}
	return result;
}

template <class T, class charT>
inline std::basic_string<char16_t> String<T, charT>::priv_utf16FromUtf32(const std::basic_string<char32_t>& utf32String)
{
	std::basic_string<char16_t> result{};
	const std::size_t sequenceLength{ utf32String.size() };
	result.reserve(sequenceLength); // UTF-16 will almost definitely be at least at long as the UTF-32 (unless invalid values are discarded)
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		char32_t current{ utf32String[i] };

		if ((current > 0x10FFFF) || ((current >= 0xD800) && (current <= 0xDFFF))) // invalid ranges
			continue;

		if (current <= 0xFFFF) // single 16-bit value
		{
			result.push_back(static_cast<char16_t>(current));
			continue;
		}

		current -= 0x10000; // now in range of 0x00000-0xFFFFF (this amount can be subtracted because 0x0000 - 0xFFFF would be encoded directly, not this way)
		result.push_back(static_cast<char16_t>(((current >> 10) & 0x3FF) + 0xD800));
		result.push_back(static_cast<char16_t>((current & 0x3FF) + 0xDC00));
	}
	return result;
}

template <class T, class charT>
inline std::basic_string<char32_t> String<T, charT>::priv_utf32FromUtf8(const std::basic_string<char>& utf8String)
{
	std::basic_string<char32_t> result{};
	const std::size_t sequenceLength{ utf8String.size() };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		const char32_t byte1{ static_cast<char32_t>(static_cast<unsigned char>(utf8String[i])) };
		if (!(byte1 & 0x80))
		{
			result.push_back(byte1 & 0x7F);
			continue;
		}

		if (++i >= sequenceLength)
			break;
		const char32_t byte2{ static_cast<char32_t>(utf8String[i]) };
		if ((byte1 & 0xE0) == 0xD0)
		{
			result.push_back(((byte1 & 0x1F) << 6) | (byte2 & 0x3F));
			continue;
		}

		if (++i >= sequenceLength)
			break;
		const char32_t byte3{ static_cast<char32_t>(utf8String[i]) };
		if ((byte1 & 0xF0) == 0xE0)
		{
			result.push_back(((byte1 & 0x0F) << 12) | ((byte2 & 0x3F) << 6) | (byte3 & 0x3F));
			continue;
		}

		if (++i >= sequenceLength)
			break;
		const char32_t byte4{ static_cast<char32_t>(utf8String[i]) };
		if ((byte1 & 0xF8) == 0xF0)
			result.push_back(((byte1 & 0x07) << 18) | ((byte2 & 0x3F) << 12) | ((byte3 & 0x3F) << 6) | (byte4 & 0x3F));
	}
	return result;
}

template <class T, class charT>
inline std::basic_string<char32_t> String<T, charT>::priv_utf32FromUtf16(const std::basic_string<char16_t>& utf16String)
{
	std::basic_string<char32_t> result{};
	const std::size_t sequenceLength{ utf16String.size() };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		char32_t unit1{ static_cast<char32_t>(utf16String[i]) };
		if ((unit1 <= 0xD7FF) || ((unit1 >= 0xE000) && (unit1 <= 0xFFFF))) 
		{
			result.push_back(unit1);
			continue;
		}

		if (++i >= sequenceLength)
			break;

		char32_t unit2{ static_cast<char32_t>(utf16String[i]) };
		if (unit2 < 0xDC00)
			continue;

		unit1 -= 0xD800;
		unit2 -= 0xDC00;
		result.push_back(((unit1 & 0x3FF) << 10) | (unit2 & 0x3FF));
	}
	return result;
}






inline Utf8String::Utf8String()
{
}

inline Utf8String::Utf8String(const char* cU8String)
{
	*this = std::basic_string<char>(cU8String);
}

inline Utf8String::Utf8String(const char16_t* cU16String)
{
	*this = std::basic_string<char16_t>(cU16String);
}

inline Utf8String::Utf8String(const char32_t* cU32String)
{
	*this = std::basic_string<char32_t>(cU32String);
}

inline Utf8String::Utf8String(const std::basic_string<char>& u8String)
{
	m_sequence = u8String;
}

inline Utf8String::Utf8String(const std::basic_string<char16_t>& u16String)
{
	*this = priv_utf8FromUtf32(priv_utf32FromUtf16(u16String));
}

inline Utf8String::Utf8String(const std::basic_string<char32_t>& u32String)
{
	*this = priv_utf8FromUtf32(u32String);
}

inline Utf8String::Utf8String(const Utf8String& utf8String)
{
	*this = utf8String;
}

inline Utf8String::Utf8String(const Utf16String& utf16String)
{
	m_sequence = priv_utf8FromUtf32(priv_utf32FromUtf16(utf16String.getSequence()));
}

inline Utf8String::Utf8String(const Utf32String& utf32String)
{
	m_sequence = priv_utf8FromUtf32(utf32String.getSequence());
}

inline Utf8String& Utf8String::operator=(const Utf8String& utf8String)
{
	m_sequence = utf8String.m_sequence;
	return *this;
}

inline Utf8String& Utf8String::operator+(const Utf8String& utf8String)
{
	m_sequence += utf8String.m_sequence;
	return *this;
}

inline Utf8String& Utf8String::operator+=(const Utf8String& utf8String)
{
	return *this = (*this + utf8String);
}

inline Utf8String Utf8String::getSubstring(const std::size_t length, const std::size_t offset) const
{
	const std::size_t sequenceLength{ m_sequence.size() };
	for (std::size_t pos{ 0u }, start{ 0u }, end{ 0u }; end < sequenceLength; ++end)
	{
		if (pos == offset)
			start = end;
		if ((static_cast<unsigned char>(m_sequence[end]) & 0xc0) != 0x80)
			++pos;
		if (pos > (length + offset))
			return m_sequence.substr(start, end - start);
		if (end + 1u == sequenceLength)
		{
			if (offset != 0u)
				return m_sequence.substr(start, end + 1u - start);
		}
	}
	return m_sequence;
}

inline void Utf8String::insert(const Utf8String& utf8String, const std::size_t offset)
{
	m_sequence = priv_utf8FromUtf32(priv_utf32FromUtf8(m_sequence).insert(offset, priv_utf32FromUtf8(utf8String.m_sequence)));
}

inline std::size_t Utf8String::find(const Utf8String& utf8String, const std::size_t offset) const
{
	return Utf32String(*this).find(Utf32String(utf8String), offset);
}

inline Utf8String::operator std::string() const
{
	return getString();
}

inline std::string Utf8String::getString() const
{
	return getSequence();
}

inline std::string Utf8String::getNonUnicodeString() const
{
	std::string s{};
	const std::size_t sequenceLength{ m_sequence.size() };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		const unsigned char element{ static_cast<unsigned char>(m_sequence[i]) };
		if ((element & 0x80) == 0x00)
			s.push_back(static_cast<char>(element));
	}
	return s;
}

inline std::size_t Utf8String::getLength() const
{
	const std::size_t sequenceLength{ m_sequence.size() };
	std::size_t count{ 0u };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		if ((static_cast<unsigned char>(m_sequence[i]) & 0xc0) != 0x80)
			++count;
	}
	return count;
}

inline char32_t Utf8String::operator[](const std::size_t index) const
{
	return Utf32String(*this)[index];
}

inline void Utf8String::erase(const std::size_t startPos, std::size_t length)
{
	const std::size_t sequenceLength{ m_sequence.size() };
	if (length > sequenceLength)
		length = sequenceLength;
	m_sequence.erase(startPos, length);
}

inline void Utf8String::clear()
{
	m_sequence.clear();
}

inline std::basic_string<char> Utf8String::getSequence() const
{
	return m_sequence;
}











inline Utf16String::Utf16String()
{
}

inline Utf16String::Utf16String(const char* cU8String)
{
	*this = std::basic_string<char>(cU8String);
}

inline Utf16String::Utf16String(const char16_t* cU16String)
{
	*this = std::basic_string<char16_t>(cU16String);
}

inline Utf16String::Utf16String(const char32_t* cU32String)
{
	*this = std::basic_string<char32_t>(cU32String);
}

inline Utf16String::Utf16String(const std::basic_string<char>& u8String)
{
	*this = priv_utf16FromUtf32(priv_utf32FromUtf8(u8String));
}

inline Utf16String::Utf16String(const std::basic_string<char16_t>& u16String)
{
	m_sequence = u16String;
}

inline Utf16String::Utf16String(const std::basic_string<char32_t>& u32String)
{
	*this = priv_utf16FromUtf32(u32String);
}

inline Utf16String::Utf16String(const Utf8String& utf8String)
{
	*this = priv_utf16FromUtf32(priv_utf32FromUtf8(utf8String));
}

inline Utf16String::Utf16String(const Utf16String& utf16String)
{
	*this = utf16String;
}

inline Utf16String::Utf16String(const Utf32String& utf32String)
{
	*this = priv_utf16FromUtf32(utf32String.getSequence());
}

inline Utf16String& Utf16String::operator=(const Utf16String& utf16String)
{
	m_sequence = utf16String.m_sequence;
	return *this;
}

inline Utf16String& Utf16String::operator+(const Utf16String& utf16String)
{
	m_sequence += utf16String.m_sequence;
	return *this;
}

inline Utf16String& Utf16String::operator+=(const Utf16String& utf16String)
{
	return *this = (*this + utf16String);
}

inline Utf16String Utf16String::getSubstring(const std::size_t length, const std::size_t offset) const
{
	const std::size_t sequenceLength{ m_sequence.size() };
	for (std::size_t pos{ 0u }, start{ 0u }, end{ 0u }; end < sequenceLength; ++end)
	{
		if (pos == offset)
			start = end;
		const char16_t element{ m_sequence[end] };
		if ((element <= 0xDBFF) || (element >= 0xE000))
			++pos;
		if (pos > (length + offset))
			return m_sequence.substr(start, end - start);
		if (end + 1u == sequenceLength)
		{
			if (offset != 0u)
				return m_sequence.substr(start, end + 1u - start);
		}
	}
	return m_sequence;
}

inline void Utf16String::insert(const Utf16String& utf16String, const std::size_t offset)
{
	m_sequence = priv_utf16FromUtf32(priv_utf32FromUtf16(m_sequence).insert(offset, priv_utf32FromUtf16(utf16String.m_sequence)));
}

inline std::size_t Utf16String::find(const Utf16String& utf16String, const std::size_t offset) const
{
	return Utf32String(*this).find(Utf32String(utf16String), offset);
}

inline Utf16String::operator std::string() const
{
	return getString();
}

inline std::string Utf16String::getString() const
{
	return priv_utf8FromUtf32(priv_utf32FromUtf16(m_sequence));
}

inline std::string Utf16String::getNonUnicodeString() const
{
	std::string s{};
	const std::size_t sequenceLength{ m_sequence.size() };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		const char16_t element{ m_sequence[i] };
		if (element <= 0x7F)
			s.push_back(static_cast<char>(static_cast<unsigned char>(element)));
	}
	return s;
}

inline std::size_t Utf16String::getLength() const
{
	const std::size_t sequenceLength{ m_sequence.size() };
	std::size_t count{ 0u };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		const char16_t element{ m_sequence[i] };
		if ((element <= 0xDBFF) || (element >= 0xE000))
			++count;
	}
	return count;
}

inline char32_t Utf16String::operator[](const std::size_t index) const
{
	return Utf32String(*this)[index];
}

inline void Utf16String::erase(const std::size_t startPos, std::size_t length)
{
	if (length > m_sequence.size())
		length = m_sequence.size();
	m_sequence.erase(startPos, length);
}

inline void Utf16String::clear()
{
	m_sequence.clear();
}

inline std::basic_string<char16_t> Utf16String::getSequence() const
{
	return m_sequence;
}









inline Utf32String::Utf32String()
{
}

inline Utf32String::Utf32String(const char* cU8String)
{
	*this = std::basic_string<char>(cU8String);
}

inline Utf32String::Utf32String(const char16_t* cU16String)
{
	*this = std::basic_string<char16_t>(cU16String);
}

inline Utf32String::Utf32String(const char32_t* cU32String)
{
	*this = std::basic_string<char32_t>(cU32String);
}

inline Utf32String::Utf32String(const std::basic_string<char>& u8String)
{
	*this = priv_utf32FromUtf8(u8String);
}

inline Utf32String::Utf32String(const std::basic_string<char16_t>& u16String)
{
	*this = priv_utf32FromUtf16(u16String);
}

inline Utf32String::Utf32String(const std::basic_string<char32_t>& u32String)
{
	m_sequence = u32String;
}

inline Utf32String::Utf32String(const Utf8String& utf8String)
{
	*this = priv_utf32FromUtf8(utf8String.getSequence());
}

inline Utf32String::Utf32String(const Utf16String& utf16String)
{
	*this = priv_utf32FromUtf16(utf16String.getSequence());
}

inline Utf32String::Utf32String(const Utf32String& utf32String)
{
	*this = utf32String;
}

inline Utf32String& Utf32String::operator=(const Utf32String& utf32String)
{
	m_sequence = utf32String.m_sequence;
	return *this;
}

inline Utf32String& Utf32String::operator+(const Utf32String& utf32String)
{
	m_sequence += utf32String.m_sequence;
	return *this;
}

inline Utf32String& Utf32String::operator+=(const Utf32String& utf32String)
{
	return *this = (*this + utf32String);
}

inline Utf32String Utf32String::getSubstring(const std::size_t length, const std::size_t offset) const
{
	return Utf32String(m_sequence.substr(offset, length));
}

inline void Utf32String::insert(const Utf32String& utf32String, const std::size_t offset)
{
	m_sequence.insert(offset, utf32String.m_sequence);
}

inline std::size_t Utf32String::find(const Utf32String& utf32String, const std::size_t offset) const
{
	return m_sequence.find(utf32String.m_sequence, offset);
}

inline Utf32String::operator std::string() const
{
	return getString();
}

inline std::string Utf32String::getString() const
{
	return priv_utf8FromUtf32(m_sequence);
}

inline std::string Utf32String::getNonUnicodeString() const
{
	std::string s{};
	const std::size_t sequenceLength{ m_sequence.size() };
	for (std::size_t i{ 0u }; i < sequenceLength; ++i)
	{
		const char32_t element{ m_sequence[i] };
		if (element <= 0x7F)
			s.push_back(static_cast<char>(static_cast<unsigned char>(element)));
	}
	return s;
}

inline std::size_t Utf32String::getLength() const
{
	return m_sequence.size();
}

inline char32_t Utf32String::operator[](const std::size_t index) const
{
	return m_sequence[index];
}

inline void Utf32String::erase(const std::size_t startPos, std::size_t length)
{
	if (length > m_sequence.size())
		length = m_sequence.size();
	m_sequence.erase(startPos, length);
}

inline void Utf32String::clear()
{
	m_sequence.clear();
}

inline std::u32string Utf32String::getSequence() const
{
	return m_sequence;
}

} // namespace yutiyeff
