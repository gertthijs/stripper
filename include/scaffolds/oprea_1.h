/**********************************************************************
oprea_1.h - Stripper
 
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



#ifndef __OPREA_1_H__
#define __OPREA_1_H__



#include <scaffolds/oprea.h>



class Oprea_1: virtual public Oprea
{
   	public:
   
      	Oprea_1(void);
      	virtual ~Oprea_1(void);
      	Oprea_1(const Oprea_1&);
      	Oprea_1& operator=(const Oprea_1&);
   
      	virtual bool CalculateScaffold(const OpenBabel::OBMol&, Options*);
};



#endif
