#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

string token;
char c;
char out[32];
int flag_count=0;
const char chars[] = {';',',', '+', '-', '*', '/', '=', '>', '<', '{', '}', '[', ']', '&', '|', '!', '(', ')'};

bool isKeyword(string p);        //§PÂ_keywords
bool isNumber(string p);         //§PÂ_number
bool isFloat(string p);           //§PÂ_float
bool isIdentifier(string p);     //§PÂ_Identifier
bool isOneOperators(string p);   //§PÂ_one_op
bool isTwoOperators(string p);   //§PÂ_two_op
bool isSpecialSymbols(string p);  //§PÂ_¯S®í²Å¸¹

string Token_all(ifstream &pfin);   //analyze all num and word
string Token_op(ifstream &pfin);    //analyze two op
string Token_comment(ifstream &pfin); //analyze comment
string Token_char(ifstream &pfin);  //analyze char '' '\n'


int main()
{
    ifstream fin("test.txt");
    ofstream fout;
    if(fin.fail())
    {
        cout << "cannot open the file!##ERROR##" << endl;
        exit(1);
    }
    else
    {

        string output = "token.txt";
        for (int j = 0; j < output.length(); j++)
            out[j] = output[j];
        fout.open(out);
    }

    int counter = 1, changLine = 1, repeat = 0;

    while(flag_count == 1 || fin.get(c)) //Åªchar,use get()
    {
        flag_count = 0;
        if(changLine == 1)
        {
            changLine = 0;
            fout << "Line  "<< counter << ":" << endl;
        }
        if(strchr("/", c)) //§PÂ_°£¸¹
        {
            token = Token_comment(fin);
            cout << token << endl;

            if (token == "/")
                fout << '\t' << '\t' << "<Operators>" << '\t' << '\t' <<'\t' <<": " << token << endl;
        }

        if(strchr("=!><&|", c))   //§PÂ_operator
        {
            token = Token_op(fin);
            cout << token << endl;

            if(isTwoOperators(token))
                fout << '\t' << '\t' << "<Operators>" << '\t' << '\t' <<'\t' << ": " << token << endl;
            repeat = 1;
        }

        if(isalnum(c) || strchr("_", c)) //§PÂ_ID KEY FLOAT ERROR
        {
            token = Token_all(fin);
            cout << token << endl;

            if(isKeyword(token))
                fout << '\t' << '\t' << "<Keywords>" << '\t' <<'\t' <<'\t' << ": " << token << endl;
            else if(isNumber(token))
                fout << '\t' << '\t' << "<Number>" << '\t' << '\t' <<'\t' << ": " << token << endl;
            else if(isFloat(token))
                fout << '\t' << '\t' << "<Number>" << '\t' << '\t' <<'\t' << ": "<< token << endl;
            else if(isIdentifier(token))
                fout << '\t' << '\t' << "<Identifier>" << '\t' << '\t' << ": " << token << endl;
            else
                fout << '\t' << '\t' << "<Error>" << '\t' << '\t' << '\t' <<'\t' << ": "<< token << endl;
        }

        if(strchr("=!><&|", c) && repeat == 0)
        {
            token = Token_op(fin);
            cout << token << endl;

            if(isTwoOperators(token))
                fout << '\t' << '\t' << "<Operators>" << '\t' << '\t'<< '\t'  << ": " << token << endl;
        }

        if(strchr("+-*", c)) //§PÂ_char,ªÅ¥Õ³B²z
        {
            token = c;
            cout << token << endl;

            if(isOneOperators(token))
                fout << '\t' << '\t' << "<Operators>" << '\t' << '\t' << '\t' <<": " << token << endl;
        }

        if(strchr("/", c))
        {
            token = Token_comment(fin);
            cout << token << endl;

            if(token == "/")
                fout << '\t' << '\t' << "<Operators>" << '\t' << '\t'<< '\t' << ": " << token << endl;
        }

        if(strchr("{}[]();,", c))
        {
            token = c;
            cout << token << endl;

            if(isSpecialSymbols(token))
                fout << '\t' << '\t' << "<Special Symbols>" << '\t' << ": " <<  token << endl;
            flag_count=0;
        }

        if(strchr("'", c))
        {
            token = Token_char(fin);
            cout << token << endl;

            fout << '\t' << '\t' << "<Char>" << '\t' << '\t' << '\t' <<'\t' << ": " << token << endl;
        }

        if(c == '\n')
        {
            counter++;
            changLine = 1;
        }
            repeat = 0;

		}
    fin.close();
    fout.close();
}

string Token_all(ifstream &pfin)
{
	string tempStr = "";
    int i = 0, stop = 0;
	while(!(c == '\n' || c == ' '))
    {
        for(i = 0; i < 19; i++)
            if(c == chars[i]) stop = 1;

        if(stop == 1) break;


		tempStr += c;
		pfin.get(c);
	}

	return tempStr;
}

string Token_op(ifstream &pfin)
{
	string tempStr = "", tempStr2 = "";

	while(1)
    {
        tempStr += c;
        tempStr2 += c;
        pfin.get(c);
        if (isalnum(c)) flag_count = 1;
        tempStr += c;
        if(tempStr == "==" || tempStr == "!="){break;}
        else if(tempStr == ">=" || tempStr == "<=") {break;}
        else if(tempStr == "&&" || tempStr == "||") {break;}
        else
            return tempStr2;

	}

	return tempStr;
}

string Token_comment(ifstream &pfin)
{
	string tempStr = "", tempStr2 = "";
	int flag = 0;

	while(1)
    {
        tempStr += c;
        tempStr2 += c;
        pfin.get(c);
        if (isalnum(c)) flag_count = 1;
        tempStr += c;
        if(tempStr == "//")
        {
            flag = 1;
            break;
        }
        else
            return tempStr2;

	}

	while(flag = 1)
    {
        pfin.get(c);
        if(c == '\n')
        {
            string no = "N/A";
            return no;
            break;
        }
    }
}

string Token_char(ifstream &pfin)
{
	string tempStr = "";

	while(1)
    {
        tempStr += c;
        pfin.get(c);

        if(c == '\'')
        {
            tempStr += c;
            break;
        }
	}
    return tempStr;
}


bool isKeyword(string p)
{
    const string keywords[] = {"int", "char", "float", "double", "return", "if", "else", "while", "break", "for", "print"};

	for (int i = 0; i < 9; i++)
    {
		if (p == keywords[i])
			return true;
	}

	return false;
}

bool isNumber(string p)
{
    for (int i = 0; i < p.length(); i++)
    {
		if (!isdigit(p[i]))
			return false;
	}

    return true;
}


bool isFloat(string p)
{
	int dots = 0;

	for (int i = 0; i < p.length(); i++)
    {
		if (!isdigit(p[i]) && p[i] != '.')
			return false;
		else if (p[i] == '.')
			dots++;
		if (dots >= 2)
			return false;
	}

	return true;
}

bool isIdentifier(string p)
{
	if(isalpha(p[0]))
    {
		for (int i = 0; i < p.length(); i++)
		{
			if (!isalnum(p[i]))
				return false;
		}
    }
    else
		return false;

	return true;
}

bool isOneOperators(string p)
{
    const string one_op[] = {"+", "-", "*", "/", "&&", "||"};

	for (int i = 0; i < 6; i++)
    {
		if (p == one_op[i])
			return true;
	}

	return false;
}

bool isTwoOperators(string p)
{
    const string two_op[] = {"=", "==", "!", "!=", "<", ">", "<=", ">=", "&&", "||"};

	for (int i = 0; i < 10; i++)
    {
		if (p == two_op[i])
			return true;
	}

	return false;
}

bool isSpecialSymbols(string p)
{
    const string specialsymbols[] = {"{", "}", "[", "]", "{}", "[]", ";", ",", "(", ")", "()"};

	for (int i = 0; i < 11; i++)
    {
		if (p == specialsymbols[i])
			return true;
	}

	return false;
}
