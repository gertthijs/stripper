/**********************************************************************
rings_with_linkers_1.cpp - Stripper
 
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



#include <scaffolds/rings_with_linkers_1.h>



Rings_With_Linkers_1::Rings_With_Linkers_1(void):
Scaffold()
{
	_name = "RINGS_WITH_LINKERS_1";
}



Rings_With_Linkers_1::~Rings_With_Linkers_1(void)
{
}



Rings_With_Linkers_1::Rings_With_Linkers_1(const Rings_With_Linkers_1& s)
{
	_name = s._name;
	_smiles = s._smiles;
}



Rings_With_Linkers_1&
Rings_With_Linkers_1::operator=(const Rings_With_Linkers_1& s)
{
   	if (this != &s)
   	{
      	_name = s._name;
     	_smiles = s._smiles;
   	}
   	return *this;
}
   


bool
Rings_With_Linkers_1::CalculateScaffold(const OpenBabel::OBMol& mol, Options* o)
{
   	OpenBabel::OBMol m(mol);
   	OpenBabel::OBAtom* atom;
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	bool removed(true);
   	while (removed)
   	{
      	removed = false;
      	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
      	{
         	if (atom->GetValence() < 2)
         	{
            	m.DeleteAtom(atom);
            	removed = true;
				break;
         	}
      	}
	}
  
   	if (!m.Empty())
	{
		_smiles = _mol2can.WriteString(&m, true);
	}
   	else
   	{
      	_smiles = "-";
      	return false;
   	}
  	return true;
}
