#include<stdio.h>
#include<stdlib.h>

#define MAX_INITIAL_PRODUCTS 100
#define MAX_PRODUCT_ID 10000
#define MAX_NAME_LENGTH 50
#define MAX_PRODUCT_PRICE 100000.0
#define MAX_PRODUCT_QUANTITY 1000000

int currNumOfProducts = 0;

typedef struct{
    int productID;
    char* productName;
    float price;
    int quantity;
} Product;

int getValidInteger(){
    int number;
    while(1){
        if(scanf("%d", &number) != 1){
            printf("Invalid input. Please enter again: ");
            while(getchar() != '\n');
        }
        else{
            if(getchar() != '\n'){
                printf("Invalid input. Please enter again: ");
                while(getchar() != '\n');
            }
            else{
                return number;
            }
        }
    }
}

float getValidFloat(){
    float number;
    while(1){
        if(scanf("%f", &number) != 1){
            printf("Invalid input. Please enter again: ");
            while(getchar() != '\n');
        }
        else{
            if(getchar() != '\n'){
                printf("Invalid input. Please enter again: ");
                while(getchar() != '\n');
            }
            else{
                return number;
            }
        }
    }
}

int getInitialNumberOfProducts(){
    int initialProducts;
    while(1){
        initialProducts = getValidInteger();
        if(initialProducts < 1 || initialProducts > MAX_INITIAL_PRODUCTS){
            printf("Please enter a number between 1-100: ");

        }
        else{
            return initialProducts;
        }
    }
}

int idExists(int productId, Product* productList){
    for(int i = 0; i < currNumOfProducts; i++){
        if(productId == (productList + i)->productID){
            return 1;
        }
    }
    return 0;
}

int getProductID(Product* productList){
    int productID;
    while(1){
        productID = getValidInteger();
        if(productID < 1 || productID > MAX_PRODUCT_ID){
            printf("Please enter a number between 1-10000: ");
        }
        else if(idExists(productID, productList)){
            printf("ID already exists! Please enter again: ");
        }
        else{
            return productID;
        }
    }
}

int getStringLength(char* string){
    if(!string) return 0;
    int length = 0;
    while(*string != '\0'){
        length++;
        string++;
    }
    return length;
}

char* getProductName(){
    char* productName = malloc(MAX_NAME_LENGTH * sizeof(char));
    fgets(productName, MAX_NAME_LENGTH, stdin);

    int length = getStringLength(productName);
    if(*(productName + length - 1) == '\n'){
        *(productName + length - 1) = '\0';
    }
    else{
        while(getchar() != '\n');
    }
    return productName;
}

float getProductPrice(){
    float productPrice;
    while(1){
        productPrice = getValidFloat();
        if(productPrice < 0.0 || productPrice > MAX_PRODUCT_PRICE){
            printf("Please enter a price between 0-100000: ");
        }
        else{
            return productPrice;
        }
    }
}

int getProductQuantity(){
    int productQuantity;
    while(1){
        productQuantity = getValidInteger();
        if(productQuantity < 0 || productQuantity > MAX_PRODUCT_QUANTITY){
            printf("Please enter a number between 0-1000000: ");
        }
        else{
            return productQuantity;
        }
    }
}

void getProductDetails(Product* productList, int index){
    printf("Product ID: ");
    (productList + index)->productID = getProductID(productList);
    printf("Product Name: ");
    (productList + index)->productName = getProductName();
    printf("Product Price: ");
    (productList + index)->price = getProductPrice();
    printf("Product Quantity: ");
    (productList + index)->quantity = getProductQuantity();
    printf("\n");
}

void displayMenu(){
    printf("\n========= INVENTORY MENU =========\n");
    printf("1. Add New Product\n");
    printf("2. View All Products\n");
    printf("3. Update Quantity\n");
    printf("4. Search Product by ID\n");
    printf("5. Search Product by Name\n");
    printf("6. Search Product by Price Range\n");
    printf("7. Delete Product\n");
    printf("8. Exit\n");
}

Product* addNewProduct(Product* productList){
    Product* tempPtr = realloc(productList, (currNumOfProducts + 1) * sizeof(Product));
    if(tempPtr != NULL){
        productList = tempPtr;
        currNumOfProducts++;
    }
    else{
        printf("\nFailed to reallocate memory!\n");
        return productList;
    }
    printf("\nEnter new product details: \n");
    getProductDetails(productList, currNumOfProducts - 1);
    printf("Product added successfully!\n");
    return productList;
}

void productDetails(Product* product){
    printf("Product ID: %-5d | Name: %-10s | Price: %-8.2f | Quantity: %-7d\n",
    product->productID, product->productName, product->price, product->quantity);
}

void viewAllProducts(Product* productList){
    printf("\n========= PRODUCT LIST =========\n");
    for(int i = 0; i < currNumOfProducts; i++){
        productDetails(productList + i);
    }
}

void updateQuantity(Product* productList){
    int id, newQuantity;
    printf("\nEnter product ID to update quantity: ");
    id = getValidInteger();
    int index = 0;
    while(index < currNumOfProducts){
        if((productList + index)->productID == id){
            break;
        }
        index++;
    }
    if(index == currNumOfProducts){
        printf("ID does not exist!\n");
        return;
    }
    printf("Enter new Quantity: ");
    newQuantity = getProductQuantity();
    (productList + index)->quantity = newQuantity;
    printf("Quantity updated successfully!\n");
}

void searchProductByID(Product* productList){
    int id;
    printf("\nEnter Product ID to search: ");
    id = getValidInteger();
    int index = 0;
    while(index < currNumOfProducts){
        if((productList + index)->productID == id){
            break;
        }
        index++;
    }
    if(index == currNumOfProducts){
        printf("ID does not exist!\n");
        return;
    }
    printf("Product Found: \n");
    productDetails(productList + index);
}

int isSubstring(char* substring, char* string){
    while(*string != '\0'){
        char* strPtr = string;
        char* subPtr = substring;
        while(*strPtr != '\0' && *subPtr != '\0' && *strPtr == *subPtr){
            strPtr++;
            subPtr++;
        }
        if(*subPtr == '\0'){
            return 1;
        }
        string++;
    }
    return 0;
}

void searchProductByName(Product* productList){
    printf("\nEnter name to search (partial allowed): ");
    char* nameToFind = getProductName();
    int found = 0;
    for(int index = 0; index < currNumOfProducts; index++){
        if(isSubstring(nameToFind, (productList + index)->productName)){
            if(!found){
                printf("Products found: \n");
                found = 1;
            }
            productDetails(productList + index);
        }
    }
    if(!found){
        printf("No product found!\n");
    }
    free(nameToFind);
}

void searchProductByPriceRange(Product* productList){
    printf("\nEnter minimum price: ");
    float minimum = getValidFloat();
    printf("Enter maximum price: ");
    float maximum = getValidFloat();
    int found = 0;
    for(int index = 0; index < currNumOfProducts; index++){
        if((productList + index)->price >= minimum && (productList + index)->price <= maximum){
            if(!found){
                printf("\nProducts in price range: \n");
                found = 1;
            }
            productDetails(productList + index);
        }
    }
    if(!found){
        printf("No products found in this price range!\n");
    }
}

void shiftProducts(Product* product, int index){
    while(index + 1 < currNumOfProducts){
        product->productID = (product + 1)->productID;
        product->productName = (product + 1)->productName;
        product->price = (product + 1)->price;
        product->quantity = (product + 1)->quantity;
        index++;
        product++;
    }
}

Product* deleteProduct(Product* productList){
    printf("Enter product ID to delete: ");
    int idToDelete = getValidInteger();
    int found = 0;
    for(int index = 0; index < currNumOfProducts; index++){
        if((productList + index)->productID == idToDelete){
            found = 1;
            free((productList + index)->productName);
            shiftProducts((productList + index), index);
            currNumOfProducts--;
            Product* tempPtr = realloc(productList, currNumOfProducts * sizeof(Product));
            if(tempPtr != NULL){
                productList = tempPtr;
            }
            else{
                printf("Item delete. Failed to reallocate memory.\n");
                return productList;
            }
            printf("Product deleted successfully!\n");
        }
    }
    if(!found){
        printf("ID does not exist!\n");
    }
    return productList;
}

void freeMemory(Product* productList){
    for(int i = 0; i < currNumOfProducts; i++){
        free((productList + i)->productName);
    }
    free(productList);
    productList = NULL;
}

void initiateManagement(Product* productList){
    int choice = 0;
    while(choice != 8){
        displayMenu();
        printf("Enter your choice: ");
        choice = getValidInteger();
        switch(choice){
            case 1:
                productList = addNewProduct(productList);
                break;
            case 2:
                viewAllProducts(productList);
                break;
            case 3:
                updateQuantity(productList);
                break;
            case 4:
                searchProductByID(productList);
                break;
            case 5:
                searchProductByName(productList);
                break;
            case 6:
                searchProductByPriceRange(productList);
                break;
            case 7:
                productList = deleteProduct(productList);
                break;
            case 8:
                freeMemory(productList);
                printf("\nMemory released successfully. Exiting program...");
                return;
            default:
                printf("Invalid input. Please try again.\n");
        }
    }
}

int main(){
    int initialProducts;
    printf("Enter initial number of products: ");
    initialProducts = getInitialNumberOfProducts();
    printf("\n");

    Product* productList = calloc(initialProducts, sizeof(Product));

    for(int i = 0; i < initialProducts; i++){
        printf("Enter details for product %d: \n", (i + 1));
        getProductDetails(productList, i);
    }
    currNumOfProducts = initialProducts;
    initiateManagement(productList);
}
