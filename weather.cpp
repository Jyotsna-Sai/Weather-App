#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <regex>

using namespace std;


// Extract a number from JSON
string getNumber(const string& data, const string& key)
{
    string patternText = "\"" + key + "\"\\s*:\\s*(-?[0-9]+\\.?[0-9]*)";

    regex pattern(patternText);
    smatch match;

    if (regex_search(data, match, pattern))
    {
        return match[1].str();
    }

    return "N/A";
}


// Extract text from JSON
string getText(const string& data, const string& key)
{
    string patternText = "\"" + key + "\"\\s*:\\s*\"([^\"]*)\"";

    regex pattern(patternText);
    smatch match;

    if (regex_search(data, match, pattern))
    {
        return match[1].str();
    }

    return "N/A";
}


int main()
{
    string city;

    // ==========================================
    // PUT YOUR OPENWEATHERMAP API KEY HERE
    // ==========================================
    string apiKey = "YOUR_API_KEY";


    cout << "========================================\n";
    cout << "             WEATHER APP\n";
    cout << "========================================\n\n";


    cout << "Enter city name: ";
    getline(cin, city);


    // Create API URL
    string url =
        "https://api.openweathermap.org/data/2.5/weather?q=" +
        city +
        "&appid=" +
        apiKey +
        "&units=metric";


    cout << "\nFetching weather information...\n";


    // Get JSON using PowerShell
    string command =
        "powershell -Command "
        "\"(Invoke-WebRequest -Uri '" +
        url +
        "' -UseBasicParsing).Content | "
        "Out-File -Encoding utf8 weather.json\"";


    int result = system(command.c_str());


    if (result != 0)
    {
        cout << "\nCould not connect to weather server.\n";
        return 1;
    }


    // Open weather.json
    ifstream file("weather.json");


    if (!file)
    {
        cout << "\nCould not open weather.json\n";
        return 1;
    }


    // Read complete JSON file
    string data;
    string line;

    while (getline(file, line))
    {
        data += line;
    }

    file.close();


    // Remove UTF-8 BOM if PowerShell added one
    if (data.size() >= 3 &&
        (unsigned char)data[0] == 0xEF &&
        (unsigned char)data[1] == 0xBB &&
        (unsigned char)data[2] == 0xBF)
    {
        data.erase(0, 3);
    }


    // Check API errors
    if (data.find("\"cod\":401") != string::npos)
    {
        cout << "\nERROR: Invalid API key.\n";
        return 1;
    }


    if (data.find("\"cod\":404") != string::npos)
    {
        cout << "\nERROR: City not found.\n";
        return 1;
    }


    // ==========================================
    // Extract weather information
    // ==========================================

    string weatherCity = getText(data, "name");

    string country = getText(data, "country");

    string temperature = getNumber(data, "temp");

    string feelsLike = getNumber(data, "feels_like");

    string humidity = getNumber(data, "humidity");

    string pressure = getNumber(data, "pressure");

    string windSpeed = getNumber(data, "speed");

    string condition = getText(data, "main");

    string description = getText(data, "description");


    // ==========================================
    // Display weather
    // ==========================================

    cout << "\n\n========================================\n";
    cout << "          WEATHER INFORMATION\n";
    cout << "========================================\n\n";

    cout << "City          : " << weatherCity << "\n";
    cout << "Country       : " << country << "\n";
    cout << "Temperature   : " << temperature << " C\n";
    cout << "Feels Like    : " << feelsLike << " C\n";
    cout << "Humidity      : " << humidity << " %\n";
    cout << "Pressure      : " << pressure << " hPa\n";
    cout << "Wind Speed    : " << windSpeed << " m/s\n";
    cout << "Condition     : " << condition << "\n";
    cout << "Description   : " << description << "\n";

    cout << "\n========================================\n";


    return 0;
}