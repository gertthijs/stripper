/**********************************************************************
oprea.h - Stripper
 
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



#ifndef __OPREA_H__
#define __OPREA_H__



#include <string>
#include <vector>
#include <list>

#include <scaffolds/scaffold.h>

#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/OBConversion.h>



class Oprea: virtual public Scaffold
{
   	public:
   
      	Oprea(void);
      	virtual ~Oprea(void);
      	Oprea(const Oprea&);
      	Oprea& operator=(const Oprea&);
   
      	virtual bool CalculateScaffold(const OpenBabel::OBMol&, Options*) = 0;
};



#endif
