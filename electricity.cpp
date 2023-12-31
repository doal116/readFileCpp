#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>

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

struct Billing
{
    // row data
    int year;
    string month;
    float electricConsumption;
    float pvProduction;
    float upNonPv;
    float upPv;

    // data after calculation
    float cumulativePv_Kwh;
    float shortage_Kwh;
    float surplus_Kwh;
    float usedCumlativePv_Kwh;

    float upNonPv_deducted_upPv_Euro;
    float upPv_Euro;
    float upNonPv_UpPv_Euro;

    float upNonPv_Euro;
    // upNonPv_Euro - upNonPv_deducted_upPv_Euro
    float savedMoney_Euro;

    // savedMoney_Euro += savedMoney_Euro
    float cummulativeSavedMoney_Euro;
};

vector<Electricity> initializingDataList()
{
    fstream electricData;
    vector<Electricity> dataImport;

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
                    dataImport.push_back(cellBuffer);
                    column = 0;
                }
            }
        }
    }
    electricData.close();
    return dataImport;
}

void displayDataList(vector<Electricity> electricDataList)
{
    for (int i = 0; i < electricDataList.size(); i++)
    {
        cout << setw(10) << left << electricDataList[i].year << "|"
             << setw(10) << left << electricDataList[i].month << "|"
             << setw(10) << left << electricDataList[i].electricConsumption << "|"
             << setw(10) << left << electricDataList[i].pvProduction << "|"
             << setw(10) << left << electricDataList[i].upNonPv << "|"
             << setw(10) << left << electricDataList[i].upPv << endl;
    }
}

vector<Billing> initializingBillingList(vector<Electricity> electricDataList)
{
    vector<Billing> resultList;
    for (int i = 0; i < electricDataList.size(); i++)
    {
        Billing buffer;
        buffer.year = electricDataList[i].year;
        buffer.month = electricDataList[i].month;
        buffer.electricConsumption = electricDataList[i].electricConsumption;
        buffer.pvProduction = electricDataList[i].pvProduction;
        buffer.upNonPv = electricDataList[i].upNonPv;
        buffer.upPv = electricDataList[i].upPv;

        buffer.cumulativePv_Kwh = 0;
        buffer.shortage_Kwh = 0;
        buffer.surplus_Kwh = 0;
        buffer.usedCumlativePv_Kwh = 0;
        buffer.upNonPv_deducted_upPv_Euro = 0;
        buffer.upPv_Euro = 0;
        buffer.upNonPv_UpPv_Euro = 0;
        buffer.upNonPv_Euro = 0;
        buffer.savedMoney_Euro = 0;
        buffer.cummulativeSavedMoney_Euro = 0;

        resultList.push_back(buffer);
    }

    return resultList;
}

void displayBillList(vector<Billing> billList)
{
    for (int i = 0; i < billList.size(); i++)
    {
        cout << setw(5) << left << billList[i].year << "|"
             << setw(10) << left << billList[i].month << "|"
             << setw(5) << left << billList[i].electricConsumption << "|"
             << setw(8) << left << billList[i].pvProduction << "|"
             << setw(8) << left << billList[i].cumulativePv_Kwh << "|"
             << setw(8) << left << billList[i].shortage_Kwh << "|"
             << setw(8) << left << billList[i].surplus_Kwh << "|"
             << setw(8) << left << billList[i].usedCumlativePv_Kwh << "|"
             << setw(7) << left << billList[i].upNonPv << "|"
             << setw(8) << left << billList[i].upPv << "|"
             << setw(8) << left << billList[i].upNonPv_deducted_upPv_Euro << "|"
             << setw(8) << left << billList[i].upPv_Euro << "|"
             << setw(8) << left << billList[i].upNonPv_UpPv_Euro << "|"
             << setw(8) << left << billList[i].savedMoney_Euro << "|"
             << setw(8) << left << billList[i].cummulativeSavedMoney_Euro << endl;
    }
}

vector<Billing> cummulativeStorageCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for (int i = 0; i < resultList.size(); i++)
    {
        if (i != 0 && resultList[i - 1].surplus_Kwh != 0)
            resultList[i].cumulativePv_Kwh = resultList[i - 1].surplus_Kwh + resultList[i - 1].cumulativePv_Kwh;

        if (resultList[i - 1].shortage_Kwh != 0 && i != 0 && resultList[i - 1].cumulativePv_Kwh != 0)
        {
            resultList[i].cumulativePv_Kwh = resultList[i - 1].cumulativePv_Kwh - resultList[i - 1].shortage_Kwh;
            if (resultList[i - 1].shortage_Kwh > resultList[i - 1].cumulativePv_Kwh)
                resultList[i].cumulativePv_Kwh = 0;

            if (resultList[i - 1].year != resultList[i].year)
            {
                resultList[i].shortage_Kwh = resultList[i].electricConsumption - resultList[i].cumulativePv_Kwh - resultList[i].pvProduction;
            }
        }
    }
    return resultList;
}

vector<Billing> shortageCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;

    for (int i = 0; i < resultList.size(); i++)
    {
        float difference = resultList[i].electricConsumption - resultList[i].pvProduction;
        if (difference > 0)
            resultList[i].shortage_Kwh = difference;
        else
        {
            resultList[i].shortage_Kwh = 0;
        }
    }

    return resultList;
}

vector<Billing> surPlusCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for (int i = 0; i < resultList.size(); i++)
    {
        resultList[i].surplus_Kwh = resultList[i].pvProduction - resultList[i].electricConsumption;
        if (resultList[i].surplus_Kwh < 0)
        {
            resultList[i].surplus_Kwh = 0;
        }
        else
        {
            resultList[i].surplus_Kwh = resultList[i].pvProduction - resultList[i].electricConsumption;
        }
    }

    return resultList;
}

vector<Billing> usedCummulativeCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for (int i = 0; i < resultList.size(); i++)
    {
        if (resultList[i].cumulativePv_Kwh >= resultList[i].shortage_Kwh)
            resultList[i].usedCumlativePv_Kwh = resultList[i].shortage_Kwh;
        else
            resultList[i].usedCumlativePv_Kwh = 0;
    }

    return resultList;
}
vector<Billing> diffUpNonPV_upPvCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for (int i = 0; i < resultList.size(); i++)
    if (resultList[i].shortage_Kwh > resultList[i].cumulativePv_Kwh)
        resultList[i].upNonPv_deducted_upPv_Euro = resultList[i].shortage_Kwh * resultList[i].upNonPv;
    else resultList[i].upNonPv_deducted_upPv_Euro = 0;

    return resultList;
}
vector<Billing> upPvCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for (int i = 0; i < resultList.size(); i++)
        resultList[i].upPv_Euro = resultList[i].pvProduction * resultList[i].upPv;

    return resultList;
}
vector<Billing> sum_DiffUpNonPvUpPvCalc_upPvCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for(int i = 0; i < resultList.size(); i++)
    resultList[i].upNonPv_UpPv_Euro = resultList[i].upNonPv_deducted_upPv_Euro + resultList[i].upPv_Euro;
    return resultList;
}
vector<Billing> savedMoneyCalc(vector<Billing> billList)
{
    vector<Billing> resultList = billList;
    for (int i = 0; i < resultList.size(); i++)
    {
        float priceWithoutPv = resultList[i].electricConsumption * resultList[i].upNonPv;
        float priceWithPv = (resultList[i].electricConsumption - resultList[i].pvProduction) * resultList[i].upNonPv;

        resultList[i].savedMoney_Euro = priceWithoutPv - (priceWithPv + resultList[i].upPv_Euro);
    }

    return resultList;
}
vector<Billing> cummulativeSavedMoneyCalc(vector<Billing> billList)
{
    vector<Billing> resultList;

    return resultList;
}
int main()
{
    vector<Electricity> electricDataList;
    electricDataList = initializingDataList();
    // displayDataList(electricDataList);
    vector<Billing> billList;

    // initializing table calc with raw data.
    billList = initializingBillingList(electricDataList);

    // surPlusCalc kwh
    billList = surPlusCalc(billList);

    // shortageCalc kwh
    billList = shortageCalc(billList);

    // cummulativeStorageCalc kwh
    billList = cummulativeStorageCalc(billList);

    // usedCummulativeCalc kwh
    billList = usedCummulativeCalc(billList);

    // emperor diffUpNonPV_upPvCalc euro
    billList = diffUpNonPV_upPvCalc(billList);

    // upPvCalc euro
    billList = upPvCalc(billList);

    // emperor sum_DiffUpNonPvUpPvCalc_upPvCalc euro
    billList = sum_DiffUpNonPvUpPvCalc_upPvCalc(billList);

    // savedMoneyCalc euro
    billList = savedMoneyCalc(billList);
    displayBillList(billList);

    // cummulativeSavedMoneyCalc euro
    billList = cummulativeSavedMoneyCalc(billList);
    displayBillList(billList);

    return 0;
}