#include "str_differencial.h"

vector<string> lexems;

void init_lexems(){
	lexems.push_back(string("+")); //0
	lexems.push_back(string("-")); //1
	lexems.push_back(string("*")); //2
	lexems.push_back(string("/")); //3
	lexems.push_back(string("(")); //4
	lexems.push_back(string("^")); //5
	lexems.push_back(string("x")); //6 ++
	lexems.push_back(string("e")); //7
	lexems.push_back(string("ln")); //8 +
	lexems.push_back(string("pi")); //9
	lexems.push_back(string("tg")); //10 +
	lexems.push_back(string("sin")); //11 +
	lexems.push_back(string("cos")); //12 +
	lexems.push_back(string("ctg")); //13 +
	lexems.push_back(string("0")); //14 
}

void push_new_lexems(int a, string s[]){
	lexems.clear();
	init_lexems();
	while(a--) lexems.push_back(s[--a]);
}

int find_lexem(string &s, int n){
	if(s[n]>47 && s[n]<58) return 14;
	for(int i=0; i<lexems.size(); i++){
		if(s.length()<lexems[i].length()+n) goto roll_baraban;
		for(int j=0; j<lexems[i].size(); j++) if(s[n+j]!=lexems[i][j]) goto roll_baraban;
		return i;
		roll_baraban:;
	}
	return -1;
}

int count_union(string &s, int &num){
	unsigned char n=0;
	while (s[++num]!=')' || n){
		switch (s[num])
		{
			case '(': n++; break;
			case ')': n--; break;
		}
	}
	return num;
}

string start_differentiation(string input){
	if(!lexems.size()) init_lexems();
	int a;
	return summ_line(input, a);
}

string proizvodnaya(string &s, int &n){
	switch (find_lexem(s, n))
	{
	case -1: 
		{
			n=s.length();
			return "!error in the formula!";
		}
	case 1:
	case 2:
	case 3:
	case 5:
		return string();
	case 4:
		{
			int a=++n, x=0;
			count_union(s, n);
			if(s.length()>n+1){
				if(s[n+1]=='^'){
					int b=n, c=n+=2;
					string fsx=proizvodnaya(string(s.c_str()+a, s.c_str()+b), x);
					string gsx=proizvodnaya(s, n);
					return "(" + gsx + "*ln" + string(s.c_str()+a-1, s.c_str()+b+1) + "+" + string(s.c_str()+c, s.c_str()+n) + "*(" + fsx + "/" + string(s.c_str()+a-1, s.c_str()+b+1) + ")*" + string(s.c_str()+a-1, s.c_str()+n);
				}
			}
			return "(" + summ_line(string(s.c_str()+a, s.c_str()+n++), x) + ")";
		}
	case 6: 
		{
			if(s.length()>++n){
				if(s[n]=='^'){
					int a=++n;
					string rezult=proizvodnaya(s, n);
					return '(' + rezult + "*lnx+" + string(s.c_str()+a, s.c_str()+n) + "/x)*x^" + string(s.c_str()+a, s.c_str()+n);
				}
			}
			return string("1");
		}
	case 7:
		{
			if(s.length()>++n){
				if(s[n]=='^'){
					int a=++n;
					string rezult=proizvodnaya(s, n);
					return rezult + "*e^" + string(s.c_str()+a, s.c_str()+n);
				}
			}
			return "0";
		}
	case 8:
		{
			int a=n+=2;
			string rezult=proizvodnaya(s, n);
			return rezult + "/" + string(s.c_str()+a, s.c_str()+n);
		}
	case 10:
		{
			int a=n+=2;
			string rezult=proizvodnaya(s, n);
			return rezult + "/(cos" + string(s.c_str()+a, s.c_str()+n)+")^2";
		}
	case 11:
		{
			int a=n+=3;
			string rezult=proizvodnaya(s, n);
			return "cos" + string(s.c_str()+a, s.c_str()+((s[n-2]==')')?n-1:n)) + "*" + rezult;
		}
	case 12:
		{
			int a=n+=3;
			string rezult=proizvodnaya(s, n);
			return "(-1)*sin" + string(s.c_str()+a, s.c_str()+((s[n-2]==')')?n-1:n)) + "*" + rezult;
		}
	case 13:
		{
			int a=n+=3;
			string rezult=proizvodnaya(s, n);
			return "(-1)*" + rezult + "/(sin" + string(s.c_str()+a, s.c_str()+n)+")^2";
		}
	case 14:
		{
// + log_diffure					
			lexems[14].clear();
			for(; n<s.length(); n++){
				if(s[n]>47 && s[n]<58) lexems[14].push_back(s[n]);
				else{
					if(s[n]=='.') lexems[14].push_back(s[n]);
					else{
						if(s[n]=='^'){
							int a=++n;
							string rezult = proizvodnaya(s, n);
							return "ln" + lexems[14] + "*" + rezult + "*" + lexems[14] + "^" + string(s.c_str()+a, s.c_str()+n);
						}
						else return "0";
					}
				}
			}
			return "0";
		}
		default: 
			{
				int a=find_lexem(s, n);
				n+=lexems[a].length();
				if(s.length()>n){
					if(s[n]=='^'){
						int b=++n;
						string rezult=proizvodnaya(s, n);
						return "ln" + 
							lexems[a] + 
							"*" + rezult + 
							"*" + lexems[a] + 
							"^" + string(s.c_str()+b, s.c_str()+n);
					}
				}
				return string("0");
			}
	}
}

string mul_line(string &s, int &n, bool f){
	string summ;
	vector<string> multi_summ;
	do{
		switch (find_lexem(s, n))
		{
		case 0:
		case 1:
		exit_line:
			summ="";
			if(multi_summ.size()){
				for(int i=0; i<multi_summ.size(); i++){
					summ += multi_summ[i];
					summ.push_back(f ? '+' : '-');
				}
				summ.pop_back();
			}
			return summ;
		case 3: 
			{
				int begin=++n;
				string sub_str=proizvodnaya(s, n);
				for(int i=0; i<multi_summ.size(); i++) multi_summ[i] += '/' + string(s.c_str()+begin, s.c_str()+n);
				multi_summ.push_back(summ);
				multi_summ.back()+="(-1)*" + sub_str + "/(" + string(s.c_str()+begin, s.c_str()+n) + ")^2";
				summ.back()='/';
				for(int j=begin; j<n; j++) summ.push_back(s[j]);
				summ.push_back('*');
				break;
			}
		case 2: {n++; break;}
		default:
			{
				int begin=n;
				string sub_str=proizvodnaya(s, n);
				for(int i=0; i<multi_summ.size(); i++) multi_summ[i] += '*' + string(s.c_str()+begin, s.c_str()+n);
				multi_summ.push_back(summ);
				multi_summ.back()+=sub_str;
				for(int j=begin; j<n; j++) summ.push_back(s[j]);
				summ.push_back('*');
			}
		}
	}
	while (n<s.length());
	goto exit_line;
}

string summ_line(string &s, int &n){
	string rezult;
	bool flag=true;
	do{
		switch (find_lexem(s, n)){
			case 0: {rezult.push_back('+'); n++; flag=true; break;}
			case 1: {rezult.push_back('-'); n++; flag=false; break;}	
			default: {rezult+=mul_line(s, n, flag); break;}
		}
	}
	while (n<s.length());
	return rezult;
}