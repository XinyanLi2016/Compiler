// C���Դʷ�������
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
//���ű�
struct symbol{
string token;
int id;
}table[1001];


//������ 1- 32
static string reserveWord[32] =
{
    "auto",     "break",    "case",   "char",   "const",   "continue",
    "default",  "do",       "double", "else",   "enum",    "extern",
    "float",    "for",      "goto",    "if",     "int",    "long",
    "register",  "return",  "short",   "signed",  "sizeof",  "static",
    "struct",   "switch",   "typedef",  "union",  "unsigned",  "void",
    "volatile", "while"
};
//�������� 33- 77
static string operatorOrDelimiter[45] = {
    "-", "--", "-=", "->", "!", "!=", "%", "%=", "&", "&&",
    "&=", "(", ")", "*", "*=", ",", ".", "/", "/=", ":",
    ";", "?", "[", "]", "^", "^=", "{", "|", "||", "|=",
    "}", "~", "+", "++", "+=", "<", "<<", "<<=", "=", "==",
    ">", ">=", ">>", ">>="
};
/* ��Ҫ�޸������׼���뺯�� */

/**********���ұ�����********/
int isReserve(string s){
    for (int i = 0; i < 32; i++)
    {
        if (reserveWord[i]==s)
        {//���ɹ����ң��򷵻��ֱ���
            return i + 1;//�����ֱ���
        }
    }
    return -1;//���򷵻�-1��������Ҳ��ɹ�����Ϊ��ʶ��
}
/**********�Ƿ�Ϊ��ĸ********/
bool isLetter(char letter)
{//ע��C���������»���ҲΪ��ʶ����һ���ֿ��Է����ײ��������ط�
    if ( (letter >= 'a'&&letter <= 'z') || (letter >= 'A'&&letter <= 'Z')|| letter=='_')
        return true;
    else
        return false;
}
/**********�Ƿ�Ϊ����********/
bool isDigit(char digit)
{
    if (digit >= '0'&&digit <= '9')
        return true;
    else
        return false;
}
void Scanner(string& prog, int &syn, int &pProject, int &cnt){

    char ch;//��Ϊ�ж�ʹ��
    ch = prog[pProject];
    while (ch == ' ' || ch == '\n' || ch == '\t' || ch== '\v' || ch=='\r')
    {//���˿ո񣬷�ֹ������ʶ���˿ո������
        pProject++;
        ch = prog[pProject];
    }
    if( isLetter(prog[pProject]) ){
        table[cnt].token += ch;
        pProject ++; //ָ���ƶ�һλ
        while( isLetter(prog[pProject]) || isDigit(prog[pProject]) ){
            ch = prog[pProject];
            table[cnt].token += ch;
            pProject ++;
        }
        //table[cnt].token += '\0';
        if(isReserve(table[cnt].token) >= 0){//�Ǳ�����
            table[cnt].id = isReserve(table[cnt].token); //���汣���ֵı��

        }
        else if( isReserve(table[cnt].token) == -1){ //�Ǳ�ʶ��
            table[cnt].id = 81;
        }
        return;
    }
    else if( isDigit(prog[pProject]) ){
        table[cnt].token += prog[pProject];
        pProject ++; //ָ���ƶ�һλ
        while(isDigit(prog[pProject]) ){
            table[cnt].token += prog[pProject];
            pProject ++;
        }
        table[cnt].id = 80; //����
        return;
    }
    else if(ch == '/'){
        table[cnt].token += prog[pProject];
        pProject ++;
        //cout<< "find / " <<endl;
        char str = prog[pProject];
        if(str == '/'){ //����ע��
            table[cnt].token += prog[pProject];
             pProject ++; //ָ���ƶ�һλ
            while(prog[pProject] != '\n'){
                table[cnt].token += prog[pProject];
                pProject ++;
            }
            table[cnt].id = 79;
        }
        else if(str == '*'){ //����ע��
            table[cnt].token += prog[pProject];
             pProject ++; //ָ���ƶ�һλ
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
        pProject++;//ָ������
        return;
    }
    else if(ch == '\"'){  //����˫�������
        table[cnt].token += prog[pProject];
        table[cnt].id = 78;


        pProject ++; //ָ���ƶ�һλ
        cnt++; //token����
        while(prog[pProject] != '\"' ){
            table[cnt].token += prog[pProject];
            pProject ++;
        }
        table[cnt].id = 81;
        cnt ++;
        table[cnt].token+= prog[pProject];
        table[cnt].id = 78;

        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
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
        pProject++;//ָ������
        return;
    }
    else if(ch=='(' || ch == ')' || ch == ',' || ch == '.' ||ch == ':' || ch == ';' ||ch== '?' ||ch == '[' ||ch == ']' || ch == '{' || ch == '}' ||ch == '~'){
         table[cnt].token += prog[pProject];
         for (int i = 0; i < 45; i++)
        {
            if (operatorOrDelimiter[i]== table[cnt].token)
            {//���ɹ����ң��򷵻��ֱ���
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
/* ���������������� */
void Analysis()
{
    string prog;
    read_prog(prog);
    //prog += '$'; //��ʾΪ�ս��
    /********* Begin *********/
    //test ��һ��prog����������ʲô  ���ֻ���ԭ���Ķ���
    /*
    printf("printf ends \n");
    for(int i = 0; i < prog.size(); i++)
    {
        printf("%c",prog[i]);
    }
    printf("already input \n");
    */

    int syn = -1; //״̬
    int cnt  = 0; //���
    //���ļ���ʼ�Ŀո���˵�
    int pProject = 0;//Դ����ָ��
    while(syn != 0){
        Scanner( prog, syn, pProject, cnt);  //����token��
        //cout<<cnt <<endl;
        //if(pProject >= prog.length()) break;
        cnt ++;
    }
    for(int i = 0; i < cnt-2; i ++)
        cout<<i+1<<": <"<<table[i].token<<","<<table[i].id<<">"<<endl;
    cout<<cnt-1<<": <"<<table[cnt-2].token<<","<<table[cnt-2].id<<">";

    //���һ��û�лس�
    /********* End *********/

}

int main()
{
    Analysis();
    return 0;
}

