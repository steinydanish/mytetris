#ifndef FILE_OPERATOR_CLASS_H
#define FILE_OPERATOR_CLASS_H

#include <iostream>
#include <vector>

class FILE_OPERATOR
{
    public:
        static int dumpFilesIntoStrings(long type, std::vector<std::pair<long, std::string>>& in);
        static void printStringToErrorLog(std::string error);
    private:
};

#endif
