/**********************************************************************
schuffenhauer.h - Stripper
 
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



#ifndef __SCHUFFENHAUER_H__
#define __SCHUFFENHAUER_H__



#include <string>
#include <vector>
#include <list>

#include <scaffolds/scaffold.h>

#include <openbabel/mol.h>
#include <openbabel/atom.h>
#include <openbabel/bond.h>
#include <openbabel/ring.h>
#include <openbabel/obconversion.h>



class Schuffenhauer: virtual public Scaffold
{
   	protected:
   
      	unsigned int _ringsToBeRetained;
      	virtual unsigned int RemoveSidechains(OpenBabel::OBMol*);
      	virtual OpenBabel::OBMol RemoveRing(OpenBabel::OBMol&, std::vector<OpenBabel::OBRing*>&, unsigned int);
      	virtual unsigned int CalculateAcyclicBonds(OpenBabel::OBMol&);
      	virtual int CalculateDelta(OpenBabel::OBMol&);
      	virtual unsigned int CalculateHeteroAtoms(OpenBabel::OBMol&, OpenBabel::OBRing*, int);
      	virtual bool HasLinkerToHeteroAtom(OpenBabel::OBMol&, OpenBabel::OBRing*);
      
      	virtual OpenBabel::OBMol Rule_1(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_2(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_3(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_4(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_6(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_7(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_8(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_10(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_11(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_12(OpenBabel::OBMol&);
      	virtual OpenBabel::OBMol Rule_13(OpenBabel::OBMol&);
   
   	public:
   
      	Schuffenhauer(void);
      	virtual ~Schuffenhauer(void);
      	Schuffenhauer(const Schuffenhauer&);
      	Schuffenhauer& operator=(const Schuffenhauer&);
   
      	virtual bool CalculateScaffold(const OpenBabel::OBMol&, Options*);
};



#endif
