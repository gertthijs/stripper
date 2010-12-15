/**********************************************************************
scaffold.h - Stripper
 
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



#ifndef __SCAFFOLD_H__
#define __SCAFFOLD_H__



#include <string>
#include <map>
#include <list>
#include <set>
#include <algorithm>

#include <openbabel/mol.h>
#include <options.h>



class Scaffold
{
   	protected:
   
      	std::string _name;
      	std::string _smiles;
		OpenBabel::OBConversion _mol2can;
    
    	virtual bool IsEndStanding(OpenBabel::OBAtom*, bool, bool);
      
   	public:
   
      	Scaffold(void);
      	virtual ~Scaffold(void);
      	Scaffold(const Scaffold&);
      	Scaffold& operator=(const Scaffold&);
   
      	virtual bool CalculateScaffold(const OpenBabel::OBMol&, Options*) = 0;
      	virtual std::string PrintName(void);
      	virtual std::string PrintScaffold(void);
};



#endif
