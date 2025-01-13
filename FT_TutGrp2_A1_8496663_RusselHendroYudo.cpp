#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>  // For consistent column formatting

using namespace std;

int minX, maxX, minY, maxY;
string cityMapFile;
int **cityData;
int rows, cols;

// Function prototypes
void readConfiguration();
void loadCityMap();
void displayCityMap();
void cleanupMemory();

int main() {
    int choice;
    do {
        // Display main menu
        cout << "Student ID: 10255876 | Name: Russel Hendro Yudo\n";
        cout << "1. Read Configuration File\n";
        cout << "2. Display City Map\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: readConfiguration(); break;
            case 2: displayCityMap(); break;
            case 3: cleanupMemory(); break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}

// Read the configuration file to initialize grid dimensions and file paths
void readConfiguration() {
    string configFile;
    cout << "Enter the configuration filename: ";
    cin >> configFile;

    ifstream config(configFile);
    if (!config) {
        cout << "Error opening configuration file.\n";
        return;
    }

    string line;
    while (getline(config, line)) {
        if (line.find("GridX_IdxRange=") != string::npos) {
            stringstream ss(line.substr(15));
            char dash;
            ss >> minX >> dash >> maxX;
        }
        else if (line.find("GridY_IdxRange=") != string::npos) {
            stringstream ss(line.substr(15));
            char dash;
            ss >> minY >> dash >> maxY;
        }
        else if (line.find("citylocation.txt") != string::npos) {
            cityMapFile = line.substr(line.find("=") + 1);
        }
    }

    // Set the grid size based on index ranges
    rows = maxY - minY + 1;
    cols = maxX - minX + 1;

    // Dynamically allocate arrays for city data
    cityData = new int*[rows];
    for (int i = 0; i < rows; i++) {
        cityData[i] = new int[cols];
    }

    // Initialize cityData array to -1 (indicating no city)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cityData[i][j] = -1;  // No city by default
        }
    }

    cout << "Configuration loaded successfully.\n";
}

// Load city locations from the citylocation.txt file
void loadCityMap() {
    ifstream cityFile(cityMapFile);
    if (cityFile) {
        string line;
        while (getline(cityFile, line)) {
            int x, y, cityId;
            string cityName;

            // Parse the city location line (e.g., [1,1]-3-Big_City)
            stringstream ss(line);
            ss.ignore(1);  // Skip '['
            ss >> x;
            ss.ignore(1);  // Skip ','
            ss >> y;
            ss.ignore(2);  // Skip ']-'
            ss >> cityId;
            ss.ignore(1);  // Skip '-'
            ss >> cityName;

            // Store city data in the grid (adjust coordinates to match 0-based indexing)
            if (x >= minX && x <= maxX && y >= minY && y <= maxY) {
                cityData[y - minY][x - minX] = cityId;  // Mark city ID on grid
            }
        }
        cityFile.close();
    } else {
        cout << "Error opening city location file.\n";
    }
}

void displayCityMap() {
    loadCityMap();
    cout << "City Map (Grid 8x8):\n";

    // Display the top border
    cout << "   # # # # # # # # # # # \n";

    // Display the grid with city markers
    for (int i = rows - 1; i >= 0; i--) {
        cout << setw(2) << i + minY << "  # ";  // Print the current row number (starting from 0)

        for (int j = 0; j < cols; j++) {
            if (cityData[i][j] == -1) {
                cout << "   ";  // Empty cell is left blank, padded to align
            } else {
                // Display city with its ID, padded to align
                cout << setw(2) << cityData[i][j] << " ";
            }
        }
        cout << "#\n";
    }

    // Display the bottom border and column indices
    cout << "   # # # # # # # # # # # \n";
    cout << "      0 1 2 3 4 5 6 7 8 \n";
}

// Cleanup memory before exiting the program
void cleanupMemory() {
    for (int i = 0; i < rows; i++) {
        delete[] cityData[i];
    }
    delete[] cityData;
    cout << "Memory cleaned up successfully.\n";
}
