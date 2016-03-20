#include <laft/text/Format.hpp>

#include <cmath>

namespace laft {
	namespace text {
		CharFormat::CharFormat(char value) :
			value_{value, '\0'}
		{
		}
		CharFormat::const_iterator CharFormat::begin() const
		{
			return value_;
		}
		CharFormat::const_iterator CharFormat::end() const
		{
			return value_ + 1;
		}
		DecimalSignedFormat::DecimalSignedFormat(int64_t value) :
			begin_(representation_ + sizeof(representation_) - 1)
		{
			//static_assert(sizeof(representation_) < std::ceil(std::log10(std::exp2(64))) + 1 /*\0*/, "SignedFormat array representation is not big enough.");
			*begin_ = '\0';
			bool neg = value < 0;
			uint64_t absValue = std::abs(value); // TODO: Do it use undefined behavior?
			
			while(absValue != 0)
			{
				--begin_;
				uint64_t digit = absValue % 10;
				absValue /= 10;
				*begin_ = static_cast<char>('0' + digit);
			}
			
			if(neg)
			{
				--begin_;
				*begin_ = '-';
			}
		}
		DecimalSignedFormat::const_iterator DecimalSignedFormat::begin() const
		{
			return begin_;
		}
		DecimalSignedFormat::const_iterator DecimalSignedFormat::end() const
		{
			return representation_ + sizeof(representation_) - 1;
		}
		DecimalUnsignedFormat::DecimalUnsignedFormat(uint64_t value) :
			begin_(representation_ + sizeof(representation_) - 1)
		{
			//static_assert(sizeof(representation_) < std::ceil(std::log10(std::exp2(64))) + 1 /*\0*/, "UnsignedFormat array representation is not big enough.");
			*begin_ = '\0';
			
			while(value != 0)
			{
				--begin_;
				uint64_t digit = value % 10;
				value /= 10;
				*begin_ = static_cast<char>('0' + digit);
			}
		}
		DecimalUnsignedFormat::const_iterator DecimalUnsignedFormat::begin() const
		{
			return begin_;
		}
		DecimalUnsignedFormat::const_iterator DecimalUnsignedFormat::end() const
		{
			return representation_ + sizeof(representation_) - 1;
		}
		CStringFormat::CStringFormat(const char* value) :
			value_(value),
			length_(strlen(value_))
		{
		}
		CStringFormat::const_iterator CStringFormat::begin() const
		{
			return value_;
		}
		CStringFormat::const_iterator CStringFormat::end() const
		{
			return value_ + length_;
		}
		StringFormat::StringFormat(const std::string& value) :
			value_(value)
		{
		}
		StringFormat::const_iterator StringFormat::begin() const
		{
			return value_.begin();
		}
		StringFormat::const_iterator StringFormat::end() const
		{
			return value_.end();
		}
		CharFormat as_text(bool value)
		{
			return CharFormat(value ? '1' : '0');
		}
		CharFormat as_text(char value)
		{
			return CharFormat(value);
		}
		DecimalSignedFormat as_text(int16_t value)
		{
			return DecimalSignedFormat(value);
		}
		DecimalSignedFormat as_text(int32_t value)
		{
			return DecimalSignedFormat(value);
		}
		DecimalSignedFormat as_text(int64_t value)
		{
			return DecimalSignedFormat(value);
		}
		DecimalUnsignedFormat as_text(uint16_t value)
		{
			return DecimalUnsignedFormat(value);
		}
		DecimalUnsignedFormat as_text(uint32_t value)
		{
			return DecimalUnsignedFormat(value);
		}
		DecimalUnsignedFormat as_text(uint64_t value)
		{
			return DecimalUnsignedFormat(value);
		}
		CStringFormat as_text(const char* value)
		{
			return CStringFormat(value);
		}
		StringFormat as_text(const std::string& value)
		{
			return StringFormat(value);
		}
	}
}
