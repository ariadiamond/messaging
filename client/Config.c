#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<arpa/inet.h>

#include"Util.h"

#define MINI_BUFFER 80

bool validName(char* buffer) {
    //reserved names
    if (strncmp(buffer, "NULL", ID_SIZE) == 0)
        return false;

    //unprintable characters + sp
    for (uint8_t i = 0; i < ID_SIZE; i++) {
        if (buffer[i] < 33 || buffer[i] > 126)
            return false;
    }

    return true;
}

void getInput(int fdesc) {
    //get name
    printf("What is your ID (%d characters)?\n", ID_SIZE);
    char buffer[MINI_BUFFER + 1];
    read(STDIN_FILENO, buffer, MINI_BUFFER);
    buffer[ID_SIZE] = 0;
    //check name
    if (!validName(buffer)) {
        printf("That's not a valid ID\n");
        exit(1);
    }
    dprintf(fdesc, "Name: %s\n", buffer);



    //address
    printf("Whaterror is the address of the server (IPv4 please)?\n");
    ssize_t bytesRead = read(STDIN_FILENO, buffer, MINI_BUFFER);
    buffer[bytesRead  - 1] = 0; //\n as last value
    struct sockaddr_in sockAddr;
    // Checking to see if it is a valid address
    if (inet_pton(AF_INET, buffer, &(sockAddr.sin_addr)) != 1) {
        printf("That's not a valid address\n");
        exit(1);
    }
    //write address
    dprintf(fdesc, "Address: ");
    write(fdesc, buffer, bytesRead - 1);
    dprintf(fdesc, "\n");

    //port
    printf("What port is the server on?\n");
    bytesRead = read(STDIN_FILENO, buffer, MINI_BUFFER);
    //check valid port
    int port = atoi(buffer);
    if (port < 1 || port > UINT16_MAX) {
        printf("That's not a valid port\n");
        exit(1);
    }
    dprintf(fdesc, "Port: %hu\n", port);

}



int main(void) {
    //create config file
    int fdesc = open(".userconfig", O_CREAT | O_TRUNC | O_WRONLY, 0644);
    if (fdesc < 0) {
        perror("Cannot open file");
        exit(1);
    }

    getInput(fdesc);

    //Cleanup
    close(fdesc);
    return 0;
}