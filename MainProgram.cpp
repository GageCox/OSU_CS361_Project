#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

struct movie
{
    std::string title;
    int year;
    std::string genre;
    std::string director;
    std::string writer;
    std::string actor;
    std::string review;
    struct movie* next;
};

std::string login();

void homepage();

void movieList(std::string username);

void addMovie(std::string username);

void helpPage();

void navigation();



int main()
{
    std::string username;
    std::string input;
    int inputNum = 0;
    
    username = login();
    homepage();

    while (input != "exit")
    {
        std::cout << "Enter a command: ";
        std::cin >> input;

        if (input == "list")
            inputNum = 1;
        else if (input == "add")
            inputNum = 2;
        else if (input == "help")
            inputNum = 3;
        else if (input == "home")
            inputNum = 4;
        else if (input == "exit")
            inputNum = 5;

        switch (inputNum)
        {
            case 1:
                movieList(username);
                break;
            case 2:
                addMovie(username);
                break;
            case 3:
                helpPage();
                break;
            case 4:
                homepage();
                break;
            case 5:
                std::cout << "Logging out...\n";
                break;
            default:
                std::cout << "Invalid command. Try again\n";
        }
    }

    return 0;
}

std::string login()
{
    int input = 0;
    std::string username;
    std::string password;
    std::string filename;

    
    std::cout << "*************\n"
              << "My Movie List\n"
              << "*************\n\n"
              << "Keep track of movies you've watched and your thoughts about them!\n\n"
              << "Enter 1 to register\n"
              << "Enter 2 to login\n";
        
    while (input != 1 && input != 2)
    {
        std::cout << "Enter (1/2): ";
        std::cin >> input;
        if (input !=  1 && input != 2)
            std::cout << "Invalid Input. Try Again\n";
    }

    if (input == 1)
    {
        std::cout << "Enter your username: ";
        std::cin >> username;
        std::cin.ignore();
        std::cout << "\nEnter your password: ";
        std::cin >> password;

        std::ofstream outfile ("users.txt", std::ios_base::app | std::ios_base::out);
        outfile << username << "," << password << std::endl;
        outfile.close();

        std::ofstream userfile (username + "_movies.csv", std::ios_base::app | std::ios_base::out);
        userfile << "Title,Year,Genre,Director,Writer,Actor,Review" << std::endl;
        userfile.close();
    }

    else if (input == 2)
    {
        bool control = 0;
        while (control == 0)
        {
            std::cout << "Enter your username: ";
            std::cin >> username;
            std::cin.ignore();
            std::cout << "\nEnter your password: ";
            std::cin >> password;

            std::string tempUser;
            std::string tempPass;
            std::ifstream infile ("users.txt", std::ios_base::in);
            getline(infile, tempUser, ',');
            getline(infile, tempPass);

            if (tempUser == username && tempPass == password)
            {
                std::cout << "Welcome back!\n";
                control = 1;
                break;
            }

            else
                std::cout << "Username or password incorrect. Try again\n";
        }
    }

    std::ofstream outfile ("users.txt");
    outfile << username << "," << password << std::endl;

    return username;
}

void homepage()
{
    std::string input;

    std::cout << "\n\n********\n"
              << "Homepage\n"
              << "********\n"
              << "Welcome to My Movie List!\n"
              << "Here you can create a list of all the movies you've watched and your thoughts on them!\n"
              << "Navigation:\n"
              << "Enter" << " \"list\"" << " to view your list\n"
              << "Enter" << " \"add\"" << " to add a new movie to your list\n"
              << "Enter" << " \"help\""  << " for more information\n"
              << "Enter" << " \"exit\"" << " to logout\n";
    return;
}

void movieList(std::string username)
{
    std::cout << "\n\n***************\n"
              << "Your Movie List\n"
              << "***************\n"
              << "\nNavigation:\n"
              << "Enter" << " \"homepage\"" << " to return to the homepage\n"
              << "Enter" << " \"help\""  << " for more information\n"
              << "Enter" << " \"exit\"" << " to logout\n";

    std::cout << "Instructions:\n"
              << "Enter \"add\" to add a movie\n"
              << "Enter the corresponding number to view a movie in the list\n";

    std::cout << "\nHere are the movies you've added to your list:\n\n";

    std::string input;
    std::string currLine;

    std::string filename = username + "_movies.csv";

    std::ifstream infile (filename, std::ios_base::in);
    if (!infile)
    {
        std::cout << "No movie file found for user " << username << "\n";
        return;
    }

    movie *head = nullptr;
    bool skipHeader = true;

    while (std::getline(infile, currLine))
    {
        if (skipHeader)
        {
            skipHeader = false;
            continue; // skip CSV header
        }

        if (currLine.empty())
            continue;

        movie *newMovie = new movie;
        newMovie->next = nullptr;
        std::string temp;

        std::istringstream ss(currLine);
        std::getline(ss, newMovie->title, ',');
        if (!std::getline(ss, temp, ','))
            temp = "0";
        try { newMovie->year = std::stoi(temp); } catch(...) { newMovie->year = 0; }
        std::getline(ss, newMovie->genre, ',');
        std::getline(ss, newMovie->director, ',');
        std::getline(ss, newMovie->writer, ',');
        std::getline(ss, newMovie->actor, ',');
        std::getline(ss, newMovie->review);

        if (head == nullptr)
            head = newMovie;
        else
        {
            movie *t = head;
            while (t->next != nullptr)
                t = t->next;
            t->next = newMovie;
        }
    }

    infile.close();

    if (head == nullptr)
    {
        std::cout << "No movies found.\n";
        return;
    }

    // Display the movies
    movie *cur = head;
    int idx = 1;
    std::cout << "\nYour movies:\n";
    while (cur)
    {
        std::cout << idx << ". " << cur->title << " (" << cur->year << ")\n";
        cur = cur->next;
        ++idx;
    }

    // free list
    cur = head;
    while (cur)
    {
        movie *tmp = cur->next;
        delete cur;
        cur = tmp;
    }

    return;
}

void addMovie(std::string username)
{
    std::string filename = username + "_movies.csv";
    std::string title;
    int year;
    std::string genre;
    std::string director;
    std::string writer;
    std::string actor;
    std::string review;

    std::cout << "\n\n*********\n"
              << "Add Movie\n"
              << "*********\n";

    std::cout << "\nNavigation:\n"
              << "Enter" << " \"homepage\"" << " to return to the homepage\n"
              << "Enter" << " \"list\"" << " to view your list\n"
              << "Enter" << " \"help\""  << " for more information\n"
              << "Enter" << " \"exit\"" << " to logout\n";
    

    std::cout << "Instructions:\n"
              << "Enter information about the movie as the prompts appear on your screen, the prompts will be:\n"
              << "Title\n"
              << "Year\n"
              << "Genre\n"
              << "Director\n"
              << "Writer\n"
              << "Main Actor\n"
              << "Your Review\n"
              << "Only the title is required, you can skip any of the other prompts by pressing Enter\n";

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Title (required): ";
    std::getline(std::cin, title);

    std::string yearStr;
    std::cout << "Year (press Enter to skip): ";
    std::getline(std::cin, yearStr);
    try { year = std::stoi(yearStr); } catch(...) { year = 0; }

    std::cout << "Genre (press Enter to skip): ";
    std::getline(std::cin, genre);
    std::cout << "Director (press Enter to skip): ";
    std::getline(std::cin, director);
    std::cout << "Writer (press Enter to skip): ";
    std::getline(std::cin, writer);
    std::cout << "Main Actor (press Enter to skip): ";
    std::getline(std::cin, actor);
    std::cout << "Your Review (press Enter to skip): ";
    std::getline(std::cin, review);

    std::ofstream outfile (filename, std::ios_base::app | std::ios_base::out);
    outfile << title << "," << year << "," << genre << "," << director << "," << writer << "," << actor << "," << review << std::endl;
    outfile.close();

    std::cout << "Movie has been added to your list!\n";

    return;
}

void helpPage()
{
    std::cout << "*********\n"
              << "Help Page\n"
              << "*********\n";

    std::cout << "\nNavigation:\n"
              << "Enter" << " \"homepage\"" << " to return to the homepage\n"
              << "Enter" << " \"list\"" << " to view your list\n"
              << "Enter" << " \"exit\"" << " to logout\n";
    
    std::cout <<"\nTo add a movie enter \"add\" from the homepage or from your list\n"
              << "To edit a movie enter \"edit\" when viewing a movie\n"
              << "To remove a movie from your list enter \"delete\" when viewing a movie\n"
              << "To logout at any time enter \"exit\"\n"
              << "If you are ever feeling lost refer to the top of the page to see the navigation options\n";

}