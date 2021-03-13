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
			char* worst = (char*) malloc((ahead + m) * sizeof(char) + 1);
			for (int i = 0; i < ahead; i++)
				worst[i] = former[iteration - ahead + i];
			worst[ahead] = '0'; // actual next (Miss)
			for (int i = 1; i < m; i++)
				worst[ahead + i] = '1';	// to the rescue
			worst[ahead + m] = '\0';
			char* one_substring = (char*) malloc(n * sizeof(char) + 1);
			for (int i = 0; i < n; i++)
				one_substring[i] = '1';	
			one_substring[n] = '\0';
			bool possible = true;
			for (int i = 0; i < m; i++){
				char* to_test = (char*) malloc(m * sizeof(char));
				strncpy(to_test, worst + i, m);
				possible = possible && (strstr(to_test, one_substring) != NULL);
				free(to_test);
			}
			free(worst);
			free(one_substring);
			return possible;
		}
		case misses_any_n_in_m:{
			return check_constraint(iteration, meets_any_n_in_m, former, m - n, m);
		}
		case misses_row_n_in_m:{
			char* zero_substring = (char*) malloc(n * sizeof(char) + 1);
			for (int i = 0; i < n; i++)
				zero_substring[i] = '0';
			zero_substring[n] = '\0';
			bool out = (strstr(appended_miss, zero_substring) == NULL); // strstr() returns pointer to start of z_s in a_m iff found
			free(zero_substring);
			return out;
		}
	}
}

// returning a sequence of hits (1) and misses (0)
void generate_sequence(constraint con, int length, float miss_probability, int n, int m){
	if (length < m) 
		printf("! Warning ! \nLength of sequence is shorter than m, any sequence would be valid \n");
	char res[length + m];
	for (int i = 0; i < length + m; i++){
		if (i < m)
			res[i] = '1'; // hit padding
		else {
			if (check_constraint(i, con,  res, n, m)){
				float x = (float)rand()/(float)RAND_MAX;
				if (x < miss_probability)
					res[i] = '0';
				else 
					res[i] = '1';
			} else
					res[i] = '1';
		}
	}
	char out[length];
	strncpy(out, res + m, length);
	printf("Substring of length %d : \n%s \n", length, out);
}

int main() {
	srand(time(NULL));
	generate_sequence(meets_any_n_in_m, 150 , 0.7, 3, 5);
	generate_sequence(meets_row_n_in_m, 150 , 0.7, 3, 7); // buggy rn
	generate_sequence(misses_any_n_in_m, 150 , 0.7, 2, 5);
	generate_sequence(misses_row_n_in_m, 150 , 0.7, 4, 15);
	return(0);
} 