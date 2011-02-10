#include <vector>
#include <string>
#include<cstdio>
using namespace std;

// just to check the numbers are right

long totalcombs(int n, int r){
	long c=1;
	if (r > n) return 0;
	for (long d=1; d <= r; d++) {
		c *= n--;
		c /= d;
	}
	return c;
}

// example code to get all combination 

int main(){
	vector<int> indx;
	string alpha("01234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ^$_-=+&!~*");
	int n=72;
	int j=1;
	int k=2;
	for(int twk=j;twk<=k;twk++){
		int r=twk;
		int total=totalcombs(n,r);
		int ccount=1;
		bool done=true;
		for(int iwk=0;iwk<r;iwk++)indx.push_back(iwk);
		while(done){
			done=false;
			for(int owk=0;owk<r;owk++){
				printf("%c",alpha[indx[owk]]) ;
			}
			printf("\n");
			for(int iwk=r-1;iwk>=0;iwk--){
				if(indx[iwk]<=(n-1)-(r-iwk)){
					indx[iwk]++;
					for(int swk=iwk+1;swk<r;swk++){
						indx[swk]=indx[swk-1]+1;
					}
					iwk=-1;
					done=true;
				}	
			}	
		}
		indx.clear();
	}
	return 0;
}
