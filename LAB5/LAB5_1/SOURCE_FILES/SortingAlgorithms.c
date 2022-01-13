#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>

int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
	case BUBBLE_SORT:
	case INSERTION_SORT:
	case SELECTION_SORT:
	case QUICK_SORT:
	case MERGE_SORT:
		return 1;
	default:
		return 0;
	}
}

/*Antalet byten i en och samma algoritm kan vara olika beroende p� implementationen. Ibland ligger datat redan p� r�tt plats och d� kan man v�lja att testa det och inte g�ra ett byte (vilket ger extra j�mf�relse) eller s� kan man �nd� g�ra ett byte (med sig sj�lv). F�ljer man de algoritmer som vi g�tt igenom p� f�rel�sningarna exakt s� g�r man en swap �ven p� ett element som ligger p� r�tt plats
 
    N�r du analyserar det data som genereras (result.txt) s� beh�ver du ha detta i �tanke */

/******************************************************************************************/
/* Era algoritmer har: */

//Local functions:
void topDownSplitMerge(ElementType *A, ElementType *B, int begin, int end, Statistics *statistics);
void topDownMerge(ElementType *A, ElementType *B, int begin, int middle, int end, Statistics *statistics);
void copyArray(ElementType *a, ElementType *b, int size);
static void qSort(ElementType *A, int low, int high, Statistics *statistics);
int partition(ElementType *A, int low, int high, Statistics *statistics);

static void bubbleSort(ElementType *arrayToSort, unsigned int size, Statistics *statistics)
{
	int n = size;
	do
	{
		int newn = 0;
		for (int i = 1; lessThan(i, n, statistics); i++)
		{
			if (greaterThan(arrayToSort[i - 1], arrayToSort[i], statistics))
			{
				swapElements(arrayToSort + (i - 1), arrayToSort + i, statistics);
				newn = i;
			}
		}
		n = newn;
	} while (lessThanOrEqualTo(1, n, statistics));
}

static void insertionSort(ElementType *arrayToSort, unsigned int size, Statistics *statistics)
{
	int i = 1;
	while (lessThan(i, size, statistics))
	{
		int j = i;
		while (greaterThan(j, 0, statistics) && greaterThan(arrayToSort[j - 1], arrayToSort[j], statistics))
		{
			swapElements(arrayToSort + j, arrayToSort + (j - 1), statistics);
			j--;
		}
		i++;
	}
}

static void selectionSort(ElementType *arrayToSort, unsigned int size, Statistics *statistics)
{
	int i, j;
	for (i = 0; lessThan(i, size - 1, statistics); i++)
	{
		int jMin = i;
		for (j = i + 1; lessThan(j, size, statistics); j++)
		{
			if (lessThan(arrayToSort[j], arrayToSort[jMin], statistics))
			{
				jMin = j;
			}
		}
		if (notEqualTo(jMin, i, statistics))
		{
			swapElements(arrayToSort + i, arrayToSort + jMin, statistics);
		}
	}
}

static void mergeSort(ElementType *arrayToSort, unsigned int size, Statistics *statistics)
{
	ElementType *arrayCopy = (ElementType *)malloc(sizeof(ElementType) * size);
	copyArray(arrayToSort, arrayCopy, size);
	topDownSplitMerge(arrayToSort, arrayCopy, 0, size, statistics);
	free(arrayCopy);
}

void topDownSplitMerge(ElementType *A, ElementType *B, int begin, int end, Statistics *statistics)
{
	if (lessThanOrEqualTo(end - begin, 1, statistics))
	{
		return;
	}
	int middle = (end + begin) / 2;
	topDownSplitMerge(B, A, begin, middle, statistics);
	topDownSplitMerge(B, A, middle, end, statistics);

	topDownMerge(B, A, begin, middle, end, statistics);
}

void topDownMerge(ElementType *A, ElementType *B, int begin, int middle, int end, Statistics *statistics)
{
	int i = begin, j = middle;

	for (int k = begin; lessThan(k, end, statistics); k++)
	{
		if (lessThan(i, middle, statistics) && (greaterThanOrEqualTo(j, end, statistics) || lessThanOrEqualTo(A[i], A[j], statistics)))
		{
			B[k] = A[i];
			i++;
		}
		else
		{
			B[k] = A[j];
			j++;
		}
	}
}

void copyArray(ElementType *a, ElementType *b, int size)
{
	for (int i = 0; i < size; i++)
	{
		b[i] = a[i];
	}
}

static void quickSort(ElementType *arrayToSort, unsigned int size, Statistics *statistics)
{
	qSort(arrayToSort, 0, size - 1, statistics);
}

static void qSort(ElementType *A, int low, int high, Statistics *statistics)
{
	if (greaterThanOrEqualTo(low, 0, statistics) && greaterThanOrEqualTo(high, 0, statistics) && lessThan(low, high, statistics))
	{
		int p = partition(A, low, high, statistics);
		qSort(A, low, p, statistics);
		qSort(A, p + 1, high, statistics);
	}
}

int partition(ElementType *A, int low, int high, Statistics *statistics)
{
	int p = A[(high + low) / 2];

	int i, j;
	while (1)
	{
		for(i = low; lessThan(A[i], p, statistics); i++);

		for(j = high; greaterThan(A[j], p, statistics); j--);
		
		if(greaterThanOrEqualTo(i, j, statistics)){
			return j;
		}
		swapElements(A + i, A + j, statistics);
	}
	
}

/******************************************************************************************/

char *getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
	case BUBBLE_SORT:
		return "  Bubble sort ";
	case SELECTION_SORT:
		return "Selection sort";
	case INSERTION_SORT:
		return "Insertion sort";
	case MERGE_SORT:
		return "  Merge sort  ";
	case QUICK_SORT:
		return "  Quick sort  ";
	default:
		assert(0 && "Ogiltig algoritm!");
		return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType *arrayToSort, unsigned int size, SortingAlgorithm algorithmToUse, Statistics *statistics)
{
	if (statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:
		bubbleSort(arrayToSort, size, statistics);
		break;
	case SELECTION_SORT:
		selectionSort(arrayToSort, size, statistics);
		break;
	case INSERTION_SORT:
		insertionSort(arrayToSort, size, statistics);
		break;
	case MERGE_SORT:
		mergeSort(arrayToSort, size, statistics);
		break;
	case QUICK_SORT:
		quickSort(arrayToSort, size, statistics);
		break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType *arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType) * sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray *current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);

		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet �r: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE *file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType *arrays[], unsigned int sizes[], FILE *file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
