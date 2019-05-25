// Quiz.Tests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "QuestionHandler.h"
#include "QuestionHandler.cpp"
#include "gtest/gtest.h"

struct QuestionHandlerInitTest : testing::Test
{
public:
	QuestionHandler* QuestionH;

	QuestionHandlerInitTest()
	{
		QuestionH = &QuestionHandler::GetInstance("testpytania.txt", "testwynik.txt");
	}
};

TEST_F(QuestionHandlerInitTest, QuestionHandlerIsProperSingleton)
{
	QuestionHandler* secondQuestionH = &QuestionHandler::GetInstance("", "");

	ASSERT_TRUE(secondQuestionH == QuestionH);
}

TEST_F(QuestionHandlerInitTest, SuccessfullyOpensQuestionFileAndReadsAllTheQuestions)
{
	QuestionH->CountQuestions();

	ASSERT_EQ(19, QuestionH->questionsAmount);
}

struct QuestionHandlerQuizTest : QuestionHandlerInitTest
{
public:

	void Initialize()
	{
		QuestionH->CountQuestions();
		QuestionH->userScore++;
		QuestionH->GetNextQuestion();
	}
};

TEST_F(QuestionHandlerQuizTest, AnswerClickCheckIfAnswerGotSaved)
{
	Initialize();
	QuestionH->AnswerClick('A');

	ASSERT_EQ(1, QuestionH->v_userAnswers.size());
	ASSERT_EQ('A', QuestionH->v_userAnswers.at(0));
}

TEST_F(QuestionHandlerQuizTest, AnswerClickRightAnswerCheckIfUserGetsScore)
{
	QuestionH->GetNextQuestion();
	ASSERT_EQ('D', QuestionH->currentQuestionRightAnswer);

	QuestionH->AnswerClick('D');

	ASSERT_EQ(2, QuestionH->v_userAnswers.size());
	ASSERT_EQ('D', QuestionH->v_userAnswers.at(1));
	ASSERT_EQ('D', QuestionH->v_rightAnswers.at(1));
	ASSERT_EQ(3, QuestionH->userScore);
}

TEST_F(QuestionHandlerQuizTest, GetNextQuestionSetsUpQuestionValuesProperly)
{
	QuestionH->GetNextQuestion();

	ASSERT_EQ("Ktory z ponizszych atrybutow definiuje blok (ramke)? (HTML)", QuestionH->currentQuestionTask);
	ASSERT_EQ("Odp A:  <td>", QuestionH->currentQuestionAnswerA);
	ASSERT_EQ("Odp B:  <table>", QuestionH->currentQuestionAnswerB);
	ASSERT_EQ("Odp C:  <p>", QuestionH->currentQuestionAnswerC);
	ASSERT_EQ("Odp D:  <div>", QuestionH->currentQuestionAnswerD);
	ASSERT_EQ('D', QuestionH->currentQuestionRightAnswer);
	ASSERT_EQ(2, QuestionH->currentQuestionPoints);
}

struct QuestionHandlerFinalizeTest : QuestionHandlerInitTest
{
public:

	void Initialize()
	{
		QuestionH->AnswerClick('B');
		for (int i = 0; i < 16; i++)
		{
			QuestionH->GetNextQuestion();
			QuestionH->AnswerClick('B');
		}
	}
};

TEST_F(QuestionHandlerFinalizeTest, CheckIfTestFinished)
{
	Initialize();

	ASSERT_TRUE(QuestionH->CheckTestFinished());
}

TEST_F(QuestionHandlerFinalizeTest, SaveUserScoreAndCheckTheFile)
{
	QuestionH->SaveScore(LPSTR("Imie Nazwisko"));

	ASSERT_EQ(19, QuestionH->v_rightAnswers.size());
	ASSERT_EQ(19, QuestionH->v_userAnswers.size());
	ASSERT_EQ(19, QuestionH->v_questionPoints.size());
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
