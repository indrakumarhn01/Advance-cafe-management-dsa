#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Item {
    char name[50];          // Item name
    char customer[50];      // Customer name
    int customerId;         // Customer ID
    int quantity;           // Quantity of the item
    float price;            // Price of the item
    struct Item *left;      // Pointer to the left subtree
    struct Item *right;     // Pointer to the right subtree
} Item;

// Function to print the current inventory (in-order traversal)
void printInventory(Item *root) {
    printf("     Current inventory     :\n");
    printf("%-20s %-20s %-20s %-20s %-20s\n", "*Customer ID", "Customer", "Item Name", "Quantity", "Price*");
    
    // Traverse the tree in-order and print the details of each item
    inOrderTraversal(root);
    printf("\n");
}

// In-order traversal of the BST
void inOrderTraversal(Item *root) {
    if (root != NULL) {
        inOrderTraversal(root->left);
        printf("%-20d %-20s %-20s %-20d %-20.2f\n", root->customerId, root->customer, root->name, root->quantity, root->price);
        inOrderTraversal(root->right);
    }
}

// Function to print the menu of available items
void printMenu() {
    printf("     Menu     :\n");
    printf("%-20s %-20s\n", "*Item Name", "Price*");
    printf("%-20s %-20s\n", "Tea", "10.0");
    printf("%-20s %-20s\n", "Coffee", "15.0");
    printf("%-20s %-20s\n", "Sandwich", "25.0");
    printf("%-20s %-20s\n", "Burger", "30.0");
    printf("%-20s %-20s\n", "Pizza", "40.0");
    printf("%-20s %-20s\n", "Pepsi", "20.0");
    printf("\n");
}

// Function to create a new item
Item *createItem(char *name, char *customer, int customerId, int quantity, float price) {
    Item *newItem = (Item *) malloc(sizeof(Item));
    strncpy(newItem->name, name, 50);
    strncpy(newItem->customer, customer, 50);
    newItem->customerId = customerId;
    newItem->quantity = quantity;
    newItem->price = price;
    newItem->left = NULL;
    newItem->right = NULL;
    return newItem;
}

// Function to insert a new item into the BST
Item *insertItem(Item *root, Item *newItem) {
    // If the BST is empty, the new item becomes the root
    if (root == NULL) {
        return newItem;
    }

    // Traverse the BST to find the correct position to insert the new item
    if (newItem->customerId < root->customerId) {
        root->left = insertItem(root->left, newItem);
    } else {
        root->right = insertItem(root->right, newItem);
    }

    return root;
}

// Function to add a new item to the inventory
Item *addItem(Item *root, char *name, char *customer, int *nextCustomerId, int quantity) {
    // Price lookup based on item and quantity
    float price = 0.0;
    if (strcmp(name, "Tea") == 0) {
        price = 10.0 * quantity;
    } else if (strcmp(name, "Coffee") == 0) {
        price = 15.0 * quantity;
    } else if (strcmp(name, "Sandwich") == 0) {
        price = 25.0 * quantity;
    } else if (strcmp(name, "Burger") == 0) {
        price = 30.0 * quantity;
    } else if (strcmp(name, "Pizza") == 0) {
        price = 40.0 * quantity;
    } else if (strcmp(name, "Pepsi") == 0) {
        price = 20.0 * quantity;
    }

    // Create a new item
    Item *newItem = createItem(name, customer, (*nextCustomerId)++, quantity, price);

    // Insert the new item into the BST
    return insertItem(root, newItem);
}

// Function to find the minimum item in the BST
Item *findMinimum(Item *root) {
    if (root == NULL)
        return NULL;
    else if (root->left == NULL)
        return root;
    else
        return findMinimum(root->left);
}

// Function to remove an item from the inventory for a specific customer
Item *removeItem(Item *root, char *customer) {
    // If the BST is empty, return NULL
    if (root == NULL)
        return NULL;

    // Traverse the BST to find the item to be removed
    if (strcmp(customer, root->customer) < 0)
        root->left = removeItem(root->left, customer);
    else if (strcmp(customer, root->customer) > 0)
        root->right = removeItem(root->right, customer);
    else {
        // Found the item to be removed
        if (root->left == NULL && root->right == NULL) {
            // Case 1: Leaf node, no children
            free(root);
            return NULL;
        } else if (root->left == NULL) {
            // Case 2: Single child (right)
            Item *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            // Case 2: Single child (left)
            Item *temp = root->left;
            free(root);
            return temp;
        } else {
            // Case 3: Two children
            Item *minRight = findMinimum(root->right);
            strcpy(root->name, minRight->name);
            strcpy(root->customer, minRight->customer);
            root->customerId = minRight->customerId;
            root->quantity = minRight->quantity;
            root->price = minRight->price;
            root->right = removeItem(root->right, minRight->customer);
        }
    }

    return root;
}

// Function to save the inventory data to a file
void saveInventoryToFile(Item *root, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Traverse the BST and write each item's details to the file (in-order traversal)
    saveInOrderTraversal(root, file);

    fclose(file);
    printf("Inventory data saved to file: %s\n", filename);
}

// In-order traversal of the BST and save the items to the file
void saveInOrderTraversal(Item *root, FILE *file) {
    if (root != NULL) {
        saveInOrderTraversal(root->left, file);
        fprintf(file, "%d;%s;%s;%d;%.2f\n", root->customerId, root->customer, root->name, root->quantity, root->price);
        saveInOrderTraversal(root->right, file);
    }
}

// Function to load the inventory data from a file
Item *loadInventoryFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return NULL;
    }

    Item *root = NULL;
    char line[256];

    // Read each line from the file and create a new item in the BST
    while (fgets(line, sizeof(line), file) != NULL) {
        Item *newItem = (Item *) malloc(sizeof(Item));
        sscanf(line, "%d;%[^;];%[^;];%d;%f", &newItem->customerId, newItem->customer, newItem->name, &newItem->quantity, &newItem->price);
        newItem->left = NULL;
        newItem->right = NULL;

        // Insert the new item into the BST
        root = insertItem(root, newItem);
    }

    fclose(file);
    printf("Inventory data loaded from file: %s\n", filename);
    return root;
}

int main() {
    Item *root = NULL;                      // Root of the BST
    int choice;
    char name[50];
    char customer[50];
    int nextCustomerId = 1;                 // Starting customer ID

    do {
        printf("===========================================\n");
        printf("              Cafe Management System             \n");
        printf("===========================================\n");
        printf("1. Print current inventory\n");
        printf("2. Add item to inventory\n");
        printf("3. Remove item from inventory\n");
        printf("4. Save inventory to file\n");
        printf("5. Load inventory from file\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printInventory(root);
                break;

            case 2:
                printMenu();
                printf("Enter the item name to add: ");
                scanf("%s", name);
                printf("Enter the customer name: ");
                scanf("%s", customer);
                printf("Enter the quantity: ");
                int quantity;
                scanf("%d", &quantity);
                root = addItem(root, name, customer, &nextCustomerId, quantity);
                break;

            case 3:
                printf("Enter the customer name: ");
                scanf("%s", customer);
                root = removeItem(root, customer);
                break;

            case 4:
                saveInventoryToFile(root, "inventory.txt");
                break;

            case 5:
                root = loadInventoryFromFile("inventory.txt");
                break;

            case 6:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        printf("\n");
    } while (choice != 6);

    // Free the memory allocated for the BST (post-order traversal)
    freeMemory(root);

    return 0;
}

// Post-order traversal of the BST and free the memory
void freeMemory(Item *root) {
    if (root != NULL) {
        freeMemory(root->left);
        freeMemory(root->right);
        free(root);
    }
}