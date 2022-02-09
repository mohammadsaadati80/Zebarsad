#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <functional>
#include <map>
using namespace std;
#define COMMAND_SEPARATOR ' '
#define EMPTY_CHARACTER ""
#define VARIABLE_INPUT_VALUE_SIGN '$'
#define FIRST_ELEMENT 0
#define EMPTY 0
#define HAVING_THE_INITIAL_VALUE_OF_THE_VARIABLES 1
#define NO_INITIAL_VALUE_OF_VARIABLES 0
#define INPUT_COMMAND_SIGN '?'
#define OUTPUT_COMMAND_SIGN '!'
#define REPLACEMENT_COMMAND_SIGN '='
#define PLUS_OPERATOR '+'
#define MINUS_OPERATOR '-'
#define MULTIPY_OPERATOR '*'
#define DIVISION_OPERATOR '/'
#define CHARACTER 1
#define NUMBER 2
#define OPERATOR -1
#define EMPTY_VALUE 0
#define OUTPUT_MASSAGE "Minimum Steps: "

class Variable
{
public:
	Variable(string name, int value);
	string getName();
	void updateValue(int value);
	int getValue();

private:
	string name;
	int value;

};

Variable::Variable(string name, int value)
{
	this->name = name;
	this->value = value;
}

string Variable::getName() { return name; }

void Variable::updateValue(int value)
{
	this->value = value;
}

int Variable::getValue() { return value; }

class Command
{
public:
	void addNewOperator(char newOperator);
	void addNewVariable(Variable newVariable);
	char getOperator(int number);
	Variable getVariable(int number);
	void updateCommand(vector<Variable> variable);
	void updateVariable(int number, int value);
	int getOperatorsSize();
	int getVariableSize();
	void ereaseVariable(int number);
	void ereaseOperator(int number);

private:
	vector<char> operators;
	vector<Variable> variables;
};

void Command::addNewOperator(char newOperator)
{
	operators.push_back(newOperator);
}

void Command::addNewVariable(Variable newVariable)
{
	variables.push_back(newVariable);
}

char Command::getOperator(int number) { return operators[number]; }

Variable Command::getVariable(int number) { return variables[number]; }

void Command::updateCommand(vector<Variable> variable)
{
	for (int current = 0; current < this->variables.size(); current++)
		for (int currentVariable = 0; currentVariable < variable.size(); currentVariable++)
			if (this->variables[current].getName() == variable[currentVariable].getName())
				this->variables[current].updateValue(variable[currentVariable].getValue());
}

void Command::updateVariable(int number, int value)
{
	variables[number].updateValue(value);
}

int Command::getOperatorsSize() { return operators.size(); }

int Command::getVariableSize() { return variables.size(); }

void Command::ereaseVariable(int number)
{
	variables.erase(variables.begin() + number);
}

void Command::ereaseOperator(int number)
{
	operators.erase(operators.begin() + number);
}

class Tools
{
public:
	Tools();
	vector<string> getInput();
	vector<int> separateTheValueOfVariables(vector<string> newLine);
	Command seperateElementsOfCommand(string command);
	vector<string> seperateSpaceFromCommand(string command);
	Command seperateOperatorFromCharacter(Command command, string line);
	bool checkForHavingOperator(char word);
	vector<string> addNewWordToOtherCammand(string word, vector<string> otherCommand);
	Command addNewVariables(Command command, vector<string> otherCommand);
	
private:
	vector<char> operators;
};

Tools::Tools()
{
	operators.push_back(INPUT_COMMAND_SIGN);
	operators.push_back(OUTPUT_COMMAND_SIGN);
	operators.push_back(REPLACEMENT_COMMAND_SIGN);
	operators.push_back(PLUS_OPERATOR);
	operators.push_back(MINUS_OPERATOR);
	operators.push_back(MULTIPY_OPERATOR);
	operators.push_back(DIVISION_OPERATOR);
}

vector<string> Tools::getInput()
{
	vector<string> inputLine;
	string line;
	while (getline(cin, line))
		inputLine.push_back(line);
	return inputLine;
}

vector<int> Tools::separateTheValueOfVariables(vector<string> newLine)
{
	vector<int> variable;
	for (int current = FIRST_ELEMENT + 1; current < newLine.size(); current++)
		variable.push_back(stoi(newLine[current]));
	return variable;
}

Command Tools::seperateElementsOfCommand(string line)
{
	Command command;
	vector<string> newCommand = seperateSpaceFromCommand(line);
	for (int current = 0; current < newCommand.size(); current++)
		command = seperateOperatorFromCharacter(command, newCommand[current]);
	return command;
}

vector<string> Tools::seperateSpaceFromCommand(string command)
{
	stringstream string_stream(command);
	string word;
	vector<string> newCommand;
	while (getline(string_stream, word, COMMAND_SEPARATOR))
		if (word != EMPTY_CHARACTER)
			newCommand.push_back(word);
	return newCommand;
}

Command Tools::seperateOperatorFromCharacter(Command command, string line)
{
	vector<string> otherCommand;
	string word;
	for (int current = 0; current < line.size(); current++)
		if (checkForHavingOperator(line[current]))
		{
			otherCommand = addNewWordToOtherCammand(word, otherCommand);
			word.erase();
			if(current == 0)
				command.addNewOperator(line[current]);
			else if(line[current] != line[current - 1])
					command.addNewOperator(line[current]);
		}
		else
			word.push_back(line[current]);
	otherCommand = addNewWordToOtherCammand(word, otherCommand);
	return addNewVariables(command, otherCommand);
}

bool Tools::checkForHavingOperator(char word)
{
	for (int currentCommand = 0; currentCommand < operators.size(); currentCommand++)
		if (word == operators[currentCommand])
			return true;
	return false;
}

vector<string> Tools::addNewWordToOtherCammand(string word, vector<string> otherCommand)
{
	if (word.size() != 0)
		otherCommand.push_back(word);
	return otherCommand;
}

Command Tools::addNewVariables(Command command, vector<string> otherCommand)
{
	for (int current = 0; current < otherCommand.size(); current++)
		if (int(otherCommand[current][FIRST_ELEMENT]) >= 48 && int(otherCommand[current][FIRST_ELEMENT]) <= 57)
			command.addNewVariable(Variable(EMPTY_CHARACTER, stoi(otherCommand[current])));
		else
			command.addNewVariable(Variable(otherCommand[current], EMPTY_VALUE));
	return command;
}

class Order
{
public:
	Order(int orderNumber, char orderType, string variableName);
	char getOrderType();
	string getVariableName();
	int getOrderNumber();

private:
	int orderNumber;
	char orderType;
	string variableName;
};

Order::Order(int orderNumber, char orderType, string variableName)
{
	this->orderNumber = orderNumber;
	this->orderType = orderType;
	this->variableName = variableName;
}

char Order::getOrderType() { return orderType; }

string Order::getVariableName() { return variableName; }

int Order::getOrderNumber() {return orderNumber;}

class Zebarsad
{
public:
	Zebarsad();
	void getInput();
	void commandsHandler();
	int checkInputVariableValue(string line);
	void checkCurrentCommand(string line);
	void checkCanInitialVariable(Command command);
	void initialVariable(string name);
	void updateVariableOrder(Command command);
	int calculateExpression(Command command);
	void showTheResultOfCalculatingTheExpression(Command command);
	void updateVariable(string name, int value);
	void checkForParallelCommand(Command command, char orderType);
	bool checkCanDoParallelCommand(Command command, char orderType, string variableNane);

private:
	Tools tools;
	vector<string> inputCommands;
	vector<Variable> variable;
	vector<int> inputVariableValue;
	int initializedVariableCounter;
	int minOfOrder;
	int orderCounter;
	vector<Order> passedOrderDetails;
};

Zebarsad::Zebarsad()
{
	initializedVariableCounter = EMPTY;
	minOfOrder = EMPTY;
	orderCounter = EMPTY;
}

void Zebarsad::getInput()
{
	inputCommands = tools.getInput();
}

void Zebarsad::commandsHandler()
{
	int result = checkInputVariableValue(inputCommands[inputCommands.size() - 1]);
	for (int current = 0; current < inputCommands.size() - result; current++)
		if (inputCommands[current] != EMPTY_CHARACTER)
			checkCurrentCommand(inputCommands[current]);
	cout << OUTPUT_MASSAGE << minOfOrder << endl;
}

int Zebarsad::checkInputVariableValue(string line)
{
	vector<string> newLine = tools.seperateSpaceFromCommand(line);
	if (newLine[FIRST_ELEMENT][FIRST_ELEMENT] == VARIABLE_INPUT_VALUE_SIGN)
	{
		inputVariableValue = tools.separateTheValueOfVariables(newLine);
		return HAVING_THE_INITIAL_VALUE_OF_THE_VARIABLES;
	}
	else
		return NO_INITIAL_VALUE_OF_VARIABLES;
}

void Zebarsad::checkCurrentCommand(string line)
{
	Command command = tools.seperateElementsOfCommand(line);
	command.updateCommand(variable);
	if (command.getOperator(FIRST_ELEMENT) == INPUT_COMMAND_SIGN)
		checkCanInitialVariable(command);
	else if (command.getOperator(FIRST_ELEMENT) == REPLACEMENT_COMMAND_SIGN)
		updateVariableOrder(command);
	else if (command.getOperator(FIRST_ELEMENT) == OUTPUT_COMMAND_SIGN)
		showTheResultOfCalculatingTheExpression(command);
}

void Zebarsad::checkCanInitialVariable(Command command)
{
	if (command.getVariableSize() > EMPTY)
	{
		initialVariable(command.getVariable(FIRST_ELEMENT).getName());
		checkForParallelCommand(command, INPUT_COMMAND_SIGN);
	}
}

void Zebarsad::checkForParallelCommand(Command command, char orderType)
{
	++orderCounter;
	string outputVariableName = command.getVariable(FIRST_ELEMENT).getName();
	command.ereaseVariable(FIRST_ELEMENT);
	if (!checkCanDoParallelCommand(command, orderType, outputVariableName))
		minOfOrder++;
	passedOrderDetails.push_back(Order(minOfOrder, orderType, outputVariableName));
}

bool Zebarsad::checkCanDoParallelCommand(Command command, char orderType, string variableName)
{
	if (passedOrderDetails.size() == EMPTY)
		return false;
	for (int currentPassedOrder = passedOrderDetails.size() - 1; currentPassedOrder >= 0; currentPassedOrder--)
		if (passedOrderDetails[currentPassedOrder].getOrderNumber() == minOfOrder)
		{
			if (passedOrderDetails[currentPassedOrder].getOrderType() == orderType && orderType == INPUT_COMMAND_SIGN)
				return false;
			for (int current = 0; current < command.getVariableSize(); current++)
				if ((command.getVariable(current).getName() == passedOrderDetails[currentPassedOrder].getVariableName()  ||  variableName == passedOrderDetails[currentPassedOrder].getVariableName())&& passedOrderDetails[currentPassedOrder].getOrderType() == INPUT_COMMAND_SIGN)
					return false;
			for (int current = 0;current < command.getVariableSize(); current++)
				if ((command.getVariable(current).getName() == passedOrderDetails[currentPassedOrder].getVariableName()  ||  variableName == passedOrderDetails[currentPassedOrder].getVariableName()) && passedOrderDetails[currentPassedOrder].getOrderType() == REPLACEMENT_COMMAND_SIGN)
					return false;
			if (variableName == passedOrderDetails[currentPassedOrder].getVariableName() && passedOrderDetails[currentPassedOrder].getOrderType() == OUTPUT_COMMAND_SIGN && orderType != OUTPUT_COMMAND_SIGN)
					return false;
			if (orderType == OUTPUT_COMMAND_SIGN)
				for (int current = 0;current < command.getVariableSize(); current++)
					if ((command.getVariable(current).getName() == passedOrderDetails[currentPassedOrder].getVariableName()  ||  variableName == passedOrderDetails[currentPassedOrder].getVariableName()) && (passedOrderDetails[currentPassedOrder].getOrderType() == INPUT_COMMAND_SIGN || passedOrderDetails[currentPassedOrder].getOrderType() == REPLACEMENT_COMMAND_SIGN))
						return false;

			if (orderType == OUTPUT_COMMAND_SIGN)
				if ((variableName == passedOrderDetails[currentPassedOrder].getVariableName()) && (passedOrderDetails[currentPassedOrder].getOrderType() == INPUT_COMMAND_SIGN || passedOrderDetails[currentPassedOrder].getOrderType() == REPLACEMENT_COMMAND_SIGN))
					return false;
			if ((variableName == passedOrderDetails[currentPassedOrder].getVariableName())&& passedOrderDetails[currentPassedOrder].getOrderType() == INPUT_COMMAND_SIGN)
				return false;
			if ((variableName == passedOrderDetails[currentPassedOrder].getVariableName()) && passedOrderDetails[currentPassedOrder].getOrderType() == REPLACEMENT_COMMAND_SIGN)
				return false;

		}
		else
			break;
	for (int current = 0;current < variable.size(); current++)
		if (variable[current].getName() == variableName && orderType == REPLACEMENT_COMMAND_SIGN)
			return false;
	return true;
}

void Zebarsad::updateVariableOrder(Command command)
{
	checkForParallelCommand(command, REPLACEMENT_COMMAND_SIGN);
	command.ereaseOperator(FIRST_ELEMENT);
	string outputVariableName = command.getVariable(FIRST_ELEMENT).getName();
	command.ereaseVariable(FIRST_ELEMENT);
	updateVariable(outputVariableName, calculateExpression(command));
}

int Zebarsad::calculateExpression(Command command)
{
	int result = command.getVariable(FIRST_ELEMENT).getValue();
	for (int currentOperator = command.getOperatorsSize(); currentOperator > 0; currentOperator--)
	{
		if (command.getOperator(currentOperator - 1) == PLUS_OPERATOR)
			result = command.getVariable(currentOperator - 1).getValue() + command.getVariable(currentOperator).getValue();
		else if (command.getOperator(currentOperator - 1) == MINUS_OPERATOR)
			result = command.getVariable(currentOperator - 1).getValue() - command.getVariable(currentOperator).getValue();
		else if (command.getOperator(currentOperator - 1) == MULTIPY_OPERATOR)
			result = command.getVariable(currentOperator - 1).getValue() * command.getVariable(currentOperator).getValue();
		else if (command.getOperator(currentOperator - 1) == DIVISION_OPERATOR)
			result = command.getVariable(currentOperator - 1).getValue() / command.getVariable(currentOperator).getValue();
		command.updateVariable(currentOperator - 1, result);
	}
	return result;
}			

void Zebarsad::initialVariable(string name)
{
	if (initializedVariableCounter < inputVariableValue.size())
	{
		variable.push_back(Variable(name, inputVariableValue[initializedVariableCounter]));
		++initializedVariableCounter;
	}
	else
		variable.push_back(Variable(name, EMPTY_VALUE));
}

void Zebarsad::updateVariable(string name, int value)
{
	for (int current = 0; current < variable.size(); current++)
		if (variable[current].getName() == name)
		{
			variable[current].updateValue(value);
			return;
		}
	variable.push_back(Variable(name, value));
}

void Zebarsad::showTheResultOfCalculatingTheExpression(Command command)
{
	command.ereaseOperator(FIRST_ELEMENT);
	cout << calculateExpression(command) << endl;
	checkForParallelCommand(command, OUTPUT_COMMAND_SIGN);
}

int main()
{
	Zebarsad zebarsad;
	zebarsad.getInput();
	zebarsad.commandsHandler();
}