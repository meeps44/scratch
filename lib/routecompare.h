#ifndef ROUTECOMPARE_H
#define ROUTECOMPARE_H

#include "ext.h"

int compareHops(hop *h1, hop *h2);
int compareAddresses(address *a1, address *a2);
int compareIndexedPaths(traceroute *t1, traceroute *t2);
int comparePaths(traceroute *t1, traceroute *t2);
char **fCompareIndexedPaths(char *file1, char *file2);
char **fComparePaths(char *file1, char *file2);

#endif