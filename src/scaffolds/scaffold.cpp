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
Scaffold::IsEndStanding(OpenBabel::OBAtom* atom, bool keepExocyclicDB = false, bool keepExolinkerDB = false)
{
	if (atom->IsInRing())
	{
		return false;
	}

	if (atom->GetValence() == 0)
	{
		return true;
	}

	if (atom->GetValence() == 1)
   	{
		if (keepExocyclicDB)
		{
			return !(atom->MatchesSMARTS("[D1]=[R]"));
		}
		
		if (keepExolinkerDB)   
		{
			return !(atom->MatchesSMARTS("[D1]=[D3,D4]"));
		}
		
		return true;
	}  
	
	return false;
}
