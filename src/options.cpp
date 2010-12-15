/**********************************************************************
options.h - Stripper
 
Copyright (C) 2005-2010 by Silicos NV
 
This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/



#include <options.h>



Options::Options(void):
_log(true)
{
   	_ifile.clear();
   	_ofile.clear();
   	_sfile.clear();
}



Options::~Options(void)
{
}



Options::Options(const Options& f):
_ifile(f._ifile),
_iconv(f._iconv),
_ofile(f._ofile),
_sfile(f._sfile),
_log(f._log)
{
}



Options&
Options::operator=(const Options& f)
{
   	_ifile = f._ifile;
   	_iconv = f._iconv;
   	_ofile = f._ofile;
   	_sfile = f._sfile;
   	_log = f._log;
    return *this;
}



bool
Options::InputFilename(const std::string& f)
{
   	_ifile = f;
   	if (_ifile.empty())
	{
		return false;
	}
   	return true;
}



std::string
Options::InputFilename(void) const
{
   	return _ifile;
}



std::string
Options::InputFormat(void) const
{
   	return std::string(_iconv.GetInFormat()->GetID());
}



std::ifstream*
Options::InputStream(void)
{
   	return &_ifs;
}



OpenBabel::OBConversion*
Options::InputConverter(void)
{
   	return &_iconv;
}



bool
Options::OutFilename(const std::string& f)
{
   	_ofile = f;
   	if (_ofile.empty())
	{
		return false;
	}
   	return true;
}



std::string
Options::OutFilename(void) const
{
   return _ofile;
}



std::ofstream*
Options::OutStream(void)
{
   return &_ofs;
}



bool
Options::ScaffoldFilename(const std::string& f)
{
   _sfile = f;
   if (_sfile.empty()) return false;
   return true;
}



std::string
Options::ScaffoldFilename(void) const
{
   return _sfile;
}



std::ifstream*
Options::ScaffoldStream(void)
{
   return &_sfs;
}



void
Options::SetLog(bool l)
{
   _log = l;
}



bool
Options::GetLog(void) const
{
   return _log;
}



std::string
Options::Print(void) const
{
   std::ostringstream os;
   os << std::endl;
   os << "## START PARSING COMMAND LINE OPTIONS" << std::endl;
   if (_sfile != "")
   {
      os << "-> Scaffolds file:    " << _sfile << " -> ";
      if (_sfs.good())
      {
         os << "ok" << std::endl;
      }
      else
      {
         os << "error" << std::endl;
      }
   }
   os << "-> Output file:       " << _ofile << " -> ";
   if (_ofs.good())
   {
      os << "ok" << std::endl;
   }
   else
   {
      os << "error" << std::endl;
   }
   os << "-> Input file:        " << _ifile << " (";
   os << Options::InputFormat() << ") -> ";
   if (_ifs.good())
   {
      os << "ok" << std::endl;
   }
   else
   {
      os << "error" << std::endl;
   }
   os << "## END PARSING COMMAND LINE OPTIONS" << std::endl;
   
   return os.str();
}
