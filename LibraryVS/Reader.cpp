#include "Reader.h"

Reader::Reader(IntType id, std::string const& fio, std::string const& passport):
    _id{ id }, _fio{ fio }, _passport{ passport } {}
	
Reader::IntType Reader::getId() const noexcept
{
	return _id;
}

std::string Reader::getFio() const
{
	return _fio;
}

std::string Reader::getPassport() const
{
    return _passport;
}

std::fstream& operator>>(std::fstream& stream, Reader& reader)
{
    stream >> reader._id >> reader._fio >> reader._passport;
    return stream;
}

std::fstream& operator<<(std::fstream& stream, Reader const& reader)
{
    static_cast<std::ostream&>(stream) << reader._id << '\t' << reader._fio << '\t' << reader._passport;
    return stream;
}

bool compareIDs(Reader const& lh, Reader const& rh) noexcept
{
    return lh._id < rh._id;
}

bool comparePassports(Reader const& lh, Reader const& rh) noexcept
{
    return lh._passport < rh._passport;
}
