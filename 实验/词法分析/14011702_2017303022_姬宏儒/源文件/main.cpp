#include <fstream>
#include <iostream>

using namespace std;

#define ERROR_INTEGERLITERAL 100  //错误的数字状态码
#define ERROR_IDENTIFIER 101    //错误的标识符状态码
//特殊字符数量以及特殊字符集合
#define SEPARATORCOUNT 8
#define KEYWORDCOUNT 19
#define OPERATORCOUNT 7
char SEPARATOR[SEPARATORCOUNT] = {'[', ']', '(', ')', '{', '}', ',', ';'};
char OPERATOR[OPERATORCOUNT] = {'=', '<', '+', '-', '*', '.', '!'};
string KEYWORD[KEYWORDCOUNT] = {"class", "public", "static", "void", "main", "String", "extends", "return", "int", "boolean",
                                "if", "else", "while", "System", "length", "true", "false", "this", "new"
                               };

//判断字符是否为合法的分隔符
bool isSeparator(char c)
{
    for(int i = 0; i < SEPARATORCOUNT; i++)
    {
        if(SEPARATOR[i] == c)
        {
            return true;
        }
    }
    return false;
}

//判断是否为合法的操作符
bool isOperator(char c)
{
    for(int i = 0; i < OPERATORCOUNT; i++)
    {
        if(OPERATOR[i] == c)
        {
            return true;
        }
    }
    return false;
}

//判断是否为关键字
bool isKeyWord(string word)
{
    for(int i = 0; i < KEYWORDCOUNT; i++)
    {
        if(KEYWORD[i] == word)
            return true;
    }
    return false;
}

//判断字符是否为数字
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}

//判断字符是否为字母
bool isLetter(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

//忽略空白和制表符
char nextChar(FILE *infile)
{
    char cur = fgetc(infile);
    while(cur == '\t' || cur == ' ')
    {
        cur = fgetc(infile);
    }
    return cur;
}

int main()
{
    cout << "Please enter file to be analyzed: ";

    char filename[60];
    cin.getline(filename, 60);

    FILE *infile;
    while(!(infile=fopen(filename,"r")))   //读取进行词法分析的文件名
    {
        cout<<"not found the file or other error \n";
        cout << "Please enter file to be analyzed: ";
        cin.getline(filename, 60);
    }
    ofstream outfile;
    outfile.open("tokenOut.txt", ios::out | ios::trunc);  //程序结果输出文件
    outfile << "EOF" << "\n\n";
    char c;    //当前字符
    string word = "";  //记录当前单词
    int line = 1;      //记录行
    int state = 1;     //记录状态机状态，1为初始状态
    c = nextChar(infile);
    while(!feof(infile))     //不断读文件至文件结尾
    {
        state = 1;
        while(c == '\n')     //遇到换行符行数+1
        {
            line ++;
            c = nextChar(infile);
        }
        if(feof(infile))    //文件到尾结束
            break;
        if(isSeparator(c))  //当前字符为分隔符，合法的分隔符
        {
            //outfile << line << "\t\t" << c << "\t\t" << "SEPARATOR" << "\n\n";
            switch(c)
            {
            case '[':
                outfile << "[\t" << "LBRACKET" << "\n\n";
                break;
            case ']':
                outfile << "]\t" << "RBRACKET" << "\n\n";
                break;
            case '(':
                outfile << "(\t" << "LPAREN" << "\n\n";
                break;
            case ')':
                outfile << ")\t" << "RPAREN" << "\n\n";
                break;
            case '{':
                outfile << "{\t" << "LBRACE" << "\n\n";
                break;
            case '}':
                outfile << "}\t" << "RBRACE" << "\n\n";
                break;
            case ',':
                outfile << ",\t" << "COMMA" << "\n\n";
                break;
            case ';':
                outfile << ";\t" << "SEMI" << "\n\n";
                break;
            }
            c = nextChar(infile);
            continue;
        }
        else if(isOperator(c))   //当前字符为操作符
        {
            if(c == '.')      //.操作符特殊处理,下一个为数字报错
            {
                c = fgetc(infile);
                if(isDigit(c))
                {
                    word = ".";
                    while(!feof(infile) && c != ' ' && c != '\t' && c != '\n' && !isSeparator(c) && !isOperator(c))
                    {
                        word += c;
                        c = fgetc(infile);
                    }
                    outfile << "ERROR: ILLEGAL INTEGERLITERAL\t" << "line: " << line << "\t: " << word << "\n\n";
                    if(c == '\t' || c == ' ')
                        c = nextChar(infile);
                    continue;
                }
                else
                {
                    //outfile << line << "\t\t" << "." << "\t\t" << "OPERATOR" << "\n\n";
                    outfile << ".\t" << "DOT" << "\n\n";
                    continue;
                }
            }
            else    //合法的操作符
            {
                //outfile << line << "\t\t" << c << "\t\t" << "OPERATOR" << "\n\n";
                switch(c)
                {
                case '=':
                    outfile << "=\t" << "EQ" << "\n\n";
                    break;
                case '<':
                    outfile << "<\t" << "LT" << "\n\n";
                    break;
                case '+':
                    outfile << "+\t" << "PLUS" << "\n\n";
                    break;
                case '-':
                    outfile << "-\t" << "SUB" << "\n\n";
                    break;
                case '*':
                    outfile << "*\t" << "STAR" << "\n\n";
                    break;
                case '!':
                    outfile << "!\t" << "BANG" << "\n\n";
                    break;
                }
            }
            c = nextChar(infile);
            continue;
        }
        else if(c == '&')  //&的处理
        {
            if((c =fgetc(infile)) == '&')     //到达终止状态3
            {
                //outfile << line << "\t\t" << "&&" << "\t\t" << "OPERATOR" << "\n\n";
                outfile << "&&\t" << "AMPAMP" << "\n\n";
                c = nextChar(infile);
                continue;
            }
            else        //停留在状态4
            {
                if(c == ' ' || c == '\t')
                    c = nextChar(infile);
                outfile << "ERROR: ILLEGAL OPERATOR\t" << "line: " << line << "\t: " << "&" << "\n\n";
                continue;
            }
        }
        else if(isDigit(c))   //数字打头的
        {
            word = "";
            word += c;
            c = fgetc(infile);
            state = 5;     //合法整数状态
            while(!feof(infile) && c != ' ' && c != '\t' && c != '\n' && (c == '.' || !isOperator(c)) && !isSeparator(c))
            {
                word += c;
                if(!isDigit(c))
                {
                    state = ERROR_INTEGERLITERAL;     //非法整数
                }
                c = fgetc(infile);
            }
            if(state == ERROR_INTEGERLITERAL)    //非法整数
            {
                outfile << "ERROR: ILLEGAL INTEGERLITERAL\t" << "line: " << line << "\t: " << word << "\n\n";
            }
            else if(state == 5)    //合法整数
            {
                //outfile << line << "\t\t" << word << "\t\t" << "INTEGERLITERAL" << "\n\n";
                outfile << word << "\t" << "INTEGERLITERAL" << "\n\n";
            }
            if(isSeparator(c) || isOperator(c) || c == '\n')
                continue;
            c = nextChar(infile);
            continue;
        }
        else if(isLetter(c))      //字母打头
        {
            state = 6;            //到达终止状态
            word = "";
            word += c;
            c = fgetc(infile);
            while(!feof(infile) && c != ' ' && c != '\t' && c != '\n' && !isSeparator(c) && !isOperator(c))
            {
                if(state == 7)   //若当前状态为7
                {
                    if(isDigit(c) || isLetter(c))   //识别数字和字母到6
                    {
                        state = 6;
                    }
                    else        //非法标识符
                    {
                        state = ERROR_IDENTIFIER;
                    }
                }
                else if(state == 6)   //当前状态为6
                {
                    if(isDigit(c) || isLetter(c))     //识别数字和字母到6
                    {
                        state = 6;
                    }
                    else if(c == '_')        //识别下划线到7
                    {
                        state = 7;
                    }
                    else             //非法标识符
                    {
                        state = ERROR_IDENTIFIER;
                    }
                }
                word += c;
                c = fgetc(infile);
            }
            if(state == 6)    //合法标识符
            {
                if(isKeyWord(word))    //关键字特殊处理
                {
                    if(word == "System")     //System.out.println处理
                    {
                        word += c;
                        for(int j = 0; j < 11; j++)
                        {
                            c = fgetc(infile);
                            word += c;
                        }
                        if(word == "System.out.println")
                        {
                            //outfile << line << "\t\t" << word << "\t\t" << "KEYWORD" << "\n\n";
                            outfile << word << "\t" << "KEYWORD" << "\n\n";
                        }
                        else
                        {
                            c = fseek(infile,0,SEEK_CUR);    //还不知道为什么
                            //outfile << line << "\t\t" << "System" << "\t\t" << "IDENTIFIER" << "\n\n";
                            outfile << "System" << "\t" << "IDENTIFIER" << "\n\n";
                        }
                    }
                    else
                    {
                        //outfile << line << "\t\t" << word << "\t\t" << "KEYWORD" << "\n\n";
                        outfile << word << "\t" << "KEYWORD" << "\n\n";
                    }
                }
                else
                {
                    //outfile << line << "\t\t" << word << "\t\t" << "IDENTIFIER" << "\n\n";
                    outfile << word << "\t" << "IDENTIFIER" << "\n\n";
                }
            }
            else        //非法标识符
            {
                outfile << "ERROR: ILLEGAL IDENTIFIER\t" << "line: " << line << "\t: " << word << "\n\n";
            }
            if(isSeparator(c) || isOperator(c) || c == '\n')
                continue;
            c = nextChar(infile);
            continue;
        }
        else      //所有未识别的字符和单词
        {
            word = "";
            word += c;
            c = fgetc(infile);
            while(!feof(infile) && c != ' ' && c != '\t' && c != '\n' && !isSeparator(c) && !isOperator(c))
            {
                word += c;
                c = fgetc(infile);
            }
            if(word.length() == 1)    //非法字符
                outfile << "ERROR: ILLEGAL CHARACTER\t" << "line: " << line << "\t: " << word << "\n\n";
            else                      //非法单词
                outfile << "ERROR: ILLEGAL IDENTIFIER\t" << "line: " << line << "\t: " << word << "\n\n";
            c = nextChar(infile);
            continue;
        }
    }
    outfile << "EOF" << "\n\n";
    fclose(infile);      //关闭输入文件
    outfile.close();     //关闭输出文件
    cout << "word analyze end \n";
    cin.get();
    cin.get();
    return 0;
}
