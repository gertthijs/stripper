/**********************************************************************
parseCommandLine.cpp - Stripper
 
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



#include <parseCommandLine.h>



void
parseCommandLine(int argc, char* argv[], Options* o)
{
  	static struct option Arguments[] =
	{
      	{ "in",        required_argument,   NULL, '1' },
		{ "out",       required_argument,   NULL, '2' },
		{ "scaffolds", required_argument,   NULL, '3' },
		{ "noLog",     no_argument,         NULL, '4' },
		{ "help",      no_argument,         NULL, 'h' },
		{ "version",   no_argument,         NULL, 'v' },
      	{ NULL,        0,                   NULL,  0  }
	};

   	// Process command line
   	int choice;
   	std::string ff;
	opterr = 0;
	int optionIndex = 0;
   	OpenBabel::OBFormat* format;
	while ((choice = getopt_long(argc, argv, "vh", Arguments, &optionIndex )) != -1)
	{
      	switch(choice)
		{
         	case 'v': 
            	version();
            	exit(0);
            	break;

         	case 'h':
            	usage();
            	exit(0);
            	break;

         	case '1':
            	(void) o->InputFilename(optarg);
            	// Check if filename is provided
            	if ((o->InputFilename()).empty())
            	{
               		std::cerr << "ERROR: No filename provided with the '--in' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
           		}
            	// Check file format
            	format = o->InputConverter()->FormatFromExt(optarg);
            	if (!format)
            	{
               		std::cerr << "ERROR: Could not detect format from the '--in' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
            	}
            	o->InputConverter()->SetInFormat(format);
            	// Check validity of file
            	o->InputStream()->open(optarg);
            	if (!o->InputStream()->good())
            	{
               		std::cerr << "ERROR: Cannot open the file as specified by the '--in' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
            	}
            	o->InputConverter()->AddOption("d", OpenBabel::OBConversion::GENOPTIONS);
            	break;
            
         	case '2':
            	(void) o->OutFilename(optarg);
            	// Check if filename is provided
            	if ((o->OutFilename()).empty())
            	{
              		std::cerr << "ERROR: No filename provided with the '--out' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
            	}
            	// Check validity of file
            	o->OutStream()->open(optarg);
            	if (!o->OutStream()->good())
            	{
               		std::cerr << "ERROR: Cannot open the file as specified by the '--out' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
            	}
            	break;

         	case '3':
            	(void) o->ScaffoldFilename(optarg);
            	// Check if filename is provided
            	if ((o->ScaffoldFilename()).empty())
            	{
               		std::cerr << "ERROR: No filename provided with the '--scaffolds' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
            	}
            	// Check validity of file
            	o->ScaffoldStream()->open(optarg);
            	if (!o->ScaffoldStream()->good())
            	{
               		std::cerr << "ERROR: Cannot open the file as specified by the '--scaffolds' option." << std::endl;
               		std::cerr << "       Please use -h or --help for more help." << std::endl;
               		exit(1);
            	}
            	break;

         	case '4':
            	(void) o->SetLog(false);
            	break;
            
			default:
            	std::cerr << "ERROR: Unknown command line option." << std::endl;
            	std::cerr << "       Please use -h or --help for more help." << std::endl;
            	exit(1);
		}
	}

	argc -= optind;
	argv += optind;

   	// Input file processing
   	if (o->InputFilename().empty())
   	{
      	std::cerr << "ERROR: Command line option '--in' is missing." << std::endl;
      	std::cerr << "       Please use -h or --help for more help." << std::endl;
      	exit(1);
   	}
}



void
usage(void) 
{
   	std::cerr << std::endl;
   	std::cerr << "TASK: " << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  STRIPPER is a tool to extract predefined scaffolds from input molecules." << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "REQUIRED OPTIONS:" << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  --in <file>" << std::endl;
   	std::cerr << "    Specifies the file containing the input molecules. The format of the" << std::endl;
   	std::cerr << "    file is specified by the file extension. Gzipped files are also read." << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "OPTIONAL OPTIONS:" << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  --out <file>" << std::endl;
   	std::cerr << "    Specifies the file to which the extracted scaffolds are written." << std::endl;
   	std::cerr << "    The file is a text file with on each row the original molecule" << std::endl;
   	std::cerr << "    and the generated scaffolds in smiles format. If not provided," << std::endl;
   	std::cerr << "    then the output is written to stdout." << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  --scaffolds <file>" << std::endl;
   	std::cerr << "    Specifies the file in which the required scaffolds have been defined." << std::endl;
   	std::cerr << "    If not provided then all scaffolds are calculated." << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  --noLog" << std::endl;
   	std::cerr << "    Suppresses the output of additional information to standard error." << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  -h  --help" << std::endl;
   	std::cerr << std::endl;
   	std::cerr << "  -v  --version" << std::endl;
   	std::cerr << std::endl;
}



void
version(void)
{
	std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cerr << "  STRIPPER v" << STRIPPER_VERSION;
   	std::cerr << "." << STRIPPER_RELEASE;
   	std::cerr << "." << STRIPPER_SUBRELEASE;
   	std::cerr << " | " << __DATE__;
   	std::cerr << " " << __TIME__ << std::endl;
	std::cerr << std::endl;
	std::cerr << "  -> GCC:         " << __VERSION__ << std::endl;
	std::cerr << "  -> Open Babel:  " << BABEL_VERSION << std::endl;
	std::cerr << std::endl;
	std::cerr << "  Copyright (C) 2005-2010 by Silicos NV" << std::endl;
	std::cerr << std::endl;
	std::cerr << "  This file is part of the Open Babel project." << std::endl;
	std::cerr << "  For more information, see <http://openbabel.sourceforge.net/>" << std::endl;
	std::cerr << std::endl;
	std::cerr << "  This program is free software; you can redistribute it and/or modify" << std::endl;
	std::cerr << "  it under the terms of the GNU General Public License as published by" << std::endl;
	std::cerr << "  the Free Software Foundation version 2 of the License." << std::endl;
	std::cerr << std::endl;
	std::cerr << "  This program is distributed in the hope that it will be useful," << std::endl;
	std::cerr << "  but WITHOUT ANY WARRANTY; without even the implied warranty of" << std::endl;
	std::cerr << "  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" << std::endl;
	std::cerr << "  GNU General Public License for more details." << std::endl;
	std::cerr << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
	std::cerr << std::endl;
}
