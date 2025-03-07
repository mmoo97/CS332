/* Sample program to read a comma separated file into a structure and
   display the array of structures */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINESIZE 1024

struct listing {
	int id, host_id, minimum_nights, number_of_reviews, calculated_host_listings_count,availability_365;
	char *host_name, *neighbourhood_group, *neighbourhood, *room_type;
	float latitude, longitude, price;
};

struct listing getfields(char* line){
	struct listing item;

	/* Note: you have to pass the string to strtok on the first 
	   invocation and then pass NULL on subsequent invocations */
	item.id = atoi(strtok(line, ","));
	item.host_id = atoi(strtok(NULL, ","));
	item.host_name = strdup(strtok(NULL, ","));
	item.neighbourhood_group = strdup(strtok(NULL, ","));
	item.neighbourhood = strdup(strtok(NULL, ","));
	item.latitude = atof(strtok(NULL, ","));
	item.longitude = atof(strtok(NULL, ","));
	item.room_type = strdup(strtok(NULL, ","));
	item.price = atof(strtok(NULL, ","));
	item.minimum_nights = atoi(strtok(NULL, ","));
	item.number_of_reviews = atoi(strtok(NULL, ","));
	item.calculated_host_listings_count = atoi(strtok(NULL, ","));
	item.availability_365 = atoi(strtok(NULL, ","));
	
	return item;
}

void displayStruct(struct listing item) { 
	printf("ID : %d\n", item.id);
	printf("Host ID : %d\n", item.host_id);
	printf("Host Name : %s\n", item.host_name);
	printf("Neighbourhood Group : %s\n", item.neighbourhood_group);
	printf("Neighbourhood : %s\n", item.neighbourhood);
	printf("Latitude : %f\n", item.latitude);
	printf("Longitude : %f\n", item.longitude);
	printf("Room Type : %s\n", item.room_type);
	printf("Price : %f\n", item.price);
	printf("Minimum Nights : %d\n", item.minimum_nights);
	printf("Number of Reviews : %d\n", item.number_of_reviews);
	printf("Calculated Host Listings Count : %d\n", item.calculated_host_listings_count);
	printf("Availability_365 : %d\n\n", item.availability_365);
}

void swap(struct listing *xp, struct listing *yp) { 
    struct listing temp = *xp;
    *xp = *yp; 
    *yp = temp; 
} 

void sort(struct listing arr[22555], int n, int field) { // field = 1 means sort by host name, field = 2 is by price
	if (field == 2) {
		int i, j; 
		for (i = 0; i < n-1; i++)       
			for (j = 0; j < n-i-1; j++)  
				if (arr[j].price > arr[j+1].price) 
				  swap(&arr[j], &arr[j+1]); 
    }

    if (field == 1) {
		int i, j; 
		for (i = 0; i < n-1; i++)       
			for (j = 0; j < n-i-1; j++)  
				if (strcmp(arr[j].host_name, arr[j+1].host_name) > 0) 
				  swap(&arr[j], &arr[j+1]); 
    }

}

void writeFile(char *dir, struct listing arr[22555]){
	int i;

	FILE *f = fopen(dir, "wb");
	
	for(i=0;i<22520;i++) { // change for big array
        fprintf(f, "ID : %d\n", arr[i].id);
		fprintf(f, "Host ID : %d\n", arr[i].host_id);
		fprintf(f, "Host Name : %s\n", arr[i].host_name);
		fprintf(f, "Neighbourhood Group : %s\n", arr[i].neighbourhood_group);
		fprintf(f, "Neighbourhood : %s\n", arr[i].neighbourhood);
		fprintf(f, "Latitude : %f\n", arr[i].latitude);
		fprintf(f, "Longitude : %f\n", arr[i].longitude);
		fprintf(f, "Room Type : %s\n", arr[i].room_type);
		fprintf(f, "Price : %f\n", arr[i].price);
		fprintf(f, "Minimum Nights : %d\n", arr[i].minimum_nights);
		fprintf(f, "Number of Reviews : %d\n", arr[i].number_of_reviews);
		fprintf(f, "Calculated Host Listings Count : %d\n", arr[i].calculated_host_listings_count);
		fprintf(f, "Availability_365 : %d\n\n", arr[i].availability_365);
     }

	fclose(f);
}

int main(int argc, char* args[]) {
	struct listing list_items[22555];
	char line[LINESIZE];
	int i, count;

	FILE *fptr = fopen("listings.csv", "r");
	if(fptr == NULL){
		printf("Error reading input file listings.csv\n");
		exit (-1);
	}

	count = 0;
	while (fgets(line, LINESIZE, fptr) != NULL){
		list_items[count++] = getfields(line);
	}

	fclose(fptr);
	
	// for (i=0; i<count; i++)
	// 	displayStruct(list_items[i]); // use for output view

	sort(list_items, 22520, 1);

	writeFile("sorted_name.txt", list_items);

	sort(list_items, 22520, 2);

	writeFile("sorted_price.txt", list_items);
	
	return 0;
}
