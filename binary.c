#include <stdio.h>
#include <stdlib.h>
#include <math.h>

long int fact(int n);

long int nCr(int n, int r) {
        long int result = 1;
        int i = 0;
        for(i = n; i > n-r; i--)
                result *= i;
        return result/fact(r);
}

void printVector(double * data, int size) {
	int i = 0;
	for(i = 0; i < size; i++)
		printf("%d, ", (int)data[i]);
}

void diffVector(double * data1, double * data2, int size, double * result) {
	int i = 0;
	for(i = 0; i < size; i++ ) {
		result[i] = data1[i] - data2[i];
	}
}

void sumVector(double * data1, double * data2, int size, double * result) {
	int i = 0;
        for(i = 0; i < size; i++ ) {
                result[i] = data1[i] + data2[i];
        }
}

double multVector(double * data1, double * data2, int size) {
	int i = 0;
	double res = 0;
	for(i = 0; i < size; i++ ) {
                res += data1[i] * data2[i];
        }
	return res;
}

void dotVector(double * data1, double * data2, int size, double * result) {
	int i = 0;
	for(i = 0; i < size; i++ ) {
                result[i] = data1[i] * data2[i];
        }
}

void multVectorMatrix(double *vector, double * matrix, int size, double * result) {
	int i = 0;
	int j = 0;
	double total_row = 0;

	for(i = 0; i < size; i++) {
		total_row = 0;
		for(j = 0; j < size; j++) {
			total_row += vector[j] * matrix[i + j * size];
		}
		result[i]= total_row;
	}
}

double likelihood(double * configure, double * stat, double * sigma, int size, double NCP ) {

	double value1 = 0;
	double value2 = 0;
	
	double * tmpresult = (double *) malloc( sizeof(double *) * size);

	value1 = NCP * multVector(configure, stat, size);
	multVectorMatrix(configure, sigma, size, tmpresult);
	value2 = NCP*NCP/2*multVector(tmpresult, configure, size);	
	
	free(tmpresult);

	return (exp(value1) * exp(-value2));
}

int nextBinary(double * data, int size) {
	int i = 0;
	int total_one = 0;	
	int index = size-1;
        int one_countinus_in_end = 0;

        while(index >= 0 && data[index] == 1) {
                index = index - 1;
                one_countinus_in_end = one_countinus_in_end + 1;
	}
	if(index >= 0) {
        	while(index >= 0 && data[index] == 0) {
               	 index = index - 1;	
		}
	}
        if(index == -1) {
                while(i <  one_countinus_in_end+1 && i < size) {
                        data[i] = 1;
                        i=i+1;
		}
                i = 0;
                while(i < size-one_countinus_in_end-1) {
                        data[i+one_countinus_in_end+1] = 0;
                        i=i+1;
		}
	}
        else if(one_countinus_in_end == 0) {
                data[index] = 0;
                data[index+1] = 1;
	}
	else {
                data[index] = 0;
                while(i < one_countinus_in_end + 1) {
                        data[i+index+1] = 1;
			if(i+index+1 >= size)
				printf("ERROR3 %d\n", i+index+1);
                        i=i+1;
		}
                i = 0;
                while(i < size - index - one_countinus_in_end - 2) {
                        data[i+index+one_countinus_in_end+2] = 0;
			if(i+index+one_countinus_in_end+2 >= size) {
				printf("ERROR4 %d\n", i+index+one_countinus_in_end+2);
			}
                        i=i+1;
		}
	}
	i = 0;
	for(i = 0; i < size; i++)
		if(data[i] == 1)
			total_one = total_one + 1;
	
	return(total_one);		
}


int nextBinary2(char * data, int size) {
        int i = 0;
        int total_one = 0;
        int index = size-1;
        int one_countinus_in_end = 0;

        while(index >= 0 && data[index] == '1') {
                index = index - 1;
                one_countinus_in_end = one_countinus_in_end + 1;
        }
        while(index >= 0 && data[index] == '0') {
                index = index - 1;
        }
        if(index == -1) {
                while(i <  one_countinus_in_end+1) {
                        data[i] = '1';
                        i=i+1;
                }
                i = 0;
                while(i < size-one_countinus_in_end) {
                        data[i+one_countinus_in_end+1] = '0';
                        i=i+1;
                }
        }
        else if(one_countinus_in_end == 0) {
                data[index] = '0';
                data[index+1] = '1';
        }
        else {
                data[index] = 0;
                while(i < one_countinus_in_end + 1) {
                        data[i+index+1] = '1';
                        i=i+1;
                }
                i = 0;
                while(i < size - index - one_countinus_in_end - 1) {
                        data[i+index+one_countinus_in_end+2] = '0';
                        i=i+1;
                }
        }
        i = 0;
        for(i = 0; i < size; i++)
                if(data[i] == '1')
                        total_one = total_one + 1;
        return(total_one);
}

double totalLikelihood(double * stat, double * sigma, int size, double NCP) {	
	long int i = 0;
	double sumLikelihood = 0;
	double tmp_likelihood = 0;
	long int total_iteration = nCr(size,0) +  nCr(size,1) +  nCr(size,2) +  nCr(size,3) +  nCr(size,4) +  nCr(size,5);;
	
	double * configure = (double *) malloc (size * sizeof(double *));	
	
	for(i = 0; i < size; i++) {
		configure[i] = 0;
	}

	i = 0;

	while(i < total_iteration) {
		nextBinary(configure, size);
		tmp_likelihood = likelihood(configure, stat, sigma, size, NCP );
		printVector(configure, size);
		printf("\t");
		printf("%lf\n", tmp_likelihood);
		sumLikelihood += tmp_likelihood;
		i++;
	}
	
	free(configure);
	return(sumLikelihood);
}

double likelihood_all_possible_set(double * d_configure, double * stat, double * sigma, int size, double NCP, int cut_off ) {
	int i = 0;
	int j = 0;
	int index = 0;
	int num_one = 0;
	int total_sets = 0;

	double total_iteration = 0;
	double tmp_likelihood = 0;

	for(i = 0; i < size; i++) {
		if(d_configure[i] == 1)
			num_one++;
	}	
	
	double * tmp_index = (double *) malloc(num_one * sizeof(double *));
	double * tmp_set   = (double *) malloc(num_one * sizeof(double *));
	double * tmp_configure = (double *) malloc(size *sizeof(double *));

	for(i = 0; i < size; i++) {
		if(d_configure[i] == 1) {
                        tmp_index[index] = i;
			index++;
		}
	}		
	for(i = 0; i < num_one; i++) {
		tmp_set[i] = 0;	
	}
	
	index = 0;
	total_iteration = pow(2, num_one);
	while(index < total_iteration) {
		for(j = 0; j < size; j++) {
			tmp_configure[j] = 0;
		}
		for(j = 0; j < num_one; j++) {
			if(tmp_set[j] == 1)
				tmp_configure[(int)tmp_index[j]] = 1;
		}
		tmp_likelihood += likelihood(tmp_configure, stat, sigma, size, NCP);	
		nextBinary(tmp_set, num_one);
		index++;
	}
	free(tmp_configure);
	free(tmp_index);
	free(tmp_set);

	return(tmp_likelihood);
}

double findOptimalSet(double * stat, double * sigma, int size, double NCP, char * configure) {
	int i = 0;
	int j = 0;
	int cut_off = 5;
	double tmp_likelihood = 0;
	double sumLikelihood = 0;
	double total_likelihood = totalLikelihood(stat, sigma, size, NCP);

	printf("Total + %lf\n", total_likelihood);

	long int total_iteration = nCr(size,0) +  nCr(size,1) +  nCr(size,2) +  nCr(size,3) +  nCr(size,4) +  nCr(size,5);

	double * d_configure = (double *) malloc(size * sizeof(double *));

        for(i = 0; i < size; i++) {
                d_configure[i] = 0;
		configure[i] = '0';
        }
	
	i = 0;
	printf("START");
        while(i < total_iteration) {
                nextBinary(d_configure, size);
		tmp_likelihood = likelihood_all_possible_set(d_configure, stat, sigma, size, NCP, cut_off );
		//printf("%d %d %lf %lf\n", i, total_iteration, sumLikelihood, total_likelihood);
		if(tmp_likelihood / total_likelihood >= 0.95) {
			for(j = 0; j < size; j++)
				if(d_configure[j] == 1) {
					configure[j] = '1';
				}	
			free(d_configure);
			return (sumLikelihood);
		}
                i++;
        }
	printf("WE are FUCKED\n");
	
	for(i = 0; i < size;i++) {
		if(abs(stat[i]) >= NCP) 
			configure[i] = '1';
	}
	
	free(d_configure);

}

long int fact(int n) {
	if(n==0)
		return 1;
	return n* fact(n-1);
}

int main() {
	printf("STRAT\n");

	int i = 0;
	int row = 4;
	double * vector = (double *) malloc( sizeof(double) * row);
	double * matrix = (double *) malloc( sizeof(double) * row * row);
	double * result = (double *) malloc( sizeof(double) * row);

	for(i=0; i < row; i++) {
		vector[i] = i;
	}	
	for(i=0; i < row * row; i++) {
		matrix[i] = i;
	}
	
	multVectorMatrix(vector, matrix,row, result);
	
	printVector(result, row);


	return 0;	
}
	
