// for sleep function
#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#include <iostream>
#include <stdlib.h>
#include <sqlite3.h>
#include <vector>

using namespace std;

// function declarations
void loginScreen();
void adminMenu();
void adminAddPatient();
void adminAddDoctor();
void doctorMenu();
void doctorUpdatePatient();
void searchPatient();
void hospitalScreen();

// database global declaration
sqlite3 *db;
char *errorMessage = 0;

// database functions declaration
static int callback(void*, int, char**, char**);
static int searchCallback(void*, int, char**, char**);

// class declaration
class Patient;
class Doctor;

// global variable declaration;
string user;
Patient *patient;
Doctor *doctor;
string nullCheck;
vector<string> fileName;

class Patient {
    private: 
        string patientsMobileNumber;
        string patientsAge;
        string patientsFileName;
        string patientsFirstName;
        string patientsMiddleName;
        string patientsLastName;
        string patientsAddress;
        string patientsGender;
        string patientsBloodGroup;
        string patientsSymptoms;
        string patientsCheckedBy;
        string patientsMedicines;
    public: 
        Patient(){

        }
        Patient(string mobileNumber, string age, string fileName, string gender, string firstName, string middleName, string lastName, string address, string bloodGroup,string symptoms, string checkBy,string medicines){
            patientsMobileNumber = mobileNumber;
            patientsAge = age;
            patientsFileName = fileName;
            patientsFirstName = firstName;
            patientsMiddleName = middleName;
            patientsLastName = lastName;
            patientsAddress = address;
            patientsGender = gender;
            patientsBloodGroup = bloodGroup;
            patientsSymptoms = symptoms;
            patientsCheckedBy = checkBy;
            patientsMedicines = medicines;
        }
        void insertPatient(){
            string insertQuery = "INSERT INTO Patients(pFileName,pFirstName,pMiddleName,pLastName,pMobileNumber,pAddress,pGender,pAge,pBloodGroup,pSymptoms,pMedicines,pCheckedBy) VALUES('" + patientsFileName + "','" + patientsFirstName + "','" + patientsMiddleName + "','" + patientsLastName + "','" + patientsMobileNumber + "','" + patientsAddress + "','" + patientsGender + "','" + patientsAge + "','" + patientsBloodGroup + "','" + patientsSymptoms + "','" + patientsMedicines + "','" + patientsCheckedBy + "')";
            
            int rc = sqlite3_exec(db, insertQuery.c_str(), NULL, 0, &errorMessage);

            if(rc != SQLITE_OK){
                system("clear");
                cout << "Sql error: " << errorMessage;
                sleep(1);
                sqlite3_free(errorMessage);
            }  else {
                cout << "Patient created successfully";
                system("clear");
            }
        }
};

class Doctor {
    private: 
        string doctorsAge;
        string doctorsMobileNumber;
        string doctorsFirstName;
        string doctorsMiddleName;
        string doctorsLastName;
        string doctorsEmail;
        string doctorsAddress;
        string doctorsDegree;
        string doctorsSpecialization;
    public:
        Doctor(){

        }
        Doctor(string age, string mobileNumber, string firstName, string middleName, string lastName,string email, string address, string degree, string specialization){
            doctorsAge = age;
            doctorsMobileNumber = mobileNumber;
            doctorsFirstName = firstName;
            doctorsMiddleName = middleName;
            doctorsLastName = lastName;
            doctorsEmail = email;
            doctorsAddress = address;
            doctorsDegree = degree;
            doctorsSpecialization = specialization;
        }

        void insertDoctor(){
            string insertDoctorQuery = "INSERT INTO Doctors(dFirstName,dMiddleName,dLastName,dMobileNumber,dEmail,dAge,dDegree,dSpecialization,dAddress) VALUES('" + doctorsFirstName + "','" + doctorsMiddleName + "','" + doctorsLastName + "','" + doctorsMobileNumber + "','" + doctorsEmail + "','" + doctorsAge + "','" + doctorsDegree + "','" + doctorsSpecialization + "','" + doctorsAddress + "');";

            int rc = sqlite3_exec(db, insertDoctorQuery.c_str(), NULL, 0, &errorMessage);

            if(rc != SQLITE_OK){
                cout << "Doctor not inserted!!! SQL ERROR: " << errorMessage;
                sqlite3_free(errorMessage);
            }
        }                                                                                                                                                                                                                                                                                      
        
        void updatePatient(string bloodGroup, string symptoms, string medicines, string checkedBy, string patientsFileName){
            string updatePatientQuery = "UPDATE Patients SET pBloodGroup='" + bloodGroup + "', pSymptoms='" + symptoms + "', pCheckedBy='" + checkedBy + "', pMedicines='" + medicines + "' WHERE pFileName='" + patientsFileName + "'";

            int rc = sqlite3_exec(db, updatePatientQuery.c_str(), NULL, 0, &errorMessage);
            
            if(rc != SQLITE_OK){
                cout << "SQL ERROR: " << errorMessage;
                sqlite3_free(errorMessage);
                getchar();
                doctorMenu();
            }
            
        }
};

int main(){
    
    string createPatientTableQuery = "CREATE TABLE IF NOT EXISTS Patients("
        "pFileName VARCHAR(50) NOT NULL,"
        "pFirstName VARCHAR(20) NOT NULL,"
        "pMiddleName VARCHAR(20) NOT NULL,"
        "pLastName VARCHAR(20) NOT NULL,"
        "pMobileNumber VARCHAR(15) NOT NULL,"
        "pAddress VARCHAR(255) NOT NULL,"
        "pGender VARCHAR(7) NOT NULL,"
        "pAge VARCHAR(255) NOT NULL,"
        "pBloodGroup VARCHAR(4) NOT NULL,"
        "pSymptoms vARCHAR(255) NOT NULL,"
        "pMedicines VARCHAR(255) NOT NULL,"
        "pCheckedBy VARCHAR(100) NOT NULL"
    ");";

    string createDoctorTableQuery = "CREATE TABLE IF NOT EXISTS Doctors("
        "dFirstName VARCHAR(20) NOT NULL,"
        "dMiddleName VARCHAR(20) NOT NULL,"
        "dLastName VARCHAR(20) NOT NULL,"
        "dMobileNumber VARCHAR(15) NOT NULL,"
        "dEmail VARCHAR(255) NOT NULL,"
        "dAge VARCHAR(255) NOT NULL,"
        "dDegree VARCHAR(4) NOT NULL,"
        "dSpecialization vARCHAR(255) NOT NULL,"
        "dAddress VARCHAR(255) NOT NULL"
    ");";
    

    int rc = sqlite3_open("hospital.db", &db);

    if(rc){
        cout << "Can't open the database: %s" << endl << sqlite3_errmsg(db); 
        return (0);
    }

    rc = sqlite3_exec(db, createPatientTableQuery.c_str(), NULL, 0 , &errorMessage);

    if(rc != SQLITE_OK){
        cout << "SQL error " << errorMessage;
        sqlite3_free(errorMessage);
        return (0);
    } 

    rc = sqlite3_exec(db, createDoctorTableQuery.c_str(), NULL, 0, &errorMessage);

    if(rc != SQLITE_OK){
        cout << "SQL error " << errorMessage;
        sqlite3_free(errorMessage);
        return (0);
    }

    hospitalScreen();

    loginScreen();
    if(user == "admin"){
        adminMenu();
    }
    if(user == "doctor"){
        doctorMenu();
    }
    return 0;
}

bool loginUser(string password){
    if(password == "admin"){
        user = "admin";
        return true;
    } else if(password == "doctor"){
        user = "doctor";
        return true;
    } else {
        return false;
    }
}

void hospitalScreen(){
    system("clear");
    cout << endl << endl;
    cout << "\t***********************************************************" << endl;
    cout << "\t*                                                         *" << endl;
    cout << "\t*                                                         *" << endl;
    cout << "\t*                                                         *" << endl;
    cout << "\t*        WELCOME TO HOSPITAL MANAGEMENT SYSTEM            *" << endl;
    cout << "\t*                                                         *" << endl;
    cout << "\t*                                                         *" << endl;
    cout << "\t*                                                         *" << endl;
    cout << "\t***********************************************************" << endl;
    sleep(1);
    
}

void loginScreen(){
    LOGIN:
        string password;
        system("clear");
        cout << endl << "============== LOGIN ============" << endl;
        cout << "---> Enter password: ";
        cin >> password;
        if(!loginUser(password)){
            goto LOGIN;
        }
        system("clear");    
        cout << "You are successfully logged IN!!!" << endl;
        sleep(1);
        system("clear");
        
}

void adminMenu(){
    int choice;
    cout << endl << "================== ADMIN MENU ========================" << endl;
    cout << "1. Add Doctor" << endl << "2. Add Patient" << endl << "3. Search Patient" << endl << "4. EXIT" << endl;
    cout <<  "Enter your choice: ";
    cin >> choice;

    switch(choice){
        case 1: {
            adminAddDoctor();
            adminMenu();
            break;
        }
        case 2: {
            adminAddPatient();
            adminMenu();
            break;
        }
        case 3: {
            searchPatient();
            adminMenu();
            break;
        }
        case 4: {
            sqlite3_close(db);
            exit(1);
        }
        default: cout << "Invalid choice!!!";
    }
}

void adminAddPatient(){
    string patientsMobileNumber;
    string patientsAge;
    string patientsFileName;
    string patientsFirstName;
    string patientsMiddleName;
    string patientsLastName;
    string patientsAddress;
    string patientsGender;
    
    system("clear");
    cout << endl << "================== ENTER PATIENT'S DETAILS ==================" << endl;
    cout << "--> Enter File Name: ";
    cin >> patientsFileName;

    cout << "--> Enter patient's first name: ";
    cin >> patientsFirstName;

    cout << "--> Enter patient's middle name: ";
    cin >> patientsMiddleName;

    cout << "--> Enter patient's last name: ";
    cin >> patientsLastName;

    cout << "--> Enter patient's age: ";
    cin >> patientsAge;

    cout << "--> Enter patient's gender: ";
    cin >> patientsGender;

    cout << "--> Enter patient's mobile number: ";
    cin >> patientsMobileNumber;

    fflush(stdin);

    cout << "--> Enter patient's address: ";
    cin >> patientsAddress;

    fflush(stdin);

    

    patient = new Patient( patientsMobileNumber, patientsAge, patientsFileName, patientsGender, patientsFirstName, patientsMiddleName, patientsLastName,patientsAddress, "", "", "", "");
    
    patient -> insertPatient();
}

void adminAddDoctor(){
    string doctorsAge;
    string doctorsMobileNumber;
    string doctorsFirstName;
    string doctorsMiddleName;
    string doctorsLastName;
    string doctorsEmail;
    string doctorsAddress;
    string doctorsDegree;
    string doctorsSpecialization;   
    
    system("clear");

    cout << "Enter doctor's first name: ";
    cin >> doctorsFirstName;

    cout << "Enter doctor's middle name: ";
    cin >> doctorsMiddleName;

    cout << "Enter doctor's last name: ";
    cin >> doctorsLastName;

    cout << "Enter doctor's degree: ";
    cin >> doctorsDegree;

    cout << "Enter doctors's specialization: ";
    cin >> doctorsSpecialization;

    fflush(stdin);

    cout << "Enter doctor's age: ";
    cin >> doctorsAge;

    fflush(stdin);

    cout << "Enter doctor's mobile number: ";
    cin >> doctorsMobileNumber;

    fflush(stdin);

    cout << "Enter doctor's email: ";
    cin >> doctorsEmail;

    cout << "Enter doctors's address: ";
    cin >> doctorsAddress;

    

    doctor = new Doctor(doctorsAge, doctorsMobileNumber, doctorsFirstName, doctorsMiddleName, doctorsLastName, doctorsEmail, doctorsAddress, doctorsDegree, doctorsSpecialization);
    
    doctor -> insertDoctor();
}

void doctorMenu(){
    int choice;
    cout << "1. Update Patient" << endl << "2. Search Patient" << endl << "3. EXIT" << endl;
    cout <<  "Enter your choice: ";
    cin >> choice;

    switch(choice){
        case 1: {
            doctorUpdatePatient();
            doctorMenu();
            break;
        }
        case 2: {
            searchPatient();
            doctorMenu();
            break;
        }
        case 3: {
            sqlite3_close(db);
            exit(1);
            break;
        }
        default: cout << "Invalid choice!!!";
    }
}
void doctorUpdatePatient(){
    string patientsBloodGroup;
    string patientsSymptoms;
    string patientsCheckedBy;
    string patientsMedicines;
    string patientsFileName;
    bool isFileFound = false;

    string findPatientQuery = "SELECT pFileName FROM Patients";

    cout << "Enter patient's file name: ";
    cin >> patientsFileName;

    int rc = sqlite3_exec(db, findPatientQuery.c_str(), callback, 0, &errorMessage);

    if(rc != SQLITE_OK){
        cout << "SQL ERROR" << endl << errorMessage << endl;
        sqlite3_free(errorMessage);
        doctorMenu();
    } 
    
    for(int i = 0; i < fileName.size(); i++){
        if(fileName[i] == patientsFileName){
            isFileFound = true;
            break;
        }
    }

    if(!isFileFound){
        cout << "File Not found!!! please try again by pressing any key!!!" << endl;
        getchar();
        system("clear");
        doctorMenu();
    } else {
        system("clear");
        cout << "Enter patient's blood group: ";
        cin >> patientsBloodGroup;

        cout << "Enter patient's symptoms: ";
        cin >> patientsSymptoms;

        cout << "Enter patient's medicines: ";
        cin >> patientsMedicines;

        cout << "Enter doctor's checked by: ";
        cin >> patientsCheckedBy;

        doctor -> updatePatient(patientsBloodGroup, patientsSymptoms, patientsMedicines, patientsCheckedBy, patientsFileName);
    }
}

static int callback(void* data, int argc, char** argv, char** azColName)
{
	int i;
	fileName.clear();
	for (i = 0; i < argc; i++) {
        if(argv[i]){
            fileName.push_back(argv[i]);
        } else {
            fileName.push_back("NULL");
        }
	}

	printf("\n");
	return 0;
}

static int searchCallback(void *NotUsed, int argc, char **argv, char **azColName) {
    int i;

    system("clear");

    cout << "Matched Search result: " << endl << endl;

    string col[12] = {"File name", "First name", "Middle name", "Last name", "Mobile number", "Address", "Gender", "Age", "Blood group", "Symptoms", "Medicines", "Checked by"};
    for(i = 0; i<argc; i++) {
      cout << "\t" << col[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl; 
    }
    printf("\n");
    return 0;
}


void searchPatient(){

    string patientsFileName;
    bool isFileFound = false; 
 
    cout << "Enter patient's file name: ";
    cin >> patientsFileName;


    string findPatientQuery = "SELECT * from Patients WHERE pFileName='" + patientsFileName + "'";

    int rc = sqlite3_exec(db, findPatientQuery.c_str(), callback, 0, &errorMessage);

    if(rc != SQLITE_OK){
        cout << "SQL ERROR: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }

    for(int i = 0; i < fileName.size(); i++){
        if(fileName[i] == patientsFileName){
            isFileFound = true;
            break;
        }
    }

    if(!isFileFound){
        cout << "File Not found!!! please try again by pressing any key!!!" << endl;
        getchar();
        system("clear");
        doctorMenu();
    }

    rc = sqlite3_exec(db, findPatientQuery.c_str(), searchCallback, 0, &errorMessage);

    if(rc != SQLITE_OK){
        cout << "SQL ERROR: " << errorMessage << endl;
        sqlite3_free(errorMessage);
    }
}

