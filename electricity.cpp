#include <iostream>
#include <fstream>
#include <string>
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
struct Table
{
    //Given Data
    int year;
    string month;
    float electricConsumption; //Denoted as Grid Consumption
    float pvProduction;
    float upNonPv;
    float upPv;

    //Calculated Data
    float cumulativeStorage_KWh;
    float shortage_KWh;
    float surplus_KWh;
    float usedFromCumulative_KWh;
    float DiffInConsumptionProduction_Euro; // Denoted A
    float costOfProducedEnergy_Euro;        //Denoted B
    float sumDiffInConsumpProduc_Euro; //Sum of A and B
    float savedCost_Euro; //This calculates (Grid Consumption * unit Price) - (A +  B)
    float cumulativeSavedCost_Euro;
};

//void displayDataList(Electricity electricDataList[24]);
//void initializingTableValues(Electricity electricDataList[24]);
void initializingDataList(Electricity (&cellBuffer)[24]);
void displayTable(Table Table[24]);
void calcShortage_KWh(Table (&Data)[24]);
void calcSurplus_KWh(Table (&Data)[24]);
void calcCumulativeStorage_KWh(Table (&Data)[24]);
void usedFromCumulative_KWh(Table (&Data)[24]);
void DiffInConsumptionProduction_Euro(Table (&Data)[24]);
void costOfProducedEnergy_Euro(Table (&Data)[24]);
void sumDiffInConsumpProduc_Euro(Table (&Data)[24]);
void savedCost_Euro(Table (&Data)[24]);
void cumulativeSavedCost_Euro(Table (&Data)[24]);
float calcAverageConsumption(Table (&Data)[24]);
float calcAverageProductionDecrease(Table (&Data)[24]);

int main()

{
    Electricity electricDataList[24];
    Table newTable[24];
    initializingDataList(electricDataList);
    for (int  i = 0; i < 24; i++)
    {
        newTable[i].year = electricDataList[i].year;
        newTable[i].month = electricDataList[i].month;
        newTable[i].electricConsumption = electricDataList[i].electricConsumption;
        newTable[i].pvProduction = electricDataList[i].pvProduction;
        newTable[i].upNonPv = electricDataList[i].upNonPv;
        newTable[i].upPv = electricDataList[i].upPv;

        newTable[i].cumulativeStorage_KWh = 0;
        newTable[i].shortage_KWh = 0;
        newTable[i].surplus_KWh = 0;
        newTable[i].usedFromCumulative_KWh = 0;
        newTable[i].DiffInConsumptionProduction_Euro = 0;
        newTable[i].costOfProducedEnergy_Euro = 0;
        newTable[i].sumDiffInConsumpProduc_Euro = 0;
        newTable[i].savedCost_Euro = 0;
        newTable[i].cumulativeSavedCost_Euro = 0;
    }
    calcShortage_KWh(newTable);
    calcSurplus_KWh(newTable);
    calcCumulativeStorage_KWh(newTable);
    usedFromCumulative_KWh(newTable);
    DiffInConsumptionProduction_Euro(newTable);
    costOfProducedEnergy_Euro(newTable);
    sumDiffInConsumpProduc_Euro(newTable);
    savedCost_Euro(newTable);
    cumulativeSavedCost_Euro(newTable);

    cout<< setw(6) << left <<  "Year" << setw(12) << left << "Month"
                << setw(10) << left << "Cnsump." << setw(10) << left << "Prodct."
                << setw(9) << left <<  "C.S(KWh)" << setw(9) << left << "Stg(KWh)"
                << setw(10) << left << "SKWh" << setw(10) << left << "UC(KWh)"
                << setw(10) << left << "UCEuro" << setw(10) << left << "UPEuro" 
                << setw(15) << left << "Msn.Elct.Euro" << setw(9) << left << "PE.Euro" 
                << setw(13) << left << "SmBlsEuro" << setw(13) << left << "SvdCstEuro"
                << setw(6) << left << "C.SvdCstEuro" << endl;

    displayTable(newTable);

    float averageConsumption = 0; float averageProduction = 0;
    averageConsumption = calcAverageConsumption(newTable);
    averageProduction = calcAverageProductionDecrease(newTable);
    cout << "Average Consumption: \t" << averageConsumption << "\t\t"
    << "1% Decrease of Average Production: \t" << averageProduction << endl;
    return 0;
}

/*void displayDataList(Electricity electricDataList[24])
{
        for (int i = 0; i < 24; i++)
        {
            cout<< setw(6) << left <<  electricDataList[i].year
                << setw(12) << left << electricDataList[i].month
                << setw(5) << left << electricDataList[i].electricConsumption
                << setw(8) << left << electricDataList[i].pvProduction
                << setw(8) << left << electricDataList[i].upNonPv
                << setw(10) << left << electricDataList[i].upPv << endl;
        }
}*/
void initializingDataList(Electricity (&cellBuffer)[24])
{
    fstream electricData;
    electricData.open("./data.txt", ios::in);

    if (!electricData)
        cout << "file doesn't exist." << endl;
    else
    {
        char buffer;
        string data = "";
        int column = 0;
        int i = 0;
        int test=0;
        while (!electricData.eof())
        {
            if (electricData.eof())
            break;
            electricData >> buffer;
            if (buffer != ',')
            {
                data += buffer;
            }
            else
            {
                switch (column)
                {
                case 0:
                    cellBuffer[i].year = stoi(data);
                    break;
                case 1:
                    cellBuffer[i].month = data;
                    break;
                case 2:
                    cellBuffer[i].electricConsumption = stof(data);
                    break;
                case 3:
                    cellBuffer[i].pvProduction = stof(data);
                    break;
                case 4:
                    cellBuffer[i].upNonPv = stof(data);
                    break;
                case 5:
                    cellBuffer[i].upPv = stof(data);
                    break;
                default:
                    break;
                }
                column += 1;
                data = "";
                if (column == 6)
                {
                    if (i == 23 && column == 6) break;
                    column = 0;
                    i++;
                }
            }
        }
    }
    electricData.close();
}
void displayTable(Table Table[24])
{
    for (int i = 0; i < 24; i++)
    {
        cout<< setw(6) << left <<  Table[i].year << setw(12) << left << Table[i].month
                << setw(10) << left << Table[i].electricConsumption << setw(11) << left << Table[i].pvProduction
                << setw(9) << left <<  Table[i].cumulativeStorage_KWh << setw(9) << left << Table[i].shortage_KWh
                << setw(10) << left << Table[i].surplus_KWh << setw(10) << left << Table[i].usedFromCumulative_KWh
                << setw(10) << left << Table[i].upNonPv << setw(10) << left << Table[i].upPv 
                << setw(15) << left << Table[i].DiffInConsumptionProduction_Euro << setw(10) << left << Table[i].costOfProducedEnergy_Euro 
                << setw(12) << left << Table[i].sumDiffInConsumpProduc_Euro << setw(13) << left << Table[i].savedCost_Euro
                << setw(6) << left << Table[i].cumulativeSavedCost_Euro << endl;
        }
}
void calcShortage_KWh(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        Data[i].shortage_KWh = Data[i].electricConsumption - Data[i].pvProduction;
        if(Data[i].shortage_KWh < 1) {Data[i].shortage_KWh = 0;}
    }
}
void calcSurplus_KWh(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        Data[i].surplus_KWh = Data[i].pvProduction - Data[i].electricConsumption;
        if(Data[i].surplus_KWh < 1) {Data[i].surplus_KWh = 0;}
    }
}
void calcCumulativeStorage_KWh(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        if (i == 0) Data[i].cumulativeStorage_KWh = 0;
        else{
            Data[i].cumulativeStorage_KWh = Data[i-1].cumulativeStorage_KWh + Data[i-1].surplus_KWh;
            if (Data[i-1].shortage_KWh > 0 && Data[i-1].surplus_KWh == 0 && Data[i].cumulativeStorage_KWh > 0) 
                Data[i].cumulativeStorage_KWh = Data[i-1].cumulativeStorage_KWh - Data[i-1].shortage_KWh;
            if (Data[i].cumulativeStorage_KWh < 0) Data[i].cumulativeStorage_KWh = 0;}
    }
}
void usedFromCumulative_KWh(Table (&Data)[24])
{
for (int i = 0; i < 24; i++)
    {
        if (i == 0) 
                Data[i].usedFromCumulative_KWh = 0;
        else if (Data[i].cumulativeStorage_KWh >  Data[i].shortage_KWh)
                Data[i].usedFromCumulative_KWh = Data[i].shortage_KWh;
        else if (Data[i].shortage_KWh > Data[i].cumulativeStorage_KWh + Data[i].pvProduction)
                Data[i].usedFromCumulative_KWh = Data[i].cumulativeStorage_KWh;
    }
}
void DiffInConsumptionProduction_Euro(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        if (Data[i].electricConsumption > (Data[i].pvProduction + Data[i].cumulativeStorage_KWh))
        Data[i].DiffInConsumptionProduction_Euro = (Data[i].electricConsumption - (Data[i].pvProduction + Data[i].cumulativeStorage_KWh)) * Data[i].upNonPv;
        else 
        Data[i].DiffInConsumptionProduction_Euro = 0;
    } 
}
void costOfProducedEnergy_Euro(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        if (Data[i].electricConsumption > Data[i].pvProduction && Data[i].cumulativeStorage_KWh <= 0)
        Data[i].costOfProducedEnergy_Euro = Data[i].pvProduction * Data[i].upPv;
        else if (Data[i].electricConsumption > Data[i].pvProduction && Data[i].cumulativeStorage_KWh > 0)
        Data[i].costOfProducedEnergy_Euro = Data[i].electricConsumption * Data[i].upPv;
        else if (Data[i].electricConsumption < Data[i].pvProduction)
        Data[i].costOfProducedEnergy_Euro = Data[i].electricConsumption * Data[i].upPv;
    }
}
void sumDiffInConsumpProduc_Euro(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        Data[i].sumDiffInConsumpProduc_Euro = Data[i].DiffInConsumptionProduction_Euro + Data[i].costOfProducedEnergy_Euro;
    }
}
void savedCost_Euro(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
        if ((Data[i].electricConsumption > Data[i].pvProduction) && (Data[i].cumulativeStorage_KWh == 0 && Data[i].shortage_KWh > 1))
        {Data[i].savedCost_Euro = (Data[i].electricConsumption * Data[i].upNonPv) - ((Data[i].shortage_KWh * Data[i].upNonPv) + (Data[i].pvProduction * Data[i].upPv));}
        else{
            Data[i].savedCost_Euro = (Data[i].electricConsumption * Data[i].upNonPv) - (Data[i].electricConsumption * Data[i].upPv);
        }
    }
}
void cumulativeSavedCost_Euro(Table (&Data)[24])
{
    for (int i = 0; i < 24; i++)
    {
    if(i == 0) Data[i].cumulativeSavedCost_Euro = Data[i].savedCost_Euro;
    else{
        Data[i].cumulativeSavedCost_Euro = Data[i].savedCost_Euro + Data[i-1].cumulativeSavedCost_Euro;
    }
    }
}
float calcAverageConsumption(Table (&Data)[24])
{
    float averageConsumption = 0;
    for (int i = 0; i < 24; i++)
    {
        averageConsumption = Data[i].electricConsumption + averageConsumption;
    }
    return averageConsumption;
}
float calcAverageProductionDecrease(Table (&Data)[24])
{
    float averageProduction = 0;
    for (int i = 0; i < 24; i++)
    {
        averageProduction = Data[i].pvProduction + averageProduction;
    }
    averageProduction = averageProduction * 0.01;
    return averageProduction;
}

