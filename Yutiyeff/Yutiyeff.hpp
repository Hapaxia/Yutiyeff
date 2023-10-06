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

#include <string>

namespace yutiyeff
{

class String
{
public:
	virtual operator std::string() const = 0;
	virtual std::string getString() const = 0 ; // get a standard std::string representing the UTF-8 formatted version of the string
	virtual std::string getNonUnicodeString() const = 0; // get a standard std::string of only the codes in the 0-127 range
	virtual size_t getLength() const = 0;

	virtual char32_t operator[](std::size_t index) const = 0;
	virtual void erase(std::size_t startPos, std::size_t length = 1u) = 0;
	virtual void clear() = 0;

protected:
	static std::basic_string<char> priv_utf8FromUtf32(const std::basic_string<char32_t>& utf32String);
	static std::basic_string<char16_t> priv_utf16FromUtf32(const std::basic_string<char32_t>& utf32String);
	static std::basic_string<char32_t> priv_utf32FromUtf8(const std::basic_string<char>& utf8String);
	static std::basic_string<char32_t> priv_utf32FromUtf16(const std::basic_string<char16_t>& utf16String);
};

class Utf16String;
class Utf32String;

class Utf8String : public String
{
public:
	Utf8String();

	Utf8String(const char* cU8String);
	Utf8String(const char16_t* cU16String);
	Utf8String(const char32_t* cU32String);
	Utf8String(const std::basic_string<char>& u8String);
	Utf8String(const std::basic_string<char16_t>& u16String);
	Utf8String(const std::basic_string<char32_t>& u32String);
	Utf8String(const Utf8String& utf8String);
	Utf8String(const Utf16String& utf16String);
	Utf8String(const Utf32String& utf32String);

	Utf8String& operator=(const Utf8String& utf8String);
	Utf8String& operator+(const Utf8String& utf8String);
	Utf8String& operator+=(const Utf8String& utf8String);

	Utf8String getSubstring(std::size_t length, std::size_t offset = 0u) const;
	void insert(const Utf8String& utf8String, std::size_t offset = 0u);
	std::size_t find(const Utf8String& utf8String, std::size_t offset = 0u) const;

	virtual operator std::string() const override final;
	virtual std::string getString() const override final;
	virtual std::string getNonUnicodeString() const override final;
	virtual std::size_t getLength() const override final;

	virtual char32_t operator[](std::size_t index) const;
	virtual void erase(std::size_t startPos, std::size_t length = 1u);
	virtual void clear();

	std::basic_string<char> getSequence() const;

private:
	std::basic_string<char> m_sequence;
};

class Utf16String : public String
{
public:
	Utf16String();

	Utf16String(const char* cU8String);
	Utf16String(const char16_t* cU16String);
	Utf16String(const char32_t* cU32String);
	Utf16String(const std::basic_string<char>& u8String);
	Utf16String(const std::basic_string<char16_t>& u16String);
	Utf16String(const std::basic_string<char32_t>& u32String);
	Utf16String(const Utf8String& utf8String);
	Utf16String(const Utf16String& utf16String);
	Utf16String(const Utf32String& utf32String);

	Utf16String& operator=(const Utf16String& utf16String);
	Utf16String& operator+(const Utf16String& utf16String);
	Utf16String& operator+=(const Utf16String& utf16String);

	Utf16String getSubstring(std::size_t length, std::size_t offset = 0u) const;
	void insert(const Utf16String& utf16String, std::size_t offset = 0u);
	std::size_t find(const Utf16String& utf16String, std::size_t offset) const;

	virtual operator std::string() const override final;
	virtual std::string getString() const override final;
	virtual std::string getNonUnicodeString() const override final;
	virtual std::size_t getLength() const override final;

	virtual char32_t operator[](std::size_t index) const;
	virtual void erase(std::size_t startPos, std::size_t length = 1u);
	virtual void clear();

	std::basic_string<char16_t> getSequence() const;

private:
	std::basic_string<char16_t> m_sequence;
};

class Utf32String final : public String
{

public:
	Utf32String();
	Utf32String(const char* cU8String);
	Utf32String(const char16_t* cU16String);
	Utf32String(const char32_t* cU32String);
	Utf32String(const std::basic_string<char>& u8String);
	Utf32String(const std::basic_string<char16_t>& u16String);
	Utf32String(const std::basic_string<char32_t>& u32String);
	Utf32String(const Utf8String& utf8String);
	Utf32String(const Utf16String& utf16String);
	Utf32String(const Utf32String& utf32String);

	Utf32String& operator=(const Utf32String& utf32String);
	Utf32String& operator+(const Utf32String& utf32String);
	Utf32String& operator+=(const Utf32String& utf32String);

	Utf32String getSubstring(std::size_t length, std::size_t offset = 0u) const;
	void insert(const Utf32String& utf32String, std::size_t offset = 0u);
	std::size_t find(const Utf32String& utf32String, std::size_t offset) const;

	virtual operator std::string() const override final;
	virtual std::string getString() const override final;
	virtual std::string getNonUnicodeString() const override final;
	virtual std::size_t getLength() const override final;

	virtual char32_t operator[](std::size_t index) const;
	virtual void erase(std::size_t startPos, std::size_t length = 1u);
	virtual void clear();

	std::u32string getSequence() const;

private:
	std::u32string m_sequence{};
};

} // namespace yutiyeff

#ifndef YUTIYEFF_NO_NAMESPACE_SHORTCUT
namespace yy = yutiyeff; // create shortcut namespace
#endif // YUTIYEFF_NO_NAMESPACE_SHORTCUT

#include "Yutiyeff.inl"
