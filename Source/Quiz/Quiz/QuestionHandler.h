#include <vector>
#include <fstream>
#include <string>
#include <windows.h>

#pragma once
class QuestionHandler
{
private:

	std::string questionsFilePath;
	std::string resultsFilePath;
	QuestionHandler(std::string questionsPath, std::string resultsPath);

public:

	int userScore = -1;
	int questionsAmount;

	std::string currentQuestionTask;
	std::string currentQuestionAnswerA;
	std::string currentQuestionAnswerB;
	std::string currentQuestionAnswerC;
	std::string currentQuestionAnswerD;
	int currentQuestionIndex = 0;
	int currentQuestionPoints = 0;
	char currentQuestionRightAnswer;

	std::vector <char> v_rightAnswers;
	std::vector <char> v_userAnswers;
	std::vector <int> v_questionPoints;

	static QuestionHandler& GetInstance(std::string questionsPath, std::string resultsPath);

	void CountQuestions();
	void AnswerClick(char answer);
	void GetNextQuestion();
	bool CheckTestFinished();
	void SaveScore(LPSTR userNameBuffer);
};

