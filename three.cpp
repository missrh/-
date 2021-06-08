#include <bits/stdc++.h>
using namespace std;

const int maxn = 1005;

struct node {//左右文法结构
	char left;
	string right;
};

struct tree_node {
	string data;
	int child_length;
	int stratum;
	vector<tree_node*> children;
};

string inputstr;//存储输入字符长度，并进行整理
int index;//查看检验到的字符位置
char character;//记录当前字符
string token;//key值,用于存储需要检验的单词
map<string, int> Symbol;//变量字典
map<string, int> Digit;//数字字典
map<string, int>::iterator temp;
string reserve[100];//保留字数组
struct Wordinfo {//每个单词的信息结构体
	Wordinfo(int c, int i, string selfv , string v = "null") {//selfv 用于记录对应的单词， v用来表示是否对应单词的值
		category = c;
		index = i;
		selfvalue = selfv;
		value = v;
	}
	int category;//种别码
	int index;//索引位置
	string selfvalue;//单词
	string value;//单词值
};
string iarray[maxn];//记录数字数组
int iarray_index = 0;
int iarray_length = 0;
string varray[maxn];//记录字符长度
int varray_index = 0;
int varray_length = 0;
int str1_length = 0;


vector <char> sta , vs;
vector <node> input;//对应的各行文法集合
map <char , int>mp; //记录非终结符对应的索引值
map <char , int>mp1; //记录终结符对应的索引值
vector <char> non_c;//non_c非终结符符号集合
vector <char> ter_c;//终结符符号集合
set <char> first[maxn];
set <char> follow[maxn];
int f[maxn] , f1[maxn];
int tableMap[maxn][maxn];
int n;

string LL1[maxn][maxn];
string H[maxn];
string L[maxn];
int H_length;
int L_length;
stack<string> als;//分析栈
stack<tree_node*> als_tree;//分析栈
string flag_endding;
bool change_row_flag;
stack<tree_node*> als_tree_deep;

string tostring(char letter){//转化单个字符位字符串
	char w[2] = {letter,0};
	string word = w;
	return word;
}
int findH(string word){//查看字符串是否在非终结符数组中，如果在返回对应的行，否则返回-1代表出错
	for (int i = 0; i < H_length; i++) {
		if (word  == H[i])
		{
			return i;
		}
	}
	return -1;
}
int findL(string word){//查看字符串是否在终结符，如果在，返回对应的列，否则返回-1代表出错
	for (int i = 0; i < L_length; i++)
		//找到对应列    
	{
		if (word == L[i])
		{
			return i;
		}
	}
	return -1;
}
int error(int i, int cnt, int len, string p, string str){//当匹配出错时候，执行该函数，并且不进行后续匹配
	cout<<cnt<<"\t\t"<<p<<"\t\t";
	for (int q = i; q<len; q++){
		cout << str[q];
	}
	cout<<"\t\terror\n";
	flag_endding = "error";
	return len;//直接返回最后一位，用于终止程序
}
//
void analyze(string str, int len) {
	int cnt = 1;//用来记录分析步骤
	int i = 0;//充当输入字符串str的索引
	char p[200] = "#E";//用来将栈中内容输出来实现步骤可视化 
	int p_index = 2;//初始化为第三个字符的索引值
	cout<<"Step\t\tStack\t\tString\t\tRule\n";
	while (i < len){//对字符串的每一个字符进行遍历，查看是否匹配
		int h, l;//储存可以用来查找LL(1)分析表的行和列值
		string ch = als.top();//将栈顶内容拿出来和当前字符串索引值进行匹配
		tree_node *now_node = new tree_node();
		now_node = als_tree.top();
		if (ch[0] >= 'A' && ch[0] <= 'Z'){//非终结符判断
			als.pop();//同时进行查找对应的行和列
			als_tree.pop();
			h = findH(ch);
			l = findL(tostring(str[i]));//因为str[i]读取出来就是一个char类型字符，所以需要使用自定义的函数进行字符串转化
			if (h != -1 && l != -1){
				int len_2 = LL1[h][l].length();//LL1是一个string类型的二维数组，每一个值都是string类型，所以有length()方法可以记录长度
				if (LL1[h][l]=="null"){
					//null代表匹配不到，可以直接通过i = len 进行退出while循环终止程序
					i = error(i, cnt, len, p, str);
					continue;
				}
				cout<<cnt<<"\t\t"<<p<<"\t\t";
				if (p[p_index - 1] != '#'){
					//如果此时栈顶不是#字符，则减少一个索引位，就相当于可视化中的出栈
					p_index--;
					p[p_index] = '\0';
					
				}
				//查看输入字符串"->"后面的字符是否为空值，如果是空值，执行减少一个索引位，就相当于可视化中的出栈
				//如果不是空值，并且则将箭头后面的值逆序覆盖刚刚的索引位，并且继续增加索引位来存储
				if (LL1[h][l][2] != '$'){
					for (int q = len_2 - 1; q > 1; q--){
						p[p_index++] = LL1[h][l][q];
						als.push(tostring(LL1[h][l][q]));
						now_node->child_length = now_node->child_length+1;
						tree_node *child = new tree_node();
						child->child_length = 0;
						child->stratum = now_node->stratum + 1;
						child->data = tostring(LL1[h][l][q]);
						now_node->children.push_back(child);
						als_tree.push(child);
					}
				}
				else{
					now_node->child_length++;
					tree_node *child = new tree_node();
					child->child_length = 0;
					child->data = "$";
					child->stratum = now_node->stratum + 1;
					now_node->children.push_back(child);
				}
				for (int q = i; q < len; q++){
					cout << str[q];
				}
				cout<<"\t\t"<<ch<<LL1[h][l]<<"\n";
			}
			else{//匹配不到行和列都存在，则报错
				i = error(i, cnt, len, p, str);
				continue;
			}
		}
		else{//终结符判断
			if (ch[0] == str[i]){//匹配成功当前非终结符
				als.pop();
				als_tree.pop();
				cout<<cnt<<"\t\t"<<p<<"\t\t";
				if (ch[0] == '#'&& str[i] == '#'){//成功匹配所有输入字符串
					cout<<"#\t\tsuccess\n";
					flag_endding ="success";
					return;
				}
				for (int q = i; q < len; q++){
					cout << str[q];
				}
				now_node->child_length++;
				tree_node *child = new tree_node();
				child->child_length = 0;
				child->data = ch;
				child->stratum = now_node->stratum + 1;
				now_node->children.push_back(child);
				cout<<"\t\t"<<ch<<"  match\n";
				p_index--;
				p[p_index] = '\0';
				i++;
			}
			else{//匹配失败报错
				i = error(i, cnt, len, p, str);
				continue;
				///报错      
			}
		}
		cnt++;
	}
}

void getFirst(char t) {
	if(!f[mp[t]]) return;
	for(int i = 0 ; i < n ; i++) {
		if(input[i].left == t) {
			if(!mp.count(input[i].right[0])) {
				first[mp[t]].insert(input[i].right[0]);
			}
			else {
				int num = 0;
				for(int j = 0 ; j < input[i].right.length() ; j++) {
					if(!mp.count(input[i].right[j])) {
						first[mp[t]].insert(input[i].right[j]);
						break;
					}
					getFirst(input[i].right[j]);
					bool flag = 0;
					for(auto it = first[mp[input[i].right[j]]].begin() ; it != first[mp[input[i].right[j]]].end() ; it++) {
						if(*it == '$')flag = 1;
						else first[mp[t]].insert(*it);
					}
					if(!flag) break;
					else {
						num += flag; flag = 0;
					}
				}
				if(num == input[i].right.length()) {
					first[mp[t]].insert('$');
				}
			}
		}
	}
	f[mp[t]] = 0;
}

void getFollow(char tmp) {
	if(!f1[mp[tmp]])return;
	for(int i = 0 ; i < n ; i++) {
		int index = -1;
		for(int j = 0 ; j < input[i].right.length() ; j++) {
			if(input[i].right[j] == tmp) {
					index = j; break;
			}
		}
		if(index > -1 && index < input[i].right.length() - 1) {
			char nxt = input[i].right[index + 1];
			if(!mp.count(nxt)) {
				follow[mp[tmp]].insert(nxt);
			}
			else {
				int flag = 0 , j;
				for(j = index + 1; j < input[i].right.length() ; j++) {
					nxt = input[i].right[j];
					if(!mp.count(nxt)) {
						follow[mp[tmp]].insert(nxt);
						break;
					}
					for(auto it = first[mp[nxt]].begin() ; it != first[mp[nxt]].end() ; it++) {
						if(*it == '$')flag = 1;
						else follow[mp[tmp]].insert(*it);
					}
					if(!flag)break;
				}
				if(j == input[i].right.length() && flag) {
					getFollow(input[i].left);
					for(auto it = follow[mp[input[i].left]].begin() ; it != follow[mp[input[i].left]].end() ; it++) {
						follow[mp[tmp]].insert(*it);
					}
				}
			}
		}
		else if(index == input[i].right.length() - 1 && input[i].left != input[i].right[index]) {
			getFollow(input[i].left);
			for(auto it = follow[mp[input[i].left]].begin() ; it != follow[mp[input[i].left]].end() ; it++) {
				follow[mp[tmp]].insert(*it);
			}
		}
	}
	f1[mp[tmp]] = 0;
}

void getTable() {
	for(int i = 0 ; i < n ; i++) {
		char tmp = input[i].right[0];
		if(!mp.count(tmp)) {
			if(tmp != '$')tableMap[mp[input[i].left]][mp1[tmp]] = i;
			else {
				for(auto it = follow[mp[input[i].left]].begin() ; it != follow[mp[input[i].left]].end() ; it++) {
					tableMap[mp[input[i].left]][mp1[*it]] = i;
				}
			}
		}
		else {
			int j;
			bool flag = 0;
			for(j = 0; j < input[i].right.length() ; j++) {
				tmp = input[i].right[j];
				if(mp1.count(tmp)) {
					tableMap[mp[input[i].left]][mp1[tmp]] = i;
					break;
				}
				for(auto it = first[mp[tmp]].begin() ; it != first[mp[tmp]].end() ; it++) {
					if(*it == '$')flag = 1;
					else tableMap[mp[input[i].left]][mp1[*it]] = i;
				}
				if(!flag)break;
			}
			if(j == input[i].right.length() && flag) {
				for(auto it = follow[mp[input[i].left]].begin() ; it != follow[mp[input[i].left]].end() ; it++) {
					tableMap[mp[input[i].left]][mp1[*it]] = i;
				}
			}
		}
	}
}

void showFirst() {
	cout << "first aggregate\n";
	for(int i = 0 ; i < non_c.size() ; i++) {
		cout << non_c[i] << ":";
		for(auto it = first[mp[non_c[i]]].begin() ; it != first[mp[non_c[i]]].end() ; it++) {
			cout << *it << " ";
		}
		cout << "\n";
	}
}

void showFollow() {
	cout << "follow aggregate\n";
	for(int i = 0 ; i < non_c.size() ; i++) {
		cout << non_c[i] << ":";
		for(auto it = follow[mp[non_c[i]]].begin() ; it != follow[mp[non_c[i]]].end() ; it++) {
			cout << *it << " ";
		}
		cout << "\n";
	}
}

void showTable() {
    cout << "Analysis table"<<endl;
	H_length = non_c.size();
	L_length = ter_c.size();
	for(int i = 0 ; i < ter_c.size() ; i++) {
		L[i] = tostring(ter_c[i]);
	}
	for(int i = 0 ; i < non_c.size() ; i++) {
		H[i] = tostring(non_c[i]);
		for(int j = 0 ; j < ter_c.size() ; j++) {
			if(tableMap[i][j] == -1) {
				LL1[i][j] = "null";
			}
			else {
				LL1[i][j] = "->" + input[tableMap[i][j]].right;
			}
		}
	}
	cout<<"\n";
	for(int i = 0 ; i < L_length ; i++) {
		cout<<"\t"<<L[i];
	}
	cout << "\n";
	for(int i = 0 ; i < H_length ; i++) {
		cout << H[i] << ":\t";
		for(int j = 0 ; j < L_length ; j++) {
				cout<<LL1[i][j]<<"\t";
		}
		cout << "\n";
	}
}

void getTree() {
	if(als_tree_deep.empty()){
		return;
	}
	else {
		tree_node *now_tree = als_tree_deep.top();
		als_tree_deep.pop();
		if(now_tree->data[0] <='Z'&& now_tree->data[0]>='A'){
			if(change_row_flag)
				
				for(int i = 0;i<now_tree->stratum;i++){
						cout<<"\t";
				}	
			else cout<<"";
			cout<<now_tree->data<<"----->\t";
		}
		else {
			if(change_row_flag)
				for(int i = 0;i<now_tree->stratum;i++)
					cout<<"\t";
			else cout<<"";
			if(now_tree->data == "i"){
				cout<<iarray[iarray_index]<<"\t";
				iarray_index++;
			}
			else if(now_tree->data == "v"){
				cout<<varray[varray_index]<<"\t";
				varray_index++;
			}
			else cout<<now_tree->data<<"\t";
		}
		if(now_tree->child_length == 0){
			cout<<"\n";
			change_row_flag = true;
		}
		else {
			if(now_tree->child_length != 1 || now_tree->data[0] <='Z'&& now_tree->data[0] >= 'A'){
				for(int i = 0; i<now_tree->child_length;i++){
					als_tree_deep.push(now_tree->children[i]);
				}
				change_row_flag = false;
			}
			else {
				cout<<"\n";
				change_row_flag = true;
			}
		}
		getTree();
	}
}

void init_reserve() {//定义的保留字列表
	reserve[0] = "if";
	reserve[1] = "then";
	reserve[2] = "else";
	reserve[3] = "while";
	reserve[4] = "begin";
	reserve[5] = "do";
	reserve[6] = "end";
}
void getChar() {//得到下一个字符
	character = inputstr[index++];
}
void getnbc() {//去除空格的读取字符
	while (character == ' ') {
		getChar();
	}
}
void concat() {//拼接字符成一个单词用于检验
	token = token + character;
}
bool letter() {//判断是否是字母
	if ((character >= 'A'&&character <= 'Z') || (character >= 'a'&&character <= 'z'))
		return true;
	return false;
}
bool digit() {//判断是否是数字
	if (character >= '0'&&character <= '9')
		return true;
	return false;
}
void retract() {//当找到一个单词后，回退一个字符，方便寻找下一次的单词的查找
	character = ' ';
	index--;
}
int Reserve() {//查看保留字表
	for (int i = 0; i < 100; i++)
		if (reserve[i] == token)return i;
	return -1;
}
string symbol() {//查看变量字典，如果已经存在则直接找到对应信息，如果不存在则添加
	temp = Symbol.find(token);
	if (temp != Symbol.end()) {
		return temp->first;
	}
	else {
		Symbol[token] = Symbol.size();
		return token;
	}
}
string constant() {//查看数字字典，如果已经存在则直接找到对应信息，如果不存在则添加
	temp = Digit.find(token);
	if (temp != Digit.end()) {
		return temp->first;
	}
	else {
		Digit[token] = Digit.size();
		return token;
	}
}
Wordinfo error() {//对于无法辨别的单词，复制该单词一个error结构
	cout << token << " --> can not find this word" << endl;
	return Wordinfo(-1, -1,"null");
}
Wordinfo LexAnalyze() {//词法分析器
	token = "";
	getChar();
	getnbc();
	string val;
	int num = -1;
	switch (character) {
	case'a':
	case'b':
	case'c':
	case'd':
	case'e':
	case'f':
	case'g':
	case'h':
	case'i':
	case'j':
	case'k':
	case'l':
	case'm':
	case'n':
	case'o':
	case'p':
	case'q':
	case'r':
	case's':
	case't':
	case'u':
	case'v':
	case'w':
	case'x':
	case'y':
	case'z':
	case'A':
	case'B':
	case'C':
	case'D':
	case'E':
	case'F':
	case'G':
	case'H':
	case'I':
	case'J':
	case'K':
	case'L':
	case'M':
	case'N':
	case'O':
	case'P':
	case'Q':
	case'R':
	case'S':
	case'T':
	case'U':
	case'V':
	case'W':
	case'X':
	case'Y':
	case'Z':
		while (letter() || digit()) {
			concat();
			getChar();
		}
		retract();
		if(token == "and") return Wordinfo(39,0,"and");
		if(token == "or") return Wordinfo(40,0,"or");
		if(token == "not") return Wordinfo(41,0,"not");
		num = Reserve();
		if (num != -1) {
			return Wordinfo(num, 0, reserve[num]);
		}
		else {
			val = symbol();
			return Wordinfo(15, Symbol[val], val, val);
		}
		break;
	case'0':
	case'1':
	case'2':
	case'3':
	case'4':
	case'5':
	case'6':
	case'7':
	case'8':
	case'9':
		while (digit()) {
			concat();
			getChar();
		}
		retract();
		val = constant();
		return Wordinfo(57, Digit[val], val , val);
		break;
	case'<':
		getChar();
		if (character == '=') return Wordinfo(42, 0, "<=", "<=");
		else if (character == '>') return Wordinfo(42,0,"<>","<>");
		else {
			retract();
			return Wordinfo(42, 0, "<", "<");
		}
		break;
	case'>':
		getChar();
		if (character == '=')return Wordinfo(42, 0, ">=", ">=");
		else {
			retract();
			return Wordinfo(42, 0, ">", ">");
		}
		break;
	case'=':
		return Wordinfo(42, 0, "=", "=");
		break;
	case':':
		concat();
		getChar();
		if (character == '=') return Wordinfo(38, 0,":=");
		else {
			retract();
			return error();
		}
		break;
	case'+':
		return Wordinfo(34, 0,"+");
		break;
	case'*':
		return Wordinfo(36, 0,"*");
		break;
	case'(':
		return Wordinfo(48, 0 ,"(");
		break;
	case')':
		return Wordinfo(49, 0 , ")");
		break;
	case';':
		return Wordinfo(8, 0,";");
		break;
	case'#':
		return Wordinfo(10, 0,"#");
		break;
	default:
		concat();
		return error();
	}
}

int main() {

	n = 9;//输入自己设计的文法有多少行
	//具体的文法内容
	string str[9] = {"E->TB",
					  "B->+TB",
					  "B->$",
					  "T->FI",
					  "I->*FI",
					  "I->$",
					  "F->i",
					  "F->v",
					  "F->(E)"};
	int flag = 0;
	int tag;
	memset(tableMap , -1 ,sizeof(tableMap));//初始化所有数值位-1
	cout<<"Grammar\n";
	for(int i = 0 ; i < n ; i++) {
		string s = str[i];
		cout<<s<<"\n";
		node t;
		t.left = s[0];
		if(!mp.count(s[0])) {
			mp[s[0]] = non_c.size();
			f[non_c.size()] = f1[non_c.size()] = 1;
			non_c.push_back(s[0]);
		}
		tag = 0;//判断输入的文法是否正确
		for(int j = 1 ; j < s.length() ; j++) {
			if(!tag && s[j] == '-')tag = 1;
			else if(tag == 1 && s[j] == '>')tag = 2;
			else if(tag == 2) {
				if(!isupper(s[j]) && !mp1.count(s[j])){
						mp1[s[j]] = ter_c.size();
						if(s[j] != '$')ter_c.push_back(s[j]);
					}
				t.right += s[j];
			}
			else {
				cout << "read error , close the program ! please restart\n";
				return 0;
			}
		}
		input.push_back(t);
	}
	cout<<"\n";
	ter_c.push_back('#');
	mp1['#'] = ter_c.size() - 1;
	for(int i = 0 ; i < non_c.size() ; i++) {
		getFirst(non_c[i]);
		
	}
	showFirst();
	for(int i = 0 ; i < non_c.size() ; i++) {
		if(i == 0)follow[i].insert('#');
		getFollow(non_c[i]);
	}
	showFollow();
	getTable();
	showTable();

	init_reserve();//初始化保留字表
	Symbol.clear();//初始化变量字典
	Digit.clear();//初始化数字字典
	
	//下列是信息初始化
	index = 0;
	character = ' ';
	token = "";

	cout << "input #~ in the end\n" << endl;
	string in;
	while (cin >> in&&in != "#~") {//将整理出来的字符串放入inputstr
		inputstr = inputstr + " " + in;
	}
	Wordinfo word(-1, -1, "null");
	string str1 ="";
	cout << "\nidentify words as follows" << endl;
	while (index < int(inputstr.size())) {//逐个读取字符
		word = LexAnalyze();//得到对应的单词
		if(word.category != -1){//判断单词是否是有表可查
			cout << word.selfvalue<<":(" << word.category << "," << word.value << ")" << endl;
			if(word.category != 15 && word.category != 57) str1 = str1 + word.selfvalue;
			else if(word.category == 15) {
				varray[varray_length] = word.value;
				varray_length++;
				str1 = str1 + "v";
			}
			else if(word.category == 57) {
				iarray[iarray_length] = word.value;
				iarray_length++;
				str1 = str1 + "i";
			}
		}
	}
	str1 = str1 +"#";

	cout<<"-----------------------------\ninput the str1 : "<<str1<<"\n";
	cout<<"i represent number \nv represent variable\n-----------------------------\n";
	int len = str1.length(); //使用string类型的长度函数将字符串长度进行储存，同时传入分析函数进行分析
	als.push("#");//以#和开始符号为栈底
	als.push("E");
	tree_node *stack_tree = new tree_node();
	stack_tree->data = "#";
	tree_node *root = new tree_node();
	root->data = "E";
	root->stratum = 0;
	root->child_length = 0;
	als_tree.push(stack_tree);
	als_tree.push(root);
	als_tree_deep.push(root);
	analyze(str1,len); //分析函数入口
	if(flag_endding == "error") cout<<"a pity result : match fail"<<endl;
	else if(flag_endding == "success") {
		cout<<"match success !"<<endl;
		getTree();
	}
	return 0;
}
