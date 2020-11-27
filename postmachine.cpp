#include "postmachine.h"

PostMachine::PostMachine()
{
    // Начальное положение метки
    tapeIndex = 0;
}

PostMachine::~PostMachine()
{
}

/* Пользователь вводит входную ленту до тех пор,
 * пока она окажется корректной (т.е. из 0 и 1).
*/
void PostMachine::readTape()
{
    int check = -1;
    while(check < 0)
    {
        check = -1;
        cout << "Entrance tape: \n";
        cin >> tape;
        for(unsigned long long i = 0; i < tape.size(); i++)
            if(tape[i] == '0' || tape[i] == '1')
                check = 1;
            else
            {
                check = INT_MIN;
                cout << "Found a character outside the alphabet\n";
                break;
            }
    }
}
/* Считывание из входных данных из файла, в котором
 * A  – собственный алфавит
 * X  – множество переменных
 * A1 – множество аксиом
 * R  – конечное множество продукций (или правил вывода)
*/
void PostMachine::readInputFile(string path)
{
    ifstream file(path);
    if (!file)
    {
        cout << "Error. File 'input' not found\n";
        exit(-1);
    }
    bool foundRules = false;
    while (!file.eof())
    {
        string str;
        getline(file, str);

        if(str == "}") foundRules = false;
        if(foundRules) rules.push_back(str.substr(0, str.find(",")));
        if(str == "R = {") foundRules = true;
    }
    file.close();
}

/* В цикле шагаем по ленте и применяем правила к ячейке,
 * на которую указывает метка.
*/
string PostMachine::start()
{
    unsigned long long currentRule = 0;
    while (true)
    {
        // Получение текущей команды
        string command;
        if(currentRule < rules.size())
            command = rules.at(currentRule);
        else
            return("An error occurred during the calculation. No rule found");

        /* Выполняем команду из 6-ти доступных.
         * Если не удается найти команду из списка,
         * то возвращаем ответ с ошибкой.
        */

        if(command.find("<-") != string::npos)
        {
            saveStateBefore();
            tapeIndex--;

            if (tapeIndex < 0)
            {
                tape.insert(0, "0");
                tapeIndex++;
            }
            saveStateAfter(command);
            currentRule = stoi(command.substr(command.find("<-") + 2)) - 1;
        }
        else if(command.find("->") != string::npos)
        {
            saveStateBefore();
            tapeIndex++;

            if (tapeIndex > tape.length())
            {
                tape.insert(tape.length(), "0");
            }
            saveStateAfter(command);
            currentRule = stoi(command.substr(command.find("->") + 2)) - 1;
        }
        else if(command.find("?") != string::npos)
        {
            saveStateBefore();
            if (tape[tapeIndex] == '0')
            {
                saveStateAfter(command);
                currentRule = stoi(command.substr(command.find("?") + 1, command.find(";"))) - 1;
                continue;
            }
            else
            {
                saveStateAfter(command);
                currentRule = stoi(command.substr(command.find(";") + 1)) - 1;
                continue;
            }
        }
        else if(command.find("V") != string::npos)
        {
            saveStateBefore();
            if (tape[tapeIndex] == '0')
                tape[tapeIndex] = '1';
            else
                return("An error occurred during the calculation. Error set label");

            saveStateAfter(command);
            currentRule = stoi(command.substr(command.find("V") + 1)) - 1;
        }
        else if(command.find("X") != string::npos)
        {
            saveStateBefore();
            if (tape[tapeIndex] == '1')
                tape[tapeIndex] = '0';
            else
                return("An error occurred during the calculation. Error erase label");

            saveStateAfter(command);
            currentRule = stoi(command.substr(command.find("X") + 1)) - 1;
        }
        else if(command.find("!") != string::npos)
        {
            return("Calculation was successful");
        }
        else
        {
            return("An error occurred during the calculation.");
        }
    }

    return("An error occurred during the calculation.");
}

/* Запись в файл состояния ленты до применения правила.
 * Если файл не найден, то он создается.
*/
void PostMachine::saveStateBefore()
{
    ofstream file_out;
    file_out.open("../PostMachine/result.txt", std::ios::app);
    file_out << tape << "\n";
    string carriage;
    for(unsigned long long i = 0; i < tape.length(); i++) if(i != tapeIndex) carriage += ' '; else carriage += '^';
    file_out << carriage << "\n";
    file_out.close();
}

/* Запись файл текущего правила и
 * состояния ленты (после применения правила).
*/
void PostMachine::saveStateAfter(string command)
{
    ofstream file_out;
    file_out.open("../PostMachine/result.txt", std::ios::app);
    file_out << command << "\n";
    file_out << tape << "\n";
    string carriage;
    for(unsigned long long i = 0; i < tape.length(); i++) if(i != tapeIndex) carriage += ' '; else carriage += '^';
    file_out << carriage << "\n";
    file_out << "**************************************************************\n";
    file_out.close();
}
