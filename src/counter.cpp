/**********************************************************************
counter.cpp - Stripper
 
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



#include <counter.h>



Counter::Counter(void):
_totalMol(0),
_scaffolds(0),
_noScaffolds(0)
{
}



Counter::~Counter(void)
{
}



Counter::Counter(const Counter& f):
_totalMol(f._totalMol),
_scaffolds(f._scaffolds),
_noScaffolds(_noScaffolds)
{
}



Counter&
Counter::operator=(const Counter& f)
{
   	_totalMol = f._totalMol;
   	_scaffolds = f._scaffolds;
   	_noScaffolds = f._noScaffolds;
   	return *this;
}



void
Counter::NewMolecule(void)
{
   	++_totalMol;
}



void
Counter::NewScaffold(void)
{
   	++_scaffolds;
}



void
Counter::FailedScaffold(void)
{
   	++_noScaffolds;
}



std::string
Counter::Results(void) const
{
   	std::ostringstream os;
   	os << "-> Molecules processed: " << _totalMol << std::endl;
   	os << "-> Scaffolds generated: " << _scaffolds << std::endl;
   	os << "-> Scaffolds failed:    " << _noScaffolds << std::endl;
   	return os.str();
}

