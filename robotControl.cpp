// Joseph Dillman, 301317623, CMPT 128 D200, Assignment 1 code 
//This code is designed to read in a data file of coin information including weight, diameter, bent/unsusable, and match/unmatch
//Coins that match the description of nickel, dime, quarter, loonie, and toonie will be recorded and sorted into a container 
//If the container contents exceed a specific amount, the container will be switched 
//Bent coins will fall into a designated container as will any coins that are not canadian or do not properly match the images of the canadian coins
//The bent coin and other coin container will also switch if the mass of the container exceeds a specific amount
//Finally, the program will record all coins counted, canadian and non canadian, and the mass of bent and other coins 

//All outputs will be printed to screen and output file
//Last edited: Nov 16, 2016
#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
using namespace std;

int main ()
{
	//declare and initialize all variables, file streams, character types, data types
	ifstream infile;
	ofstream outfile;

	int count =0;
	int next =0;
	int lines =0;
	int x=0;
	int otherbentcount =0;
	int othercount =0;
	int containersother =0;
	int currentothercount =0;

	int nickels =0;
	int currentnickels =0;
	int rollsn =0;

	int dimes =0;
	int currentdimes =0;
	int rollsd =0;

	int quarters =0;
	int currentquarters =0;
	int rollsq =0;

	int loonies =0;
	int currentloonies =0;
	int rollsl =0;

	int twonies =0;
	int currenttwonies =0;
	int rollst =0;

	double weight =0.0;
	double grams =0.0;
	double diameter =0.0;
	double mm=0.0;
	double otherbentweight =0.0;
	double currentotherbentweight =0.0;
	double otherweight =0.0;
	double currentotherweight =0.0;
	
	string blank;
	string usableorbent;
	string match;
	string bent = "bent";
	string usable = "usable";
	string twomatch = "BothMatch";
	string onematch = "OneMatch";
	string nomatch = "NoMatch";

	char c = ' ';
	char infilename[1024] = {'\0'};
	char outfilename[1024] = {'\0'};

	//Make constats for all numbers and for weight and error values for nickel, dime, quarter, loonie, twonie. Repeat for diameter
	const double MINLINES = 1.0;
	const double MAXLINES = 5000.0;
	const double MINDATA = 0.0;
	const double MAXDATA = 255.0;
	const double MAXOTHERWEIGHT = 200.0;
	const double MAXBENTWEIGHT = 100.0;
	
	const double NW = 3.95;
	const double NWE = 0.35;
	const double ND = 21.0;
	const double NDE = 0.8;

	const double DW = 1.75;
	const double DWE = 0.45;
	const double DD = 18.0;
	const double DDE = 0.7;

	const double QW = 4.40;
	const double QWE = 0.4;
	const double QD = 23.7;
	const double QDE = 0.9;

	const double LW = 7.00;
	const double LWE = 0.5;
	const double LD = 26.00;
	const double LDE = 1.0;

	const double TW = 7.3;
	const double TWE = 0.55;
	const double TD = 28.0;
	const double TDE = 1.1;

	const double MAXNROLL = 40.0;
	const double MAXDROLL = 50.0;
	const double MAXQROLL = 40.0;
	const double MAXLROLL = 25.0;
	const double MAXTROLL = 25.0;

	cout << fixed << setprecision(2);
	outfile << fixed << setprecision(2);
	//prompt user to enter the name of the input file, allow 3 tries, terminate program after 3 failures
	do 
	{
		if(count >= 3)
		{
			cerr << "ERROR: You exceeded maximum number of tries allowed" << endl;
			cerr << "while entering the input file name" << endl;
			return(1);
		}
		cout << "Type the name of the input file containing sensor readings:" << endl;
		cin >> infilename;
		infile.open(infilename);
		if(infile.fail())
		{
			cerr << "ERROR: File "<< infilename <<" could not be opened for input" << endl;
		}
		count++;
	}while (infile.fail());

	//Prompt user to enter the output file, allow 3 tries, terminate program after 3 failures 
	count=0;
	do 
	{
		if(count >= 3)
		{
			cerr << "ERROR: You exceeded maximum number of tries allowed" << endl;
			cerr << "while entering the output file name" << endl;
			return(2);
		}
		cout << "Type the name of the output file which will hold the simulation results:" << endl;
		cin >> outfilename;
		outfile.open(outfilename);
		if(outfile.fail())
		{
			cerr << "ERROR: File "<< outfilename <<" could not be opened" << endl;
		}
		count++;
	}while (outfile.fail());
	
	//ensure that the input file is not empty, or that the first value in the file is an integer
	//Then check if the integer is within the correct boundaries 
	if(!(infile >> lines))
	{
		if(infile.eof())
		{
			cerr << "ERROR: Input data file is empty" << endl;
			return (3);
		}
		else
		{
			cerr << "ERROR: First piece of data in the file is not an integer" << endl;
			return (4);
		}
	}
	if( lines < MINLINES || lines > MAXLINES)
	{
		cerr << "ERROR: The number of sensor readings is out of range" << endl;
		return (5);
	}
	cout << setprecision(2);
	outfile << setprecision(2);
	//begin analysis of input file, repeat analysis for number of lines as read from the first value
	for(x=0;x<lines;x++)
	{
		//if the weight cannot be read, check for end of file or non integer value, leave line for either situation
		if(!(infile >> weight))
		{
			if(infile.eof())
			{
				cerr << "ERROR: No more data" << endl;
				cerr << "Simulation completed early before line " << x+1 << " of input" << endl;
				break;
			}
			else
			{
				cerr << "ERROR: Weight sensor value read on line "<< x+1 <<" is not an integer" << endl;
				cerr << "Simulation terminated early: Please correct your data file" << endl;
				break;
			}
		}
		//if there is no diameter value, ignore line and omit data
		c=infile.peek();
		while(c== ' ' || c== '	')
		{
			c=infile.get();
			c=infile.peek();
		}
		if(c== '\n')
		{
			cerr << "ERROR: Weight sensor measurement only" << endl;
			cerr << "Ignoring line " << x+1<< " of the input file" << endl;
			continue;
		}
		//if diameter value could not be read, print error
		if(!(infile >> diameter))
		{
			cerr << "ERROR: Diameter sensor value read on line "<< x+1 << " is not an integer" << endl;
			cerr << "Simulation terminated early: Please correct your data file" << endl;
			break;
		}
		//if there is no value for usable/bent, ignore line and omit data
		c=infile.peek();
		while(c== ' ' || c== '	')
		{
			c=infile.get();
			c=infile.peek();
		}
		if(c== '\n')
		{
			cerr << "ERROR: Weight and diameter sensor measurements only" << endl;
			cerr << "Ignoring line " << x+1<< " of the input file" << endl;
			continue;
		}
		//if the next input is not a string of either "bent" or "usable", print error and omit data
		infile >> usableorbent;
		if(usableorbent != "usable" && usableorbent != "bent")
		{
			cerr << "ERROR: Result of test to determine if coin is bent at line "<< x+1 << " is invalid" << endl;
			cerr << "Ignoring this line of data" << endl;
			infile.ignore(1000, '\n');
			continue;
		}
		//if there is no value for matching coin faces, ignore line and omit data
		c=infile.peek();
		while(c== ' ' || c== '	')
		{
			c=infile.get();
			c=infile.peek();
		}
		if(c== '\n')
		{
			cerr << "ERROR: Weight and diameter sensor measurements and bent string only" << endl;
			cerr << "Ignoring line " << x+1<< " of the input file" << endl;
			continue;
		}
		//if the next input is not a string of either "BothMatch", "OneMatch or "NoMatch", print error and omit data
		infile >> match;
		if(match != "BothMatch" && match != "OneMatch" && match != "NoMatch")
		{
			cerr << "ERROR: image processing result at line "<< x+1 << " is invalid" << endl;
			cerr << "Ignoring this line of data" << endl;
			continue;
		}
		//if there is extra data (end of file), print an error 
		c=infile.peek();
		while(c== ' ' || c== '	')
		{
			c=infile.get();
			c=infile.peek();
		}
		if( c != '\n')
		{
			cerr << "ERROR: Extra data at line " << x+1 << ".  Ignoring extra data" << endl;
			infile.ignore(1000, '\n');
		}
		//Calculate the grams and cm of diameter values
		grams = 2*weight*5/MAXDATA;
		mm = 10 + diameter/8.5;
		//If the coin is bent, send it to the bent container, print message, record weight
		//If the weight exceeds 100g, print message to replace container
		if(usableorbent == "bent")
		{
			otherbentcount += 1;
			currentothercount += 1;
			otherbentweight += grams;
			currentotherbentweight += grams;
			if(currentotherbentweight > MAXBENTWEIGHT)
			{
				cout << "The Coin Sorter has sent this coin to the bent coin container" << endl;
				cout << "This coin does not fit in the bent coin container" << endl;
				cout << "The bent coin container has been replaced" << endl;
				cout << "The coin in the new bent coin container weighs " << grams <<" grams" << endl;
				outfile << "The Coin Sorter has sent this coin to the bent coin container" << endl;
				outfile << "This coin does not fit in the bent coin container" << endl;
				outfile << "The bent coin container has been replaced" << endl;
				outfile << "The coin in the new bent coin container weighs " << grams <<" grams" << endl;
				currentotherbentweight = grams;
				currentothercount =1;
			}
			else
			{
				cout << "The Coin Sorter has sent this coin to the bent coin container" << endl;
				cout << "The coins in the bent coin container now weigh  " << setw(5) << currentotherbentweight << " grams" << endl;
				outfile << "The Coin Sorter has sent this coin to the bent coin container" << endl;
				outfile << "The coins in the bent coin container now weigh  " << setw(5) << currentotherbentweight << " grams" << endl;
			}
			continue;
		}
		//If the coin does not have matching images, send to the other container, print message, record weight
		//If the weight exceeds 200g, print message to replace container 
		else if(match == "NoMatch" || match == "OneMatch")
		{
			othercount += 1;
			otherweight += grams;
			currentotherweight += grams;
			if(currentotherweight > MAXOTHERWEIGHT)
			{
				currentotherweight = grams;
				cout << "The Coin Sorter has sent this coin to the other coin container" << endl;
				cout << "The coins in the other coin container now weigh "<< setw(6) << currentotherweight << " grams" << endl;
				outfile << "The Coin Sorter has sent this coin to the other coin container" << endl;
				outfile << "The coins in the other coin container now weigh "<< setw(6) << currentotherweight << " grams" << endl;
				containersother += 1;
			}
			else
			{
				cout << "The Coin Sorter has sent this coin to the other coin container" << endl;
				cout << "The coins in the other coin container now weigh "<< setw(6) << currentotherweight << " grams" << endl;
				outfile << "The Coin Sorter has sent this coin to the other coin container" << endl;
				outfile << "The coins in the other coin container now weigh "<< setw(6) << currentotherweight << " grams" << endl;
			}
		
			continue;
		}
		//if any of the weight or diameter data points were outside the sensor reading range, print error and omit data
		if(weight <= MINDATA || weight > MAXDATA || diameter <= MINDATA || diameter > MAXDATA)
		{
			cerr << "ERROR: Sensor reading out of range, ignoring line " << x+1 << " in the input file" << endl;
			continue;
		}
		//If the coin is a nickel, print message and send to nickel, if the number of nickels exceeds 40, switch container 
		else if((abs(grams - NW) <= NWE) && (abs(mm - ND) <= NDE))
		{
			nickels += 1;
			currentnickels += 1;
			if(currentnickels >= MAXNROLL)
			{
				cout << "The Coin Sorter has sent one coin to the nickel wrapper" << endl;
				cout << "The nickel wrapper is now full" << endl;
				cout << "The nickel wrapper has now been replaced" << endl;
				outfile << "The Coin Sorter has sent one coin to the nickel wrapper" << endl;
				outfile << "The nickel wrapper is now full" << endl;
				outfile << "The nickel wrapper has now been replaced" << endl;
				currentnickels = 0;
				rollsn += 1;
			}
			else
			{
			cout << "The Coin Sorter has sent one coin to the nickels wrapper" << endl;
			cout << "There are now "<< currentnickels << " coins in the nickels wrapper" << endl;
			outfile << "The Coin Sorter has sent one coin to the nickels wrapper" << endl;
			outfile << "There are now "<< currentnickels << " coins in the nickels wrapper" << endl;
			}
			continue;
		}
		//check if dime, if the number of dimes exceeds 50, switch container
		else if((abs(grams - DW) <= DWE) && (abs(mm - DD) <= DDE))
		{
			dimes += 1;
			currentdimes += 1;
			if(currentdimes >= MAXDROLL)
			{
				cout << "The Coin Sorter has sent one coin to the dimes wrapper" << endl;
				cout << "The dime wrapper is now full" << endl;
				cout << "The dime wrapper has now been replaced" << endl;
				outfile << "The Coin Sorter has sent one coin to the dimes wrapper" << endl;
				outfile << "The dime wrapper is now full" << endl;
				outfile << "The dime wrapper has now been replaced" << endl;
				currentdimes = 0;
				rollsd += 1;
			}
			else
			{
			cout << "The Coin Sorter has sent one coin to the dimes wrapper" << endl;
			cout << "There are now "<< currentdimes << " coins in the dimes wrapper" << endl;
			outfile << "The Coin Sorter has sent one coin to the dimes wrapper" << endl;
			outfile << "There are now "<< currentdimes << " coins in the dimes wrapper" << endl;
			}
			continue;
		}
		//check if quarter, , if the number of quarters exceeds 40, switch container
		else if((abs(grams - QW) <= QWE) && (abs(mm - QD) <= QDE))
		{
			quarters += 1;
			currentquarters += 1;
			if(currentquarters >= MAXQROLL)
			{
				cout << "The Coin Sorter has sent one coin to the quarters wrapper" << endl;
				cout << "The quarter wrapper is now full" << endl;
				cout << "The quarter wrapper has now been replaced" << endl;
				outfile << "The Coin Sorter has sent one coin to the quarters wrapper" << endl;
				outfile << "The quarter wrapper is now full" << endl;
				outfile << "The quarter wrapper has now been replaced" << endl;
				currentquarters = 0;
				rollsq += 1;
			}
			else
			{
			cout << "The Coin Sorter has sent one coin to the quarters wrapper" << endl;
			cout << "There are now "<< currentquarters << " coins in the quarters wrapper" << endl;
			outfile << "The Coin Sorter has sent one coin to the quarters wrapper" << endl;
			outfile << "There are now "<< currentquarters << " coins in the quarters wrapper" << endl;
			}
			continue;
		}
		//check if loonie, if the number of loonies exceeds 25, switch container
		else if((abs(grams - LW) <= LWE) && (abs(mm - LD) <= LDE))
		{
			loonies += 1;
			currentloonies += 1;
			if(currentloonies >= MAXLROLL)
			{
				cout << "The Coin Sorter has sent one coin to the loonies wrapper" << endl;
				cout << "The loonie wrapper is now full" << endl;
				cout << "The loonie wrapper has now been replaced" << endl;
				outfile << "The Coin Sorter has sent one coin to the loonies wrapper" << endl;
				outfile << "The loonie wrapper is now full" << endl;
				outfile << "The loonie wrapper has now been replaced" << endl;
				currentloonies = 0;
				rollsl += 1;
			}
			else
			{
			cout << "The Coin Sorter has sent one coin to the loonies wrapper" << endl;
			cout << "There are now "<< currentloonies << " coins in the loonies wrapper" << endl;
			outfile << "The Coin Sorter has sent one coin to the loonies wrapper" << endl;
			outfile << "There are now "<< currentloonies << " coins in the loonies wrapper" << endl;
			}
			continue;
		}
		//check if twonie, if the number of toonies exceeds 25, switch container
		else if((abs(grams - TW) <= TWE) && (abs(mm - TD) <= TDE))
		{
			twonies += 1;
			currenttwonies += 1;
			if(currenttwonies >= MAXTROLL)
			{
				cout << "The Coin Sorter has sent one coin to the toonies wrapper" << endl;
				cout << "The toonie wrapper is now full" << endl;
				cout << "The toonie wrapper has now been replaced" << endl;
				outfile << "The Coin Sorter has sent one coin to the toonies wrapper" << endl;
				outfile << "The toonie wrapper is now full" << endl;
				outfile << "The Toonie wrapper has now been replaced" << endl;
				currenttwonies = 0;
				rollst += 1;
			}
			else
			{
			cout << "The Coin Sorter has sent one coin to the toonies wrapper" << endl;
			cout << "There are now "<< currenttwonies << " coins in the toonies wrapper" << endl;
			outfile << "The Coin Sorter has sent one coin to the toonies wrapper" << endl;
			outfile << "There are now "<< currenttwonies << " coins in the toonies wrapper" << endl;
			}
			continue;
		}
		//anything remaining coins are not canadian coins and thus go to the other container, replace container if mass exceeds 200g
		else 
		{
			othercount += 1;
			currentothercount += 1;
			otherweight += grams;
			currentotherweight += grams;
			if(currentotherweight > MAXOTHERWEIGHT)
			{
				currentotherweight = grams;
				cout << "The Coin Sorter has sent this coin to the other coin container" << endl;
				cout << "The coins in the other coin container now weigh "<< setw(6) << currentotherweight << " grams" << endl;
				outfile << "The Coin Sorter has sent this coin to the other coin container" << endl;
				outfile << "The coins in the other coin container now weigh " << setw(6) << currentotherweight << " grams" << endl;
				containersother += 1;
				currentothercount = 1;
			}
			else
			{
				cout << "The Coin Sorter has sent this coin to the other coin container" << endl;
				cout << "The coins in the other coin container now weigh " << setw(6) << currentotherweight << " grams" << endl;
				outfile << "The Coin Sorter has sent this coin to the other coin container" << endl;
				outfile << "The coins in the other coin container now weigh " << setw(6) << currentotherweight << " grams" << endl;
			}
			continue;
		}
	}
	//Print a summary of all data 
	cout << endl << endl << endl << fixed << setprecision(0);
	cout << "SUMMARY" << endl;
	cout << "The Coin Sorter made   "<< rollsn <<" rolls of nickels." << endl;
	cout << "    There are  " << currentnickels <<" nickels in the partially full roll." << endl;
	cout << "The Coin Sorter made   " << rollsd << " rolls of dimes." << endl;
    cout << "    There are  " << currentdimes <<" dimes in the partially full roll." << endl;
	cout << "The Coin Sorter made   " << rollsq <<" rolls of quarters." << endl;
    cout << "    There are  " << currentquarters << " quarters in the partially full roll." << endl;
	cout << "The Coin Sorter made   " << rollsl << " rolls of loonies." << endl;
    cout << "    There are  " << currentloonies <<" loonies in the partially full roll." << endl;
	cout << "The Coin Sorter made   "<< rollst <<" rolls of toonies." << endl;
    cout << "    There are  "<< currenttwonies <<" toonies in the partially full roll." << endl;
	cout << "The Coin Sorter processed    " << othercount <<" other coins." << endl;
    cout << "    The other coins completely filled   "<< containersother <<" containers" << endl;
    cout << "    There were  " << currentothercount <<" other coins in the partially full container" << endl;
	cout << setprecision(3);
    cout << "    The total weight of the other coins was   "<< otherweight<<" grams" << endl;
	cout << setprecision(4);
	cout << "The Coin Sorter processed     " << otherbentweight << " g of bent coins";
	outfile << endl << endl << endl << fixed << setprecision(0);
	outfile << "SUMMARY" << endl;
	outfile << "The Coin Sorter made   "<< rollsn <<" rolls of nickels." << endl;
	outfile << "    There are  " << currentnickels <<" nickels in the partially full roll." << endl;
	outfile << "The Coin Sorter made   " << rollsd << " rolls of dimes." << endl;
    outfile << "    There are  " << currentdimes <<" dimes in the partially full roll." << endl;
	outfile << "The Coin Sorter made   " << rollsq <<" rolls of quarters." << endl;
    outfile << "    There are  " << currentquarters << " quarters in the partially full roll." << endl;
	outfile << "The Coin Sorter made   " << rollsl << " rolls of loonies." << endl;
    outfile << "    There are  " << currentloonies <<" loonies in the partially full roll." << endl;
	outfile << "The Coin Sorter made   "<< rollst <<" rolls of toonies." << endl;
    outfile << "    There are  "<< currenttwonies <<" toonies in the partially full roll." << endl;
	outfile << "The Coin Sorter processed    " << othercount <<" other coins." << endl;
    outfile << "    The other coins completely filled   "<< containersother <<" containers" << endl;
    outfile << "    There were  " << currentothercount <<" other coins in the partially full container" << endl;
	outfile << setprecision(3);
    outfile << "    The total weight of the other coins was   "<< otherweight<<" grams" << endl;
	outfile << setprecision(4);
	outfile << "The Coin Sorter processed     " << otherbentweight << " g of bent coins";

	infile.close ();
	outfile.close();
	return(0);
}