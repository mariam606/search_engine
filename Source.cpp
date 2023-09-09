#include<bits/stdc++.h>
#include <unordered_map>
using namespace std;

unordered_map<string, vector<string>> web_graph;
unordered_map<string, vector<string>> keyword;
unordered_map<string, vector<int>> no_of_impressions;
unordered_map<string, double> page_rank;
unordered_map<string, double>::iterator it1;
unordered_map<string, vector<int>>::iterator it2;
vector<string> websites;
vector<string> key;
map<string, int> vis;
map<double, string, greater <double> > target_sites;
void screens(int);

void page_rank_algo() {
    //iteration 0:

    for (int i = 0; i < websites.size() - 1; i++)
        page_rank[websites[i]] = 1.0 / (websites.size() - 1);

    //rest of iterations:
    double error = 0.0001;
    unordered_map<string, double> current;
    current = page_rank;
    double temp_rank;
    while (true) { //loop until the diffence between 2 two iterations is less than 0.00001
        int correct = 1;
        for (auto i : websites)
            vis[i] = 0;
        for (auto i : websites) //parent
            for (auto j : web_graph[i]) {//children
                temp_rank = page_rank[i] / (web_graph[i].size());
                if (!vis[j]) {
                    current[j] = temp_rank;
                    vis[j] = 1;
                }
                else current[j] += temp_rank;
            }
        //Stopping the loop if the difference between the pagerank in two successive iterations is less than error allowed
        for (auto i : websites)
            if (abs(page_rank[i] - current[i]) < error)
                correct++;

        page_rank = current;
        if (correct == websites.size() - 1)
            break;
    }

    double x, great_prev = 10000.0;
    string temp;
    for (auto i : websites)
        vis[i] = 0;
    for (int i = websites.size() - 1; i >= 1; i--) {
        x = 0.0;
        for (auto j : websites)
            if (page_rank[j] >= x && page_rank[j] < great_prev && !vis[j]) {
                x = page_rank[j];
                temp = j;
            }
        vis[temp] = 1;
        page_rank[temp] = i;
        great_prev = x;
    }
}

void read_files(bool file1, bool file2, bool file3)
{
    fstream fin, fin2, fin3;
    string line, fword, word, temp;

    //Reading the web graph file
    if (file1) {
        fin.open("web graph file.csv", ios::in);
        if (fin.fail()) {
            cout << "Error: cannot open web graph file \n";
            exit(0);
        }
        while (!fin.eof()) {

            getline(fin, line);
            stringstream ss(line);

            getline(ss, fword, ',');
            //cout << fword << endl;
            while (getline(ss, word, ',')) {
                web_graph[fword].push_back(word);
            }
        }
    }

    //Reading the keyword file
    if (file2) {
        fin2.open("keyword file.csv", ios::in);
        if (fin2.fail()) {
            cout << "Error: cannot open keyword file \n";
            exit(0);
        }
        while (!fin2.eof()) {

            getline(fin2, line);
            stringstream s(line);

            getline(s, fword, ',');
            websites.push_back(fword);
            while (getline(s, word, ',')) {
                keyword[fword].push_back(word);
            }
        }
    }
    //Reading the number of impressions file
    if (file3) {
        fin3.open("number of impressions file.csv", ios::in);
        if (fin3.fail()) {
            cout << "Error: cannot open keyword file \n";
            exit(0);
        }
        while (!fin3.eof()) {

            getline(fin3, line);
            stringstream s(line);

            getline(s, fword, ',');
            while (getline(s, word, ',')) {
                int x;
                x = stoi(word);
                no_of_impressions[fword].push_back(x);
            }
        }
    }
}

void update_files(string site_name, int col) // col 0-> impressions    1-> clicks
{
    // File pointer
    fstream fin, fout;

    // Open an existing record
    fin.open("number of impressions file.csv", ios::in);
    if (fin.fail()) {
        cout << "Error: cannot open web graph file \n";
        exit(0);
    }

    // Create a new file to store updated data
    fout.open("new number of impressions file.csv", ios::out);
    if (fout.fail()) {
        cout << "Error: cannot open web graph file \n";
        exit(0);
    }

    //searching for the website name
    vector<string> row;
    string line, fword, word;
    int temp;
    while (!fin.eof()) {

        row.clear();

        getline(fin, line);
        stringstream s(line);

        getline(s, fword, ',');
        row.push_back(fword);
        if (fword == site_name) {
            if (col == 0) {
                getline(s, word, ',');
                temp = stoi(word);
                temp++;
                word = to_string(temp);
                row.push_back(word);
                getline(s, word, ',');
                row.push_back(word);
            }
            else if (col == 1) {
                getline(s, word, ',');
                row.push_back(word);
                getline(s, word, ',');
                temp = stoi(word);
                temp++;
                word = to_string(temp);
                row.push_back(word);
            }
        }
        else
            while (getline(s, word, ','))
                row.push_back(word);

        int row_size = row.size();
        if (!fin.eof()) {
            for (int i = 0; i < row_size - 1; i++)
                fout << row[i] << ", ";
            fout << row[row_size - 1] << "\n";
        }
    }
    fin.close();
    fout.close();

    // removing the existing file
    remove("number of impressions file.csv");

    // renaming the updated file with the existing file name
    rename("new number of impressions file.csv", "number of impressions file.csv");
    read_files(0, 0, 1);
}

// printing the final results of the research
void final_printing() {
    cout << endl;
    int c = 1;
    cout << "Search results: \n";
    for (auto i : target_sites) {
        update_files(i.second, 0);
        cout << c << ".   " << i.second << endl;
        c++;
    }
    cout << endl;
}

//Calculating the score of each website and arrange then descendingly in map
void CTR() {
    string temp;
    int tempPR = 1, tempImpres = 1, tempClicks = 1;
    vector<double> scores;
    vector<string> sites;
    double score;
    for (auto i : target_sites) {
        it1 = page_rank.find(i.second);
        tempPR = it1->second;
        it2 = no_of_impressions.find(i.second);
        tempImpres = it2->second[0];
        tempClicks = it2->second[1];
        score = 0.4 * tempPR + ((1 - ((0.1 * tempImpres) / (1 + 0.1 * tempImpres))) * tempPR + ((0.1 * tempImpres) / (1 + 0.1 * tempImpres)) * (((1.0 * tempClicks) / tempImpres) *100)) * 0.6;
        scores.push_back(score);
        string tempSite = i.second;
        sites.push_back(tempSite);
    }
    target_sites.clear();
    for (int i = 0; i < scores.size(); i++)
        target_sites[scores[i]] = sites[i];
    final_printing(); //to print the ordered map
}

//Searching for the websites that contain this keyword
void search() {
    cout << "Please enter a keyword: ";
    string query;
    cin.ignore(std::numeric_limits<streamsize>::max(), '\n');
    getline(cin, query);
    target_sites.clear();

    for (auto i : websites)
        vis[i] = 0;
    vector<string> temp_sites;
    double c = 100000.0;
    vector<string> words;
    int type = 3;

    //Find type of query
    if (query[0] == '"') {
        words.push_back(query.substr(1, query.size() - 2));
        type = 1;
    }
    else {
        stringstream ss(query);
        string temp;
        while (ss >> temp) {
            if (temp == "AND") {
                type = 2;
                continue;
            }
            else if (temp == "OR") continue;
            words.push_back(temp);
        }
    }
    for (auto i : websites)
        vis[i] = 0;
    //The ANDing case
    if (type == 2) {
        for (auto word : words) {
            for (auto i : websites)
                for (auto j : keyword[i]) {
                    if (word == j)
                        vis[i] += 1;
                    if (vis[i] == words.size()) {
                        target_sites[c] = i;
                        c++;
                    }

                }
        }
    }
    else {//The ORing case
        for (auto word : words) {
            for (auto i : websites)
                for (auto j : keyword[i])
                    if (word == j) {
                        if (vis[i] == 0)
                            target_sites[c] = i;
                        c++;
                        vis[i] += 1;
                    }
        }
    }

    if (!target_sites.empty()) {
        CTR(); //To calculate the score of each website and arrange then descendingly
        screens(4);
    }
    else {
        cout << "Key word is not found!\n";
        search();
    }
}

void screens(int choice) {
    //screen 2:
    if (choice == 1) {
        search();
    }
    if (choice == 2) exit(0);
    if (choice == 4) {
        while (true) {
            cout << "1. Choose a webpage to open\n" << "2. New Search\n" << "3. Exit\n";
            cout << "Type in your choice: \n";
            cin >> choice;
            if (choice == 3) exit(0);
            if (choice == 1) {
                cout << "Enter the URL of the website without spaces\n";
                string str;
                cin >> str;
                bool found = 0;
                for (auto i : target_sites)
                    if (str == i.second) {
                        found = 1;
                        update_files(str, 1);
                        cout << "You're now veiwing " << str << endl;
                    }
                if (!found) cout << "Error: this website is not found!\n";
                cout << "1. Back to search results\n" << "2. New Search\n" << "3. Exit\n";
                cout << "Type in your choice: \n";
                cin >> choice;
                if (choice == 1) final_printing();
                if (choice == 2) screens(1);
                if (choice == 3) exit(0);
            }
            if (choice == 2) screens(1);
        }
    }
}

void user_interface() {
    cout << "Welcome!" << endl << "What would you like to do?\n";
    cout << "1. New Search" << endl << "2. Exit" << endl;
    cout << "Type in your choice: " << endl;
    int choice;
    cin >> choice;
    if (choice == 1) screens(choice);
    if (choice == 2) exit(0);
}

int main() {

    read_files(1, 1, 1);
    page_rank_algo();
    user_interface();

    return 0;
}
