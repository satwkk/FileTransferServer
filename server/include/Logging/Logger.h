#pragma once

#include <ostream>
#include <string>
#include <iostream>

class Logger
{
public:
	Logger(std::ostream& outputStream, const std::string& prefix) 
    : m_Prefix(prefix)
    , m_Stream(outputStream)
	{
	}

	std::ostream& operator<<(const std::string& msg)
	{
		m_Stream << "[" << m_Prefix << "]" << ": " << msg;
		return m_Stream;
	}

private:
	std::string m_Prefix;
	std::ostream& m_Stream;
};

static Logger eLog (std::cerr, "ERROR");
static Logger iLog (std::cout, "LOG");
static Logger wLog (std::cout, "WARN");
static Logger dLog (std::cout, "DEBUG");

#define nl std::endl;