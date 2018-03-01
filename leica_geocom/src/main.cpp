#include <string.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <geocom_class_leica/geocom_class_leica.h>
#include <iomanip>                   //sirve para poder establecer la precision de std::cout
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>
#include <sstream>
#include <chrono>
#include <ctime>
#include <geocom_class_leica/QtcpSocket/my_client/mytcpclient.h>
#include <stdexcept>

using std::fstream;
using namespace std;
GeoCom_Class_Leica leica;
bool finishThread=false;
float x,y,z,measureTime;
myTCPclient dataSendClient;

void continuousMeaThread()
{
    std::vector<std::string> coordinateMea;
    while (finishThread==false) {
        coordinateMea = leica.TMC_GetCoordinate(1000,1);
        if (stod(coordinateMea[0])==0 || stod(coordinateMea[0])==1284){
            x = stod(coordinateMea[1]);
            y = stod(coordinateMea[2]);
            z = stod(coordinateMea[3]);
            measureTime = stod(coordinateMea[4]);

            ///**************************************************/
            std::string sendDataPosition = "{" + std::to_string(x) + ";" + std::to_string(y) + ";" + std::to_string(z) + ";" + std::to_string(measureTime) + "}";
            if(dataSendClient.writeData(QByteArray(sendDataPosition.c_str(),sendDataPosition.size()))==false){
                std::cout << "Error sending position through the socket to the server" << std::endl;
            }
            ///**************************************************/

        }
        else if(stod(coordinateMea[0])==1285){
            std::cout << "Angle values okay, but no valid distance. Prism lost." << std::endl;
        }
        else{
            std::cout << "Measurement could not be registered." << std::endl;
        }
    }
}

/******************************************************************************************************************************************************************************/

int main(int argc, char* argv[])
{
    if (argc < 3) {
            std::cout << "Missings arguments:" << std::endl;
            std::cout << "geocom_Leica.exe" << " <IP> <Port>" << std::endl;
            exit(0);
        }
    char direcIP[20];

    int port = atoi(argv[2]);
    sscanf(argv[1], "%s", direcIP);

    ///**************************************************/
    std::cout << "Connecting to PC with IP " << direcIP << " and port " << port << "..." << std::endl;
    if(dataSendClient.connectToHost(direcIP,port)==false)
    {
        std::cout << "Connection to PC failed" << std::endl;
    }
    else{
        std::cout << "Connection to PC successful" << std::endl;
    }

    ///**************************************************/


    if(leica.connectSocket(1212,"192.168.254.3")==false)
    {
        std::cout << "Connection to TS Leica failed" << std::endl;
        return 0;
    }
    else{
        std::cout << "Connection to TS Leica successful" << std::endl;
    }



    leica.initTotalStation();

    char input[1000];

    bool fin=false;
    while(fin==false)
    {
        std::cout << "\n****************************OPERATIONS MENU****************************" << std::endl;
        std::cout << "- Press 1 to measure flight trajectory." << std::endl;
        std::cout << "- Press 2 to take one simple measurement." << std::endl;
        std::cout << "- Press 3 to visualize last measure point." << std::endl;
        std::cout << "- Press 4 to EXIT." << std::endl;
        std::cin.getline(input,1000);
        if(strlen(input)==1){
            switch (*input) // Different replys for the menu
            {
                case '1':
                {
                    finishThread=false;
                    std::thread measThread (continuousMeaThread);
                    std::cout << "\nMeasuring trajectory..." << std::endl;
                    std::cout << "\nPress F to finish measure" << std::endl;
                    while(finishThread==false)
                    {
                        std::cin.getline(input,1000);
                        if ((*input=='f' || *input=='F') && strlen(input)==1){
                            finishThread = true;
                            measThread.join();
                        }
                        else {
                            std::cout << "\nERROR. That is not the F letter. Try again" << std::endl;
                        }
                    }
                    break;
                }
                case '2' :
                {
                    std::vector<std::string> coordinateMea;
                    coordinateMea = leica.TMC_GetCoordinate(100,1);
                    x = stod(coordinateMea[1]);
                    y = stod(coordinateMea[2]);
                    z = stod(coordinateMea[3]);
                    measureTime = stod(coordinateMea[4]);
                    std::cout << std::setprecision(10) << "\nVisualizing measured point...\n" << "X: " << x << "\nY: " << y << "\nZ: " << z << "\nMeasure Time: " << measureTime << std::endl;
                    break;
                }
                case '3' :
                {
                    std::cout << std::setprecision(10) << "\nVisualizing last measured point...\n" << "X: " << x << "\nY: " << y << "\nZ: " << z << "\nMeasure Time: " << measureTime <<std::endl;
                    break;
                }
                case '4':
                {
                    std::cout << "\nEXIT..." << std::endl;
                    fin = true;
                    break;
                }
                default:
                {
                    std::cout << "\nERROR. That is not a number of the menu. Try again" << std::endl;
                    break;
                }
            }
        }
        else {
            std::cout << "\nERROR. That is not a number of the menu. Try again" << std::endl;
        }
    }
    return 0;
}
