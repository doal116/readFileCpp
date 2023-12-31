#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

struct Electricity
{
    int year;
    string month;
    float electricConsumption;
    float pvProduction;
    float upNonPv;
    float upPv;
};

void initializingDataList(vector<Electricity> &electricDataList)
{
    fstream electricData;
    electricData.open("./data.txt", ios::in);

    if (!electricData)
        cout << "file doesn't exist." << endl;
    else
    {
        char buffer;
        string data = "";
        Electricity cellBuffer;
        int column = 0;

        while (!electricData.eof())
        {
            electricData >> buffer;
            if (electricData.eof())
                break;
            if (buffer != ',')
                data += buffer;
            else
            {
                switch (column)
                {
                case 0:
                    cellBuffer.year = stoi(data);
                    break;
                case 1:
                    cellBuffer.month = data;
                    break;
                case 2:
                    cellBuffer.electricConsumption = stof(data);
                    break;
                case 3:
                    cellBuffer.pvProduction = stof(data);
                    break;
                case 4:
                    cellBuffer.upNonPv = stof(data);
                    break;
                case 5:
                    cellBuffer.upPv = stof(data);
                    break;
                default:
                    break;
                }
                column += 1;
                data = "";
                if (column == 6)
                {
                    electricDataList.push_back(cellBuffer);
                    column = 0;
                }
            }
        }
    }

    electricData.close();
}

void displayDataList(vector<Electricity> electricDataList)
{
    for (int i = 0; i < electricDataList.size(); i++)
    {
        cout << setw(10) << electricDataList[i].year << "|"
             << setw(10) << electricDataList[i].month << "|"
             << setw(10) << electricDataList[i].electricConsumption << "|"
             << setw(10) << electricDataList[i].pvProduction << "|"
             << setw(10) << electricDataList[i].upNonPv << "|"
             << setw(10) << electricDataList[i].upPv << endl;
    }
}

int main()
{
    vector<Electricity> electricDataList;
    initializingDataList(electricDataList);
    displayDataList(electricDataList);
    return 0;
}