/**********************************************************************
schuffenhauer.cpp - Stripper
 
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



#include <scaffolds/schuffenhauer.h>



Schuffenhauer::Schuffenhauer(void):
Scaffold()
{
	_name = "SCHUFFENHAUER";
	_ringsToBeRetained = 0;
}



Schuffenhauer::~Schuffenhauer(void)
{
}



Schuffenhauer::Schuffenhauer(const Schuffenhauer& s)
{
	_name = s._name;
	_smiles = s._smiles;
	_ringsToBeRetained = s._ringsToBeRetained;
}



Schuffenhauer&
Schuffenhauer::operator=(const Schuffenhauer& s)
{
   	if (this != &s)
   	{
      	_name = s._name;
      	_smiles = s._smiles;
      	_ringsToBeRetained = s._ringsToBeRetained;
   	}
   	return *this;
}



unsigned int
Schuffenhauer::RemoveSidechains(OpenBabel::OBMol* mol)
{
   	OpenBabel::OBAtom* atom;
   	OpenBabel::OBBondIterator bi;
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	bool removed(true);
   	unsigned int numberRemoved(0);
   	while (removed)
   	{
      	removed = false;
      	for (atom = mol->BeginAtom(avi); atom; atom = mol->NextAtom(avi))
      	{
         	if (IsEndStanding(atom, true, false))
         	{   
 	         	mol->DeleteAtom(atom);
				++numberRemoved;
           		removed = true;
				break;
         	}
       	}
   	}
   	return numberRemoved;
}



OpenBabel::OBMol
Schuffenhauer::RemoveRing(OpenBabel::OBMol& oldMol, 
std::vector<OpenBabel::OBRing*>& rings, unsigned int ringIdx)
{
	OpenBabel::OBMol newMol(oldMol);

	// Make list of the ring bonds
	std::set<OpenBabel::OBBond*> ringBonds;
	OpenBabel::OBBond* bond;
	std::vector<OpenBabel::OBBond*>::iterator bvi;
	for (bond = newMol.BeginBond(bvi); bond; bond = newMol.NextBond(bvi))
	{
		if (rings[ringIdx]->IsMember(bond))
		{
			ringBonds.insert(bond);
		}
	}
	
	// Make list of delocalizable bonds (aromatic, single and flanked by two double bonds)
	std::set<OpenBabel::OBBond*> delocalizableBonds;
	std::set<OpenBabel::OBBond*>::iterator bli;
	OpenBabel::OBBondIterator bi;
	OpenBabel::OBAtom* atom;
	OpenBabel::OBBond* nbrBond;
	unsigned int n;
	for (bli = ringBonds.begin(); bli != ringBonds.end(); ++bli)
	{
		bond = *bli;
		if ((bond->GetBondOrder() == 1) && 
			bond->IsAromatic())
		{
			n = 0;
			atom = bond->GetBeginAtom();
			for (nbrBond = atom->BeginBond(bi); nbrBond; nbrBond = atom->NextBond(bi))
			{
				if ((nbrBond != bond) && 
					ringBonds.count(nbrBond) && 
					(nbrBond->GetBondOrder() == 2))
				{
					++n;
				}
			}
			atom = bond->GetEndAtom();
			for (nbrBond = atom->BeginBond(bi); nbrBond; nbrBond = atom->NextBond(bi))
			{
				if ((nbrBond != bond) && 
					ringBonds.count(nbrBond) &&
					(nbrBond->GetBondOrder() == 2))
				{
					++n;
				}
			}
		}
		if (n == 2)
		{
			delocalizableBonds.insert(bond);
		}
	}
	
	// Make list of bonds which form the fusion with other rings
	std::set<OpenBabel::OBBond*> fusionBonds;
	for (bli = ringBonds.begin(); bli != ringBonds.end(); ++bli)
	{
		bond = *bli;
		for (unsigned int i(0); i < rings.size(); ++i)
		{
			if (i != ringIdx)
			{
				if (rings[i]->IsMember(bond))
				{
					fusionBonds.insert(bond);
				}
			}
		}
	}
   
	// Make list of bonds which are the fusion between aromatic and non-aromatic
	std::set<OpenBabel::OBBond*> aromaticNonaromaticFusionBonds;
	if (rings[ringIdx]->IsAromatic())
	{
		for (bli = fusionBonds.begin(); bli != fusionBonds.end(); ++bli)
		{
			bond = *bli;
			for (unsigned int i(0); i < rings.size(); ++i)
			{
				if (i != ringIdx)
				{
					if (rings[i]->IsMember(bond) && 
						!rings[i]->IsAromatic())
					{
						aromaticNonaromaticFusionBonds.insert(bond);
					}
				}
			}
		}
	}
   
	// Make list of bonds which are the fusion between aromatic and aromatic
	std::set<OpenBabel::OBBond*> aromaticAromaticFusionBonds;
	if (rings[ringIdx]->IsAromatic())
	{
		for (bli = fusionBonds.begin(); bli != fusionBonds.end(); ++bli)
		{
			bond = *bli;
			for (unsigned int i(0); i < rings.size(); ++i)
			{
				if (i != ringIdx)
				{
					if (rings[i]->IsMember(bond) &&
						rings[i]->IsAromatic())
					{
						aromaticAromaticFusionBonds.insert(bond);
					}
				}
 			}
		}
	}

	// Remove ring
	std::set<OpenBabel::OBBond*> bondsToBeDeleted;  
	std::set<OpenBabel::OBAtom*> atomsToBeDeleted;  
	OpenBabel::OBAtom* nbrAtom[2];
	for (bli = ringBonds.begin(); bli != ringBonds.end(); ++bli)
	{
		bond = *bli;
		if (fusionBonds.count(bond))
		{
			continue;
		}
		else
		{
			bondsToBeDeleted.insert(bond);
			nbrAtom[0] = bond->GetBeginAtom();
			nbrAtom[1] = bond->GetEndAtom();
			if (nbrAtom[0] && nbrAtom[1])
			{
				if (nbrAtom[0]->MemberOfRingCount() == 1)
				{
					atomsToBeDeleted.insert(nbrAtom[0]);
				}
				if (nbrAtom[1]->MemberOfRingCount() == 1)
				{
					atomsToBeDeleted.insert(nbrAtom[1]);
				}
			}
		}
	}
	newMol.BeginModify();
	for (bli = bondsToBeDeleted.begin(); bli != bondsToBeDeleted.end(); ++bli)
	{
		newMol.DeleteBond(*bli);
	}
	newMol.EndModify();
	newMol.BeginModify();
	std::set<OpenBabel::OBAtom*>::iterator ali;
	for (ali = atomsToBeDeleted.begin(); ali != atomsToBeDeleted.end(); ++ali)
	{
		newMol.DeleteAtom(*ali);
	}
	newMol.EndModify();
	
	// Correct the bond orders of the ex-fusion bond(s)
	newMol.BeginModify();
	for (bond = newMol.BeginBond(bvi); bond; bond = newMol.NextBond(bvi))
	{
		if (aromaticNonaromaticFusionBonds.count(bond))
		{
			bond->SetBondOrder(2);
		} 
		else
		if (aromaticAromaticFusionBonds.count(bond) && 
			delocalizableBonds.count(bond))
		{
			bond->SetBondOrder(2);
		} 
 	}
	newMol.EndModify();
   
	// Remove single atoms that originate from exocyclic bonds at ring
	(void) RemoveSidechains(&newMol);
	
	// Check if there are atoms with valences that are not allowed
	std::vector<OpenBabel::OBAtom*>::iterator avi;
	for (atom = newMol.BeginAtom(avi); atom; atom = newMol.NextAtom(avi))
	{
		if (atom->IsCarbon() &&
			(atom->BOSum() > 4))
		{
			newMol.Clear();
			break;
		}
		else
		if (atom->IsNitrogen() &&
			(atom->BOSum() > 3))
		{
			newMol.Clear();
			break;
		}
		else
		if (atom->IsOxygen() &&
			(atom->BOSum() > 2))
		{
			newMol.Clear();
			break;
		}
	}
	
	// Check if there are no discontinuous fragments
	if (newMol.Separate().size() > 1)
	{
		newMol.Clear();
	}
	
	return newMol;
}



unsigned int
Schuffenhauer::CalculateAcyclicBonds(OpenBabel::OBMol& mol)
{
   	unsigned int nbonds(0);
   	OpenBabel::OBAtom* nbratom[2];
   	OpenBabel::OBBond* bond;
   	std::vector<OpenBabel::OBBond*>::iterator bvi;
   	for (bond = mol.BeginBond(bvi); bond; bond = mol.NextBond(bvi))
   	{
      	nbratom[0] = bond->GetBeginAtom();
      	nbratom[1] = bond->GetEndAtom();
		if (nbratom[0] && nbratom[1])
		{
      		if (!bond->IsInRing() && 
				(nbratom[0]->GetValence() > 1) && 
				(nbratom[1]->GetValence() > 1))
      		{
         		++nbonds;
      		}
		}
   	}
   	return nbonds;
}



int
Schuffenhauer::CalculateDelta(OpenBabel::OBMol& mol)
{
   	OpenBabel::OBMol m(mol);
   	std::vector<OpenBabel::OBRing*> rings(m.GetSSSR());
   	int nr(rings.size());
   
   	// Count fusion bonds
   	std::set<OpenBabel::OBBond*> fusionBonds;
   	OpenBabel::OBBond* bond;
   	std::vector<OpenBabel::OBBond*>::iterator bvi;
   	for (bond = m.BeginBond(bvi); bond; bond = m.NextBond(bvi))
   	{
      	for (unsigned int i(0); i < rings.size(); ++i)
      	{
         	for (unsigned int j(i); j < rings.size(); ++j)
         	{
            	if (i != j)
            	{
               		if (rings[i]->IsMember(bond) && rings[j]->IsMember(bond))
               		{
                  		fusionBonds.insert(bond);
               		}
            	}
         	}
      	}
   	}
   	int nrrb(fusionBonds.size());
   	return (nrrb - (nr - 1));
}



unsigned int
Schuffenhauer::CalculateHeteroAtoms(OpenBabel::OBMol& mol, OpenBabel::OBRing* ring, int a = 0)
{
   	unsigned int n(0);
   	OpenBabel::OBAtom* atom;
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	for (atom = mol.BeginAtom(avi); atom; atom = mol.NextAtom(avi))
   	{
      	if (ring->IsMember(atom) && (atom->GetAtomicNum() == a))
      	{
         	++n;
      	}
      	if (!a && ring->IsMember(atom))
      	{
         	if ((atom->GetAtomicNum() == 7) ||
				(atom->GetAtomicNum() == 8) ||
				(atom->GetAtomicNum() == 16))
         	{
            	++n;
         	}
      	}
   	}
   	return n;
}



bool
Schuffenhauer::HasLinkerToHeteroAtom(OpenBabel::OBMol& mol, OpenBabel::OBRing* ring)
{
   	std::vector<OpenBabel::OBBond*>::iterator bvi;
   	OpenBabel::OBBond* bond;
   	OpenBabel::OBAtom* nbrAtom[2];
   	for (bond = mol.BeginBond(bvi); bond; bond = mol.NextBond(bvi))
   	{
      	nbrAtom[0] = bond->GetBeginAtom();
      	nbrAtom[1] = bond->GetEndAtom();
      	if
		(
			// Neighbours are real
			nbrAtom[0] &&
			nbrAtom[1] &&
			
			// Bond should be acyclic
			!bond->IsInRing() &&
			
			// Both atoms have to be ring atoms
			nbrAtom[0]->IsInRing() && 
			nbrAtom[1]->IsInRing() && 
			
			// At least one of the atoms should be hetero
			(nbrAtom[0]->IsHeteroatom() || nbrAtom[1]->IsHeteroatom()) &&
			
			// One of the atoms, but not both, should be part of this ring	
			((ring->IsMember(nbrAtom[0]) && !ring->IsMember(nbrAtom[1])) ||
			(ring->IsMember(nbrAtom[1]) && !ring->IsMember(nbrAtom[0])))
		)
		{
         	return true;
     	}
   	}
   	return false;
}



OpenBabel::OBMol
Schuffenhauer::Rule_1(OpenBabel::OBMol& oldMol)
{
   	if (oldMol.GetSSSR().size() <= _ringsToBeRetained)
	{
		return oldMol;
	}
   
   	OpenBabel::OBMol newMol(oldMol);
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	OpenBabel::OBBondIterator bi;
   	OpenBabel::OBAtom* atom;
   	OpenBabel::OBAtom* nbrAtom[2];
   	for (atom = newMol.BeginAtom(avi); atom; atom = newMol.NextAtom(avi))
   	{
      	if ((atom->MemberOfRingSize() == 3) &&
          	(atom->IsNitrogen() || atom->IsOxygen()) &&
          	(atom->MemberOfRingCount() == 1) &&
          	(atom->GetHvyValence() == 2))
      	{
         	nbrAtom[0] = atom->BeginNbrAtom(bi);
         	nbrAtom[1] = atom->NextNbrAtom(bi);
			if (nbrAtom[0] && nbrAtom[1])
			{
         		newMol.DeleteAtom(atom);
         		newMol.GetBond(nbrAtom[0], nbrAtom[1])->SetBondOrder(2);
			}
      	}
   	}
   	return newMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_2(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
	{
		return oldMol;
	}
   
   	// Count rings and macrocycles
   	unsigned int smallRings(0);
   	unsigned int macrocycles(0);
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	if (allrings[i]->Size() < 12)
		{
			++smallRings;
		}
      	else
		{
			++macrocycles;
		}
   	}
   
   	// If there are no small rings then there is nothing to do in this step
   	if (!smallRings)
	{
		return oldMol;
	}
   	else if (!macrocycles)
   	{
   		// If there are no macrocycles then there is nothing to do in this step
      	return oldMol;
   	}
   	else 
   	{
      	std::vector<OpenBabel::OBMol> mols;
      	std::vector<int> rings;
      	for (unsigned int i(0); i < allrings.size(); ++i)
      	{
         	if (allrings[i]->Size() < 12)
         	{
            	mols.push_back(oldMol);
            	rings.push_back(i);
         	}
      	}

      	std::vector<OpenBabel::OBMol> validMols;
      	for (unsigned int i(0); i < mols.size(); ++i)
      	{
         	mols[i] = RemoveRing(mols[i], allrings, rings[i]);
         	if (!mols[i].Empty())
         	{
            	validMols.push_back(mols[i]);
         	}
      	}
      	if (!validMols.empty())
      	{
         	return validMols[0];
      	}
   	}
   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_3(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}

	// Does the molecule have acyclic bonds?
	if (!CalculateAcyclicBonds(oldMol))
	{
		return oldMol;
	}
   
   	std::vector<OpenBabel::OBMol> mols;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols.push_back(oldMol);
   	}
   
   	std::vector<OpenBabel::OBMol> validMols;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, i);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
      	}
   	}
	if (validMols.empty())
	{
		return oldMol;
	}

 	// Choose the molecule with the smallest number of acyclic bonds
    std::vector<int> acyclicBonds;
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		acyclicBonds.push_back(CalculateAcyclicBonds(validMols[i]));
	}
	int minimum = acyclicBonds[0];
	for (unsigned int i(1); i < acyclicBonds.size(); ++i)
	{
		if (acyclicBonds[i] < minimum)
		{
			minimum = acyclicBonds[i];
		}
	}
	unsigned int oldMolecules = validMols.size();
	std::vector<OpenBabel::OBMol> remainingMols;
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		if (acyclicBonds[i] == minimum)
		{
			remainingMols.push_back(validMols[i]);
		}
	}
 	if (remainingMols.size() == 1)
	{
 		return remainingMols[0];
	}

   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_4(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}

	// Only focus on ringsystems with more than one ring
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	std::vector<OpenBabel::OBMol> mols;
   	OpenBabel::OBAtom* atom;
	std::vector<int> fusedRings;
	for (unsigned int i(0); i < allrings.size(); ++i)
	{
   		for (atom = oldMol.BeginAtom(avi); atom; atom = oldMol.NextAtom(avi))
		{
			if (allrings[i]->IsMember(atom) && (atom->MemberOfRingCount() > 1))
			{
				fusedRings.push_back(i);
		     	mols.push_back(oldMol);
				break;
			}
		}
	}
	if (fusedRings.empty())
	{
		return oldMol;
	}
  
   	std::vector<OpenBabel::OBMol> validMols;
   	for (unsigned int i(0); i < fusedRings.size(); ++i)
   	{
     	mols[i] = RemoveRing(mols[i], allrings, fusedRings[i]);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
      	}
   	}
    	if (validMols.empty())
   	{
		return oldMol;
	}
    
	int delta;
	int absdelta;
	std::vector<int> score;
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		delta = CalculateDelta(validMols[i]);
		absdelta = abs(delta);
		score.push_back(1000 * absdelta + delta);
	}
	int maximum = score[0];
	for (unsigned int i(1); i < validMols.size(); ++i)
	{
		if (score[i] > maximum)
		{
			maximum = score[i];
		}
	}
	unsigned int oldMolecules = validMols.size();
	std::vector<OpenBabel::OBMol> remainingMols;
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		if (score[i] == maximum)
		{
			remainingMols.push_back(validMols[i]);
		}
	}
	if (remainingMols.size() == 1)
	{
		return remainingMols[0];
    }
   
   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_6(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}
   
   	std::vector<OpenBabel::OBMol> mols;
   	std::vector<int> rings;
   	int size;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	size = allrings[i]->Size();
      	if ((size == 3) || (size == 5) || (size == 6))
      	{
         	mols.push_back(oldMol);
         	rings.push_back(i);
      	}
   	}
	if (mols.empty())
	{
		return oldMol;
	}

	// Only focus on ringsystems with more than one ring
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	OpenBabel::OBAtom* atom;
	std::vector<int> fusedRings;
	for (unsigned int i(0); i < rings.size(); ++i)
	{
   		for (atom = oldMol.BeginAtom(avi); atom; atom = oldMol.NextAtom(avi))
		{
			if (allrings[rings[i]]->IsMember(atom) && (atom->MemberOfRingCount() > 1))
			{
				fusedRings.push_back(rings[i]);
				break;
			}
		}
	} 
	if (fusedRings.empty())
	{
		return oldMol;
	}
  
   	std::vector<OpenBabel::OBMol> validMols;
   	for (unsigned int i(0); i < fusedRings.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, fusedRings[i]);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
      	}
   	}

 	if (validMols.size() == 1)
   	{
      	return validMols[0];
   	}
   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_7(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}

   	// Are all atoms and bonds aromatic?
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	OpenBabel::OBAtom* atom;
   	for (atom = oldMol.BeginAtom(avi); atom; atom = oldMol.NextAtom(avi))
   	{
      	if (!atom->IsAromatic())
      	{
         	return oldMol;
      	}
   	}
   	std::vector<OpenBabel::OBBond*>::iterator bvi;
   	OpenBabel::OBBond* bond;
   	for (bond = oldMol.BeginBond(bvi); bond; bond = oldMol.NextBond(bvi))
   	{
      	if (!bond->IsAromatic())
      	{
         	return oldMol;
      	}
   	}

   	std::vector<OpenBabel::OBMol> mols;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols.push_back(oldMol);
   	}
   
   	std::vector<OpenBabel::OBMol> validMols;
   	for (unsigned int i(0); i < mols.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, i);
      	if (!mols[i].Empty())
      	{
         	// Has aromaticity been broken?
         	bool broken(false);
         	for (atom = mols[i].BeginAtom(avi); atom; atom = mols[i].NextAtom(avi))
         	{
            	if (atom->IsInRing() && !atom->IsAromatic())
            	{
               		broken = true;
               		break;
            	}
         	}
         	if (!broken)
         	{
            	validMols.push_back(mols[i]);
         	}
      	}
   	}

	if (validMols.size() == 1)
   	{
      	return validMols[0];
   	}

   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_8(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}

   	std::vector<OpenBabel::OBMol> mols;
   	std::vector<int> score;
   	int sum;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	sum  = 0;
      	sum += +1000000 * CalculateHeteroAtoms(oldMol, allrings[i], 0);
      	sum += +10000   * CalculateHeteroAtoms(oldMol, allrings[i], 7);
      	sum += +100     * CalculateHeteroAtoms(oldMol, allrings[i], 8);
      	sum += +1       * CalculateHeteroAtoms(oldMol, allrings[i], 16);
      	score.push_back(sum);
      	mols.push_back(oldMol);
   	}
	if (mols.empty())
	{
		return oldMol;
	}
   
   	std::vector<OpenBabel::OBMol> validMols;
   	std::vector<int> validRings;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, i);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
         	validRings.push_back(i);
      	}
   	}

   	if (validRings.empty())
   	{
		return oldMol;
	}
    
	// Remove rings with smallest score first
	int minimum = score[validRings[0]];
	for (unsigned int i(1); i < validMols.size(); ++i)
	{
		if (score[validRings[i]] < minimum)
		{
			minimum = score[validRings[i]];
		}
	}
	unsigned int oldMolecules = validMols.size();
	std::vector<OpenBabel::OBMol> remainingMols;
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		if (score[validRings[i]] == minimum)
		{
			remainingMols.push_back(validMols[i]);
		}
	}
      
	if (remainingMols.size() == 1)
	{
		return remainingMols[0];
	}
    
   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_10(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}
   
   	std::vector<OpenBabel::OBMol> mols;
   	std::vector<int> ringsize;
   	std::vector<int> score;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols.push_back(oldMol);
      	ringsize.push_back(allrings[i]->Size());
   	}
	if (mols.empty())
	{
		return oldMol;
	}

   	std::vector<OpenBabel::OBMol> validMols;
   	std::vector<int> validRings;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, i);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
         	validRings.push_back(i);
      	}
   	}

   	if (validMols.empty())
   	{
		return oldMol;
	}
	
	// Smaller rings are removed before larger rings
	int minimum = ringsize[validRings[0]];
	for (unsigned int i(1); i < validMols.size(); ++i)
	{
		if (ringsize[validRings[i]] < minimum)
		{
			minimum = ringsize[validRings[i]];
		}
	}
	unsigned int oldMolecules = validMols.size();
	std::vector<OpenBabel::OBMol> remainingMols;
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		if (ringsize[validRings[i]] == minimum)
		{
			remainingMols.push_back(validMols[i]);
		}
	}
      
	if (remainingMols.size() == 1)
	{
		return remainingMols[0];
	}
   
   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_11(OpenBabel::OBMol& oldMol)
{
   	// Return if the molecule contains an acyclic linker
   	std::vector<OpenBabel::OBAtom*>::iterator avi;
   	OpenBabel::OBAtom* atom;
   	for (atom = oldMol.BeginAtom(avi); atom; atom = oldMol.NextAtom(avi))
   	{
      	if (!atom->IsInRing() && atom->GetValence() >= 2)
      	{
         	return oldMol;
      	}
   	}
   
   	// Make sure we are dealing with a mixed aromatic/nonaromatic system
   	bool notaromatic(false);
   	bool aromatic(false);
   	for (atom = oldMol.BeginAtom(avi); atom; atom = oldMol.NextAtom(avi))
   	{
      	if (atom->IsAromatic())
      	{
         	aromatic = true;
      	}
      	else
      	{
         	notaromatic = true;
      	}
   	}
   	if (aromatic && notaromatic)
   	{
      	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
     	if (allrings.size() <= _ringsToBeRetained)
      	{
         	return oldMol;
      	}
      
      	std::vector<OpenBabel::OBMol> mols;
      	std::vector<unsigned int> aromaticRings;
      	for (unsigned int i(0); i < allrings.size(); ++i)
      	{
         	if (allrings[i]->IsAromatic())
         	{
            	mols.push_back(oldMol);
            	aromaticRings.push_back(i);
         	}
      	}
   
      	std::vector<OpenBabel::OBMol> validMols;
      	for (unsigned int i(0); i < aromaticRings.size(); ++i)
      	{
         	mols[i] = RemoveRing(mols[i], allrings, aromaticRings[i]);
         	if (!mols[i].Empty())
         	{
            	validMols.push_back(mols[i]);
         	}
      	}

      	if (validMols.size() == 1)
      	{
         	return validMols[0];
      	}
   }

   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_12(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}
   	std::vector<OpenBabel::OBMol> mols;
   	std::vector<unsigned int> rings;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	if (HasLinkerToHeteroAtom(oldMol, allrings[i]))
      	{
         	mols.push_back(oldMol);
         	rings.push_back(i);
      	}
   	}

   	std::vector<OpenBabel::OBMol> validMols;
   	for (unsigned int i(0); i < mols.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, rings[i]);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
      	}
   	}

   	if (validMols.size() == 1)
   	{
      	return validMols[0];
   	}
  
   	return oldMol;
}



OpenBabel::OBMol
Schuffenhauer::Rule_13(OpenBabel::OBMol& oldMol)
{
   	std::vector<OpenBabel::OBRing*> allrings(oldMol.GetSSSR());
   	if (allrings.size() <= _ringsToBeRetained)
   	{
      	return oldMol;
   	}
   	std::vector<OpenBabel::OBMol> mols;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
         mols.push_back(oldMol);
   	}
   
   	std::vector<OpenBabel::OBMol> validMols;
   	for (unsigned int i(0); i < allrings.size(); ++i)
   	{
      	mols[i] = RemoveRing(mols[i], allrings, i);
      	if (!mols[i].Empty())
      	{
         	validMols.push_back(mols[i]);
      	}
   	}

	std::vector<OpenBabel::OBMol> remainingMols;
	unsigned int oldMolecules = validMols.size();
	if (validMols.empty())
	{
		return oldMol;
	}
	
	OpenBabel::OBConversion mol2can;
	mol2can.SetOutFormat("can");
	mol2can.AddOption("n", OpenBabel::OBConversion::OUTOPTIONS);
	std::string minimum = mol2can.WriteString(&(validMols[0]), true);
	std::string smi;
	for (unsigned int i(1); i < validMols.size(); ++i)
	{
		smi = mol2can.WriteString(&(validMols[i]), true);
		if (smi < minimum)
		{
			minimum = smi;
		}
	}
	for (unsigned int i(0); i < validMols.size(); ++i)
	{
		smi = mol2can.WriteString(&(validMols[i]), true);
		if (smi == minimum)
		{
			remainingMols.push_back(validMols[i]);
			break;
		}
	}

   	if (remainingMols.size() == 1)
   	{
      	return remainingMols[0];
   	}
  
   	return oldMol;
}



bool
Schuffenhauer::CalculateScaffold(const OpenBabel::OBMol& mol, Options* o)
{
	OpenBabel::OBMol oldMol(mol);
   	OpenBabel::OBMol newMol(mol);

	if (o->GetLog())
	{
   		std::cerr << "New molecule             :   ";
   		std::cerr <<  _mol2can.WriteString(&oldMol, true) << std::endl;
	}
	
	while (1)
   	{
		newMol = oldMol;
      	if (RemoveSidechains(&newMol)) 
		{
			if (o->GetLog())
         	{
            	std::cerr << "Removed sidechains       :   ";
            	std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
         	}
         	oldMol = newMol;
         	continue;
		}
      	oldMol = newMol;

		newMol = Rule_1(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_1 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_2(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_2 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_3(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_3 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_4(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_4 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
 			continue;
		}
		oldMol = newMol;

		newMol = Rule_6(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_6 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_7(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_7 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_8(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_8 :   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_10(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_10:   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
 		}
		oldMol = newMol;

		newMol = Rule_11(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_11:   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_12(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_12:   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		newMol = Rule_13(oldMol);
		if (newMol.NumAtoms() < oldMol.NumAtoms())
		{
			if (o->GetLog())
			{
				std::cerr << "Removed according rule_13:   ";
				std::cerr <<  _mol2can.WriteString(&newMol, true) << std::endl;
			}
			oldMol = newMol;
			continue;
		}
		oldMol = newMol;

		break;
	}

	if (!oldMol.Empty())
	{
		_smiles = _mol2can.WriteString(&oldMol, true);
	}
	else
	{
		_smiles = "-";
		return false;
	}
	return true;
}
