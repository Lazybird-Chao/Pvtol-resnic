// File: Exception.h
//
// Contents:
//      PVTOL Exception class.
//
// Revision History:
//      March 2000 - Written by Jim Daly
//      October 2007 - Sanjeev Mohindra
//		1. Inherit from std::exception
//		2.provide what();
/////////////////////////////////////////////////////////////////////////////

#ifndef PVTOL_EXCEPTION_H
#define PVTOL_EXCEPTION_H

//---------------------------------------------------------------
// Include files
//---------------------------------------------------------------
#include <PvtolBasics.h>
#include <iostream>
#include <string>
#include <stdexcept>
//---------------------------------------------------------------
// namespace
//---------------------------------------------------------------
using std::string;
using std::ostream;
using std::cerr;
using std::endl;


namespace ipvtol {

	//---------------------------------------------------------------
	// Exception
	//---------------------------------------------------------------
	
	class Exception : public std::exception {
	public:
		// CONSTRUCTORS

		Exception(const string& what, const string& file, const int line) :
		  m_what(what), m_file(file), m_line(line)
		  {
#ifdef PRINT_EXCEPTIONS
			  print(cerr);
			  cerr << endl;
#endif
		  };

		  Exception(const string& what) :
		  m_what(what), m_file("UnknownFile"), m_line(0)
		  {
#ifdef PRINT_EXCEPTIONS
			  print(cerr);
			  cerr << endl;
#endif
		  }

		  Exception(const string& file, const int line) :
		  m_what("Unspecified exception"), m_file(file), m_line(line)
		  {
#ifdef PRINT_EXCEPTIONS
			  print(cerr);
			  cerr << endl;
#endif
		  };

		  Exception() :
		  m_what("Unspecified exception"), m_file("UnknownFile"), m_line(0)
		  {
#ifdef PRINT_EXCEPTIONS
			  print(cerr);
			  cerr << endl;
#endif
		  }

		  virtual ~Exception() throw() {}

		  // ACCESSORS
		  void print(ostream& os) const {
			  os << "Exception:" << m_file << ":" << m_line << "   " << m_what;
		  }


		  virtual const char* what() const throw()
		  {
			  return m_what.c_str();
		  }


	private:
		const string m_what;
		const string m_file;
		const int    m_line;
	};


	//---------------------------------------------------------------
	// NotImplementedYet exception
	//---------------------------------------------------------------
	class NotImplementedYet : public Exception {
	public:
		NotImplementedYet(const string file, const int line)
			: Exception("NotImplementedYet",file,line) { }
	};


	//---------------------------------------------------------------
	// BadParameter exception
	//---------------------------------------------------------------
	class BadParameter : public Exception {
	public:
		BadParameter(const string file, const int line)
			: Exception("BadParameter",file,line) { }
	};


	//---------------------------------------------------------------
	// BadNumProcs exception
	//---------------------------------------------------------------
	class BadNumProcs : public Exception {
	public:
		BadNumProcs(const string file, const int line)
			: Exception("BadNumProcs",file,line) { }
	};


	//---------------------------------------------------------------
	// BadDataSize exception
	//---------------------------------------------------------------
	class BadDataSize : public Exception {
	public:
		BadDataSize(const string file, const int line)
			: Exception("BadDataSize",file,line) { }
	};

	//---------------------------------------------------------------
	// BadDataType exception
	//---------------------------------------------------------------
	class BadDataType : public Exception {
	public:
		BadDataType(const string file, const int line)
			: Exception("BadDataType",file,line) { }
	};

	//---------------------------------------------------------------
	// SizeMismatch exception
	//---------------------------------------------------------------
	class SizeMismatch : public Exception {
	public:
		SizeMismatch(const string file, const int line)
			: Exception("SizeMismatch",file,line) { }
	};

	//---------------------------------------------------------------
	// NotCompatible exception
	//---------------------------------------------------------------
	class NotCompatible : public Exception {
	public:
		NotCompatible(const string file, const int line)
			: Exception("NotCompatible",file,line) { }
	};

	//---------------------------------------------------------------
	// DataNotLocal exception
	//---------------------------------------------------------------
	class DataNotLocal : public Exception {
	public:
		DataNotLocal(const string file, const int line)
			: Exception("DataNotLocal",file,line) { }
	};

	//---------------------------------------------------------------
	// BadMatrixIndex exception
	//---------------------------------------------------------------
	class BadMatrixIndex : public Exception {
	public:

		BadMatrixIndex(const string file, const int line)
			: Exception("BadMatrixIndex",file,line) { }
	};





	//-------------------------------------------------------------------------
	// Output operator
	//-------------------------------------------------------------------------
	inline std::ostream& operator<<(std::ostream& os, const ipvtol::Exception& E)
	{
		E.print(os);
		return os;
	}


} // end namespace 

#endif // PVTOL_EXCEPTION_H

