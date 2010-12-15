/**********************************************************************
oprea_2.h - Stripper
 
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



#ifndef __OPREA_2_H__
#define __OPREA_2_H__



#include <scaffolds/oprea.h>



class Oprea_2: virtual public Oprea
{
   	public:
   
      	Oprea_2(void);
      	virtual ~Oprea_2(void);
      	Oprea_2(const Oprea_2&);
      	Oprea_2& operator=(const Oprea_2&);
   
      	virtual bool CalculateScaffold(const OpenBabel::OBMol&, Options*);
};



#endif
