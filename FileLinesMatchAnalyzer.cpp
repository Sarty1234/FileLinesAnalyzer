#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>


const bool TTRP_CHECK = true;
const bool POP_FROM_VECTOR = true;


bool CheckIfLineIsInFile(std::string filename, std::string targetline);
bool CheckIfLineIsInVector(std::vector<std::string>& vec, const std::string& target, bool pop = false);
bool CheckIfStringAInB(std::string A, std::string B);
std::vector<std::string> TurnFileIntoVector(std::string FilePath, int expectedLinesAmount = 1000);
void SaveVectorToFile(std::vector<std::string>& v, std::ofstream& file);


std::vector<std::string> FileBData = std::vector<std::string>();

int main()
{
    const std::string outFolderPath = "out";
    const std::string outInANotBFilePath = outFolderPath + "\\InANotB.txt";
    const std::string outInBNotAFilePath = outFolderPath + "\\InBNotA.txt";
    const std::string outInAInBFilePath = outFolderPath + "\\InAInB.txt";



    std::string firstFilePath = "";
    std::string secondFilePath = "";
    std::string expectedLinesInFileBStr = "";
    int expectedLinesInFileB = 1000;



    std::cout << "Enter path to file with objects to find: ";
    std::getline(std::cin, firstFilePath);
    std::cout << "Enter path to file with objects to compare: ";
    std::getline(std::cin, secondFilePath);
    std::cout << "Enter expected lines count of file B: ";
    if (std::getline(std::cin, expectedLinesInFileBStr)) {
        expectedLinesInFileB = std::stoi(expectedLinesInFileBStr);
    }


    std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();



    // making sure out directory exists
    if (!std::filesystem::exists(outFolderPath)) {
        std::filesystem::create_directory(outFolderPath);
    }
    if (!std::filesystem::exists(outInANotBFilePath)) {
        std::ofstream outInANotBFileCreation(outInANotBFilePath);
        outInANotBFileCreation.close();
    }
    if (!std::filesystem::exists(outInBNotAFilePath)) {
        std::ofstream outInBNotAFileCreation(outInBNotAFilePath);
        outInBNotAFileCreation.close();
    }
    if (!std::filesystem::exists(outInAInBFilePath)) {
        std::ofstream outInAInBFileCreation(outInAInBFilePath);
        outInAInBFileCreation.close();
    }



    std::ifstream firstFile(firstFilePath);
    std::ofstream InANotBFile(outInANotBFilePath, std::ios::trunc);
    std::ofstream InBNotAFile(outInBNotAFilePath, std::ios::trunc);
    std::ofstream InAInBFile(outInAInBFilePath, std::ios::trunc);


    if (!firstFile.is_open()) {
        std::cerr << "ERROR while opening first file" << std::endl;
        return 1;
    }
    if (!InANotBFile.is_open()) {
        std::cerr << "ERROR while opening out InANotB file" << std::endl;
        return 1;
    }
    if (!InBNotAFile.is_open()) {
        std::cerr << "ERROR while opening out InBNotA file" << std::endl;
        return 1;
    }
    if (!InAInBFile.is_open()) {
        std::cerr << "ERROR while opening out InAInB file" << std::endl;
        return 1;
    }


    FileBData = TurnFileIntoVector(secondFilePath, expectedLinesInFileB);




    std::cout << "File analyze started:" << std::endl;


    std::string firstFileLine = "";
    int totalOperations = 0;
    int mathchingLinesCount = 0;
    int InANotBLinesCount = 0;
    while (std::getline(firstFile, firstFileLine)) {
        // For TTRP
         if (TTRP_CHECK && !CheckIfStringAInB("https:", firstFileLine)) continue;



        totalOperations++;
        if (CheckIfLineIsInVector(FileBData, firstFileLine, POP_FROM_VECTOR))
        {
            mathchingLinesCount++;

            InAInBFile << firstFileLine << std::endl;
        }
        else {
            InANotBLinesCount++;

            InANotBFile << firstFileLine << std::endl;
        }

        std::cout << "Total: " << totalOperations << ", "
            << "InAInB: " << mathchingLinesCount << ", "
            << "InANotB: " << InANotBLinesCount << ".\r";
        std::cout.flush();
    }
    if (POP_FROM_VECTOR) {
        SaveVectorToFile(FileBData, InBNotAFile);
    }
    firstFile.close();
    InAInBFile.close();
    InANotBFile.close();
    InBNotAFile.close();

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
        << "InAInB: " << mathchingLinesCount << ", "
        << "InANotB: " << InANotBLinesCount << "." << std::endl;
    std::cout << "Time: " << hours << "h "
        << minutes << "m "
        << seconds << "s "
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
        if (TTRP_CHECK && !CheckIfStringAInB("https:", openedFileLine)) continue;

        if (targetline == openedFileLine) {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}



bool CheckIfLineIsInVector(std::vector<std::string>& vec, const std::string& target, bool pop) {
    // 1. Find the first element not less than target (O(log n))
    auto it = std::lower_bound(vec.begin(), vec.end(), target);

    // 2. Check if we actually found the target
    if (it != vec.end() && *it == target) {
        if (pop) {
            // 3. Erase it directly using the iterator (O(n) for shifting, but no second search)
            vec.erase(it);
        }
        return true;
    }
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


std::vector<std::string> TurnFileIntoVector(std::string FilePath, int expectedLinesAmount) {
    std::ifstream file(FilePath);
    std::vector<std::string> resultVector = std::vector<std::string>();
    resultVector.reserve(expectedLinesAmount);


    if (!file.is_open()) {
        std::cerr << "ERROR while opening first file" << std::endl;
        return resultVector;
    }


    std::string openedFileLine = "";
    while (std::getline(file, openedFileLine)) {
        // For TTRP
        if (TTRP_CHECK && !CheckIfStringAInB("https:", openedFileLine)) continue;

        resultVector.push_back(std::move(openedFileLine));
        // move передає право власності поінтера
    }

    file.close();

    std::sort(resultVector.begin(), resultVector.end());
    return resultVector;
}


void SaveVectorToFile(std::vector<std::string> &v, std::ofstream &file)
{
    for (std::string value : v) {
        file << value << std::endl;
    }
}