/**********************************************************************
oprea_1.cpp - Stripper
 
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



#include <scaffolds/oprea_1.h>



Oprea_1::Oprea_1(void):
Oprea()
{
	_name = "OPREA_1";
}



Oprea_1::~Oprea_1(void)
{
}



Oprea_1::Oprea_1(const Oprea_1& s)
{
	_name = s._name;
	_smiles = s._smiles;
}



Oprea_1&
Oprea_1::operator=(const Oprea_1& s)
{
	if (this != &s)
	{
		_name = s._name;
		_smiles = s._smiles;
	}
	return *this;
}
   


bool
Oprea_1::CalculateScaffold(const OpenBabel::OBMol& mol, Options* o)
{
	OpenBabel::OBMol m(mol);
	OpenBabel::OBAtom* atom;
	OpenBabel::OBAtom* nbrAtom[2];
	OpenBabel::OBBondIterator bi;
	std::vector<OpenBabel::OBAtom*>::iterator avi;
	OpenBabel::OBBond* bond;
	std::vector<OpenBabel::OBBond*>::iterator bvi;
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

	// Make all atoms as neutral C and all bond orders equal to 1
 	m.BeginModify();
	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
	{
		atom->SetAtomicNum(6);
		atom->SetFormalCharge(0);
	}
	for (bond = m.BeginBond(bvi); bond; bond = m.NextBond(bvi))
	{
		bond->SetBondOrder(1);
	}
	m.EndModify();
   
   	// Transform all neighbouring linker atoms into a single bond
   	removed = true;
   	while (removed)
   	{
		removed = false;
		for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
		{
			if (!atom->IsInRing() && (atom->GetValence() == 2))
			{
		        nbrAtom[0] = atom->BeginNbrAtom(bi);
		        nbrAtom[1] = atom->NextNbrAtom(bi);
				if (nbrAtom[0] && nbrAtom[1])
				{
					m.BeginModify();
		        	m.AddBond(nbrAtom[0]->GetIdx(), nbrAtom[1]->GetIdx(), 1);
		        	m.DeleteAtom(atom);
		      		m.EndModify();
					removed = true;
					break;
				}
			}
		}
   	}

   	// Shrink all rings to their minimum size
   	removed = true;
   	while (removed)
   	{
      	removed = false;
      	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
      	{
         	if ((atom->MemberOfRingSize() > 3) &&
				(atom->GetValence() == 2))
         	{
            	nbrAtom[0] = atom->BeginNbrAtom(bi);
            	nbrAtom[1] = atom->NextNbrAtom(bi);
				if (nbrAtom[0] && nbrAtom[1])
				{
            		m.BeginModify();
            		m.AddBond(nbrAtom[0]->GetIdx(), nbrAtom[1]->GetIdx(), 1);
            		m.DeleteAtom(atom);
            		m.EndModify();
            		removed = true;
            		break;
				}
         	}
      	}
   	}
   
   	if (!m.Empty())
	{
      	// Drop stereo information
      	m.DeleteData(OpenBabel::OBGenericDataType::StereoData);
		_smiles = _mol2can.WriteString(&m, true);
	}
   	else
   	{
      	_smiles = "-";
      	return false;
   	}
   	return true;
}
