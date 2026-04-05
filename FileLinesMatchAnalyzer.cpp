#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>


const bool TTRPCHECK = true;


bool CheckIfLineIsInFile(std::string filename, std::string targetline);
bool CheckIfStringAInB(std::string A, std::string B);

int main()
{
    std::string firstFilePath = "";
    std::string secondFilePath = "";

    const std::string outFolderPath = "out";
    const std::string outMissingFilePath = outFolderPath + "\\Missing.txt";
    const std::string outMathcingFilePath = outFolderPath + "\\Mathcing.txt";


    std::cout << "Enter path to file with objects to find: ";
    std::getline(std::cin, firstFilePath);
    std::cout << "Enter path to file with objects to compare: ";
    std::getline(std::cin, secondFilePath);


    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();



    // making sure out directory exists
    if (!std::filesystem::exists(outFolderPath)) {
        std::filesystem::create_directory(outFolderPath);
    }
    if (!std::filesystem::exists(outMissingFilePath)) {
        std::ofstream outMissingFileCreation(outMissingFilePath);
        outMissingFileCreation.close();
    }
    if (!std::filesystem::exists(outMathcingFilePath)) {
        std::ofstream outMatchingFileCreation(outMathcingFilePath);
        outMatchingFileCreation.close();
    }



    std::ifstream firstFile(firstFilePath);
    std::ofstream missingFile(outMissingFilePath, std::ios::trunc);
    std::ofstream matchingFile(outMathcingFilePath, std::ios::trunc);


    if (!firstFile.is_open()) {
        std::cerr << "ERROR while opening first file" << std::endl;
        return 1;
    }
    if (!missingFile.is_open()) {
        std::cerr << "ERROR while opening out missing file" << std::endl;
        return 1;
    }
    if (!matchingFile.is_open()) {
        std::cerr << "ERROR while opening out matching file" << std::endl;
        return 1;
    }




    std::cout << "File analyze started:" << std::endl;


    std::string firstFileLine = "";
    int totalOperations = 0;
    int mathchingLinesCount = 0;
    int missingLinesCount = 0;
    while (std::getline(firstFile, firstFileLine)) {
        // For TTRP
         if (TTRPCHECK && !CheckIfStringAInB("https:", firstFileLine)) continue;



        totalOperations++;
        if (CheckIfLineIsInFile(secondFilePath, firstFileLine))
        {
            mathchingLinesCount++;

            matchingFile << firstFileLine << std::endl;
        }
        else {
            missingLinesCount++;

            missingFile << firstFileLine << std::endl;
        }

        std::cout << "Total: " << totalOperations << ", "
            << "Matching: " << mathchingLinesCount << ", "
            << "Missing: " << missingLinesCount << ".\r";
        std::cout.flush();
    }
    firstFile.close();

    std::chrono::steady_clock::time_point endTime = std::chrono::steady_clock::now();
    std::chrono::milliseconds functionTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    long long functionTimeInt = functionTime.count();
    int hours = functionTimeInt / 1000 / 60 / 60;
    functionTimeInt -= hours * 1000 * 60 * 60;
    int minutes = functionTimeInt / 1000 / 60;
    functionTimeInt -= minutes * 1000 * 60;
    int seconds = functionTimeInt / 1000;
    functionTimeInt -= seconds * 1000;

    std::cout << "Total: " << totalOperations << ", "
        << "Matching: " << mathchingLinesCount << ", "
        << "Missing: " << missingLinesCount << "." << std::endl;
    std::cout << "Time: " << hours << "h "
        << minutes << "m "
        << seconds / 1000 << "s "
        << functionTimeInt << "mil " << std::endl;


    std::cout << "File analyze finished." << std::endl;
    std::cout << "\n\nPress Enter to exit...";
    std::cout.flush();
    std::getchar();
}


bool CheckIfLineIsInFile(std::string filename, std::string targetline) 
{
    std::ifstream file(filename);


    if (!file.is_open()) {
        std::cerr << "ERROR while opening first file" << std::endl;
        return false;
    }


    std::string openedFileLine = "";
    while (std::getline(file, openedFileLine)) {
        // For TTRP
        if (TTRPCHECK && !CheckIfStringAInB("https:", openedFileLine)) continue;

        if (targetline == openedFileLine) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}


bool CheckIfStringAInB(std::string A, std::string B)
{
    if (B.find(A) != std::string::npos) {
        return true;
    }
    else {
        return false;
    }
}