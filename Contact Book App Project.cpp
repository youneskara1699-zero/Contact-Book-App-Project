#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
using namespace std;

string FileName = "Contact Book.txt";

enum enContactBookOptions {ContactsList = 1, AddContact = 2, EditPhoneNumber = 3,
     DeleteContact = 4, FindContact = 5, Exit = 6 };

struct sContact
{
    string FirstName = "";
    string LastName = "";
    string PhoneNumber = "";
    bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Delim)
{
    vector <string> vString;
    int pos = 0;
    string word = "";
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        word = S1.substr(0, pos);
        if (word != "")
        {
            vString.push_back(word);
        }

        S1.erase(0, pos+Delim.length());

    }
    
    if (S1 != "")
    {
        vString.push_back(S1); 
    }

    return vString;
}

string ConvertRecordToLine(sContact Contact, string Seperator = "#//#")
{
    string RecordLine = "";
    RecordLine += Contact.FirstName + Seperator;
    RecordLine += Contact.LastName + Seperator;
    RecordLine += Contact.PhoneNumber; 
    return RecordLine;
}

sContact ConvertLineToRecord(string Line, string Seperator = "#//#")
{ 
    vector<string> vContactData = SplitString(Line, Seperator);
    sContact Contact;
    Contact.FirstName = vContactData[0];
    Contact.LastName = vContactData[1];
    Contact.PhoneNumber = vContactData[2];

    return Contact;
}

vector<sContact> LoadDataFromFile()
{
    vector<sContact> vContacts;
    fstream MyFile;
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())
    {
        sContact Contact;
        string Line;
        while (getline(MyFile, Line))
        {
            Contact = ConvertLineToRecord(Line);

            if (Contact.MarkForDelete == false)
            {
                vContacts.push_back(Contact);  
            }
        }

        MyFile.close();
    }
    
    return vContacts;
}

void SaveContactsDataToFile(vector<sContact>& vContacts)
{
    fstream MyFile;
    MyFile.open(FileName, ios:: out);
    if (MyFile.is_open())
    {
       string Line;

       for (sContact& Contact : vContacts)
       {
            if (Contact.MarkForDelete == false)
            {
                Line = ConvertRecordToLine(Contact);
                MyFile << Line << endl;
            }
       }
       
       MyFile.close();
    }
    
}

void HeaderOfScreen(string Title, string SubTitle)
{
    cout << "=================================================================" << endl;
    cout << Title << endl;

    if (!SubTitle.empty())
    {
        cout << SubTitle << endl;
    }

    cout << "=================================================================" << endl;
}

void PrintContactData(sContact Contact)
{
    cout << "|" << left << setw(25) << Contact.FirstName;
    cout << "|" << left << setw(25) << Contact.LastName;
    cout << "|" << left << setw(11) << Contact.PhoneNumber << "|" << endl; 
}

bool IsContactExist(string FirstName, string LastName)
{
    vector<sContact> vContacts = LoadDataFromFile();
    for (sContact& Contact : vContacts)
    {
        if (Contact.FirstName == FirstName && Contact.LastName == LastName)
        {
            return true;
        }
    }
    
    return false;
}

bool IsContactExist(string FirstName)
{
    vector<sContact> vContacts = LoadDataFromFile();
    for (sContact& Contact : vContacts)
    {
        if (Contact.FirstName == FirstName)
        {
            return true;
        }
    }
    
    return false;
}

string GetName(string Message)
{
    string Name;
    cout << Message;
    getline(cin >> ws, Name);
    return Name;
}

bool IsAllDigits(string S1)
{
    for (char C : S1)
    {
        if (!isdigit(C))
        {
            return false;
        }        
    }
    
    return true;
}

string GetPhoneNumber(string Message)
{
    string PhoneNumber;
    do
    {
       cout << Message;
       cin  >> PhoneNumber;

       if (!((PhoneNumber.length() == 10) && (IsAllDigits(PhoneNumber))))
       {
           cout << "Invalid Phone Number!" << endl;
           cout << "You should enter 10 Digits for Phone Number."<< endl;
           cout << "Please, Try Again :" << endl;
       }
       
    } while (!((PhoneNumber.length() == 10) && (IsAllDigits(PhoneNumber))));
    
    
    return PhoneNumber;
}

void ShowMenuScreen();

void BackToMainMenuScreen()
{
    cout << "\nPress any key to Go Back to Main Menu ..." << endl;
    system("pause>0");
    ShowMenuScreen();
}

void ShowContactsListScreen()
{
    vector<sContact> vContacts = LoadDataFromFile();
    string Title = "\t\t\tCONTACTS LIST";
    string SubTitle = "\t\t\t(" + to_string(vContacts.size()) + ") Contact(s)";
    HeaderOfScreen(Title, SubTitle);
    cout << "|" << left << setw(25) << "First Name";
    cout << "|" << left << setw(25) << "Last Name";
    cout << "|" << left << setw(11) << "PhoneNumber" << "|" << endl;
    cout << "=================================================================" << endl;
    
    if (vContacts.empty())
    {
        cout << "|" << setw(62) << "No contacts found." << "|" << endl;
    }
    
    else
    {
       for (sContact& Contact : vContacts)
       {
            PrintContactData(Contact);
            cout << endl; 
       }
    }
    
    cout << "=================================================================" << endl;
    
}

void AddNewContactScreen()
{
    HeaderOfScreen("\t\t\tADD NEW CONTACT", "");

    char Answer = 'Y';
    vector<sContact>vContacts = LoadDataFromFile();
    sContact NewContact;
    string FirstName = "", LastName = "", PhoneNumber = "";
    
    do
    {
        FirstName = GetName("Please, Enter First Name: ");
        LastName =  GetName("Please, Enter Last Name: ");

        while (IsContactExist(FirstName, LastName))
        {
            cout << "\nContact already exists! Please enter different names.\n";
            FirstName = GetName("\nPlease, Enter First Name: ");
            LastName =  GetName("Please, Enter Last Name: ");
        }
    
        PhoneNumber = GetPhoneNumber("Please, Enter Phone Number: ");
        NewContact.FirstName = FirstName;
        NewContact.LastName = LastName;
        NewContact.PhoneNumber = PhoneNumber;

        vContacts.push_back(NewContact);
        SaveContactsDataToFile(vContacts);
        cout << "\nContact added successfully!\n" << endl;
        cout << "Add another contact? (y/n): ";
        cin >> Answer;
    } while (toupper(Answer) == 'Y');

}

void EditPhoneNumberScreen()
{
    HeaderOfScreen("\t\t\tEDIT PHONE NUMBER", "");
    vector<sContact> vContacts = LoadDataFromFile();
    sContact Contact;
    string FirstName = "", LastName = "", NewPhoneNumber = "";
    FirstName = GetName("Please, Enter First Name: ");
    LastName =  GetName("Please, Enter Last Name: ");

    while (!IsContactExist(FirstName, LastName))
    {
        cout << "\nContact not found. Please enter another one: " << endl;
        FirstName = GetName("Please, Enter First Name: ");
        LastName =  GetName("Please, Enter Last Name: ");
    }
    
    NewPhoneNumber = GetPhoneNumber("Please, Enter Phone Number: ");
    bool found = false;


    for (sContact& Contact : vContacts)
    {
        if ((Contact.FirstName == FirstName) && (Contact.LastName == LastName))
        {
            Contact.PhoneNumber = NewPhoneNumber; 
            found = true;
            break;
        }
        
    }
    
    if (found)
    {
       SaveContactsDataToFile(vContacts);
       cout << "\nPhone number updated successfully!\n" << endl;
    }
    
}

bool ContactMarkedForDelete(vector<sContact>& vContacts, string FirstName, string LastName)
{

    for (sContact& Contact : vContacts)
    {
        if ((Contact.FirstName == FirstName) && (Contact.LastName == LastName))
        {
            Contact.MarkForDelete = true;
            return true;
        }    
    }
     
    return false;
}

void DeleteContactScreen()
{
    HeaderOfScreen("\t\t\tDELETE CONTACT", "");
    vector<sContact>vContacts = LoadDataFromFile();
    sContact Contact;
    string FirstName = "", LastName = "", NewPhoneNumber = "";
    FirstName = GetName("Please, Enter First Name: ");
    LastName =  GetName("Please, Enter Last Name: ");

    while (!IsContactExist(FirstName, LastName))
    {
        cout << "Contact NOT found. Please enter another one: " << endl;
        FirstName = GetName("Please, Enter First Name: ");
        LastName =  GetName("Please, Enter Last Name: ");
    }
    
    char Confirm = 'Y';
    cout << "\nAre you sure you want to delete this contact? (y/n): ";
    cin >> Confirm;
    
    if (toupper(Confirm) == 'Y')
    {
       if (ContactMarkedForDelete(vContacts, FirstName, LastName))
       {
          SaveContactsDataToFile(vContacts);
          cout << "Contact Deleted Successfully :-)" << endl;
       }
    }
    
    else
    {       
        cout << "Deletion cancelled :-(" << endl;
    }
}

void FindContactScreen()
{
    HeaderOfScreen("\t\t\tFIND CONTACT","");
    vector<sContact>vContacts = LoadDataFromFile();
    sContact Contact;
    string FirstName = "", LastName = "", NewPhoneNumber = "";
    FirstName = GetName("Please, Enter First Name: ");

    while (!IsContactExist(FirstName))
    {
        cout << "Contact NOT found. Please enter another one: " << endl;
        FirstName = GetName("Please, Enter First Name: ");
    }
    
    cout << "=================================================================" << endl;
    cout << "\t\t\tContact Informations;" << endl;
    cout << "=================================================================" << endl;

    for (sContact& Contact : vContacts)
    {
        if ((Contact.FirstName == FirstName))
        {
           PrintContactData(Contact);
        }    
    }
    
    cout << "=================================================================" << endl;
}

int ReadChoice()
{
    int Choice = 0;
    do
    {
        cout << "What do you want to do in Contact Book App (1 to 6): ";
        cin  >> Choice;
        if (Choice > 6 || Choice < 1)
        {
           cout << "Please, Enter Between 1 to 4 !\n"; 
        }
        
    } while (Choice > 6 || Choice < 1);
    
    return Choice;
}

void PerformContactBookOption(enContactBookOptions Choice)
{
    
    switch (Choice)
    {
    case enContactBookOptions::ContactsList:
    {
        system("cls");
        ShowContactsListScreen();
        BackToMainMenuScreen();
        break;
    }
     
    case enContactBookOptions::AddContact:
    {
        system("cls");
        AddNewContactScreen();
        BackToMainMenuScreen();
        break;
    }

    case enContactBookOptions::EditPhoneNumber:
    {
        system("cls");
        EditPhoneNumberScreen();
        BackToMainMenuScreen();
        break;
    }

    case enContactBookOptions::FindContact:
    {
        system("cls");
        FindContactScreen();
        BackToMainMenuScreen();
        break;
    }

    case enContactBookOptions::DeleteContact:
    {
        system("cls");
        DeleteContactScreen();
        BackToMainMenuScreen();
        break;
    }

    case enContactBookOptions::Exit:
    {   
        system("cls");
        cout << "\nThank you for using Contact Book App. Goodbye!\n" << endl;
        BackToMainMenuScreen();
        break;
    }
       
     default:
        break;
    }
}

void ShowMenuScreen()
{
    system("cls");
    HeaderOfScreen("\t\t\tCONTACT BOOK APP", "");
    cout << "\t[1] Show Contacts List           " << endl;
    cout << "\t[2] Add Contact                  " << endl;
    cout << "\t[3] Edit Phone Number            " << endl;
    cout << "\t[4] Delete Contact               " << endl;
    cout << "\t[5] Find Contact                 " << endl;
    cout << "\t[6] Exit                         " << endl;
    cout << "=================================================================" << endl;
    cout << "\n";
    PerformContactBookOption((enContactBookOptions) ReadChoice());
}

int main()
{
    while (true)
    {
        ShowMenuScreen();
    }
    
    return 0;
}