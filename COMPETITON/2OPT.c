/*

https://en.wikipedia.org/wiki/2-opt
2optSwap(route, i, k) {
1. take route[1] to route[i-1] and add them in order to new_route
2. take route[i] to route[k] and add them in reverse order to new_route
3. take route[k+1] to end and add them in order to new_route
return new_route;
}

repeat until no improvement is made {
start_again:
best_distance = calculateTotalDistance(existing_route)
for (i = 0; i < number of nodes eligible to be swapped - 1; i++) {
for (k = i + 1; k < number of nodes eligible to be swapped; k++) {
new_route = 2optSwap(existing_route, i, k)
newDistance = calculateTotalDistance(new_route)
if (newDistance < best_distance) {
existing_route = new_route
goto start_again
}
}
}
}
d(c1 ,c2) = nearestint(√(x1−x2 )^2+(y1 − y2 )^2)
*/

#include <stdio.h>      //print functions
#include <stdlib.h>     //atoi function
#include <string.h>     //string functions
#include <math.h>       //square root function
#include <limits.h>     //floor functions and INT_MAX and LONG_MAX
#include <omp.h>        //parallel functions and timings

//create a city struct to hold the city ID, x, y, and prepare the distance calculation for edge weight
struct city {
	int id, x, y;
	int* edgeWeight;
};

	int* buildAdjacenyList(struct city** cities, int numCities) {
	    int *arr[i] = (int *)malloc(numCities * sizeof(int));
	    int i, j;

	    for (i = 0;  i < numCities; i++) {
	        arr[i] = (int *)malloc(numCities * sizeof(int));
	    }
				#pragma omp parallel for
	    for (i = 0; i < numCities; i++) {
	        for (j = 0; j < numCities; j++) {
	            arr[i][j] = distance(cities[i], cities[j]);
	        }
	    }

	    return arr;
	}


//distance sum for the total weight of the tour
double sumDistance(struct city** tour, int numCities) {
	int i = 0;
	double sum = 0;
	//sum up all the tours per city
	for (i = 0; i < numCities; i++) {
		int destination = tour[i + 1]->id;
		sum += adjacency[(tour[i]->id)][destination];
	}
	return sum;
}
//sum += tour[i]->edgeWeight[destination];
//scans the file for the associated digit and then passes cities by reference
struct city** assignCities(char** characters, int numCities) {
	int i;
	struct city** cities = malloc(sizeof(struct city*)*numCities);
	for (i = 0; i < numCities; i++) {
		cities[i] = malloc(sizeof(struct city));
		//scans for the integers fro mthe characters parsed
		sscanf(characters[i], "%i %i %i",
			&cities[i]->id,
			&cities[i]->x,
			&cities[i]->y);
	}
	return cities;
};

//start of the algorithm where it looks through each city and then removes the ones that have been visited.
struct city** startTour(array*,struct city** cities, int numCities) {
	struct city** citiesLeft = malloc(sizeof(struct city*)*numCities);
	struct city** tour = malloc(sizeof(struct city *)*(numCities + 1));
	int i = 0,
		a = 0,
		index = 0,
		cityQueue = numCities,
		currentIndex = 0;
	long smallestTour, currentDist;

	//set the cities that you have to visit
	for (i = 0; i<numCities; i++)
		citiesLeft[i] = cities[i];
	//set the Starting City
	tour[0] = citiesLeft[0];
	cityQueue = removeCities(citiesLeft, index, cityQueue);

	//iterate through the cities
	for (i = 0; i < numCities - 1; i++) {
		smallestTour = INT_MAX;
		for (a = 0; a < cityQueue; a++) {
			int destination = citiesLeft[a]->id;
			currentDist = adjacency[(tour[i]->id)][destination];
			if (currentDist < smallestTour) {
				smallestTour = currentDist;
				index = a;
			}
		}
		currentIndex++;
		tour[currentIndex] = citiesLeft[index];
		//current Visited city now removed since it has been visited once
		cityQueue = removeCities(citiesLeft, index, cityQueue);
	}
	// the original city is addede back into the tour
	tour[currentIndex + 1] = cities[0];

	return tour;
}

//swap the cities
struct city** opt2Swap(struct city** tour, struct city** newTour, int i, int b, int numCities) {
	int a = 0, id = 0;
	//iterate through the current city list and add to the newTour city list
	//1. take route[1] to route[i-1] and add them in order to new_route
	for (a = 0; a < i; a++) {
		newTour[id] = tour[a];
		id++;
	}
	//2. take route[i] to route[k] and add them in reverse order to new_route
	for (a = b; a >= i; a--) {
		newTour[id] = tour[a];
		id++;
	}
	//3. take route[k+1] to end and add them in order to new_route
	for (a = b + 1; a < numCities + 1; a++) {
		newTour[id] = tour[a];
		id++;
	}
	//return new_route;
	return newTour;
}

//repeat until no improvement is made
struct city** start2(struct city** tour, long* bestDistance, long* currentDistance, int numCities) {
	int i, a, b;
	long newDistance;
	struct city** newTour = malloc(sizeof(struct city**)*(numCities + 1));
	for (i = 0; i<numCities; i++) {
		newTour[i] = malloc(sizeof(struct city));
	}
	//for (i = 0; i < number of nodes eligible to be swapped - 1; i++)
	for (i = 1; i < numCities - 1; i++) {
		//for (k = i + 1; k < number of nodes eligible to be swapped; k++)
		for (a = i + 1; a < numCities; a++) {
			newTour = opt2Swap(tour, newTour, i, a, numCities);
			newDistance = sumDistance(newTour, numCities);
			if (newDistance < *bestDistance) {
				*currentDistance = newDistance;
				return newTour;
			}
		}
	}
	*currentDistance = *bestDistance;
	return tour;
}

//timings from openMP
struct city** checkTime(struct city** old_tour, int numCities, int nonCompetition) {
	long newDistance = sumDistance(old_tour, numCities), bestDistance = LONG_MAX, currentDistance = 0;
	int i;
	struct city** newTour = malloc(sizeof(struct city*)*(numCities + 1));
#pragma omp parallel for
	for (i = 0; i<numCities + 1; i++) {
		newTour[i] = malloc(sizeof(struct city));
		newTour[i] = old_tour[i];
	}
	//timing methods from http://web.engr.oregonstate.edu/~mjb/cs575/Projects/proj00.html
	double time1, time0;
	time0 = omp_get_wtime();



	while (newDistance < bestDistance) {
		time1 = (double)(omp_get_wtime() - time0);
		//limit of 180 seconds for competition 1 is true
		if (time1 >= 180.0 && nonCompetition == 1) {
			printf("Timing over 3 minutes.\n");
			break;
		}
		//limit of 30 minutes if not competition
		else if (time1 >= 1800) {
			printf("Timing over 30 minutes.\n");
			break;
		}
		bestDistance = newDistance;
		//best_distance = calculateTotalDistance(existing_route)
		newTour = start2(newTour, &bestDistance, &currentDistance, numCities);
		newDistance = currentDistance;
	}
	printf("Current Timing:%f ", time1);
	return newTour;
}

//cities are assigned from scanning the file here.
// arguement error here and scan file format http://www.cplusplus.com/forum/beginner/91308/
char** buildCities(int* i, char *argv) {
	int maxLines = 10000, maxLength = 512, a;
	char **characters = (char **)malloc(sizeof(char*)*maxLines);
	FILE *fp = fopen(argv, "r");
	for (*i = 0; 1; *i = *i + 1) {
		//check to see if the memory needs to be doubled
		if (*i >= maxLines) {
			int new_size;
			new_size = maxLines * 2;
			characters = (char **)realloc(characters, sizeof(char*)*new_size);
			maxLines = new_size;
		}

		/* Allocate space for the next line */
		characters[*i] = malloc(maxLength);
		if (fgets(characters[*i], maxLength - 1, fp) == NULL)
			break;

		//https://stackoverflow.com/questions/9628637/how-can-i-get-rid-of-n-from-string-in-c. Used the \- solution
		for (a = strlen(characters[*i]) - 1; a >= 0 && (characters[*i][a] == '\n' || characters[*i][a] == '\r'); a--)
			characters[*i][a + 1] = '\0';
	}

	fclose(fp);
	return characters;
}

//remove cities from the queue
int removeCities(struct city** cities, int city_index, int cityQueue) {
	int i = 0;
	for (i = city_index; i < cityQueue - 1; i++)
		cities[i] = cities[i + 1];
	return cityQueue - 1;
}

//calculate the distance
int distance(struct city* a, struct city* b) {
	double deltax, deltay;
	deltax = (double)b->x - (double)a->x;
	deltay = (double)b->y - (double)a->y;
	return floor(sqrt((pow(deltax, 2) + pow(deltay, 2))) + 0.5);
}

//int main
int main(int argc, char **argv) {
#ifndef _OPENMP
	fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
	return 1;
#endif


	omp_set_num_threads(24);
	int a, b, c, i, numCities = 0, nonCompetition = 0;
	if (argc < 2) {
		printf("No file inputted.\n");
		return 0;
	}
	//competition setting to run for shorter/time limit
	if (argc > 2 && strcmp(argv[2], "-c") == 0)
		nonCompetition = 1;
	FILE * output;
	//create the corresponding example output




	char *tourOutput = ".tour";
	char buffer[256];
	snprintf(buffer, sizeof buffer, "%s%s", argv[1], tourOutput);
	output = fopen(buffer, "w");

	char** characters = buildCities(&numCities, argv[1]);

	struct city** cities = assignCities(characters, numCities);
  int *adjacency = buildAdjacenyList(cities, &numCities);


	struct city** tour = startTour(adjacency,cities, numCities);
	tour = checkTime(tour, numCities, nonCompetition);
	long smallestDistance = sumDistance(tour, numCities);
	printf("Smallest Distance Calculated:%ld\n", smallestDistance);

	//send to output file
	//https://stackoverflow.com/questions/11573974/write-to-txt-file
	fprintf(output, "%ld\n", smallestDistance);
	for (c = 0; c<numCities; c++)
		fprintf(output, "%i\n", tour[c]->id);
	//close output file
	fclose(output);
	return 0;
}
