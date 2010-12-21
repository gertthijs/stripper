/**********************************************************************
oprea_3.cpp - Stripper
 
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



#include <scaffolds/oprea_3.h>



Oprea_3::Oprea_3(void):
Oprea()
{
	_name = "OPREA_3";
}



Oprea_3::~Oprea_3(void)
{
}



Oprea_3::Oprea_3(const Oprea_3& s)
{
	_name = s._name;
	_smiles = s._smiles;
}



Oprea_3&
Oprea_3::operator=(const Oprea_3& s)
{
   	if (this != &s)
   	{
      	_name = s._name;
      	_smiles = s._smiles;
   	}
   	return *this;
}
   


bool
Oprea_3::CalculateScaffold(const OpenBabel::OBMol& mol, Options* o)
{
   	OpenBabel::OBMol m(mol);

   	// Tag all HBD
   	std::vector<bool> hbd(m.NumAtoms() + 1);
   	for (OpenBabel::OBMolAtomIter atom(m); atom; ++atom)
   	{    
		if (atom->MatchesSMARTS("[NH,NH2,NH3,OH,nH]"))
      	{
         	hbd[atom->GetIdx()] = true;
      	}
      	else
      	{
         	hbd[atom->GetIdx()] = false;
      	}
   	}
   
   	// Tag all HBA
   	std::vector<bool> hba(m.NumAtoms() + 1);
   	for (OpenBabel::OBMolAtomIter atom(m); atom; ++atom)
   	{   
 		if (!atom->IsAmideNitrogen() &&        // No amide nitrogen
			!atom->IsAromatic() &&             // Not aromatic
			(atom->GetFormalCharge() <= 0) &&    // No + charge
			atom->MatchesSMARTS("[NH0]"))        // No hydrogens
      	{
         	hba[atom->GetIdx()] = true;
      	}
      	else
		if (atom->IsNitrogen() &&              // Nitrogen
          	atom->IsAromatic() &&              // Aromatic
			atom->MatchesSMARTS("[nH0]") && 	   // No hydrogens
         	(atom->GetHvyValence() <= 2) &&    // Maximal two non-H atoms connected
         	(atom->GetFormalCharge() <= 0))    // No + charge
      	{
         	hba[atom->GetIdx()] = true;
      	}
      	else
      	if (atom->IsOxygen() &&                // Oxygen
         	(atom->GetFormalCharge() <= 0))    // No + charge
      	{
         	hba[atom->GetIdx()] = true;
     	}
      	else
      	{
          	hba[atom->GetIdx()] = false;
      	}
   	}
   
   	// Mark the C(=O) or S(=O) also as HBA
   	for (OpenBabel::OBMolAtomIter atom(m); atom; ++atom)
   	{   
		if (atom->MatchesSMARTS("C=O"))
		{
         	hba[atom->GetIdx()] = true;
		}
		else
		if (atom->MatchesSMARTS("S=O"))
		{
         	hba[atom->GetIdx()] = true;
		} 
	}

   	// Make all atoms as neutral C, N (HBD), or O (HBA)
   	m.BeginModify();
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	OpenBabel::OBAtom* atom;
   	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
   	{
      	if (hba[atom->GetIdx()])
		{
			atom->SetAtomicNum(8);
		}
      	else
		if (hbd[atom->GetIdx()])
		{
			atom->SetAtomicNum(7);
		}
      	else
		{
			atom->SetAtomicNum(6);
		}
      	atom->SetFormalCharge(0);
   	}
   	m.EndModify();

	// Remove endstanding atoms
   	OpenBabel::OBBondIterator bi;
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
   
   	// Make all bond orders equal to 1
   	m.BeginModify();
   	for (bond = m.BeginBond(bvi); bond; bond = m.NextBond(bvi))
   	{
      	bond->SetBondOrder(1);
   	}
   	m.EndModify();

   	// Shrink all linkers to their minimal size
   	removed = true;
	OpenBabel::OBAtom* nbrAtom[2];
   	while (removed)
   	{
       	removed = false;
      	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
      	{
         	if (atom->IsInRing())
			{
				continue;
			}
            nbrAtom[0] = atom->BeginNbrAtom(bi);
            nbrAtom[1] = atom->NextNbrAtom(bi);
			if (nbrAtom[0] && nbrAtom[1])
			{
            	if (nbrAtom[0]->GetAtomicNum() == atom->GetAtomicNum())
            	{
           			m.BeginModify();
              		m.AddBond(nbrAtom[0]->GetIdx(), nbrAtom[1]->GetIdx(), 1);
              		m.DeleteAtom(atom);
              		m.EndModify();
               		removed = true;
               		break;
         		}
				else
            	if (nbrAtom[1]->GetAtomicNum() == atom->GetAtomicNum())
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

	// Shrink all rings
   	removed = true;
   	while (removed)
   	{
      	removed = false;
      	for (atom = m.BeginAtom(avi); atom; atom = m.NextAtom(avi))
      	{
         	if ((atom->MemberOfRingSize() > 3) && (atom->GetValence() == 2))
         	{
            	nbrAtom[0] = atom->BeginNbrAtom(bi);
            	nbrAtom[1] = atom->NextNbrAtom(bi);
				if (nbrAtom[0] && nbrAtom[1])
				{
            		if (nbrAtom[0]->GetAtomicNum() == atom->GetAtomicNum())
            		{
               			m.BeginModify();
               			m.AddBond(nbrAtom[0]->GetIdx(), nbrAtom[1]->GetIdx(), 1);
               			m.DeleteAtom(atom);
               			m.EndModify();
               			removed = true;
               			break;
            		}
            		else
					if (nbrAtom[1]->GetAtomicNum() == atom->GetAtomicNum())
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
