/*
	Applying Semantic Networks to find Triangle's factor
	Copyright (C) 2018  Sang Tanh Le
	Contact sang.tanhle@gmail.com
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	
	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <iostream>
#include <cmath>
#include <conio.h>
#include "consolelib.h"
#define PI 3.141592654
using namespace std;
struct Point{
	int x,y;
	Point(int i, int j):x(i), y(j){}
};
void drawLine(Point start, int lenght, short direct, int symbolCode);
void drawRect(Point topLeft, Point bottomRight);
int SelectionMenu(string Index[], int number_Of_Items, int x0, int y0, int x1, int y1);//menu to input value for a factor was knew of triangle
constexpr int ft = 14;		//numbers of factor
constexpr int fm = 10;		//numbers of formula
int matrix[ft][10] = {{0}};	//matrix ft-1 triangle's factor, 10 formula
int factor[ft-1] = {0};		//we have ft-1 factors and 0 represent for a unknown factor, 1 reverse
double value[ft-1] = {0};	//we have ft-1 factors each factor has its own value
int savePosition[ft-1] = {0};	//save position of a factor which was found inorder
int saveFormula[ft-1] = {0};	//save formula number of a factor which was found inorder
string factorName[ft] = {	//make a menu for inputting
	"Alpha", "Beta", "Delta", "a", "b", "c", "S", "hc", "p", "hb", "ha", "R", "r", "DONE"
};
enum pos{			//every factor should be assigned position corresponds
	alpha = 1, beta = 2, delta = 3, a = 4, b = 5, c = 6, S = 7, hc = 8, p = 9, hb = 10, ha = 11, R = 12, r = 13
};
void InitAdjacencyMatrix();
void MarkFormulas(int factor_position);
void GiveKnownFactor();
void Instruction(int fml);
void Answer(int u, int v);	//The answer of factor u with formula v
void Formula(int u);
/*each formula store its factors*/
int Formula_1[4] = {1,2,4,5};	//1,2,4,5 - alpha, beta, a, b belong to formula 1
int Formula_2[4] = {2,3,5,6};	//2,3,5,6 - beta, beta, b, c belong to formula 2
int Formula_3[5] = {4,5,6,7,9};	//4,5,6,7,9 - a, b, c, S, p belong to formula 3
int Formula_4[3] = {1,2,3};	//1,2,3 - alpha, beta, delta belong to formula 4
int Formula_5[5] = {4,5,6,7,12};//4,5,6,7,12 - a, b, c, S, R belong to formula 5
int Formula_6[4] = {4,5,6,9};	//4,5,6,9 - a, b, c, p belong to formula 6
int Formula_7[3] = {7,9,13};	//7,9,13 - S, p, r belong to formula 7
int Formula_8[3] = {5,7,10};	//5,7,10- b, S, hb belong to formula 8
int Formula_9[3] = {6,7,8};	//6,7,8 - c, S, hc belong to formula 9
int Formula_10[3] = {4,7,11};	//4,7,11 - a, S, ha belong to formula 10
int flag = 1;			//1 factor we need was found, 0 reverse
int factorNeeded;		//position of factor we need
int fml;			//go to line 542
int main(){
	Print_at(30, 1, "APPLYING SEMANTIC NETWORKS TO FIND RIANGLE'S FACTORS");
	InitAdjacencyMatrix();
	GiveKnownFactor();
	if(flag==1){
		Moveto(0, 22);
		cout << "position of factors was found inorder:\nsavePosition[u]= ";
		for (int u = 1; u < ft; u++){
			if(savePosition[u]==0) continue;
			cout << savePosition[u] << "\t";
		}
		Moveto(0, 24);
		cout << "formulas corresponds with each factor above:\nsaveFormula[u]= ";
		for (int u = 1; u < ft; u++){
			if(saveFormula[u]==0) continue;
			cout << saveFormula[u] << "\t";
		}
		Moveto(0, 26);
		cout << "\n";
		Instruction(fml);
	}
	getch();
	return 0;
}
void InitAdjacencyMatrix(){
	for (int i = 1; i < ft ; i++){//assign -1 for each formula which appear the factor
		switch(i){
			case 1:{//alpha
				matrix[i][1] = matrix[i][4] = -1;
				break;
			}
			case 2:{//beta
				matrix[i][1] = matrix[i][2] = matrix[i][4] = -1;
				break;
			}
			case 3:{//delta
				matrix[i][2] = matrix[i][4] = -1;
				break;
			}
			case 4:{//a
				matrix[i][1] = matrix[i][3] = matrix[i][5] = matrix[i][6] = matrix[i][10] = -1;
				break;
			}
			case 5:{//b
				matrix[i][1] = matrix[i][2] = matrix[i][3] = matrix[i][5] = matrix[i][6] = matrix[i][8] = -1;
				break;
			}
			case 6:{//c
				matrix[i][2] = matrix[i][3] = matrix[i][5] = matrix[i][6] = matrix[i][9] = -1;
				break;
			}
			case 7:{//S
				matrix[i][3] = matrix[i][5] = matrix[i][7] = matrix[i][8] = matrix[i][9] = matrix[i][10] = -1;
				break;
			}
			case 8:{//hc
				matrix[i][9] = -1;
				break;
			}
			case 9:{//p
				matrix[i][3] = matrix[i][6] = matrix[i][7] = -1;
				break;
			}
			case 10:{//hb
				matrix[i][8] = -1;
				break;
			}
			case 11:{//ha
				matrix[i][10] = -1;
				break;
			}
			case 12:{//R
				matrix[i][5] = -1;
				break;
			}
			case 13:{//r
				matrix[i][7] = -1;
				break;
			}
		}
	}
}
void MarkFormulas(int factor_position){
	for(int j = 1; j <= fm; j++){//activate formula
		matrix[factor_position][j] = (matrix[factor_position][j]==-1)?1:0;
	}
}
void GiveKnownFactor(){
	int choice;
	Setcolor(YELLOW);
	Print_at(1, 3, "Input factor was knew");
	Setcolor(WHITE);
	do{
		choice = SelectionMenu(factorName, ft, 5, 5, 15, 20);
		fflush(stdin);
		double temp;
		switch(choice){
			case 0: break;
			case 1:{
				cout << "alpha = ";
				cin >> temp;
				value[1] = temp;//input value for each factor if it was knew
				factor[1] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 2:{
				cout << "beta = ";
				cin >> temp;
				value[2] = temp;//input value for each factor if it was knew
				factor[2] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 3:{
				cout << "delta = ";
				cin >> temp;
				value[3] = temp;//input value for each factor if it was knew
				factor[3] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 4:{
				cout << "a = ";
				cin >> temp;
				value[4] = temp;//input value for each factor if it was knew
				factor[4] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 5:{
				cout << "b = ";
				cin >> temp;
				value[5] = temp;//input value for each factor if it was knew
				factor[5] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 6:{
				cout << "c = ";
				cin >> temp;
				value[6] = temp;//input value for each factor if it was knew
				factor[6] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 7:{
				cout << "S = ";
				cin >> temp;
				value[7] = temp;//input value for each factor if it was knew
				factor[7] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 8:{
				cout << "hc = ";
				cin >> temp;
				value[8] = temp;//input value for each factor if it was knew
				factor[8] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 9:{
				cout << "p = ";
				cin >> temp;
				value[9] = temp;//input value for each factor if it was knew
				factor[9] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 10:{
				cout << "hb = ";
				cin >> temp;
				value[10] = temp;//input value for each factor if it was knew
				factor[10] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 11:{
				cout << "ha = ";
				cin >> temp;
				value[11] = temp;//input value for each factor if it was knew
				factor[11] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 12:{
				cout << "R = ";
				cin >> temp;
				value[12] = temp;//input value for each factor if it was knew
				factor[12] = 1;//mark a factor = 1 to show that we knew that one
			}break;
			case 13:{
				cout << "r = ";
				cin >> temp;
				value[13] = temp;//input value for each factor if it was knew
				factor[13] = 1;//mark a factor = 1 to show that we knew that one
			}break;
		}
	}while(choice!=ft);
	
	Setcolor(LIGHTGREEN);
	Print_at(56, 3, "A FACTOR YOU NEED");
	Setcolor(WHITE);
	factorNeeded = SelectionMenu(factorName, ft-1, 60, 5, 70, 20);//Chose factor we need to find
	
	for(int i = 1; i < ft; i++){//Activate the formula with known factors
		if(factor[i]==1){					
			MarkFormulas(i);
		}
	}
	//Scan the matrix to calculate the computable factor
	int factor_position, t;
	int u=1;							//index of savePosition[] and saveFormula[]
scanAgain:
	for(int j = 1; j <= fm; j++){
		int count = 0;
		for (int i = 1; i < ft; i++){
			if(matrix[i][j]==-1){
				factor_position = i;
				count++;
			}
		}//has completed a column
		if(count==1){//Know the n-1 factor to calculate the remaining factor
			if(factor_position==5 && j==1){
			 	double b = (double)( sin(value[pos::beta]*PI/180) * value[pos::a] )/ sin(value[pos::alpha]*PI/180); 
			 	value[factor_position] = b;
			 	MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==4 && j==1){
				double a = (double)( sin(value[pos::alpha]*PI/180) * value[pos::b] )/ sin(value[pos::beta]*PI/180);
				value[factor_position] = a;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==2 && j==1){
				if(value[pos::alpha]==0 || value[pos::delta]==0){//chi tinh goc bang formula 1 khi mot trong hai goc con lai(hoac ca hai) chua co gia tri
					double sinbeta = (double)(sin(value[pos::alpha]*PI/180)*value[pos::b])/value[pos::a];
					value[factor_position] = asin(abs(sinbeta)) * 180.0 / PI;
					MarkFormulas(factor_position); //After finding the factor, activate the related formula
					savePosition[u] = factor_position; //Assign the position for savePosition[u]
					saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
					u++;
				}
			}else if(factor_position==1 && j==1){//chi tinh goc bang formula 1 khi mot trong hai goc con lai(hoac ca hai) chua co gia tri
				if(value[pos::beta]==0 || value[pos::delta]==0){
					double sinaplha = (double)(sin(value[pos::beta]*PI/180)*value[pos::a])/value[pos::b];
					value[factor_position] = asin(abs(sinaplha)) * 180.0 / PI;
					MarkFormulas(factor_position); //After finding the factor, activate the related formula
					savePosition[u] = factor_position; //Assign the position for savePosition[u]
					saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
					u++;
				}
			}
			//formula 2
			if(factor_position==6 && j==2){
				double c = (double)(sin(value[pos::delta]*PI/180) * value[pos::b])/sin(value[pos::beta]*PI/180);
				value[factor_position] = c;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==5 && j==2){
				double b = (double)(sin(value[pos::beta]*PI/180) * value[pos::c]) / sin(value[pos::delta]*PI/180);
				value[factor_position]=b;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==3 && j==2){//chi tinh goc bang formula 2 khi mot trong hai goc con lai(hoac ca hai) chua co gia tri
				if(value[pos::beta]==0 || value[pos::alpha]==0){
					double sindelta = (sin(value[pos::beta]*PI/180)*value[pos::c])/value[pos::b];
					value[factor_position] = asin(abs(sindelta)) * 180/ PI;
					MarkFormulas(factor_position); //After finding the factor, activate the related formula
					savePosition[u] = factor_position; //Assign the position for savePosition[u]
					saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
					u++;
				}
			}else if(factor_position==2 && j==2){//chi tinh goc bang formula 2 khi mot trong hai goc con lai(hoac ca hai) chua co gia tri
				if(value[pos::alpha]==0 || value[pos::delta]==0){
					double sinbeta = (sin(value[pos::delta]*PI/180)*value[pos::b])/value[pos::c];
					value[factor_position] = asin(abs(sinbeta)) * 180/ PI;
					MarkFormulas(factor_position); //After finding the factor, activate the related formula
					savePosition[u] = factor_position; //Assign the position for savePosition[u]
					saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
					u++;
				}
			}
			//formula 3
			if(factor_position==7 && j==3){
				double exp1 = (double)value[pos::p]*(value[pos::p] - value[pos::a]);
				double exp2 = (double)value[pos::p] - value[pos::b];
				double exp3 = (double)value[pos::p] - value[pos::c];
				double S = sqrt(exp1*exp2*exp3);
				value[factor_position]=S;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 4
			if(factor_position==3 && j==4){
				double delta = 180 - value[pos::alpha] - value[pos::beta];
				value[factor_position] = delta;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==2 && j==4){
				double beta = 180 - value[pos::alpha] - value[pos::delta];
				value[factor_position] = beta;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u]= j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==1 && j==4){
				double alpha = 180 - value[pos::beta] - value[pos::delta];
				value[factor_position] = alpha;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 5
			if(factor_position==12 && j==5){
				double R = (double)(value[pos::a]*value[pos::b]*value[pos::c])/(4*value[pos::S]);
				value[factor_position] = R;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==7 && j==5){
				double S = (double)(value[pos::a]*value[pos::b]*value[pos::c])/(4*value[pos::R]);
				value[factor_position] = S;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==6 && j==5){
				double c = (double)(4*value[pos::R]*value[pos::S])/(value[pos::a]*value[pos::b]);
				value[factor_position] = c;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==5 && j==5){
				double b = (double)(4*value[pos::R]*value[pos::S])/(value[pos::a]*value[pos::c]);
				value[factor_position] = b;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==4 && j==5){
				double a = (double)(4*value[pos::R]*value[pos::S])/(value[pos::b]*value[pos::c]);
				value[factor_position] = a;		
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 6
			if(factor_position==9 && j==6){
				double p = (double)(value[pos::a] + value[pos::b] + value[pos::c])/2;
				value[factor_position] = p;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==6 && j==6){
				double c = (double)(value[pos::p]*2)-value[pos::a] - value[pos::b];
				value[factor_position]=c;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==5 && j==6){
				double b = (double)(value[pos::p]*2)-value[pos::a] - value[pos::c];
				value[factor_position]=b;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==4 && j==6){
				double a = (double)(value[pos::p]*2)-value[pos::b] - value[pos::c];
				value[factor_position]=a;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 7
			if(factor_position==13 && j==7){
				double r = (double)(value[pos::S])/value[pos::p];
				value[factor_position] = r;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==9 && j==7){
				double p = (double)(value[pos::S])/value[pos::r];
				value[factor_position] = p;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==7 && j==7){
				double S = (double)(value[pos::p])*value[pos::r];
				value[factor_position] = S;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 8
			if(factor_position==10 && j==8){
				double hb = (double)(2*value[pos::S])/value[pos::b];
				value[factor_position] = hc;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==7 && j==8){
				double S = (double)(value[pos::hb]*value[pos::b])/2;
				value[factor_position] = S;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==5 && j==8){
				double b = (double)(2*value[pos::S])/value[pos::hb];
				value[factor_position] = b;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 9
			if(factor_position==8 && j==9){
				double hc = (double)(2*value[pos::S])/value[pos::c];
				value[factor_position] = hc;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==7 && j==9){
				double S = (double)(value[pos::hc]*value[pos::c])/2;
				value[factor_position] = S;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==6 && j==9){
				double c = (double)(2*value[pos::S])/value[pos::hc];
				value[factor_position] = c;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			//formula 10
			if(factor_position==11 && j==10){
				double ha = (double)(2*value[pos::S])/value[pos::a];
				value[factor_position] = ha;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==7 && j==10){
				double S = (double)(value[pos::ha]*value[pos::a])/2;
				value[factor_position] = S;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}else if(factor_position==4 && j==10){
				double a = (double)(2*value[pos::S])/value[pos::ha];
				value[factor_position] = a;
				MarkFormulas(factor_position); //After finding the factor, activate the related formula
				savePosition[u] = factor_position; //Assign the position for savePosition[u]
				saveFormula[u] = j;	  //Assign the formula for saveFormula[u]
				u++;
			}
			/*
			Add the formula here as the format above
			*/
		}
	}
	if(value[factorNeeded]==0) {
		goto scanAgain;//If the factor not found(aka its value = 0) then the matrix was scanned again
	}else if(isnan(value[factorNeeded])){
		Moveto(0, 24);
		cout << "Sorry! Wrong Data";
		flag = 0;
		return;
	}
	for(int u = 1; u < ft; u++){//Based on the factor to derive the formula
		if(savePosition[u]==factorNeeded){
			fml = saveFormula[u];
			break;
		}
	}
	return;
}

int wasPrinted[10] = {0};
void Instruction(int fml){
	switch(fml){
		case 1:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_1)/sizeof(int); v++){
					if(savePosition[u] == Formula_1[v]){ //if u factor in formula 1 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[1] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[1] = 1;
						}
					}
				}
			}
		}break;
		case 2:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_2)/sizeof(int); v++){
					if(savePosition[u] == Formula_2[v]){ //if u factor in formula 2 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[2] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[2] = 1;
						}
					}
				}
			}
		}break;
		case 3:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_3)/sizeof(int); v++){
					if(savePosition[u] == Formula_3[v]){ //if u factor in formula 3 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[3] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[3] = 1;
						}
					}
				}
			}
		}break;
		case 4:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_4)/sizeof(int); v++){
					if(savePosition[u] == Formula_4[v]){ //if u factor in formula 4 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[4] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[4] = 1;
						}
					}
				}
			}
		}break;
		case 5:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_5)/sizeof(int); v++){
					if(savePosition[u] == Formula_5[v]){ //if u factor in formula 5 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[5] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[5] = 1;
						}
					}
				}
			}
		}break;
		case 6:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_6)/sizeof(int); v++){
					if(savePosition[u] == Formula_6[v]){ //if u factor in formula 6 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[6] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[6] = 1;
						}
					}
				}
			}
		}break;
		case 7:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_7)/sizeof(int); v++){
					if(savePosition[u] == Formula_7[v]){ //if u factor in formula 7 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[7] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[7] = 1;
						}
					}
				}
			}
		}break;
		case 8:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_8)/sizeof(int); v++){
					if(savePosition[u] == Formula_8[v]){ //if u factor in formula 8 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[8] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[8] = 1;
						}
					}
				}
			}
		}break;
		case 9:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_9)/sizeof(int); v++){
					if(savePosition[u] == Formula_9[v]){ //if u factor in formula 9 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[9] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[9] = 1;
						}
					}
				}
			}
		}break;
		case 10:{
			for(int u = 1; u < ft; u++){
				for(int v = 0; v < sizeof(Formula_10)/sizeof(int); v++){
					if(savePosition[u] == Formula_10[v]){ //if u factor in formula 10 then print formula out
						if(saveFormula[u] != fml){
							Instruction(saveFormula[u]);
						}else if(wasPrinted[10] == 0){
							Answer(savePosition[u], saveFormula[u]);	cout << "\n";
							wasPrinted[10] = 1;
						}
					}
				}
			}
		}break;
	}
}
void Formula(int u){
	switch(u){
		case 1:{
			cout << "a/sin(alpha) = b/sin(beta)";
		}break;
		case 2:{
			cout << "c/sin(delta) = b/sin(beta)";
		}break;
		case 3:{
			cout << "S = sqrt(p(p-a)(p-b)(p-c))";
		}break;
		case 4:{
			cout << "Alpha + Beta + Delta = 180";
		}break;
		case 5:{
			cout << "S = abc/4R";
		}break;
		case 6:{
			cout << "p = (a+b+c)/2";
		}break;
		case 7:{
			cout << "S = pr";
		}break;
		case 8:{
			cout << "S = (1/2)*hb*b";
		}break;
		case 9:{
			cout << "S = (1/2)*hc*c";
		}break;
		case 10:{
			cout << "S = (1/2)*ha*a";
		}break;
	}
}
void Answer(int u, int v){
	switch(u){
		case 1:{
			Formula(v); cout << " => Alpha = " << value[pos::alpha] << "\n";
		}break;
		case 2:{
			Formula(v); cout << " => Beta = " << value[pos::beta] << "\n";
		}break;
		case 3:{
			Formula(v); cout << " => Delta = " << value[pos::delta] << "\n";
		}break;
		case 4:{
			Formula(v); cout << " => a = " << value[pos::a] << "\n";
		}break;
		case 5:{
			Formula(v); cout << " => b = " << value[pos::b] << "\n";
		}break;
		case 6:{
			Formula(v); cout << " => c = " << value[pos::c] << "\n";
		}break;
		case 7:{
			Formula(v); cout << " => S = " << value[pos::S] << "\n";
		}break;
		case 8:{
			Formula(v); cout << " => hc = " << value[pos::hc] << "\n";
		}break;
		case 9:{
			Formula(v); cout << " => p = " << value[pos::p] << "\n";
		}break;
		case 10:{
			Formula(v); cout << " => hb = " << value[pos::hb] << "\n";
		}break;
		case 11:{
			Formula(v); cout << " => ha = " << value[pos::ha] << "\n";
		}break;
		case 12:{
			Formula(v); cout << " => R = " << value[pos::R] << "\n";
		}break;
		case 13:{
			Formula(v); cout << " => r = " << value[pos::r] << "\n";
		}break;
	}
}
void drawRect(Point topLeft, Point bottomRight){
	Print_at(topLeft.x, topLeft.y, (char)218);
	Print_at(bottomRight.x, topLeft.y, (char)191);
	Print_at(bottomRight.x, bottomRight.y, (char)217);
	Print_at(topLeft.x, bottomRight.y, (char)192);
	drawLine(Point(topLeft.x+1, topLeft.y), bottomRight.x-1, 1, 196);
	drawLine(Point(topLeft.x+1, bottomRight.y), bottomRight.x-1, 1, 196);
	drawLine(Point(topLeft.x, topLeft.y+1), bottomRight.y-1, 2, 179);
	drawLine(Point(bottomRight.x, topLeft.y+1), bottomRight.y-1, 2, 179);
	Moveto(topLeft.x+1, (bottomRight.y-topLeft.y)/2 + topLeft.y);
	return;
}
void drawLine(Point start, int lenght, short direct, int symbolCode){
	if(direct==1){
		for (int i = start.x; i <= lenght; i++)
			Print_at(i, start.y, (char)symbolCode);
	}else if(direct==2){
		for (int j = start.y; j <= lenght; j++)
			Print_at(start.x, j, (char)symbolCode);
	}
	return;
}
int SelectionMenu(string Index[], int number_Of_Items, int x0, int y0, int x1, int y1){
	drawRect(Point(x0-1,y0-1), Point(x1, y1));
	const int UpperBound = y0-1;
	const int LowerBound = y1;
	int row = y0;	int col = x0;
	int i = 0;		int element = 0;
	int numbers_of_index_per_page = y1-y0-1;
	int flag = 0;
again:
	Clear_at(x0,y0,x1-1,y1-1);
	int countLineNumber = 0;
	for (i ; i < number_Of_Items ; i++)
	{
		if(row==LowerBound)
	    	break;
	    Print_at(col, row, Index[i]);
	    row += 1;
	    countLineNumber++;
	}
	HighLight(BLACK, WHITE);
	if(flag==0)
		row -= countLineNumber;
	else
		row = LowerBound - 1;
	Print_at(col, row, Index[element]);
	Normal(WHITE, BLACK);
	Moveto(x1+2, row);
	while(true){
	  char catch_char = _getch();
	  switch (catch_char) 
	  {
	    case KEY_UP:if (element + 1	> 1){
	    				Normal(WHITE, BLACK);
		  		       	Print_at(x1, row, (char)179);
		              	Print_at(col, row, Index[element]);
		              	row -= 1;
		              	element--;
		              	if(row==UpperBound)
		              	{
		              		row = y0;
							col = x0;
							i = element - numbers_of_index_per_page;
							flag = 1;
		              		goto again;
						}
		              	HighLight(BLACK, WHITE);
		              	Print_at(col, row, Index[element]);
		              	Normal(WHITE, BLACK);
		            	Moveto(x1+2, row);
	  			 	}break;
	  	case KEY_DOWN:if (element + 1 < number_Of_Items){
	  					Normal(WHITE, BLACK);
		  		        Print_at(x1, row, (char)179);
		              	Print_at(col, row, Index[element]);
		              	row += 1;
		              	element++;
		              	if(row==LowerBound)
		              	{
							i = element;
							row = y0;
							col = x0;
							flag = 0;
		              		goto again;
						}	
						HighLight(BLACK, WHITE);
			            Print_at(col, row, Index[element]);
			            Normal(WHITE, BLACK);
			            Moveto(x1+2, row);
	  				}break;
	  	case ENTER:	return element + 1;
	  }
	}
}
