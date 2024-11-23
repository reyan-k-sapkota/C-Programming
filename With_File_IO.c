#include <stdio.h>
#include <time.h>
#include <string.h>


struct ElectricityBill {
    char houseName[50];
    int customerID;
    int unitsConsumed;
    float pricePerUnit;
    float totalAmount;
    int rebateDays;
    float rebateAmount;
    int fineDays;
    float fineAmount;
    float grandTotal;
};


void calculateBill(struct ElectricityBill* bill, int billReadingDay, int billReadingMonth, int billReadingYear) {
    bill->totalAmount = 0;
    bill->rebateDays = 0;
    bill->rebateAmount = 0;
    bill->fineDays = 0;
    bill->fineAmount = 0;

    
    if (bill->unitsConsumed >= 0 && bill->unitsConsumed <= 20) {
        bill->totalAmount = bill->unitsConsumed * 4.0;
    } else if (bill->unitsConsumed > 20 && bill->unitsConsumed <= 50) {
        bill->totalAmount = bill->unitsConsumed * 7.30;
    } else if (bill->unitsConsumed > 50 && bill->unitsConsumed <= 150) {
        bill->totalAmount = bill->unitsConsumed * 8.60;
    } else if (bill->unitsConsumed > 150 && bill->unitsConsumed <= 250) {
        bill->totalAmount = bill->unitsConsumed * 9.50;
    } else if (bill->unitsConsumed > 250) {
        bill->totalAmount = bill->unitsConsumed * 11.00;
    }

    
    time_t now;
    time(&now);
    struct tm* paymentDate = localtime(&now);
    int todayDay = paymentDate->tm_mday;
    int todayMonth = paymentDate->tm_mon + 1;
    int todayYear = paymentDate->tm_year + 1900; 

    
    int daysFromReadingDate = (todayYear - billReadingYear) * 365 +
                             (todayMonth - billReadingMonth) * 30 +
                             (todayDay - billReadingDay);

    if (daysFromReadingDate >= 1 && daysFromReadingDate <= 7) {
        bill->rebateDays = daysFromReadingDate;
        bill->rebateAmount = bill->totalAmount * 0.02; // 2% rebate
    } else if (daysFromReadingDate >= 16 && daysFromReadingDate <= 30) {
        bill->fineDays = daysFromReadingDate - 15;
        bill->fineAmount = bill->totalAmount * 0.05; // 5% fine
    } else if (daysFromReadingDate >= 31 && daysFromReadingDate <= 40) {
        bill->fineDays = daysFromReadingDate - 30;
        bill->fineAmount = bill->totalAmount * 0.10; // 10% fine
    } else if (daysFromReadingDate > 40 && daysFromReadingDate <= 60) {
        bill->fineDays = daysFromReadingDate - 40;
        bill->fineAmount = bill->totalAmount * 0.25; // 25% fine
    } else if (daysFromReadingDate > 60) {
        // No rebate after 60 days, apply 25% fine
        bill->fineDays = 60;
        bill->fineAmount = bill->totalAmount * 0.25; // 25% fine
    }

    bill->grandTotal = bill->totalAmount - bill->rebateAmount + bill->fineAmount;
}

int main() {
    
    struct ElectricityBill customers[48];

    
    FILE* file = fopen("customer_data.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int customerID;
    char customerName[50];
    int numCustomers = 0;

    while (fscanf(file, "%d %[^\n]", &customerID, customerName) != EOF) {
        customers[numCustomers].customerID = customerID;
        strcpy(customers[numCustomers].houseName, customerName);
        numCustomers++;

        if (numCustomers == 49) {
            break;
        }
    }

    fclose(file);

   
    int inputID;
    int customerIndex = -1;
    while (customerIndex == -1) {
        printf("Enter the Customer ID: ");
        scanf("%d", &inputID);

        
        for (int i = 0; i < numCustomers; i++) {
            if (inputID == customers[i].customerID) {
                customerIndex = i;
                break;
            }
        }

        if (customerIndex == -1) {
            printf("Customer ID not found in the database. Please enter a valid Customer ID within the given range.\n");
        }
    }

   
    struct ElectricityBill* bill = &customers[customerIndex];

    printf("Enter the units consumed: ");
    scanf("%d", &bill->unitsConsumed);

    int billReadingDay, billReadingMonth, billReadingYear;
    printf("Enter the bill reading date (mm/dd/yyyy): ");
    scanf("%d/%d/%d", &billReadingMonth, &billReadingDay, &billReadingYear);

    calculateBill(bill, billReadingDay, billReadingMonth, billReadingYear);

    printf("\nElectricity Bill Details:\n");
    printf("House Name: %s\n", bill->houseName);
    printf("Customer ID: %d\n", bill->customerID);
    printf("Units Consumed: %d\n", bill->unitsConsumed);
    printf("Total Amount: Rs%.2f\n", bill->totalAmount);

    if (bill->rebateDays >= 0) {
        printf("Rebate Days: %d\n", bill->rebateDays);
        printf("Rebate Amount: Rs%.2f\n", bill->rebateAmount);
    }

    if (bill->fineDays >= 0) {
        printf("Fine Days: %d\n", bill->fineDays);
        printf("Fine Amount: Rs%.2f\n", bill->fineAmount);
    }

    printf("Grand Total: Rs%.2f\n", bill->grandTotal);

    return 0;
}
