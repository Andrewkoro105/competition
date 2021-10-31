#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <Windows.h>
#include <ctime>
using namespace std;

string word;
vector<string> words{};
bool start = false;

void search(vector<string> bufferWords, string beginning);

int main()
{
    system("cls");
    SetConsoleCP(1251);// установка кодовой страницы win-cp 1251 в поток ввода
    SetConsoleOutputCP(1251); // установка кодовой страницы win-cp 1251 в поток вывода

    start = (_access("а.txt", 00) == -1) || 0;

    if (start)
    {
        string line;
        vector<char> alphabet = {'а'};

        ifstream mainText("russian_nouns.txt"); // окрываем файл для чтения (https://harrix.dev/blog/2018/russian-nouns/)
        ofstream letterText;
        letterText.open("а.txt");

        if (mainText.is_open())
        {
            cout << "|                   Loading                    |\n ";
            while (getline(mainText, line))
            {
                if (alphabet[alphabet.size() - 1] != line[0])
                {
                    string str = { line[0] };
                    letterText.close();//закрытие старого файла
                    letterText.open(str + (string)".txt", ios::app);// и открытия нового

                    cout << "-";
                    alphabet.push_back(line[0]);//добавление данных в алфавит
                    letterText << line << '\n';
                }
                else
                {
                    letterText << line << '\n';
                }
            }
        }
        else
        {
            cout << "unable to open the file\n";
        }
        mainText.close();     // закрываем файл
        cout << '\n';
    }

    while (true)
    {
        words = {};
        cout << "в ведите слово :";
        cin >> word;

        if (word == "stop")
            break;
        else if (word == "cls")
            system("cls");


        int startTime = clock();

        string bufferWord = "";
        for (unsigned i = 0; i < word.size(); i++)
        {
            if (bufferWord.size() == 0)
            {
                bufferWord += word[i];
            }
            else
            {
                bool buffer = 1;
                for (unsigned i2 = 0; i2 < bufferWord.size(); i2++)
                {
                    if (bufferWord[i2] == word[i])
                        buffer = 0;
                }

                if (buffer)
                    bufferWord += word[i];
            }

        }

        for (unsigned i = 0; i < bufferWord.size(); i++)
        {
            ifstream letterText(bufferWord[i] + (string)".txt");
            vector<string> bufferWords{};
            string line;

            while (getline(letterText, line))
            {
                bufferWords.push_back(line + "\n");
            }

            search(bufferWords, { bufferWord[i] });
        }
        int endTime = clock();

        cout << "слова состаящие из букв этого слова : " << endl;
        for (unsigned i = 0; i < words.size(); i++)
            cout << words[i];

        cout << "\nбыло найдено " << words.size() << (words.size() > 4  ? " слов" : " слова") << " за " << ((endTime - startTime) / 1000) << '.' << ((endTime - startTime) % 1000) << "секунд\n\---------------------------------------------------------------------\n";

    }
    return 0;
}

void search(vector<string> bufferWords, string beginning)
{
    string sifted = "";
    //поиск не использованых букв
    {
        string bufferBeginning = beginning;
        for (unsigned i = 0; i < word.size(); i++)
        {
            if (bufferBeginning.size() == 0)
            {
                sifted += word[i];
            }
            else
            for (unsigned i2 = 0; i2 < bufferBeginning.size(); i2++)
            {
                if (word[i] == bufferBeginning[i2])
                {
                    bufferBeginning.erase(i2, 1);
                    break;
                }
                else
                    sifted += word[i];
            }
        }
    }

    char previousLetter = 'a';
    bool branding = 0;
    int removedItems = 0;
    vector<string> newBufferWords;

    for (unsigned i = 0; i < bufferWords.size(); i++)
    {
        char nowLetter = bufferWords[i][beginning.size()];
        if (nowLetter == '\n')
        {
            words.push_back(bufferWords[i]);
            continue;
        }
        else
        {
            if (previousLetter != nowLetter)
            {
                if (branding)
                {
                    search(newBufferWords, beginning + previousLetter);//абвгдеёжзийклмнопрстуфхцчшщъыьэюя-абвгдеёжзийклмнопрстуфхцчшщъыьэюя-абвгдеёжзийклмнопрстуфхцчшщъыьэюя-абвгдеёжзийклмнопрстуфхцчшщъыьэюя-
                    newBufferWords.clear();
                }
                previousLetter = nowLetter;
                branding = 0;
                for (unsigned i = 0; i < sifted.size(); i++)
                    if (sifted[i] == nowLetter) 
                    {
                        branding = 1; 
                        break;
                    }
            }


            if (!branding)
            {
        
                bufferWords.erase(bufferWords.begin() + i);
                removedItems++;
                i--;
            }
            else
                newBufferWords.push_back(bufferWords[i]);
        }
    }
    if (branding)
        search(newBufferWords, beginning + previousLetter);
}