/**********************************************************************
parseScaffoldDefinitions.cpp - Stripper
 
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



#include <parseScaffoldDefinitions.h>



std::vector<Scaffold*>
parseScaffoldDefinitions(Options* o)
{
   	std::vector<Scaffold*> definitions;

   	// If no --scaffolds command line option is provided, then all
   	// scaffolds are calculated
   	if (o->ScaffoldFilename() == "")
   	{
      	definitions.push_back(new Rings_With_Linkers_1());
      	definitions.push_back(new Rings_With_Linkers_2());
      	definitions.push_back(new Murcko_1());
      	definitions.push_back(new Murcko_2());
      	definitions.push_back(new Oprea_1());
      	definitions.push_back(new Oprea_2());
      	definitions.push_back(new Oprea_3());
      	definitions.push_back(new Schuffenhauer_1());
      	definitions.push_back(new Schuffenhauer_2());
      	definitions.push_back(new Schuffenhauer_3());
      	definitions.push_back(new Schuffenhauer_4());
      	definitions.push_back(new Schuffenhauer_5());
      	return definitions;
   	}

   	std::cerr << std::endl;
   	std::cerr << "## START PARSING SCAFFOLD DEFINITION FILE \"";
   	std::cerr << o->ScaffoldFilename() << "\"" << std::endl;

   	// Variables
   	unsigned int linecount(0);
   	std::string parameter;
   	std::string line;
   	const std::string whiteSpace("\n\t \r");
   
   	// Check correctness of keywords
   	while (o->ScaffoldStream()->good())
   	{
      	(void) getline(*(o->ScaffoldStream()), line);
      	++linecount;
      
      	// Reset
      	line.erase(0, line.find_first_not_of(whiteSpace, 0));
      	parameter.clear();
      
      	// Get the parameter out of the string
      	parameter = line.substr(0, line.find_first_of(whiteSpace, 0));
      	for_each(parameter.begin(), parameter.end(), toupper);
      
      	// Handle according value of parameter
      	if ((parameter.empty()) || (parameter.substr(0,1) == "#") || (parameter.substr(0,2) == "//"))
      	{
        	continue;
      	}
      
      	// Add valid scaffold definitions
      	if (parameter == "RINGS_WITH_LINKERS_1")
      	{
         	definitions.push_back(new Rings_With_Linkers_1());
 			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
        	continue;
      	}
      	else
		if (parameter == "RINGS_WITH_LINKERS_2")
      	{
         	definitions.push_back(new Rings_With_Linkers_2());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "MURCKO_1")
      	{
         	definitions.push_back(new Murcko_1());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "MURCKO_2")
      	{
         	definitions.push_back(new Murcko_2());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "OPREA_1")
      	{
         	definitions.push_back(new Oprea_1());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "OPREA_2")
      	{
         	definitions.push_back(new Oprea_2());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "OPREA_3")
      	{
         	definitions.push_back(new Oprea_3());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "SCHUFFENHAUER_1")
      	{
         	definitions.push_back(new Schuffenhauer_1());
 			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
        	continue;
      	}
      	else
		if (parameter == "SCHUFFENHAUER_2")
      	{
         	definitions.push_back(new Schuffenhauer_2());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "SCHUFFENHAUER_3")
      	{
         	definitions.push_back(new Schuffenhauer_3());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "SCHUFFENHAUER_4")
      	{
        	definitions.push_back(new Schuffenhauer_4());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
		if (parameter == "SCHUFFENHAUER_5")
      	{
         	definitions.push_back(new Schuffenhauer_5());
			std::cerr << "-> ";
			std::cerr << (definitions[definitions.size() - 1])->PrintName() << std::endl;
         	continue;
      	}
      	else
      	// MISMATCH
      	{
         	std::cerr << "ERROR: Unrecognized keyword when parsing scaffold file: ";
         	std::cerr << parameter << std::endl;
         	exit(1);
      	}
   	}

   	std::cerr << "## END PARSING SCAFFOLD DEFINITION FILE" << std::endl;
   	return definitions;
}
