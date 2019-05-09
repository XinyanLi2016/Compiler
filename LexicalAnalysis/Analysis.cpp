// C语言词法分析器
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
//符号表
struct symbol{
string token;
int id;
}table[1001];


//保留字 1- 32
static string reserveWord[32] =
{
    "auto",     "break",    "case",   "char",   "const",   "continue",
    "default",  "do",       "double", "else",   "enum",    "extern",
    "float",    "for",      "goto",    "if",     "int",    "long",
    "register",  "return",  "short",   "signed",  "sizeof",  "static",
    "struct",   "switch",   "typedef",  "union",  "unsigned",  "void",
    "volatile", "while"
};
//界符运算符 33- 77
static string operatorOrDelimiter[45] = {
    "-", "--", "-=", "->", "!", "!=", "%", "%=", "&", "&&",
    "&=", "(", ")", "*", "*=", ",", ".", "/", "/=", ":",
    ";", "?", "[", "]", "^", "^=", "{", "|", "||", "|=",
    "}", "~", "+", "++", "+=", "<", "<<", "<<=", "=", "==",
    ">", ">=", ">>", ">>="
};
/* 不要修改这个标准输入函数 */

/**********查找保留字********/
int isReserve(string s){
    for (int i = 0; i < 32; i++)
    {
        if (reserveWord[i]==s)
        {//若成功查找，则返回种别码
            return i + 1;//返回种别码
        }
    }
    return -1;//否则返回-1，代表查找不成功，即为标识符
}
/**********是否为字母********/
bool isLetter(char letter)
{//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if ( (letter >= 'a'&&letter <= 'z') || (letter >= 'A'&&letter <= 'Z')|| letter=='_')
        return true;
    else
        return false;
}
/**********是否为数字********/
bool isDigit(char digit)
{
    if (digit >= '0'&&digit <= '9')
        return true;
    else
        return false;
}
void Scanner(string& prog, int &syn, int &pProject, int &cnt){

    char ch;//作为判断使用
    ch = prog[pProject];
    while (ch == ' ' || ch == '\n' || ch == '\t' || ch== '\v' || ch=='\r')
    {//过滤空格，防止程序因识别不了空格而结束
        pProject++;
        ch = prog[pProject];
    }
    if( isLetter(prog[pProject]) ){
        table[cnt].token += ch;
        pProject ++; //指针移动一位
        while( isLetter(prog[pProject]) || isDigit(prog[pProject]) ){
            ch = prog[pProject];
            table[cnt].token += ch;
            pProject ++;
        }
        //table[cnt].token += '\0';
        if(isReserve(table[cnt].token) >= 0){//是保留字
            table[cnt].id = isReserve(table[cnt].token); //保存保留字的标号

        }
        else if( isReserve(table[cnt].token) == -1){ //是标识符
            table[cnt].id = 81;
        }
        return;
    }
    else if( isDigit(prog[pProject]) ){
        table[cnt].token += prog[pProject];
        pProject ++; //指针移动一位
        while(isDigit(prog[pProject]) ){
            table[cnt].token += prog[pProject];
            pProject ++;
        }
        table[cnt].id = 80; //常数
        return;
    }
    else if(ch == '/'){
        table[cnt].token += prog[pProject];
        pProject ++;
        //cout<< "find / " <<endl;
        char str = prog[pProject];
        if(str == '/'){ //单行注释
            table[cnt].token += prog[pProject];
             pProject ++; //指针移动一位
            while(prog[pProject] != '\n'){
                table[cnt].token += prog[pProject];
                pProject ++;
            }
            table[cnt].id = 79;
        }
        else if(str == '*'){ //多行注释
            table[cnt].token += prog[pProject];
             pProject ++; //指针移动一位
            while(prog[pProject] != '*' || prog[pProject+1] != '/'){
                table[cnt].token += prog[pProject];
                pProject ++;
            }
            table[cnt].token += '*';
            table[cnt].token += '/';
            pProject++;
            table[cnt].id = 79;
        }
        else if(str == '='){
            table[cnt].token += prog[pProject];
            table[cnt].id = 51;
        }
        else{
            table[cnt].id = 50;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch == '\"'){  //处理双引号情况
        table[cnt].token += prog[pProject];
        table[cnt].id = 78;


        pProject ++; //指针移动一位
        cnt++; //token增加
        while(prog[pProject] != '\"' ){
            table[cnt].token += prog[pProject];
            pProject ++;
        }
        table[cnt].id = 81;
        cnt ++;
        table[cnt].token+= prog[pProject];
        table[cnt].id = 78;

        pProject++;//指针下移
        return;
    }
    else if( ch =='-'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '-'){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 34;
        }
        else if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 35;
        }
        else if(prog[pProject+1] == '>'){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 36;
        }
        else{
            table[cnt].id = 33;
        }
        pProject++;//指针下移
        return ;
    }
    else if(ch == '!'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 38;
        }else {
            table[cnt].id = 37;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch == '%'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 40;
        }else {
            table[cnt].id = 39;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch == '&'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '&'){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 42;
        } else if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 43;
        }
        else {
            table[cnt].id = 41;
        }
        pProject++;//指针下移
        return;
    }
    else if( ch == '*'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 47;
        }else {
            table[cnt].id = 46;
        }
        pProject++;//指针下移
        return;
    }
    else if( ch == '^'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 58;
        }else {
            table[cnt].id = 57;
        }
        pProject++;//指针下移
        return;
    }
    else if( ch == '|'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '|'){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 61;
        }
        else  if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 62;
        }
        else {
            table[cnt].id = 60;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch == '+'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '+'){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 66;
        }
        else  if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 67;
        }
        else {
            table[cnt].id = 65;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch == '<'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '<'){
            table[cnt].token += prog[++pProject];
            if(prog[pProject+1] == '='){
                table[cnt].id = 70;
            }else
            table[cnt].id = 69;
        }
        else  if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 71;
        }
        else {
            table[cnt].id = 68;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch == '='){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 73;
        }else {
            table[cnt].id = 72;
        }
        pProject++;//指针下移
        return;
    }
    else if( ch == '>'){
        table[cnt].token += prog[pProject];
        if(prog[pProject+1] == '>'){
            table[cnt].token += prog[++pProject];
            if(prog[pProject+1] == '='){
                table[cnt].id = 77;
            }else
            table[cnt].id = 76;
        }
        else  if(prog[pProject+1] == '='){
            table[cnt].token += prog[++pProject];
            table[cnt].id = 75;
        }
        else {
            table[cnt].id = 74;
        }
        pProject++;//指针下移
        return;
    }
    else if(ch=='(' || ch == ')' || ch == ',' || ch == '.' ||ch == ':' || ch == ';' ||ch== '?' ||ch == '[' ||ch == ']' || ch == '{' || ch == '}' ||ch == '~'){
         table[cnt].token += prog[pProject];
         for (int i = 0; i < 45; i++)
        {
            if (operatorOrDelimiter[i]== table[cnt].token)
            {//若成功查找，则返回种别码
            table[cnt].id = i + 33;
            break;
            }
        }
        pProject++;
        return;
    }
    else{
        syn = 0;
        return;
    }
}
void read_prog(string& prog)
{
    char c;
    while(scanf("%c",&c)!=EOF)
    {
        prog += c;
    }
}
/* 你可以添加其他函数 */
void Analysis()
{
    string prog;
    read_prog(prog);
    //prog += '$'; //表示为终结符
    /********* Begin *********/
    //test 看一看prog里面输入了什么  发现还是原来的东西
    /*
    printf("printf ends \n");
    for(int i = 0; i < prog.size(); i++)
    {
        printf("%c",prog[i]);
    }
    printf("already input \n");
    */

    int syn = -1; //状态
    int cnt  = 0; //标号
    //把文件开始的空格过滤掉
    int pProject = 0;//源程序指针
    while(syn != 0){
        Scanner( prog, syn, pProject, cnt);  //存在token里
        //cout<<cnt <<endl;
        //if(pProject >= prog.length()) break;
        cnt ++;
    }
    for(int i = 0; i < cnt-2; i ++)
        cout<<i+1<<": <"<<table[i].token<<","<<table[i].id<<">"<<endl;
    cout<<cnt-1<<": <"<<table[cnt-2].token<<","<<table[cnt-2].id<<">";

    //最后一行没有回车
    /********* End *********/

}

int main()
{
    Analysis();
    return 0;
}

