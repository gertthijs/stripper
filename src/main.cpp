/**********************************************************************
main.cpp - Stripper
 
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



#include <options.h>
#include <calculator.h>
#include <parseCommandLine.h>
#include <parseScaffoldDefinitions.h>
#include <counter.h>



int main (int argc, char* argv[])
{
   	// Parse command line
   	Options* uo = new Options();
   	parseCommandLine(argc, argv, uo);
   	version();
   	std::cerr << uo->Print();
   
   	// Initiate the scaffold definitions
   	std::vector<Scaffold*> scaffolds = parseScaffoldDefinitions(uo);

   	// Calculate
   	std::cerr << std::endl;
   	std::cerr << "## START OF CALCULATIONS" << std::endl;
   	Calculator calculator;
   	Counter counts = calculator.Calculate(uo, scaffolds);
   	std::cerr << counts.Results();
   	std::cerr << "## END OF CALCULATIONS" << std::endl;

   	// Return
   	delete uo;
   	uo = NULL;
   	for (unsigned int i(0); i < scaffolds.size(); ++i)
   	{
      	delete scaffolds[i];
      	scaffolds[i] = NULL;
   	}
   	return 0;
}
