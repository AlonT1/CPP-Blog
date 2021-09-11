// 75a_io_files.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>


int main()
{
    //c style file io
    FILE* fp; //file pointer
    if (!(fp = fopen("text.txt", "r+"))) // r+ means open exisiting file - r&w to it (w+ means the same but creates new file)
        printf("%s", "error");
    char buff[255];
    while(fgets(buff, 255, fp)!=NULL) // copy each line from txt in fp to buff
        printf("%s", buff);
    fprintf(fp, "%s\n", "Shalom"); //write to file - stdout for files
    fclose(fp);

    std::cout << "-------------------------" << std::endl;

    //cpp style io
    //fstream - r&w, ofstream - w, ifstream - r
    std::fstream myfile; 
    myfile.open("text.txt");
    if (myfile.is_open())
        for (std::string str; getline(myfile, str);)
            std::cout << str << std::endl;

    myfile.clear(); // at the end of getline for loop, we get a failbit-
    //when loop tries to read for the time but only EOF is left to read, therefore a "clear" is necessary
    myfile << "ken@#!#" << std::endl; //like cout, stdout
    myfile.close();
}