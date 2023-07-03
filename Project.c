#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxname 50
#define maxmenuitem 100
#define maxdays 7

struct menu
{
    char name[maxname];
    char type[10];
    float price;
    struct menu *next;
};

typedef struct sale
{
    char date[20];
    int vegitems;
    int nonvegitems;
} dailysales;

void displaymenu(struct menu *menu)  //soham
{
    printf("Menu Items:\n");
    printf("--------------------------------------------------------\n");
    printf("%-20s | %-10s | %-10s\n", "Name", "Type", "Price");
    printf("--------------------------------------------------------\n");

    struct menu *ptr;
    ptr = menu;
    while (ptr != NULL)
    {
        printf("%-20s | %-10s | %-10.2f\n", ptr->name, ptr->type, ptr->price);
        ptr = ptr->next;
    }

    printf("--------------------------------------------------------\n");
}

void readsalesdata(dailysales salesdata[], const char* filename) {  //sayan
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[100];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        char* date = strtok(line, ",");
        char* veg = strtok(NULL, ",");
        char* nonveg = strtok(NULL, ",");

        if (date != NULL && veg != NULL && nonveg != NULL) {
            strcpy(salesdata[count].date, date);
            salesdata[count].vegitems = atoi(veg);
            salesdata[count].nonvegitems = atoi(nonveg);
            count++;
        }
    }

    fclose(file);
}

void readmenu(struct menu **menuitems, const char *filename)//soham
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file: %s\n", filename);
        return;
    }

    char line[1000];
    struct menu *last = NULL;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *name = strtok(line, ",");
        char *type = strtok(NULL, ",");
        char *pricestr = strtok(NULL, ",");

        if (name != NULL && type != NULL && pricestr != NULL)
        {
            struct menu *item = (struct menu *)malloc(sizeof(struct menu));
            strcpy(item->name, name);
            strcpy(item->type, type);
            item->price = atof(pricestr);
            item->next = NULL;

            if (*menuitems == NULL)
            {
                *menuitems = item;
            }
            else
            {
                last->next = item;
            }
            last = item;
        }
    }

    fclose(file); 
}
    

void suggestmenu(struct menu *menuitems, float budget, const char *preference) //sayan
{
    printf("Suggested Optimized Menu:\n");
    printf("-------------------------\n");

    struct menu *currentitem = menuitems;
    float mindiff = budget;

    float total = 0.0;

    while (currentitem != NULL)
    {
        if ((strcmp(preference, "Veg") == 0 && strcmp(currentitem->type, "Veg") == 0) ||
            (strcmp(preference, "Non-Veg") == 0 && strcmp(currentitem->type, "Non-Veg") == 0))
        {

            if (currentitem != NULL && currentitem->price <= (budget - total))
            {
                printf("%s - %.2f\n", currentitem->name, currentitem->price);
                total += currentitem->price;
            }
        }
        currentitem = currentitem->next;
    }

    if (total <= budget)
    {
        printf("Remaining Budget: %.2f\n", budget - total);
    }

    printf("-------------------------\n");
}

void generatedailyreport(struct menu* menuItems, const dailysales salesdata[]) { //soham
    printf("Daily Menu Report:\n");
    printf("------------------\n");

    for (int i = 0; i < maxdays; i++) {
        int vegsold = salesdata[i].vegitems;
        int nonvegsold = salesdata[i].nonvegitems;

        printf("Date: %s\n", salesdata[i].date);
        printf("Veg Items Sold: %d\n", vegsold);
        printf("Non-Veg Items Sold: %d\n\n", nonvegsold);

        printf("Menu Items:\n");
        struct menu* current = menuItems;
        while (current != NULL) {
            if (strcmp(current->type, "Veg") == 0) {
                printf("%s - %.2f\n", current->name, current->price * vegsold);
            } else if (strcmp(current->type, "Non-Veg") == 0) {
                printf("%s - %.2f\n", current->name, current->price * nonvegsold);
            }
            current = current->next;
        }

        printf("\n------------------\n\n");
    }
}


int main()
{
    struct menu *menuitems = NULL;
    dailysales salesdata[maxdays];
    readmenu(&menuitems, "menu.csv");
    readsalesdata(salesdata, "sales.csv");
    float budget;
    char preference[10];
    int choice;
    int wh;
    system("cls");
    while (1)
    {
        printf("1. Generate Daily Menu Report\n");
        printf("2. Show The Menu\n");
        printf("3. Suggest Me Best Dishes According to my Budget\n");
        printf("4. Exit\n");
        printf("Please Enter Your Choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
                system("cls");
                generatedailyreport(menuitems, salesdata);
                printf("\nPress 0 to return to the previous menu\n");
                scanf("%d", &wh);
                if (wh == 0) {
                    system("cls");
                    break;
                }
        case 2:
            displaymenu(menuitems);
            printf("\nPress 0 to return to the previous menu\n");
            scanf("%d", &wh);
            if (wh == 0)
            {
                system("cls");
                break;
            }
        case 3:
            system("cls");
            printf("Enter your budget: ");
            scanf("%f", &budget);
            printf("Enter your preference (Veg/Non-Veg): ");
            scanf("%s", preference);
            suggestmenu(menuitems, budget, preference);
            printf("\nPress 0 to return to the previous menu\n");
            scanf("%d", &wh);
            if (wh == 0)
            {
                system("cls");
                break;
            }
        case 4:
            system("cls");
            exit(0);
        }
    }
    return 0;
}
