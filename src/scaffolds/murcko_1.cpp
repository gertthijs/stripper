/**********************************************************************
murcko_1.cpp - Stripper
 
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



#include <scaffolds/murcko_1.h>



Murcko_1::Murcko_1(void):
Scaffold()
{
	_name = "MURCKO_1";
}



Murcko_1::~Murcko_1(void)
{
}



Murcko_1::Murcko_1(const Murcko_1& s)
{
	_name = s._name;
	_smiles = s._smiles;
}



Murcko_1&
Murcko_1::operator=(const Murcko_1& s)
{
	if (this != &s)
	{
		_name = s._name;
		_smiles = s._smiles;
	}
	return *this;
}



bool
Murcko_1::CalculateScaffold(const OpenBabel::OBMol& mol, Options* o)
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
			if (IsEndStanding(atom, false, false))
			{
				m.DeleteAtom(atom);
				removed = true;
				break;
			}
		}
	}

	// Make all atoms neutral C and all bond orders equal to 1
	m.BeginModify();
	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
	{
		atom->SetAtomicNum(6);
		atom->SetFormalCharge(0);
	}
   	OpenBabel::OBBond* bond;
	std::vector<OpenBabel::OBBond*>::iterator bvi;
	for (bond = m.BeginBond(bvi); bond; bond = m.NextBond(bvi))
	{
		bond->SetBondOrder(1);
	}
	m.EndModify();

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
