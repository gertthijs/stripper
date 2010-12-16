/**********************************************************************
calculator.cpp - Stripper
 
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



#include <calculator.h>



Calculator::Calculator(void)
{
}



Calculator::~Calculator(void)
{
}



Calculator::Calculator(const Calculator& f)
{
}



Calculator&
Calculator::operator=(const Calculator& f)
{
   	return *this;
}



Counter
Calculator::Calculate(Options* o, const std::vector<Scaffold*>& scaffolds)
{
   	Counter counter;
   	Scaffold* s;

   	// Print title line
   	if (o->OutFilename().empty())
   	{
      	std::cout << "NAME\tMOLECULE";
   	}
   	else
   	{
      	*(o->OutStream()) << "NAME\tMOLECULE";
   	}
   	for (unsigned int i(0); i < scaffolds.size(); ++i)
   	{
      	s = dynamic_cast<Scaffold*>(scaffolds[i]);
      	if (o->OutFilename().empty())
      	{
        	std::cout << "\t" << s->PrintName();
      	}
      	else
      	{
         	*(o->OutStream()) << "\t" << s->PrintName();
      	}
   	}
   	if (o->OutFilename().empty())
   	{
     	std::cout << std::endl;
   	}
   	else
   	{
      	*(o->OutStream()) << std::endl;
    }
      
   	// Process molecules
   	OpenBabel::OBMol mol;
   	std::string title;
   	OpenBabel::OBConversion mol2can;
   	mol2can.SetOutFormat("can");
   	mol2can.AddOption("n", OpenBabel::OBConversion::OUTOPTIONS);
		
		// first set the stream (needed in case of a zip stream)
		o->InputConverter()->SetInStream(o->InputStream());
   	while (o->InputConverter()->Read(&mol))
   	{
      	// Drop stereo information
      	mol.DeleteData(OpenBabel::OBGenericDataType::StereoData);

      	// Extract a suitable title from molecule
      	counter.NewMolecule();
      	title.clear();
      	title = mol.GetTitle();
      	if (title == "")
      	{
         	mol.SetTitle(mol2can.WriteString(&mol, true).c_str());
      	}
      	if (o->OutFilename().empty())
      	{
         	std::cout << mol.GetTitle();
      	}
      	else
      	{
         	*(o->OutStream()) << mol.GetTitle();
      	}
      	
		// Print smiles
      	if (o->OutFilename().empty())
      	{
         	std::cout << "\t" << mol2can.WriteString(&mol, true);
      	}
      	else
      	{
         	*(o->OutStream()) << "\t" << mol2can.WriteString(&mol, true);
      	}
      
      	// Extract scaffolds
      	for (unsigned int i(0); i < scaffolds.size(); ++i)
      	{
         	s = dynamic_cast<Scaffold*>(scaffolds[i]);
         	if (s->CalculateScaffold(mol, o))
         	{
            	counter.NewScaffold();
         	}
         	else
         	{
            	counter.FailedScaffold();
         	}
         	if (o->OutFilename().empty())
         	{
            	std::cout << "\t" << s->PrintScaffold();
         	}
         	else
         	{
            	*(o->OutStream()) << "\t" << s->PrintScaffold();
         	}
      	}
      	if (o->OutFilename().empty())
      	{
         	std::cout << std::endl;
      	}
      	else
      	{
         	*(o->OutStream()) << std::endl;
      	}
      	mol.Clear();
   	}

   	// Return
   	return counter;
}
