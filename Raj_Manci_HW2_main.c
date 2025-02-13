/**************************************************************
* Class::  CSC-415-02 Spring 2025
* Name:: Manci Raj
* Student ID:: 922823535
* GitHub-Name:: manciraj
* Project:: Assignment 2 – Stucture in Memory and Buffering
*
* File:: Raj_Manci_HW2_main.c
*
* Description:: The program dynamically constructs a
* personalInfo structure, buffers incoming text, and
* then commits them as blocks with commitBlock. It
* ensures proper memory usage and structure manipulation.
*
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Raj_Manci_HW2_Header.h"

int main(int argc, char *argv[]) {
    // It will make sure at least 3 arguments are provided which are FirstName, LastName, and Message.
    if (argc < 4) {
        printf("Usage: %s <FirstName> <LastName> <Message>\n", argv[0]);
        return 1;
    }

    // It will Allocate the memory for personalInfo structure.
    personalInfo *info = (personalInfo *)malloc(sizeof(personalInfo));
    if (!info) {
        printf("Memory allocation failed for personalInfo\n");
        return 1;
    }

    // This will Allocate the memory for firstName and lastName.
    info->firstName = (char *)malloc(strlen(argv[1]) + 1); 
    info->lastName = (char *)malloc(strlen(argv[2]) + 1);
    
    // It will Check if memory allocation for names was successful or not
    if (!info->firstName || !info->lastName) {
        printf("Memory allocation for names failed\n");

        // Ensure to free any allocated memory before exiting
        free(info->firstName);
        free(info->lastName);
        free(info);
        return 1;
    }

    // THis will Copy firstName and lastName from command line arguments
    strcpy(info->firstName, argv[1]);
    strcpy(info->lastName, argv[2]);

    // Assigning the student ID and level as required.
    info->studentID = 922823535; 
    info->level = SENIOR;

    // Setting the known programming languages as required.
    info->languages = 0;
    info->languages |= KNOWLEDGE_OF_C;
    info->languages |= KNOWLEDGE_OF_JAVA;
    info->languages |= KNOWLEDGE_OF_ARM_ASSEMBLER;

    // Below will copy the message successfully using strncpy function to prevent buffer overflow.
    strncpy(info->message, argv[3], sizeof(info->message) - 1);
    info->message[sizeof(info->message) - 1] = '\0'; // Ensure null termination it prevent undefined behavior or memory corruption.

    // Below will Write the personal info to memory
    if (writePersonalInfo(info) != 0) {
        printf("Error writing personal info.\n");

        // Free allocated memory before exiting to minimize the memory leak.
        free(info->firstName);
        free(info->lastName);
        free(info);
        return 1;
    }

    // This will ensure to free the allocated memory for structure after using it.
    free(info->firstName);
    free(info->lastName);
    free(info);

    // This will ensure allocate buffer for block storage.
    char *buffer = (char *)malloc(BLOCK_SIZE);
    if (!buffer) {
        printf("Buffer allocation failed\n");
        return 1;
    }

    int offset = 0;  // It will track the position in buffer.
    const char *data; // It will Store data retrieved from getNext().

    // Retrieve and store strings in the buffer
    while ((data = getNext()) != NULL) {
        int len = strlen(data) + 1; // Include null terminator

        // Checking if adding the string exceeds the buffer size or not.
        if (offset + len > BLOCK_SIZE) {
            commitBlock(buffer); // Commit the full buffer.
            free(buffer); // Ensure to free old buffer to prevent memory leak.

            // Allocate a new buffer for the next block to ensure the proper memory usage.
            buffer = (char *)malloc(BLOCK_SIZE);
            if (!buffer) {
                printf("Buffer allocation failed\n");
                return 1;
            }
            offset = 0; // Reset offset to ensure new buffer does not try to continue storing data at the old position.
        }

        // Copy data into the buffer at the correct position.
        memcpy(buffer + offset, data, len);
        offset += len; // Update the offset to track next available space.
    }

    // Commit any remaining data in the buffer to ensure the proper memory usage.
    if (offset > 0) {
        commitBlock(buffer);
    }

    // Free buffer memory before exiting.
    free(buffer);

    // Verify memory layout using checkIt() function as required.
    return checkIt();
}
