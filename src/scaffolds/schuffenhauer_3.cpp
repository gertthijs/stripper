/**********************************************************************
schuffenhauer_3.cpp - Stripper
 
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



#include <scaffolds/schuffenhauer_3.h>




Schuffenhauer_3::Schuffenhauer_3(void):
Scaffold()
{
	_name = "SCHUFFENHAUER-ERTL_3";
	_ringsToBeRetained = 3;
}



Schuffenhauer_3::~Schuffenhauer_3(void)
{
}



Schuffenhauer_3::Schuffenhauer_3(const Schuffenhauer_3& s)
{
	_name = s._name;
	_smiles = s._smiles;
	_ringsToBeRetained = s._ringsToBeRetained;
}



Schuffenhauer_3&
Schuffenhauer_3::operator=(const Schuffenhauer_3& s)
{
   if (this != &s)
   {
      _name = s._name;
      _smiles = s._smiles;
      _ringsToBeRetained = s._ringsToBeRetained;
   }
   return *this;
}
