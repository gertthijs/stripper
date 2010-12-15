/**********************************************************************
oprea_3.h - Stripper
 
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



#ifndef __OPREA_3_H__
#define __OPREA_3_H__



#include <scaffolds/oprea.h>



class Oprea_3: virtual public Oprea
{
   	public:
   
      	Oprea_3(void);
      	virtual ~Oprea_3(void);
      	Oprea_3(const Oprea_3&);
      	Oprea_3& operator=(const Oprea_3&);
   
      	virtual bool CalculateScaffold(const OpenBabel::OBMol&, Options*);
};



#endif
