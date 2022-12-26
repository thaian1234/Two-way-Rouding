#include <iostream>
#include<iomanip>
#include <math.h>
#include <fstream>
#define SIZE 100
#define MAX_INT 100

using namespace std;
int m = 0, n, socanh = 0, A, B, Trace[SIZE] = {0}, Network[SIZE][3] = {0}, maxflow = 0, dem = 0, c[SIZE][SIZE] = {0};
double S[SIZE] = {0}, Shv[SIZE] = {0}, f[SIZE][SIZE] = {0}, Delta[SIZE] = {0};

struct CANH{
	double desirability=0 ;
	int i = 0, j = 0;
};
CANH listcanh[SIZE];
void nhapMang(int A[], int n);
void nhapMang(double A[], int n);
void xuatMang(int A[], int n);
void xuatMang(double A[], int n);
void createB(double B[], int n, double A[], int Hvi[]);
double Min(double a, double b);
double sum(double A[], int n);
void ghiFile(double A[], double B[], int Hvi[], int n);
void createInitialArc(double A[], double B[], int Hvi[], int n);
void createNetwork(double A[], double B[], int Hvi[], int n);
void Sort(int n);
void xuatList(CANH listcanh[], int n);
void enter();
bool findPath();
void incFlow();
void tinhMaxFlow();
void printResult();
void ford_fulkerson();
void addArc();
void twoWayRounding();

int main(){
	cout << "Nhap so phan tu: ";
	cin >> n;
	double A[SIZE], B[SIZE];
	int Hvi[SIZE];
	cout << "Nhap cac phan tu: ";
	nhapMang(A,n);
	double s = sum(A,n);
	double r = ceil(s);
	if (s != r){
		A[n] = r-s;
		Hvi[n] = n+1;
		n++;
	}		
	m = r;
	cout << "m=" << m << endl;
	cout << "Cac phan tu: ";
	xuatMang(A,n);
	cout << "Nhap tap hoan vi: ";
	nhapMang(Hvi,n);
	//xuatMang(Hvi,n);
	createB(B,n,A,Hvi);
	cout << "Cac phan tu sau khi hoan vi la: ";
	xuatMang(B,n);
	createNetwork(A,B,Hvi,n);
	createInitialArc(A,B,Hvi,n);
	twoWayRounding();
	return 0;
}

void twoWayRounding(){
	enter();
	do{
		ford_fulkerson();
		tinhMaxFlow();
		//cout << maxflow << " ";
		if (maxflow == m)
			break;
		addArc();
		
	} while (true);
	cout << "\n";
	double kq = 1-listcanh[dem].desirability;
	printResult();
	cout << "Discrepancy: " << setprecision(5) << kq << "\n";
}

void addArc(){
	Network[socanh][0] = listcanh[dem-1].i;
	Network[socanh][1] = listcanh[dem-1].j;
	Network[socanh][2] = MAX_INT;
	//cout << Network[socanh][0] << " " << Network[socanh][1] << " " << Network[socanh][2] << endl;
	c[Network[socanh][0]][Network[socanh][1]] = Network[socanh][2];
	dem--;
	socanh++;
}

void ford_fulkerson(){
	do{
		if (!findPath())
			break;
		incFlow();
	} while (true);
}

void printResult(){
	for (int u = 1; u <= B; u++)
		for (int v = 1; v <= B; v++)
			if (c[u][v] > 0){
				cout << "f(" << u << "," << v << ") = " << f[u][v] << endl;
			}
	cout << "MAX FLOW: " << maxflow << endl;
	for (int i = 0; i < n; i++)
		cout << "x" << i+1 << "=" << f[m+i+2][m+n+i+2] << endl;
}

void tinhMaxFlow(){	// In ket qua
	maxflow = 0;
	for (int u = 1; u <= B; u++)
		for (int v = 1; v <= B; v++)
			if (c[u][v] > 0){
				if (u == A)
					maxflow += f[A][v];
			}
}

void incFlow(){//	Tang luong doc theo duong tang luong
	int u, v;
	double incvalue = Delta[B];	// Nhan Delta[B] chinh la trong so nho nhat tren cac cung cua duong tang luong
	v = B;	// Truy vet duong di, tang luong doc theo duong di
	do{
		u = Trace[v];	// Xet cung (|u|,v) tren duong tang luong
		if (u > 0)
			f[u][v] = f[u][v] + incvalue;	// (|u|,v) la cung thuan thi tang f[u,v]
		else{
			u = -u;
			f[v][u] = f[v][u] - incvalue;	// (|u|,v) la cung nghich thi giam f[v,|u|]
		}
		v = u;		
	} while (v != A);
}

bool findPath(){
	for (int i = 0; i < SIZE; i++)
		Trace[i] = 0;
    int Queue[SIZE] = {0}, First = 1, Last = 1, u;
	Queue[1] = A;
	Trace[A] = B+1; // Chi can khac 0 de danh dau ma thoi, so duong nao cung duoc ca
	Delta[A] = MAX_INT; // Khoi tao nhan
	do{
		u = Queue[First];
		First++; // Lay u khoi Queue
		for (int v = 1; v <= B; v++)
			if (Trace[v] == 0) // Xet nhung dinh v chua danh dau tham
			{
				if (f[u][v] < c[u][v]){ // Neu (u,v) la cung thuan tren Gf va co trong so la c[u,v]-f[u,v]
					Trace[v] = u;	// Luu vet, Trace[v] mang dau duong
					Delta[v] = Min(Delta[u],c[u][v]-f[u][v]);				
				}
				else{
					if (f[v][u] > 0){	// Neu (u,v) la cung nghich tren Gf va co trong so la f[v][u]
						Trace[v] = -u;	// Luu vet, Trace[v] mang dau am
						Delta[v] = Min(Delta[u],f[v][u]);
					}
				}
				if (Trace[v] != 0){	// Trace[v] khac 0 co nghia la tu u co the tham v
					if (v == B)	// Co duong tang luong tu A den B
						return true;
					Last++;
					Queue[Last] = v;
				}
			}
	} while (First <= Last);
	return false;
}

void enter(){ // Nhap du lieu
	int u, v;
	A = 1;
	B = 2*m+2*n+2;
	for (int i = 0; i < socanh; i++){
		u = Network[i][0];
		v = Network[i][1];
		c[u][v] = Network[i][2];
	}
}

void xuatList(CANH listcanh[], int n){
	for (int i = 0; i < n; i++)
		cout << listcanh[i].i << " " << listcanh[i].j << " " << listcanh[i].desirability << endl;
}

void createNetwork(double A[], double B[], int Hvi[], int n){
	for (int i = 0; i < m; i++){
		Network[socanh][0] = 1;
		Network[socanh][1] = i+2;
		Network[socanh][2] = 1;
		socanh++;
		Network[socanh][0] = m+2*n+i+2;
		Network[socanh][1] = 2*m+2*n+2;
		Network[socanh][2] = 1;
		socanh++;
	}
	for (int i = 0; i < n; i++){
		Network[socanh][0] = m+i+2;
		Network[socanh][1] = m+i+2+n;
		Network[socanh][2] = 1;
		socanh++;
	}
}

void createInitialArc(double A[], double B[], int Hvi[], int n){
	S[0] = A[0];
	Shv[0] = B[0];
	for (int i = 1; i < n; i++){
		S[i] = S[i-1]+A[i];
		Shv[i] = Shv[i-1]+B[i];
	}
	cout << "Tap S: ";
	xuatMang(S,n);
	cout << "Tap S hoan vi: ";
	xuatMang(Shv,n);
	S[-1]=0;
	Shv[-1] = 0;
	for (int j = 0; j < m; j++)
		for (int k = 0; k < n; k++){
			if (S[k]>j && S[k-1]<j+1){
				listcanh[dem].i = j+2;
				listcanh[dem].j = m+k+2;
				listcanh[dem].desirability = Min(S[k]-j,j+1-S[k-1]);
				dem++;
			}
			if (Shv[k]>j && Shv[k-1]<j+1){
				listcanh[dem].i = m+n+Hvi[k]-1+2;
				listcanh[dem].j = m+2*n+j+2;
				listcanh[dem].desirability = Min(Shv[k]-j,j+1-Shv[k-1]);
				dem++;
			}
		}
	Sort(dem);
}

void Sort(int n){
	for (int i = 0; i < n-1; i++)
		for (int j = i+1; j < n; j++)
			if (listcanh[i].desirability>listcanh[j].desirability){
				swap(listcanh[i],listcanh[j]);
			}
}
void ghiFile(double A[], double B[], int Hvi[], int n){
	fstream output;
	output.open("Network.txt",ios::out);
	if (output.fail()){
		cout << "Khong the mo file output!";
		exit(0);
	}
	else{
		cout << "Da ghi file.";
		output << 2*n+2*m+2 << " " << socanh << " " << 1 << " " << 2*n+2*m+2 << endl;
		for (int i = 0; i < socanh; i++)
			output << Network[i][0] << " " << Network[i][1] << " " << Network[i][2] << endl;
	}
	output.close();
}

double sum(double A[], int n){
	double s;
	for (int i = 0; i < n; i++)
		s+=A[i];
	return s;
}

double Min(double a, double b){
	if (a<b)
		return a;
	return b;
}

void createB(double B[], int n, double A[], int Hvi[]){
	for (int i = 0; i < n; i++)
		B[i] = A[Hvi[i]-1];	
}

void nhapMang(int A[], int n){
	for (int i = 0; i < n; i++)
		cin >> A[i];
}

void nhapMang(double A[], int n){
	for (int i = 0; i < n; i++)
		cin >> A[i];
}

void xuatMang(int A[], int n){
	for (int i = 0; i < n; i++)
		cout << A[i] << " ";
	cout << endl;
}

void xuatMang(double A[], int n){
	for (int i = 0; i < n; i++)
		cout << A[i] << " ";
	cout << endl;
}
