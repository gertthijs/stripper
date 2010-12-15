/**********************************************************************
calculator.h - Stripper
 
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



#ifndef __CALCULATOR_H__
#define __CALCULATOR_H__



#include <vector>

#include <openbabel/mol.h>

#include <options.h>
#include <counter.h>
#include <scaffolds/scaffold.h>



class Calculator
{
   	public:
      
      	Calculator(void);
      	virtual ~Calculator(void);
      	Calculator(const Calculator&);
      	Calculator& operator=(const Calculator&);
  
      	virtual Counter Calculate(Options*, const std::vector<Scaffold*>&);
};



#endif
