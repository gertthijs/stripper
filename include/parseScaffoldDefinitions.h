/**********************************************************************
parseScaffoldDefinitions.h - Stripper
 
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



#ifndef __PARSESCAFFOLDDEFINITIONS_H__
#define __PARSESCAFFOLDDEFINITIONS_H__



#include <vector>

#include <options.h>

#include <scaffolds/scaffold.h>
#include <scaffolds/rings_with_linkers_1.h>
#include <scaffolds/rings_with_linkers_2.h>
#include <scaffolds/murcko_1.h>
#include <scaffolds/murcko_2.h>
#include <scaffolds/oprea.h>
#include <scaffolds/oprea_1.h>
#include <scaffolds/oprea_2.h>
#include <scaffolds/oprea_3.h>
#include <scaffolds/schuffenhauer.h>
#include <scaffolds/schuffenhauer_1.h>
#include <scaffolds/schuffenhauer_2.h>
#include <scaffolds/schuffenhauer_3.h>
#include <scaffolds/schuffenhauer_4.h>
#include <scaffolds/schuffenhauer_5.h>



std::vector<Scaffold*> parseScaffoldDefinitions(Options*);



#endif
