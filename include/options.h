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



#ifndef __OPTIONS_H__
#define __OPTIONS_H__



#include <string>
#include <sstream>

#include <openbabel/obconversion.h>



class Options
{
   	protected:

      	OpenBabel::OBConversion _iconv;
      	std::ifstream _ifs;
      	std::string _ifile; // --in

      	std::ofstream _ofs;
      	std::string _ofile; // --out

      	std::ifstream _sfs;
      	std::string  _sfile; // --scaffold

      	bool _log; // --noLog

   	public:
      
      	Options(void);
      	virtual ~Options(void);
      	Options(const Options&);
      	Options& operator=(const Options&);

      	virtual bool InputFilename(const std::string&);
      	virtual std::string InputFilename(void) const;
      	virtual std::string InputFormat(void) const;
      	virtual OpenBabel::OBConversion* InputConverter(void);
      	virtual std::ifstream* InputStream(void);

      	virtual bool OutFilename(const std::string&);
      	virtual std::string OutFilename(void) const;
      	virtual std::ofstream* OutStream(void);

      	virtual bool ScaffoldFilename(const std::string&);
      	virtual std::string ScaffoldFilename(void) const;
      	virtual std::ifstream* ScaffoldStream(void);

      	virtual std::string Print(void) const;     

      	virtual void SetLog(bool);     
      	virtual bool GetLog(void) const;     
};



#endif
