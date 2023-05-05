/*

Programming Project 1

Class: EECS 4760 Computer Security
Professor: Dr. Thomas
Name: Collin Creps
Due Date: 09/18/2022

Description:

	This program is meant to be run from the command-line and produce a frequency analysis on 3 given files.

	Programming Project 1's objective is to take 3 test files of Shakespeare (.TXT, .DES, and .CBC) and output the frequency distributions of characters in a horizontal
	bar graph, the ascii value of the character,  as well as the counts for each character's appearance in a file. At the end of the graph there will be a summary line 
	containing the MIN, MAX, AVG, and DEV.


	CHANGE LOG
	----------

	09/10/2022 : Added a file reader and check to make sure the file is being read, otherwise the program will exit with code 1. I created the logic necessary for the average, min, max, dev, and counts of
		characters. The bar graph is scaled based upon the maximum occcurence of a character in the run. A large majority of time was simply spend creating formatting for the outputs. I used iomanip to allow
		me to pad strings with 0s, left or right align, etc.

	09/11/2022 : Edited the cout for outputs. The integer values of the ascii were not outputting, but rather strange symbols. I added a line to close the file which was ruining the reads and output. The
		zero padding for the summary line is not correct. It is padding on the right, and the decimal is not being placed correctly. I made this work by changing datatype to double and adding the padded
		zeroes after the complete double was written if it did not fulfill 9 spots. To wrap things up I made sure it runs in the console by navigating to the solutions x64/Debug executable and running in
		command prompt with the file name after the executable path.

	09/12/2022 : Tested the code and made sure that the average * 256 aligns with the file size in bytes. Figure out that if the file is not opened in binary mode /r/n gets translated to solely /n and
		my output started to be correct. I also added handling for the edge case of being presented an empty text file. I also tweaked the average and deviation precicision for the output because it was
		rounding some of the decimal places and filling with zeros.

	09/13/2022 : Tested with the Shakespeare files that were given to us and wrote a short report on the findings of which encrypted file was best. With the new files I was getting -nan(ind) as my deviation
		which happened to be due to trying to perform the calculation with a double and unsigned int mismatch. I also realized the reader was not capturing ascii beyond 127 which ruined results for the encrypted
		files. To fix this the reader needed to capture unsigned char not char.\

	09/15/2022 : Class announcement that 0 is a possible minimum, so altered the logic to capture minimum value. Caught the reader getting an extra byte which was related to end of file.

	TO RUN
	------

	Open the solution in VS and right click to open in file explorer. Navigate to the x64/Release folder and shift + right click on the exe to copy as path. In command prompt navigate to the directory of
	the executable (./x64/Release), then paste the path you copied followed by the file name you are testing. Upon hitting enter the program will run.

*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
using namespace std;

const int ARR_SIZE = 256;

//prints each line of the graph and its required amount of asterisks
void printFormat(unsigned int count, int index, double maximum) {
	
	//determine the number of pixels we need in bar graph for this ascii char
	int pixelCount = ceil(count / (maximum / 80));

	cout << setfill('0') << setw(3) << index; cout << ':';
	cout << right << setfill(' ') << setw(9) << int(count); cout << "|";
	cout << string(pixelCount, '*') + "\n";
}

int main(int argc, char* argv[]) {

	//holds the character
	unsigned char c;
	//setup the array to hold the 256 indicies of ascii values
	unsigned int charArray[256] = {};
	//var to hold MAX/MIN/AVG/DEV
	unsigned int max = 0;
	unsigned int min;
	double avg;
	double dev;

	//hold the total number of occurrences for the average calculation
	double totalCount = 0.0;
	//holds the sum of each individual occurrence squared
	double devSum = 0.0;
	//hold the ascii of char with maximum occurrences
	int maxChar;

	//setup the file reader object and open the file from the command-line arg in binary to capture carriage returns on top of new line
	std::ifstream inFile; inFile.open(argv[1], std::ios::binary);



	//check that the file is open and reading properly
	if (inFile.is_open()) {

		//check if the file is empty. If so, exit
		if (inFile.peek() == EOF) {
			cout << "No characters to be read in file.";
			exit(1);
		}

		//store each incoming character to the array
		while (inFile.good()) {
			
			c = inFile.get();

			//make sure we do not store the end of file byte
			if (inFile.eof()) {
				break;
			}

			charArray[c]++;

		}

		//close the text file
		inFile.close();

		//define min as the first character of the array
		min = charArray[0];

		//logic to store the min, max, and necessary values for avg/dev
		for (int i = 0; i < ARR_SIZE; i++) {
			if (charArray[i] > max) {
				max = charArray[i];
				maxChar = i;
			}
			if (charArray[i] < min) {
				min = charArray[i];
			}
			totalCount += charArray[i];
			devSum += pow((double)charArray[i], 2);
		}

		//calculate the average occurrence for letters amongst 256
		avg = totalCount / 256.0;
		//calculate the standard deviation
		dev = sqrt(((devSum - ((pow(totalCount, 2)) / 256.0)) / 256.0));

		//beginning of bar graph
		cout << string(13, ' ') + "+" + string(80, '-') + "\n";

		//pass control to the printing function which generates the bar graph for me
		for (int i = 0; i < ARR_SIZE; i++) {
			printFormat(charArray[i], i, max);
		}

		//end of bar graph
		cout << string(13, ' ') + "+" + string(80, '-') + "\n\n";

		//output the summary line of min, max, avg, dev
		cout << "Min:"; cout << right << setfill(' ') << setw(9) << min;
		cout << " ";

		cout << "Max  ("; cout << setfill('0') << setw(3) << maxChar; cout << "):"; cout << right << setfill(' ') << setw(9) << max;
		cout << " ";

		//set the precision for the avg and dev to not exceed 9 slots (. takes a spot)
		cout.precision(8);

		cout << "AVG:"; cout << left << setfill('0') << setw(9) << avg;
		cout << " ";


		cout << "DEV:"; cout << left << setfill('0') << setw(9) << dev;

		//program exits with success
		exit(0);
	}

	//if the file cannot be opened return with error code
	else {
		cout << "Input file not found.";
		exit(1);
	}
}
