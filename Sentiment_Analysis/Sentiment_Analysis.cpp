//=======================================================
//Vincent Bell, Joseph Goodwin
//Programming Assignment #3
//CS 331
//5/22/20
//=======================================================

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector> 

using namespace std;

// This will store a string of each word, # of times its pos/neg, and the probabities of it being pos/neg
struct Word
{
	string text = " ";
	double positive = 0;
	double negative = 0;
	double posprob = 0;
	double negprob = 0;
};

int linecount = 0;
// sorts every word in the vocab
bool textSort(const Word& a, const Word& b)
{
	return a.text < b.text;
}

bool wordMatch(string sentence, string word)
{
	stringstream s(sentence);
	string temp;
	while (s >> temp)
	{
		if (temp.compare(word) == 0)
		{
			return true;
		}
	}
	return false;
}

//bool add_word(Word& w) {
//	vocab.push_back(&w);
//	return true;
//}

vector<Word> preProcessing(ifstream& in, ofstream& out) {
	string line;
	string word = "";
	vector<Word> vocab;
	vector<string> vocab01;
	string sentence = "";

	// myWord pushes its values onto the vocab by storing The Word contents
	// in it, then pushes it onto the vocab
	Word myWord;
	// removes punctuation and digits and conversts capital letters to lower case
	if (in.is_open()) {
		while (!in.eof()) {
			getline(in, line);
			if (line.empty()) {
			}
			else
			{
				linecount++;
				char posNeg = line[line.length() - 2];
				for (int i = 0; i < line.size(); i++) {
					// erases all digits, tabs, punctuation, and changes upper case letters to lowercase
					if (ispunct(line[i])) {
						line.erase(i--, 1);
					}
					else if (isdigit(line[i])) {
						line.erase(i--, 1);
					}
					else if (line[i] == '\t') {
						line.erase(i--, 1);
					}
					else if (isupper(line[i])) {
						line[i] = tolower(line[i]);
					}
				}
				/* pushes each word onto vocab. auto x should be a character, this is basically a for loop that loops
				through each character in a line. */ 
				for (auto x : line) {
					/* If char x is empty, then we know that we've found a word. We can then check 
					PosNeg to see check if the word comes from a positive review or a negative review
					We can then push the word onto the vocab along with a 1 for a positive review or 
					a 1 for a negative review*/
					if (x == ' ') {
						if (posNeg == '1')
						{
							myWord.text = word;
							myWord.positive = 1;
							myWord.negative = 0;
							vocab.push_back(myWord);							
						}
						if (posNeg == '0')
						{
							myWord.text = word;
							myWord.positive = 0;
							myWord.negative = 1;
							vocab.push_back(myWord);
						}
						word = "";
						sentence = sentence + x;
					}
					// If x isn't ' ', then keep on adding letters to form a word.  
					else {
						word = word + x;
						sentence = sentence + x;
					}
				}
				// This makes the featurized data. Represented as 0/1
				sentence = sentence + " ";
				sentence = sentence + posNeg;
				vocab01.push_back(sentence);
				sentence = "";
			}
		}
	}
	// These will keep a count of the number of times a word appears as pos/neg
	double p = 0;
	double n = 0;
	// alphabetize all words
	sort(vocab.begin(), vocab.end(), textSort);
	/*This will store the vocab with no duplicate words and the amount of times each 
	word appeard as pos/neg*/
	vector<Word> voc;

	/*starts at the first word, then compares it to the next word. If the next word is the 
	same, then increment the counter based off the pos/neg value of both words. Keep on doing
	this until the next word is different, then push that word and a p/n onto the new vocab voc*/
	for (int i = 0; i < vocab.size(); i++) {
		for (int j = i + 1; j < vocab.size(); j++) {
			if (vocab[i].text == vocab[j].text) {
				if (vocab[j].positive == 1) {
					p++;
				}
				if (vocab[j].negative == 1) {
					n++;
				}
			}
			else if (vocab[i].text != vocab[j].text) {
				// gets the pos/neg value of the first word
				if (vocab[i].positive == 1) {
					p++;
				}
				if (vocab[i].negative == 1) {
					n++;
				}
				Word tmp;
				tmp.text = vocab[i].text;
				tmp.positive = p;
				tmp.negative = n;
				voc.push_back(tmp);
				p = 0;
				n = 0;
				i = j;
			}
		}
	}
	// uncomment this to print out each word and the number of times they appeared as pos/neg
	/*for (int i = 0; i < voc.size(); i++) {
		cout << voc[i].text << " " << voc[i].positive << " " << voc[i].negative << ", ";
	}
	cout << endl;*/

	// removes duplicates in vocab. This will be used in the featurization step
	for (int i = 0; i < vocab.size(); i++) {
		for (int j = i + 1; j < vocab.size(); j++) {
			if (vocab[i].text == vocab[j].text) {
				vocab[j].text = vocab[vocab.size() - 1].text;
				vocab.pop_back();
				j--;
			}
		}
	}

	// need to alphabetize all words again after removing some since some words get out of order
	sort(vocab.begin(), vocab.end(), textSort);

	//This loop creates the vocab and outputs the featurized data under the vocab
	if (out.is_open())
	{
		for (int i = 0; i < linecount + 1; i++)
		{
			if (i == 0)
			{
				for (int j = 0; j < vocab.size(); j++)
				{
					out << vocab[j].text << ", ";
					//cout << vocab[i].text << ", ";
				}
				out << "classlabel\n";
			}
			else
			{
				for (int z = 0; z < vocab.size(); z++)
				{
					if (wordMatch(vocab01[i - 1], vocab[z].text))
					{
						//uncomment to debug
						//out << vocab[z].text;
						out << "1, ";
					}
					else
					{
						out << "0, ";
					}
				}
				out << vocab01[i - 1].back() << "\n";
			}
		}
	}


	/*This is how we calculate the probability of each word. The loop will loop through every word in the 
	vocab and will use the dirilecht prior calculation to calculate the probability the word came from a positive 
	review and negative review*/
	for (int i = 0; i < voc.size(); i++) {
		voc[i].posprob = (((voc[i].positive) + 1) / (2 + (voc[i].positive + voc[i].negative)));
		voc[i].negprob = (((voc[i].negative) + 1) / (2 + (voc[i].positive + voc[i].negative)));
	}

	// We will use the input file again in the next function so clear it, then do in.seekg to go back to the beginning of the file
	in.clear();
	in.seekg(0, ios::beg);

	// the same vocab will be used in the classification step so return it
	return voc;
}

void classification(ifstream& in, ofstream& out, vector<Word> vocab) {

	string line;
	// we will store the test vocabulry in a vector<string>
	vector<string> testVoc;
	string word = "";
	// These will store the posprob/negprob of each word in a review
	vector<double> multpos = { 0 };
	vector<double> multneg = { 0 };
	// These values will be used to store our prediction of each review being pos/neg
	double lineProbPos = 1;
	double lineProbNeg = 1;
	// These will eventually be stored with 1/0 showing the our prediction for each review
	// and the actual pos/neg value of each review
	vector<double> prediction = {};
	vector<double> actual = {};
	// These will be incremented to show how many positive and negative reviews we predict
	double predictedPos = 0;
	double predictedNeg = 0;
	// Thes wil be incremented with the amount of actual positive and negative reviews
	double actualPos = 0;
	double actualNeg = 0;

	if (in.is_open()) {
		while (!in.eof()) {
			getline(in, line);
			if (line.empty()) {

			}
			else
			{
				linecount++;
				char posNeg = line[line.length() - 2];
				for (int i = 0; i < line.size(); i++) {
					// We will want to get rid of punctuation and change upper case from the test file, but we'll want to
					// keep tabs and numbers to find out if the reviews are pos/neg for each review
					if (ispunct(line[i])) {
						line.erase(i--, 1);
					}
					else if (isupper(line[i])) {
						line[i] = tolower(line[i]);
					}
				}
				// pushes each word onto vocab
				for (auto x : line) {
					if (x == ' ') {
						// we will push each word from the test file onto testVoc for testing
						testVoc.push_back({ word });

						for (int i = 0; i < vocab.size(); i++) {
							// If the test vocab == "/t", then that means we have reached the end of the review.
							if (testVoc.back() == "\t") {
								// This is how we multiply every element of a vector to one another. These loops will
								// loop all the pos/neg probabilies of each word in a review and multiply them to one another 
								for (const auto& a : multneg) {
									if (a != 0)
									lineProbNeg *= a;
								}
								for (const auto& e : multpos) {
									if (e != 0)
									lineProbPos *= e;
								}
								// This step predicts if the review is positive or negative and will increment predictedPos or predictedNeg.
								// It also pushes a 1 or a 0 onto the prediction vector which will be compared to the pos/neg values of the actual review later
								if (lineProbPos > lineProbNeg) {
									predictedPos++;
									prediction.push_back(1);
								}
								else if (lineProbPos < lineProbNeg) {
									predictedNeg++;
									prediction.push_back(0);
								}
								// Set these values to their default values so they can be used with the next review
								multpos.clear();
								multneg.clear();
								lineProbPos = 1;
								lineProbNeg = 1;
							}
							// if the word on the testVoc is the same as the word in the vocab, push its pos/neg probabilities onto multpos and multneg
							// These will be used once we reach the end of the review			
							else if (vocab[i].text == testVoc.back()) {
								multpos.push_back(vocab[i].posprob);
								multneg.push_back(vocab[i].negprob);
							}
						}
						word = "";
					}
					// keep on adding char x to the word 
					else {
						word = word + x;
					}
				}
			}
		}
	}

	/*for (int i = 0; i < testVoc.size(); i++) {
		cout << testVoc[i] << " ";
	}*/

	/*loops through the entire testVoc. If we've reached a tab, then we've reached the end 
	of a review. find if the next value of testVoc is 1/0, then increment actualPos or actualNeg 
	depending on that value. Then push 1/0 on the actual vector. actual will compared to predicted
	later on to find out how accurate our predictions are*/
	for (int i = 0; i < testVoc.size(); i++) {
		if (testVoc[i] == "\t") {
			if (testVoc[i + 1] == "1") {
				actualPos++;
				actual.push_back(1);
			}
			if (testVoc[i + 1] == "0") {
				actualNeg++;
				actual.push_back(0);
			}
		}
	}

	double correctPos = 0;
	double correctNeg = 0;

	/*This is how we will find the number of correct predictions. We will do this by comparing
	the actual value stored on the actual vector to the predicted value stored on the prediction vector
	Then increment correctPos/correctNeg*/
	for (int i = 0; i < prediction.size(); i++) {
		if ((actual[i] == 1) && (prediction[i] == 1)) {
			correctPos++;
		}
		else if ((actual[i] == 0) && (prediction[i] == 0)) {
			correctNeg++;
		}
	}

	// Find the accuracy of positive prediction, negative predictions, and our total accuracy
	double posAccuracy = correctPos / actualPos;
	double negAccuracy = correctNeg / actualNeg;
	double accuracy = (posAccuracy + negAccuracy) / 2;

	cout << "predicted positive: " << predictedPos << endl;
	cout << "predicted negative: " << predictedNeg << endl;
	cout << "actual positives: " << actualPos << endl;
	cout << "actual negatives: " << actualNeg << endl;
	cout << "correctly predicted positives: " << correctPos << endl;
	cout << "correctly predicted negatives: " << correctNeg << endl;
	cout << "positive accuracy: " << posAccuracy << endl;
	cout << "negative accuracy: " << negAccuracy << endl;
	cout << "total accuracy: " << accuracy << endl;
	cout << endl;

	out << "predicted positive: " << predictedPos << endl;
	out << "predicted negative: " << predictedNeg << endl;
	out << "actual positives: " << actualPos << endl;
	out << "actual negatives: " << actualNeg << endl;
	out << "correctly predicted positives: " << correctPos << endl;
	out << "correctly predicted negatives: " << correctNeg << endl;
	out << "positive accuracy: " << posAccuracy << endl;
	out << "negative accuracy: " << negAccuracy << endl;
	out << "total accuracy: " << accuracy << endl;
	out << endl;

	// clear the output file and use out.seekp to go back to the beggining of it since were going to do the classification
	// step again for the training set
	out.clear();
	out.seekp(0, ios::end);
}

int main()
{
	ifstream training_in("trainingSet.txt");
	ifstream test_in("testSet.txt");

	ofstream training_out("preprocessed_train.txt");
	ofstream test_out("preprocessed_test.txt");
	ofstream results("Results.txt");

	vector<Word> voc;
	voc = preProcessing(training_in, training_out);

	results << "test set results: " << endl;
	cout << "test set results: " << endl;
	classification(test_in, results, voc);

	results << "training set results: " << endl;
	cout << "training set results: " << endl;
	classification(training_in, results, voc);

	return 0;
}





