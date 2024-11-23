#include <stdio.h>
#include <time.h>
#include <string.h>

// Structure to represent the electricity bill

/** WAP to prepare an Electricty bill payment system with the following details:
a. for consumed units of 0-20, rate is Rs 4 per unit
b. for 21-50, rate is Rs 7.30 per unit
c. for 51-150, rate is 8.60 per unit
d. for 151-250, rate is 9.50 per unit
e. for more than 250 units, rate is Rs 11.00 per unit.

Add rebate and fine system as well. Bill reading date is taken from the user. The date of program execution is the date of bill payment. 
a. If bill is paid from 1 to 7 days from the date of bill reading, 2% of the total amount is rebated.
b. If from 8 to 15 days, no rebate and fine.
c. If from 16 to 30 days, 5% fine.
d. If from 31 to 40 days, 10% fine.
e. If after 40 days, 25% fine.

Add necessary structure.

Store respective customer ID and customer name is array. Write the code in C.
 **/ 

struct ElectricityBill {
    char customerName[50];
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

// Function to calculate the electricity bill
void calculateBill(struct ElectricityBill* bill, int billReadingDay, int billReadingMonth, int billReadingYear) {
    bill->totalAmount = 0;
    bill->rebateDays = 0;
    bill->rebateAmount = 0;
    bill->fineDays = 0;
    bill->fineAmount = 0;

    // Calculate total amount based on units consumed
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

    // Get current date (date of program execution)
    time_t now;
    time(&now);
    struct tm* paymentDate = localtime(&now);
    int todayDay = paymentDate->tm_mday;
    int todayMonth = paymentDate->tm_mon + 1; // tm_mon is 0-indexed
    int todayYear = paymentDate->tm_year + 1900; // tm_year is years since 1900

    // Calculate rebate days from the date of bill reading
    int daysFromReadingDate = (todayYear - billReadingYear) * 365 +
                             (todayMonth - billReadingMonth) * 30 +
                             (todayDay - billReadingDay);

    if (daysFromReadingDate >= 0 && daysFromReadingDate <= 7) {
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
    // Array of structures to store customer information
    struct ElectricityBill customers[20];

    // Database of customer IDs with respective usernames
    int customerIDs[20] = {101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
                           111, 112, 113, 114, 115, 116, 117, 118, 119, 120};

    char customerNames[20][50] = {"Pawan Niroula", "Prajwal Adhikari", "Pritiz Lamichhane", "Pawan Khanal", "Rahul Chaurasiya",
                                  "Prabesh Khatiwada", "Progress Khatiwada", "Pawan Khanal", "Ram Khanal", "Rohan Kushwaha",
                                  "Nishant ", "Nischal Pandey", "Prashan Adhikari", "Rajiv",
                                  "Nairitya Luitel", "Rittika Parajuli", "Ruby Bhattarai", "Pooja Murau", "Niharika",
                                  "Pramesh Rajbanshi"};

    // Get customer information
    int inputID;
    int customerIndex = -1;
    while (customerIndex == -1) {
        printf("Enter the Customer ID: ");
        scanf("%d", &inputID);

        // Search for the customer ID in the database
        for (int i = 0; i < 20; i++) {
            if (inputID == customerIDs[i]) {
                customerIndex = i;
                break;
            }
        }

        if (customerIndex == -1) {
            printf("Customer ID not found in the database. Please enter a valid Customer ID within the given range.\n");
        }
    }

    // Assign customer details from the database
    struct ElectricityBill* bill = &customers[customerIndex];
    strcpy(bill->customerName, customerNames[customerIndex]);
    bill->customerID = customerIDs[customerIndex];

    printf("Enter the units consumed: ");
    scanf("%d", &bill->unitsConsumed);

    int billReadingDay, billReadingMonth, billReadingYear;
    printf("Enter the bill reading date (mm/dd/yyyy): ");
    scanf("%d/%d/%d", &billReadingMonth, &billReadingDay, &billReadingYear);

    calculateBill(bill, billReadingDay, billReadingMonth, billReadingYear);

    printf("\nElectricity Bill Details:\n");
    printf("Customer Name: %s\n", bill->customerName);
    printf("Customer ID: %d\n", bill->customerID);
    printf("Units Consumed: %d\n", bill->unitsConsumed);
    printf("Total Amount: Rs%.2f\n", bill->totalAmount);

    if (bill->rebateDays >= 0) {
        //printf("Rebate Days: %d\n", bill->rebateDays);
        printf("Rebate Amount: Rs%.2f\n", bill->rebateAmount);
    }

    if (bill->fineDays > 0) {
        //printf("Fine Days: %d\n", bill->fineDays);
        printf("Fine Amount: Rs%.2f\n", bill->fineAmount);
    }

    printf("Grand Total: Rs%.2f\n", bill->grandTotal);

    return 0;
}

