void print(char* string);
void intToCharArray(int number, char *result);
void intToHexCharArray(int number, char *result);

char* TextBuffer = (char*)0xb8000;


typedef struct {
 
	unsigned int BaseLow;
	unsigned int BaseHigh;
	unsigned int LengthLow;
	unsigned int LengthHigh;
	unsigned int Type;
	unsigned int ACPI;
 
} MemoryMapEntry;


void start(unsigned int count, MemoryMapEntry* val){

    char buffer[1024];

    print("Hello :)\n\n");

    print("count: ");
    intToCharArray(count, buffer);
    print(buffer);
    print("\n");

    print("stack bottom: ");
    intToHexCharArray((int)&val[count], buffer);
    print(buffer);
    print("\n");

    print("stack top: ");
    intToHexCharArray((int)val, buffer);
    print(buffer);
    print("\n");
    

    unsigned int totalSpace = 0;
    unsigned int usableSpace = 0;

    for(int i = 0; i < count; i++){
        print("base: ");
        intToHexCharArray(val[i].BaseLow, buffer);
        print(buffer);

        print(", length: ");
        intToCharArray(val[i].LengthLow, buffer);
        print(buffer);

        print(", type: ");
        intToCharArray(val[i].Type, buffer);
        print(buffer);

        print("\n");

        totalSpace += val[i].LengthLow;
        if(val[i].Type == 1){
            usableSpace += val[i].LengthLow;
        }
    }

    print("\ntotal space(kb): ");
    intToCharArray((totalSpace / 1024), buffer);
    print(buffer);

    print("\nusable space(kb): ");
    intToCharArray((usableSpace / 1024), buffer);
    print(buffer);

    return;
}



void print(char* string){

    for(int i = 0; string[i] != '\0'; i++){

        switch (string[i])
        {
        case '\n':
            //TextBuffer += 160;
            TextBuffer -= 0xb8000;
            TextBuffer += 160 - ((int)TextBuffer % 160);
            TextBuffer += 0xb8000;
            break;
        
        default:
            *TextBuffer = string[i];
            TextBuffer += 2;
            break;
        }
    }
}

void intToCharArray(int number, char* result) {
    // Check for negative number
    int isNegative = 0;
    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    // Convert the number to string (character array)
    int index = 0;
    do {
        result[index++] = '0' + number % 10;
        number /= 10;
    } while (number > 0);

    // Add negative sign if needed
    if (isNegative) {
        result[index++] = '-';
    }

    // Null-terminate the string
    result[index] = '\0';

    // Reverse the string since it was generated in reverse order
    int start = 0;
    int end = index - 1;
    while (start < end) {
        char temp = result[start];
        result[start] = result[end];
        result[end] = temp;
        start++;
        end--;
    }
}

void intToHexCharArray(int number, char *result) {
    // Check for negative number
    int isNegative = 0;
    if (number < 0) {
        isNegative = 1;
        number = -number;
    }

    // Convert the number to hexadecimal string (character array)
    int index = 0;
    do {
        int digit = number % 16;
        if (digit < 10) {
            result[index++] = '0' + digit;
        } else {
            result[index++] = 'A' + (digit - 10);
        }
        number /= 16;
    } while (number > 0);

    // Add 0x to show that it is in hex
    result[index++] = 'x';
    result[index++] = '0';

    // Add negative sign if needed
    if (isNegative) {
        result[index++] = '-';
    }

    // Null-terminate the string
    result[index] = '\0';

    // Reverse the string since it was generated in reverse order
    int start = 0;
    int end = index - 1;
    while (start < end) {
        char temp = result[start];
        result[start] = result[end];
        result[end] = temp;
        start++;
        end--;
    }
}