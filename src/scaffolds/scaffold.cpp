/**********************************************************************
scaffold.cpp - Stripper
 
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



#include <scaffolds/scaffold.h>



Scaffold::Scaffold(void):
_name("SCAFFOLD"),
_smiles("-")
{
	_mol2can.SetOutFormat("can");
   	_mol2can.AddOption("n", OpenBabel::OBConversion::OUTOPTIONS);
}



Scaffold::~Scaffold(void)
{
}



Scaffold::Scaffold(const Scaffold& s):
_name(s._name),
_smiles(s._smiles),
_mol2can(s._mol2can)
{
}



Scaffold&
Scaffold::operator=(const Scaffold& s)
{
	if (this != &s)
	{
		_name = s._name;
		_smiles = s._smiles;
		_mol2can = s._mol2can;
	}
	return *this;
}
   


std::string
Scaffold::PrintName(void)
{
	return _name;
}
   


std::string
Scaffold::PrintScaffold(void)
{
	return _smiles;
}



bool
Scaffold::IsEndStanding(OpenBabel::OBAtom* atom)
{
	bool endStanding(false);
	if ((atom->GetValence() > 1) || 
		atom->IsInRing())
	{
		endStanding = false;
	}
   	else
	if (atom->GetValence() == 0)
	{
		endStanding = true;
	}
   	else
   	{
		// Valence == 1
		OpenBabel::OBBondIterator bi;
		OpenBabel::OBBond* bond(atom->BeginBond(bi));
		if (bond->GetBondOrder() == 2)
		{
			OpenBabel::OBAtom* nbrAtom(bond->GetNbrAtom(atom));
			OpenBabel::OBBondIterator bi2;
			OpenBabel::OBAtom* a;
			unsigned int nconnect(0);
			for (a = nbrAtom->BeginNbrAtom(bi2); a; a = nbrAtom->NextNbrAtom(bi2))
			{
				if (a->GetValence() > 1) { ++nconnect; }
			}
 			if (nconnect == 0)
			{ 
				endStanding = true;
			}
         	else
			if (nconnect == 1)
			{
				endStanding = true;
			}
			else
			{
				endStanding = false;
			}
		}
		else
		{ 
			endStanding = true;
		}
	}
	return endStanding;
}
