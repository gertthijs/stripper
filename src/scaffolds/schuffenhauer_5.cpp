/**********************************************************************
schuffenhauer_5.cpp - Stripper
 
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



#include <scaffolds/schuffenhauer_5.h>




Schuffenhauer_5::Schuffenhauer_5(void):
Scaffold()
{
	_name = "SCHUFFENHAUER-ERTL_5";
	_ringsToBeRetained = 5;
}



Schuffenhauer_5::~Schuffenhauer_5(void)
{
}



Schuffenhauer_5::Schuffenhauer_5(const Schuffenhauer_5& s)
{
	_name = s._name;
	_smiles = s._smiles;
	_ringsToBeRetained = s._ringsToBeRetained;
}



Schuffenhauer_5&
Schuffenhauer_5::operator=(const Schuffenhauer_5& s)
{
   if (this != &s)
   {
      _name = s._name;
      _smiles = s._smiles;
      _ringsToBeRetained = s._ringsToBeRetained;
   }
   return *this;
}
