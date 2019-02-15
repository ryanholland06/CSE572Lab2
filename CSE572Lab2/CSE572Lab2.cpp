#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool isTerminal(string symbol, vector <string> terminals)
{
	bool foundTerminal = false;
	for (int i = 0; i < terminals.size(); i++)
	{
		if (symbol == terminals[i] || symbol == "e")
		{
			foundTerminal = true;
			break;
		}
		else
			foundTerminal = false;
	}
	if (foundTerminal == true)
		return true;
	else
		return false;
}
string findDuplicates(vector <string> production, vector <string> terminals, int counter)
{
	bool terminalFound = false;
	string symbol = production[counter].substr(3, 1);
	string tempsymbol = symbol;
	while (terminalFound == false)
	{
		if (terminalFound == false)
			terminalFound = isTerminal(symbol, terminals);
		for (counter = counter + 1; counter < production.size(); counter++)
		{
			string searchedSymbol = production[counter].substr(0, 1);
			if (symbol == searchedSymbol)
			{
				symbol = production[counter].substr(3, 1);
				break;
			}
		}
		terminalFound = isTerminal(symbol, terminals);
		if (terminalFound == false)
			findDuplicates(production, terminals, counter);

	}

	string temp = "";//tempsymbol;
	string firstset = symbol;
	for (counter = counter + 1; counter < production.size(); counter++)
	{
		if (production[counter].substr(0, 1) == tempsymbol)
		{
			temp = "" + findDuplicates(production, terminals, counter);
		}
		firstset += temp;
	}
	return firstset;

}
string findSymbol(vector<string> production, vector <string> terminals, int counter, string & tempSymbol)
{

	bool terminalFound = false;
	bool checkduplicates = false;
	string symbol = production[counter].substr(3, 1);
	tempSymbol = symbol;
	while (terminalFound == false)
	{
		if (terminalFound == false)
			terminalFound = isTerminal(symbol, terminals);
		for (counter = counter + 1; counter < production.size(); counter++)
		{
			string searchedSymbol = production[counter].substr(0, 1);
			if (symbol == searchedSymbol)
			{
				symbol = production[counter].substr(3, 1);
				break;
			}
		}
		terminalFound = isTerminal(symbol, terminals);
		if (terminalFound == false)
			findSymbol(production, terminals, counter, tempSymbol);
	}

	string firstset = symbol;
	return firstset;
}

string FindFollow( vector <string> firstSet,vector <string> grammar, vector <string> terminals, int count, int& pastForm, int& currentForm, string & followTag)
{
	bool followSetFound = false;
	string followSet = "";
	string followPred = grammar[count].substr(3);
	bool hasEpslion = false;
	if (count == 0)
		followSet = "$";
	else
	{
		if (grammar[count].substr(3, 1) == "e") // since our vector is not formated right ebislon is placed first 
		{
			hasEpslion = true;
		}
		if (pastForm >= 6) //Rule of alpha - A - beta then grab first set
		{
			for (int i = 0; i < firstSet.size(); i++)
				if(firstSet[i].substr(0,1) == followTag)
				{
					if (firstSet[i].substr(2, 1) == "e" || firstSet[i].substr(2, 1) == "I")
						followSet = firstSet[i].substr(3, 1);
					else
						followSet = firstSet[2].substr(2, 1);
				}
		}
		if (pastForm == 5)
		{
			followSet = "F" + followTag;
		}
	}
	if (hasEpslion == true)
		pastForm = grammar[count].length() - 1; // We dont want to count Epsilon has part of the rules

	else
		pastForm = grammar[count].length(); // We dont want to count Epsilon has part of the rules
	followTag = grammar[count].substr(0, 1);

	return followSet;
}
int main()
{
	string line;
	int count = 0;
	bool isproduction = false;
	vector <string> production;
	vector <string> terminals;
	vector <string> grammar;
	vector <string> firstSet;
	vector <string> grammarOutput;
	int startSymbol = 0;

	ifstream file("grammar.txt");
	if (file.is_open())
	{
		cout << "Displaying grammar" << endl;
		cout << "-----------------------------" << endl;
		while (getline(file, line))
		{
			cout << line << endl;
			if (line == "$")
			{
				count++;
				if (count == 0)
					isproduction = false;
				else if (count == 1)
					isproduction = true;
			}
			else {
				if (count == 1)
					grammarOutput.push_back(line);
				if (isproduction == true)
					production.push_back(line);
				else
					terminals.push_back(line);
			}
		}
	}
	cout << "-----------------------------" << endl;
	string temp = "";
	for (int i = 0; i < production.size(); i++)
	{
		int counter = 0;
		string firstSetString = findSymbol(production, terminals, i, temp);
		int tempcount = 0;
		for (counter = counter + 1; counter < production.size(); counter++)
		{
			if (production[counter].substr(0, 1) == temp)
			{
				if (tempcount == 0) // one extra sumbol is added so we want to avoid it by this if statment
					tempcount++;
				else
					firstSetString += "" + findDuplicates(production, terminals, counter);
			}
		}
		firstSet.push_back(production[i].substr(0, 1) + "=" + firstSetString);

	}
	int i = 0;
	string tempGrammar = "";
	for (int j = 0; j < firstSet.size(); j++)
	{
		tempGrammar = firstSet[j];
		bool found = false;
		for (int i = 0; i < grammar.size(); i++)
		{
			if (firstSet[j].substr(0, 1) == grammar[i].substr(0, 1))
			{
				tempGrammar += grammar[i].substr(2);
				grammar[i] = tempGrammar;
				found = true;
				break;
			}
		}
		if (!found)
			grammar.push_back(tempGrammar);
	}

	for (int i = 0; i < grammar.size(); i++)
	{
		if (grammar[i].substr(2, 1) == "e")
			cout << "First(" + grammar[i].substr(0, 1) + ") = {" + grammar[i].substr(3) + "e" "}" << endl;
		else
		{
			cout << "First(" + grammar[i].substr(0, 1) + ") = {" + grammar[i].substr(2) + "}" << endl;
		}
	}
	int pastForm = 0;
	int currentForm = 0;
	string followSymbol = "";

	vector<string> followGrammar;
	for (int j = 0; j < grammarOutput.size(); j++)
	{
		tempGrammar = grammarOutput[j];
		bool found = false;
		for (int i = 0; i < followGrammar.size(); i++)
		{
			if (firstSet[j].substr(0, 1) == followGrammar[i].substr(0, 1))
			{
				tempGrammar += followGrammar[i].substr(3);
				followGrammar[i] = tempGrammar;
				found = true;
				break;
			}
		}
		if (!found)
			followGrammar.push_back(tempGrammar);
	}
	string test = "";

	for (int i = 0; i < followGrammar.size(); i++)
	{
		currentForm = pastForm;
		test = FindFollow(grammar,followGrammar, terminals, i, pastForm, currentForm, followSymbol);
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
