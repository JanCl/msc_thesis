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
	strncpy(appended_miss, former, iteration);
	appended_miss[iteration] = '0';
	switch (con){
		case meets_any_n_in_m:{
			int ones = 0;
			for (int i = 0; i < m; i++){
				ones += (int) appended_miss[iteration - i] - 48 ;
			}
			return (ones >= n);
		}
		
		case meets_row_n_in_m:{
			int ahead = m - 1;
			char* best_case = (char*) malloc(2 * m * sizeof(char));
			for (int i = 0; i < ahead; i++)
				best_case[i] = former[iteration - ahead + i];
			best_case[ahead] = '0'; // actual next (Miss)
			for (int i = 1; i < m; i++)
				best_case[ahead + i] = '1';	// to the rescue
			best_case[ahead + m] = '\0';
			char* one_substring = (char*) malloc((n + 1) * sizeof(char));
			for (int i = 0; i < n; i++)
				one_substring[i] = '1';	
			one_substring[n] = '\0';
			bool possible = true;
			for (int i = 0; i < m; i++){
				char* to_test = (char*) malloc(m * sizeof(char));
				strncpy(to_test, best_case + i, m);
				possible = possible && (strstr(to_test, one_substring) != NULL);
				free(to_test);
			}
			free(best_case);
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
			float x = (float)rand()/(float)RAND_MAX;
			if (check_constraint(i, con,  res, n, m) && x < miss_probability){
					res[i] = '0';
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