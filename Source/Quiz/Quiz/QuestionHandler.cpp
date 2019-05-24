#include "QuestionHandler.h"
using namespace std;

QuestionHandler::QuestionHandler(string questionsPath, string resultsPath)
{
	questionsFilePath = questionsPath;
	resultsFilePath = resultsPath;
}

QuestionHandler& QuestionHandler::GetInstance(string questionsPath, string resultsPath)
{
	static QuestionHandler objectInstance(questionsPath, resultsPath);

	return objectInstance;
}

void QuestionHandler::CountQuestions()
{
	string line;
	fstream in_file;
	in_file.open(questionsFilePath, ios::in);
	int linesNumber = 0;
	while (getline(in_file, line)) 
		linesNumber++;
	in_file.close();
	questionsAmount = linesNumber / 6;
}

void QuestionHandler::AnswerClick(char answer)
{
	if (currentQuestionRightAnswer == answer)
	{
		userScore += currentQuestionPoints;
	}

	v_userAnswers.push_back(answer);
}

void QuestionHandler::GetNextQuestion()
{
	string line, charPoints;
	string task, ansA, ansB, ansC, ansD;
	fstream inFile;
	inFile.open(questionsFilePath, ios::in);
	int lineNumber = 0;
	currentQuestionIndex++;
	while (getline(inFile, line))
	{
		lineNumber++;
		if (lineNumber == (currentQuestionIndex - 1) * 6 + 1) task = line;
		if (lineNumber == (currentQuestionIndex - 1) * 6 + 2) ansA = line;
		if (lineNumber == (currentQuestionIndex - 1) * 6 + 3) ansB = line;
		if (lineNumber == (currentQuestionIndex - 1) * 6 + 4) ansC = line;
		if (lineNumber == (currentQuestionIndex - 1) * 6 + 5) ansD = line;
		if (lineNumber == (currentQuestionIndex - 1) * 6 + 6)
		{
			currentQuestionRightAnswer = line[0];
			v_rightAnswers.push_back(currentQuestionRightAnswer);
			charPoints = line[1];
			currentQuestionPoints = atoi(charPoints.c_str());
			v_questionPoints.push_back(currentQuestionPoints);
			break;
		}
	}
	inFile.close();
	currentQuestionTask = task;
	currentQuestionAnswerA = "Odp A:  " + ansA;
	currentQuestionAnswerB = "Odp B:  " + ansB;
	currentQuestionAnswerC = "Odp C:  " + ansC;
	currentQuestionAnswerD = "Odp D:  " + ansD;
}

bool QuestionHandler::CheckTestFinished()
{
	if (currentQuestionIndex == questionsAmount)
	{
		return true;
	}
	else
	{
		GetNextQuestion();
		return false;
	}
}

void QuestionHandler::SaveScore(LPSTR userNameBuffer)
{
	fstream outFile;
	outFile.open(resultsFilePath, ios::out);
	outFile << "Imie i Nazwisko:\t" << userNameBuffer << endl;
	outFile << "Numer pytania\t\t";
	if (questionsAmount > 10)
	{
		for (int i = 1; i <= 10; i++) outFile << " " << i << " ";
		for (int i = 11; i <= questionsAmount; i++) outFile << i << " ";
	}
	else
	{
		for (int i = 1; i <= questionsAmount; i++) outFile << " " << i << " ";
	}
	outFile << endl;
	outFile << "Poprawna ODP\t\t";
	for (const auto& e : v_rightAnswers) outFile << " " << e << " ";
	outFile << endl;
	outFile << "Uzyskana ODP\t\t";
	for (const auto& f : v_userAnswers) outFile << " " << f << " ";
	outFile << endl;
	outFile << "Punkty do zdobycia:\t";
	for (const auto& g : v_questionPoints) outFile << " " << g << " ";
	outFile << endl << endl;
	outFile << "Ilosc zdobytych punktow: " << userScore;
	outFile.close();
}
