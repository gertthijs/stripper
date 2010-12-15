/**********************************************************************
counter.h - Stripper
 
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



#ifndef __COUNTER_H__
#define __COUNTER_H__



#include <vector>
#include <algorithm>
#include <sstream>



class Counter
{
   	protected:
   
      	unsigned int _totalMol;
      	unsigned int _scaffolds;
      	unsigned int _noScaffolds;

   	public:
   
      	Counter(void);
     	virtual ~Counter(void);
      	Counter(const Counter&);
      	Counter& operator=(const Counter&);

      	virtual void NewMolecule(void);
      	virtual void NewScaffold(void);
      	virtual void FailedScaffold(void);
      
      	virtual std::string Results(void) const;
};



#endif
