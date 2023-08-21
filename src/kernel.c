void print(char* string);
void intToCharArray(int number, char *result);
void intToHexCharArray(int number, char *result);

char* TextBuffer = (char*)0xb8000;


typedef struct {
 
	unsigned long long Base;
	unsigned long long Length;
	//unsigned int LengthLow;
	//unsigned int LengthHigh;
	unsigned int Type;
	unsigned int ACPI;
 
} MemoryMapEntry;


void start(){

    char buffer[1024];
    MemoryMapEntry* memMap = (MemoryMapEntry*)0x7000;

    print("Hello :)\n\n");

    intToHexCharArray(memMap[0].Length, &buffer);
    print("Length: ");
    print(buffer);
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