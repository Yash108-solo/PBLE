/*
 * PBLE 1: Smart Delivery Planning - Fractional Knapsack
 * Subject: Analysis of Algorithms (AOA)
 *
 * Algorithm:
 * 1. Calculate value/weight ratio for every package.
 * 2. Sort packages in descending order of ratio.
 * 3. Select a whole package when it fits; otherwise take the fraction
 *    that fits in the remaining capacity.
 *
 * Time Complexity: O(n^2) for insertion sort; O(n) for selection.
 * Overall worst-case time complexity: O(n^2).
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_PACKAGES 100

typedef struct {
    int id;
    double value;
    double weight;
    double ratio;
    double fractionTaken;
    double valueTaken;
    double weightTaken;
} Package;

void calculateRatios(Package packages[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        packages[i].ratio = packages[i].value / packages[i].weight;
    }
}

void sortByRatio(Package packages[], int n) {
    int i, j;
    Package key;

    /* Insertion sort: easy to follow for an AOA lab experiment. */
    for (i = 1; i < n; i++) {
        key = packages[i];
        j = i - 1;
        while (j >= 0 && packages[j].ratio < key.ratio) {
            packages[j + 1] = packages[j];
            j--;
        }
        packages[j + 1] = key;
    }
}

void displayPackages(Package packages[], int n, int showRatio) {
    int i;
    printf("\n%-10s %-14s %-14s", "Package", "Value", "Weight");
    if (showRatio) printf(" %-14s", "Value/Weight");
    printf("\n");
    for (i = 0; i < 58; i++) printf("-");
    printf("\n");

    for (i = 0; i < n; i++) {
        printf("%-10d %-14.2f %-14.2f", packages[i].id,
               packages[i].value, packages[i].weight);
        if (showRatio) printf(" %-14.2f", packages[i].ratio);
        printf("\n");
    }
}

void findMaximumValue(Package packages[], int n, double capacity) {
    int i;
    double remaining = capacity, totalValue = 0.0, totalWeight = 0.0;

    for (i = 0; i < n; i++) {
        packages[i].fractionTaken = 0.0;
        packages[i].valueTaken = 0.0;
        packages[i].weightTaken = 0.0;
    }

    calculateRatios(packages, n);
    sortByRatio(packages, n);

    printf("\nPackages sorted by decreasing Value/Weight ratio:\n");
    displayPackages(packages, n, 1);

    for (i = 0; i < n && remaining > 0.000001; i++) {
        if (packages[i].weight <= remaining) {
            packages[i].fractionTaken = 1.0;
            packages[i].weightTaken = packages[i].weight;
            packages[i].valueTaken = packages[i].value;
        } else {
            packages[i].fractionTaken = remaining / packages[i].weight;
            packages[i].weightTaken = remaining;
            packages[i].valueTaken =
                packages[i].value * packages[i].fractionTaken;
        }

        remaining -= packages[i].weightTaken;
        totalWeight += packages[i].weightTaken;
        totalValue += packages[i].valueTaken;
    }

    printf("\nSelected packages:\n");
    printf("%-10s %-14s %-14s %-14s %-14s\n",
           "Package", "Fraction", "Weight Taken", "Value Taken", "Ratio");
    for (i = 0; i < 70; i++) printf("-");
    printf("\n");
    for (i = 0; i < n; i++) {
        if (packages[i].fractionTaken > 0.000001) {
            printf("%-10d %-14.2f %-14.2f %-14.2f %-14.2f\n",
                   packages[i].id, packages[i].fractionTaken,
                   packages[i].weightTaken, packages[i].valueTaken,
                   packages[i].ratio);
        }
    }

    printf("\nTotal weight used : %.2f\n", totalWeight);
    printf("Unused capacity   : %.2f\n", capacity - totalWeight);
    printf("Maximum value     : %.2f\n", totalValue);
}

int main(void) {
    Package packages[MAX_PACKAGES];
    int n = 0, choice, i, dataEntered = 0, ratiosReady = 0;
    double capacity = 0.0;

    do {
        printf("\n========== SMART DELIVERY PLANNING ==========\n");
        printf("1. Enter Package Details\n");
        printf("2. Display Package Details\n");
        printf("3. Calculate Value/Weight Ratio\n");
        printf("4. Sort Packages by Ratio\n");
        printf("5. Find Maximum Value\n");
        printf("6. Display Selected Packages\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a menu number.\n");
            while (getchar() != '\n') { }
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
                printf("Enter number of packages (1-%d): ", MAX_PACKAGES);
                if (scanf("%d", &n) != 1 || n < 1 || n > MAX_PACKAGES) {
                    printf("Invalid number of packages.\n");
                    n = 0;
                    dataEntered = 0;
                    while (getchar() != '\n') { }
                    break;
                }
                for (i = 0; i < n; i++) {
                    packages[i].id = i + 1;
                    printf("\nPackage %d value/profit: ", i + 1);
                    if (scanf("%lf", &packages[i].value) != 1 ||
                        packages[i].value <= 0) {
                        printf("Value must be a positive number. Re-enter details.\n");
                        n = 0; dataEntered = 0;
                        while (getchar() != '\n') { }
                        break;
                    }
                    printf("Package %d weight: ", i + 1);
                    if (scanf("%lf", &packages[i].weight) != 1 ||
                        packages[i].weight <= 0) {
                        printf("Weight must be a positive number. Re-enter details.\n");
                        n = 0; dataEntered = 0;
                        while (getchar() != '\n') { }
                        break;
                    }
                    packages[i].ratio = 0.0;
                    packages[i].fractionTaken = 0.0;
                    packages[i].valueTaken = 0.0;
                    packages[i].weightTaken = 0.0;
                }
                if (n > 0) {
                    printf("Enter vehicle capacity: ");
                    if (scanf("%lf", &capacity) != 1 || capacity <= 0) {
                        printf("Capacity must be a positive number.\n");
                        dataEntered = 0;
                        while (getchar() != '\n') { }
                    } else {
                        dataEntered = 1;
                        ratiosReady = 0;
                        printf("Package details saved successfully.\n");
                    }
                }
                break;

            case 2:
                if (!dataEntered) {
                    printf("Please enter valid package details first.\n");
                } else {
                    displayPackages(packages, n, ratiosReady);
                    printf("Vehicle capacity: %.2f\n", capacity);
                }
                break;

            case 3:
                if (!dataEntered) {
                    printf("Please enter valid package details first.\n");
                } else {
                    calculateRatios(packages, n);
                    ratiosReady = 1;
                    printf("\nCalculated Value/Weight ratios:\n");
                    displayPackages(packages, n, 1);
                }
                break;

            case 4:
                if (!dataEntered) {
                    printf("Please enter valid package details first.\n");
                } else {
                    calculateRatios(packages, n);
                    sortByRatio(packages, n);
                    ratiosReady = 1;
                    printf("Packages sorted in decreasing ratio order.\n");
                    displayPackages(packages, n, 1);
                }
                break;

            case 5:
                if (!dataEntered) {
                    printf("Please enter valid package details first.\n");
                } else {
                    findMaximumValue(packages, n, capacity);
                    ratiosReady = 1;
                }
                break;

            case 6:
                if (!dataEntered) {
                    printf("Please enter valid package details first.\n");
                } else {
                    int found = 0;
                    printf("\nSelected packages (run option 5 first):\n");
                    for (i = 0; i < n; i++) {
                        if (packages[i].fractionTaken > 0.000001) {
                            printf("Package %d: fraction %.2f, weight %.2f, value %.2f\n",
                                   packages[i].id, packages[i].fractionTaken,
                                   packages[i].weightTaken, packages[i].valueTaken);
                            found = 1;
                        }
                    }
                    if (!found) printf("No selection available. Choose option 5 first.\n");
                }
                break;

            case 7:
                printf("Exiting program. Thank you!\n");
                break;

            default:
                printf("Invalid menu choice. Please select 1 to 7.\n");
        }
    } while (choice != 7);

    return 0;
}
