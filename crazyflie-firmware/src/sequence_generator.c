#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

enum constraint{meets_any_n_in_m, meets_row_n_in_m, misses_any_n_in_m, misses_row_n_in_m};
bool check = false;

// returns true if the constraint is still fullfilled even if appended a 0 / Miss
bool check_constraint(int iteration, constraint con, char former[], int n, int m){
	char appended_miss[iteration+1];
	for (int i = 0; i < iteration; i++){
		appended_miss[i] = former[i];
	}
	appended_miss[iteration] = '0';
	switch (con){
		case meets_any_n_in_m:{
			if (m < iteration){
				int ones = 0;
				for (int i = 0; i < m; i++){
					ones += (int) appended_miss[iteration - i] - 48 ;
				}
				return (ones >= n);
			}
			if (check){ // kind of buggy rn 
				for (int i = 0; i < iteration - m; i++){
					int ones = 0;
					for (int j = 0; j < m; j++){
						ones += (int) appended_miss[i+j] - 48 ; // Casting '1' to 1 and '0' to 0
					}
					assert(ones >= n);
				}	
			} 
			if (iteration <= m){
				int ones = 0;
				for (int i = 0; i < iteration; i++){
					ones += (int) appended_miss[i] - 48 ;
				}
				if (ones + m - iteration == n){
					return false;
				}
			}
			return true;
		}
		case meets_row_n_in_m:{
			int ahead = m - 1;
			if (iteration < ahead)
				ahead = iteration;
			char* worst = (char*) malloc((ahead + m) * sizeof(char));
			for (int i = 0; i < ahead; i++)
				worst[i] = former[iteration - ahead + i];
			worst[ahead] = '0'; // actual next (Miss)
			for (int i = 1; i < m; i++)
				worst[ahead + i] = '1';	// to the rescue
			char* one_substring = (char*) malloc(n * sizeof(char));
			for (int i = 0; i < n; i++)
				one_substring[i] = '1';	
			return (strstr(worst, one_substring) != NULL); // strstr() returns pointer to start of z_s in a_m iff found
		}
		case misses_any_n_in_m:{
			return check_constraint(iteration, meets_any_n_in_m, former, m - n, m);
		}
		case misses_row_n_in_m:{
			char* zero_substring = (char*) malloc(n * sizeof(char));
			for (int i = 0; i < n; i++)
				zero_substring[i] = '0';
			return (strstr(appended_miss, zero_substring) == NULL); // strstr() returns pointer to start of z_s in a_m iff found
		}
	}
}

// returning a sequence of hits (1) and misses (0)
void generate_sequence(constraint con, int length, float miss_probability, int n, int m){
	if (length < m) 
		printf("! Warning ! \nLength of sequence is shorter than m, any sequence would be valid \n");
	char res[length];
	for (int i = 0; i < length; i++){
		if (check_constraint(i, con,  res, n, m)){
			float x = (float)rand()/(float)RAND_MAX;
			if (x < miss_probability)
				res[i] = '0';
			else 
				res[i] = '1';
		} else
				res[i] = '1';
	}
	printf("%s \n", res);
}

int main() {
	srand(time(NULL));
	//generate_sequence(meets_any_n_in_m, 150 , 0.7, 3, 5);
	generate_sequence(meets_row_n_in_m, 150 , 0.7, 3, 5); // buggy rn
	//generate_sequence(misses_any_n_in_m, 150 , 0.7, 2, 5);
	//generate_sequence(misses_row_n_in_m, 150 , 0.7, 2, 10);
	return(0);
} 